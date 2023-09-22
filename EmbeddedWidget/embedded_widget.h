#pragma once

#include <QWidget>
#include <QProcess>
#include <Windows.h>

class EmbeddedWidget : public QWidget {
  Q_OBJECT

public:
  explicit EmbeddedWidget(QWidget* parent = nullptr);

  void SetProgram(const QString& program) const;
  void SetArguments(const QStringList& arguments) const;

public slots:
  void Start();
  void FindWinId(QProcess::ProcessState state);
  void FoundWindowFinished();

private:
  // HWND: 句柄，DWORD：进程号
  using ProcessData = std::pair<HWND, DWORD>;

  QProcess* process_;
  QTimer* timer_;
  bool found_window_;

  std::wstring proc_window_name_;
  std::wstring proc_class_name_;

  void FindWindowByProcessId();
  void FindWindowByWindowName();
  void EmbedWindow(HWND hwnd);
  void LoadProgramProperty();
  void WaitForLaunching();

  static bool IsMainWindow(HWND hwnd);
  static BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM param);

signals:
  // 通过 Win32 API 获取到了程序的句柄
  void FoundWindow();
  // 部分进程会重新拉起新进程，导致进程实际以 detach 模式启动
  void ProcessStartDetached();
  void StartProgram();
};
