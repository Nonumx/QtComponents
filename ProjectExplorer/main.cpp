#include <QApplication>

#include "project_explorer.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QList<ProjectFilter> files;
	files.append({ "Src" });
	files.append({ "Hdr"});
	ProjectExplorer pe(nullptr, files);
	pe.show();
	return QApplication::exec();
}
