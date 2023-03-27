#include <QApplication>

#include "tool_bar.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  ToolBar tb;
  tb.show();
  return QApplication::exec();
}
