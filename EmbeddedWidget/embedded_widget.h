#pragma once

#include <QWidget>

#ifdef Q_OS_WIN
class WindowFinderWin;
using WindowFinder = WindowFinderWin;
#endif

class EmbeddedWidget : public QWidget {
  Q_OBJECT

public:
  explicit EmbeddedWidget(QWidget* parent = nullptr);

  void SetProgram(const QString& program) const;
  void SetArguments(const QStringList& arguments) const;

public slots:
  void Start() const;
  void EmbedWindow(WId wid);

private:
  WindowFinder* m_finder;
};
