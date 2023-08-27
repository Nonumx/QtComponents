#pragma once

#include <QFileSystemWatcher>
#include <QTreeWidget>

enum class ProjectExplorerItem {
	Directory,
	File,
};

class ProjectExplorer : public QTreeWidget {
  Q_OBJECT

 public:
  explicit ProjectExplorer(QWidget* parent = nullptr);

 public slots:
  void ResolveCustomContextMenuRequest(const QPoint& pos);

 private:
  QFileSystemWatcher* watcher_;
};
