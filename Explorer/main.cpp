#include <QApplication>
#include <QCommandLineParser>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QScreen>
#include <QScroller>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	QCommandLineParser parser;
	parser.setApplicationDescription("Qt Dir View Example");
	parser.addHelpOption();
	parser.addVersionOption();
	QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Set QFileSystemModel::DontUseCustomDirectoryIcons");
	parser.addOption(dontUseCustomDirectoryIconsOption);
	QCommandLineOption dontWatchOption("w", "Set QFileSystemModel::DontWatch");
	parser.addOption(dontWatchOption);
	parser.addPositionalArgument("directory", "The directory to start in.");
	parser.process(app);
	const QString rootPath = parser.positionalArguments().isEmpty();
	QFileSystemModel model;
	QFileIconProvider iconProvider;
	model.setIconProvider(&iconProvider);
	model.setRootPath("");
	if (parser.isSet(dontUseCustomDirectoryIconsOption))
		model.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
	if (parser.isSet(dontWatchOption))
		model.setOption(QFileSystemModel::DontWatchForChanges);
	QTreeView tree;
	tree.setModel(&model);
	tree.setAnimated(false);
	tree.setIndentation(20);
	tree.setSortingEnabled(true);
	const QSize availableSize = tree.screen()->availableGeometry().size();
	tree.resize(availableSize / 2);
	tree.setColumnWidth(0, tree.width() / 3);
	QScroller::grabGesture(&tree, QScroller::TouchGesture);
	tree.show();
	return QApplication::exec();
}