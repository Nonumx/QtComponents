#pragma once

#include <map>

#include <QTreeWidget>
#include <QFileSystemWatcher>

struct ProjectFilter
{
	QString filter_name;
	QStringList paths;
};

class ProjectExplorer: public QTreeWidget
{
	Q_OBJECT

public:
	explicit ProjectExplorer(QWidget* parent, const QList<ProjectFilter>& project_filters);

private:
	std::map<QString, QTreeWidgetItem*> item_by_dir_;
	QFileSystemWatcher* watcher_;

	void UpdateProject();
};
