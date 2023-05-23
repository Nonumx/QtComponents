#include <map>

#include "project_explorer.h"

ProjectExplorer::ProjectExplorer(QWidget* parent, const QList<ProjectFilter>& project_filters) : QTreeWidget(parent)
{

	watcher_ = new QFileSystemWatcher(this);

	for (const auto& filter : project_filters)
	{
		item_by_dir_[filter.filter_name] = new QTreeWidgetItem(this, { filter.filter_name });
		watcher_->addPaths(filter.paths);
	}

	setHeaderHidden(true);

	UpdateProject();
}

void ProjectExplorer::UpdateProject()
{
	clear();
	
	auto full_file_list = watcher_->files();
	
}
