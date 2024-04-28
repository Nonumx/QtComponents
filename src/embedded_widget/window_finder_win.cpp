#include "window_finder_win.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QTimer>
#include <Windows.h>
#include <set>
// #include <toml11/toml.hpp>


struct ProcessData
{
    DWORD m_pid;
    std::set<HWND> m_occurs;

    ProcessData()
        : m_pid(0)
    {
    }

    ProcessData(DWORD pid)
        : m_pid(pid)
    {
    }
};

bool IsMainWindow(HWND hwnd)
{
    return !(GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD);
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM param)
{
    auto* data = reinterpret_cast<ProcessData*>(param);
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (data->m_pid == pid && IsMainWindow(hwnd))
    {
        data->m_occurs.insert(hwnd);
    }
    return TRUE;
}

WindowFinderWin::WindowFinderWin(QObject* parent)
    : IWindowFinder(parent), m_timer(new QTimer(this)),
      m_foundWindow(false)
{
    connect(m_process, &QProcess::stateChanged, this,
            &WindowFinderWin::StartFindWindow);
    connect(this, &WindowFinderWin::FoundWindow, this,
            &WindowFinderWin::FindWindowFinished);
    connect(this, &WindowFinderWin::ProcessStartDetached, this,
            &WindowFinderWin::FindWindowFinished);
}

void WindowFinderWin::StartFindWindow(QProcess::ProcessState state)
{
    qDebug() << "进程" << m_process->program() << "运行状态:" << state;
    if (state == QProcess::Running)
    {
        // 部分程序会直接拉起，所以进程从 NotRunning -> Running
        // 变为稳定状态，这时需要不停去搜索窗口句柄，在窗口启动时
        // 成功获取到句柄
        FindWindowByProcessId();
    }
    else if (!m_foundWindow && state == QProcess::NotRunning)
    {
        // 这种情况下，程序可能是由当前进程拉起的
        // 一个新进程，所以程序从 Running -> NotRunning
        emit ProcessStartDetached();
        FindWindowByProperty();
    }
}

void WindowFinderWin::FindWindowFinished()
{
    m_foundWindow = true;
    m_timer->stop();
    disconnect(m_timer, &QTimer::timeout, nullptr, nullptr);
}

void WindowFinderWin::LoadProgramProperty()
{
    // try
    // {
    //     auto setting_file = QApplication::applicationDirPath() +
    //                         "/../Setting/program_property.toml";
    //     auto doc = toml::parse(setting_file.toStdString());
    //     auto program_name = QFileInfo(m_process->program()).baseName();
    //     auto program_property = doc[program_name.toStdString()];
    //     auto program_class_name =
    //         find_or(program_property, "className", "");
    //     m_className = QString::fromStdString(program_class_name).toStdWString();
    // }
    // catch (std::exception& e)
    // {
    //     qWarning() << e.what();
    // }
}

void WindowFinderWin::FindWindowByProcessId()
{
    auto data = std::make_shared<ProcessData>(m_process->processId());
    auto try_find_window = [=]
    {
        if (m_foundWindow)
        {
            return;
        }
        data->m_occurs.clear();
        EnumWindows(EnumWindowsCallback,
                    reinterpret_cast<LPARAM>(data.get()));
        for (auto* hwnd : data->m_occurs)
        {
            uint style = GetWindowLong(hwnd, GWL_STYLE);
            if ((style & 0x14c70000) == 0x14c70000)
            {
                auto wid = reinterpret_cast<WId>(hwnd);
                emit FoundWindow(wid);
            }
        }
    };
    connect(m_timer, &QTimer::timeout, try_find_window);
    m_timer->start(300);
    try_find_window();
}

void WindowFinderWin::FindWindowByProperty()
{
    auto try_find_window = [=]
    {
        if (m_foundWindow)
        {
            return;
        }
        auto* hwnd = FindWindow(m_className.c_str(), nullptr);
        if (hwnd != nullptr)
        {
            auto wid = reinterpret_cast<WId>(hwnd);
            emit FoundWindow(wid);
        }
    };
    connect(m_timer, &QTimer::timeout, try_find_window);
    m_timer->start(300);
    try_find_window();
}
