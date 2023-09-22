#include <QApplication>
#include "embedded_widget.h"
#include <QDebug>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  EmbeddedWidget w;
  // 一般需要足够大的空间才能更好展示嵌入效果
  w.resize(800, 600);
  w.show();

  // 设置程序路径，请小心嵌入 Chrome 类进程，容易抓取到错误的窗口
  w.SetProgram(R"("D:\Projects\XA21S_MXQFXYAQPG_master\07_prototype\bin\Tools\M-Design\M-Design.exe")");
  w.Start();

  return QApplication::exec();
}