#pragma once

#include "i_window_finder.h"
#include <qwindowdefs.h>

class WindowFinderWin : public IWindowFinder
{
    Q_OBJECT

public:
    explicit WindowFinderWin(QObject* parent = nullptr);

public slots:
    void StartFindWindow(QProcess::ProcessState state);
    void FindWindowFinished();

private:
    QTimer* m_timer;
    bool m_foundWindow;
    std::wstring m_className;

    void LoadProgramProperty();
    void FindWindowByProcessId();
    void FindWindowByProperty();

signals:
    void FoundWindow(WId wid);
    void ProcessStartDetached();
};
