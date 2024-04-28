#include <QApplication>
#include "project_explorer/project_explorer.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ProjectExplorer w(R"(D:\NewProjects\QtComponents\)");
    w.resize(600, 400);
    w.show();
    return QApplication::exec();
}