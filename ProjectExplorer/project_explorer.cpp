#include "project_explorer.h"
#include <QHeaderView>
#include <QMenu>


ProjectExplorer::ProjectExplorer(QWidget* parent)
    : QTreeWidget(parent), watcher_(new QFileSystemWatcher(this)) {
  setHeaderLabel(QStringLiteral("工程文件夹"));
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QTreeWidget::customContextMenuRequested, this,
          &ProjectExplorer::ResolveCustomContextMenuRequest);
}

void ProjectExplorer::ResolveCustomContextMenuRequest(const QPoint& pos) {
    auto item = itemAt(pos);
    if (item == nullptr) {
        QMenu menu;
        menu.addAction(QStringLiteral("新增目录"));
        menu.exec(mapToGlobal(pos));
    }
}