#include "embedded_widget.h"
#include <QDebug>
#include <QTimer>
#include <QWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileInfo>
#include <toml.hpp>

EmbeddedWidget::EmbeddedWidget(QWidget* parent)
  : QWidget(parent), process_(new QProcess(this)), timer_(new QTimer(this)),
    found_window_(false) {
  // 设置控件的布局，用于存放嵌入的程序
  new QVBoxLayout(this);
  connect(process_, &QProcess::stateChanged, this, &EmbeddedWidget::FindWinId);
  // 计时器主要用于进程启动后，窗口还没创建前，轮询 Win32 API 来等待窗口句柄，
  // 无论控件是找到了窗口，还是发现进程以 detach 模式启动，都应该停止计时器
  connect(this, &EmbeddedWidget::FoundWindow, this,
          &EmbeddedWidget::FoundWindowFinished);
  connect(this, &EmbeddedWidget::ProcessStartDetached, this,
          &EmbeddedWidget::FoundWindowFinished);
}

void EmbeddedWidget::SetProgram(const QString& program) const {
  process_->setProgram(program);
}

void EmbeddedWidget::SetArguments(const QStringList& arguments) const {
  process_->setArguments(arguments);
}

void EmbeddedWidget::Start() {
  LoadProgramProperty();
  process_->start();
}

void EmbeddedWidget::FindWinId(QProcess::ProcessState state) {
  qDebug() << state;
  if (state == QProcess::Running) {
    // 部分程序会直接拉起，所以进程从 NotRunning -> Running
    // 变为稳定状态，这时需要不停去搜索窗口句柄，在窗口启动时
    // 成功获取到句柄
    FindWindowByProcessId();
  } else if (!found_window_ && state == QProcess::NotRunning) {
    // 这种情况下，程序可能是由当前进程拉起的
    // 一个新进程，所以程序从 Running -> NotRunning
    emit ProcessStartDetached();
    FindWindowByWindowName();
  }
}

void EmbeddedWidget::FoundWindowFinished() {
  found_window_ = true;
  timer_->stop();
  disconnect(timer_, &QTimer::timeout, nullptr, nullptr);
}

void EmbeddedWidget::FindWindowByProcessId() {
  ProcessData data(nullptr, process_->processId());
  auto try_find_window = [=] {
    bool flag = EnumWindows(EnumWindowsCallback,
                            reinterpret_cast<LPARAM>(&data));
    if (!flag && data.first != nullptr) {
      auto window_style = GetWindowLong(data.first, GWL_STYLE);
      emit FoundWindow();
      EmbedWindow(data.first);
      if (window_style == 0x4C20000) {
        WaitForLaunching();
      }
    }
  };
  connect(timer_, &QTimer::timeout, try_find_window);
  timer_->start(300);
  try_find_window();
}

void EmbeddedWidget::FindWindowByWindowName() {
  auto program = process_->program();
  auto program_basename = QFileInfo(program).baseName();
  auto try_find_window = [=] {
    HWND hwnd = FindWindow(proc_class_name_.c_str(),
                           proc_window_name_.c_str());
    if (hwnd != nullptr) {
      emit FoundWindow();
      EmbedWindow(hwnd);
    }
  };
  connect(timer_, &QTimer::timeout, try_find_window);
  timer_->start(300);
  try_find_window();
}

void EmbeddedWidget::EmbedWindow(HWND hwnd) {
  qDebug() << "1" << GetWindowLong(hwnd, GWL_STYLE);
  auto wid = reinterpret_cast<WId>(hwnd);
  auto window = QWindow::fromWinId(wid);
  auto container = QWidget::createWindowContainer(window, this);
  layout()->addWidget(container);
}

void EmbeddedWidget::LoadProgramProperty() {
  try {
    auto doc = toml::parse("./setting/config.toml");
    auto program_basename = QFileInfo(process_->program()).baseName();
    auto program_property = doc[program_basename.toStdString()];
    auto program_class_name = toml::find_or(program_property, "className", "");
    auto program_window_name =
        toml::find_or(program_property, "windowName", "");
    proc_class_name_ = QString::fromStdString(program_class_name).
        toStdWString();
    proc_window_name_ = QString::fromStdString(program_window_name).
        toStdWString();
  } catch (std::exception& e) {
    qWarning() << e.what();
  }
}

void EmbeddedWidget::WaitForLaunching() {
  auto wait_for_launching = [=] {
    static HWND hwnd = nullptr;
    ProcessData data(nullptr, process_->processId());
    bool flag = EnumWindows(EnumWindowsCallback,
                            reinterpret_cast<LPARAM>(&data));
    if (!flag && data.first != nullptr) {
      auto window_style = GetWindowLong(data.first, GWL_STYLE);
      qDebug() << "2" << window_style;
      if (window_style == 0x4C70000) {
        hwnd = data.first;
      }
      if (window_style < 0 && hwnd != nullptr) {
        // 移除最开始加入的启动界面
        layout()->takeAt(0)->widget()->deleteLater();
        EmbedWindow(hwnd);
        timer_->stop();
        disconnect(timer_, &QTimer::timeout, nullptr, nullptr);
      }
    }
  };
  connect(timer_, &QTimer::timeout, wait_for_launching);
  timer_->start(150);
}

bool EmbeddedWidget::IsMainWindow(HWND hwnd) {
  return !(GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD);
}

BOOL EmbeddedWidget::EnumWindowsCallback(HWND hwnd, LPARAM param) {
  auto data = reinterpret_cast<ProcessData*>(param);
  DWORD apid;
  GetWindowThreadProcessId(hwnd, &apid);
  if (data->second == apid && IsMainWindow(hwnd)) {
    data->first = hwnd;
    return FALSE;
  }
  return TRUE;
}
