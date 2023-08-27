#include <QApplication>

#include "project_explorer.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ProjectExplorer pe;
	pe.show();
	return QApplication::exec();
}
