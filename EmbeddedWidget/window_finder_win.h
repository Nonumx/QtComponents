#pragma once

#include <QObject>
#include <QProcess>
#include <qwindowdefs.h>

class WindowFinderWin : public QObject {
  Q_OBJECT

public:
  explicit WindowFinderWin(QObject* parent = nullptr);

  void SetProgram(const QString& program);
  void SetArguments(const QStringList& arguments);

public slots:
  void Start();
  void StartFindWindow(QProcess::ProcessState state);
  void FindWindowFinished();

private:
  QProcess* m_process;
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
