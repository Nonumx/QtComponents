#include "embedded_widget/embedded_widget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    EmbeddedWidget w;
    auto* program = R"(D:\Programs\Notepad3\Notepad3.exe)";
    w.SetProgram(program);
    w.Start();
    w.resize(1200, 800);
    w.show();
    return QApplication::exec();
}