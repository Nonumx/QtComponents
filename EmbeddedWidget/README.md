# 程序嵌入窗口

## 控件描述

说实话这个控件就很奇葩，它之所以出现，是因为有一些第三方程序没有SDK，但是又需要借助这些程序的功能，这时就需要将进程创建出来，然后把程序嵌入到原本的窗口作为一个子窗口的样式。

但是由于每个程序都对自己的窗体或多或少有一些改动，因此极有可能出现bug，操作失灵之类的问题。有些窗体还不易抓取，抓取后也会出现bug。

例如以 Chrome 为例的进程，众所周知每个人的电脑上都会有数个Chrome，其中以 Electron 最为著名，恰巧每个 Electron 程序都是一个 Chrome。

在做这种嵌入任务时，一个不小心就是把莫名其妙的程序嵌入进来，然后最后嵌入窗口被析构时，这些程序就莫名躺枪被关闭。

所以这个控件只能说是权衡之举，** 请小心使用！**

## 使用方法

```C++
#include <QApplication>
#include "embedded_widget.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  
  EmbeddedWidget w;
  // 一般需要足够大的空间才能更好展示嵌入效果
  w.resize(800, 600);
  w.show();

  // 设置程序路径，请小心嵌入 Chrome 类进程，容易抓取到错误的窗口
  w.SetProgram(R"("C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe")");
  w.Start();

  return QApplication::exec();
}
```
