#pragma once

#include "../pc_widgets_global.h"
#include <QFileSystemWatcher>
#include <QTreeWidget>
#include <map>


class PC_WIDGETS_EXPORT ProjectExplorer : public QTreeWidget
{
    Q_OBJECT

public:
    ProjectExplorer(const QString& root_directory, QWidget* parent = nullptr);

private:
    QFileSystemWatcher m_fsw;
    std::map<QString, QTreeWidgetItem*> m_dictPath2Item;

private slots:
    void RootDirectoryChanged(const QString& root_directory);
};