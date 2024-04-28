#include "project_explorer.h"
#include <QDebug>
#include <QDirIterator>
#include <queue>

ProjectExplorer::ProjectExplorer(const QString& root_directory, QWidget* parent) : QTreeWidget(parent)
{
    auto root_info = QFileInfo(root_directory);
    if (!root_info.isDir())
    {
        qWarning() << "根节点不为文件夹";
        return;
    }

    auto root_dir = root_info.dir();
    auto root_name = root_dir.dirName();
    setHeaderLabel(root_name);

    m_fsw.addPath(root_directory);
    // 初始化时需要将所有文件加入树中
    auto root_absolute_path = root_dir.absolutePath();
    RootDirectoryChanged(root_absolute_path);
}

void ProjectExplorer::RootDirectoryChanged(const QString& root_directory)
{
    std::queue<QString> entry_queue;
    entry_queue.push(root_directory);

    m_dictPath2Item.clear();
    m_dictPath2Item[root_directory] = nullptr;

    while (!entry_queue.empty())
    {
        auto path = entry_queue.front();
        entry_queue.pop();

        QFileInfo info(path);
        if (info.isFile())
        {
            // 文件为文件树上的叶子节点
            auto parent_dir = info.dir();
            auto parent_dirpath = parent_dir.absolutePath();
            if (!m_dictPath2Item.count(parent_dirpath))
            {
                qWarning() << "找不到文件" << info.absoluteFilePath() << "的目录节点";
                continue;
            }
            auto* parent_item = m_dictPath2Item[parent_dirpath];
            auto entry_name = info.fileName();
            auto* entry_item = new QTreeWidgetItem({entry_name});
            entry_item->setIcon(0, QIcon(":/resources/document.svg"));
            if (parent_item == nullptr)
            {
                // 根目录下的文件，直接加入树中
                addTopLevelItem(entry_item);
            }
            else
            {
                parent_item->addChild(entry_item);
            }
        }
        else if (info.isDir())
        {
            // 遍历文件夹下的所有文件与文件夹，并加入到队列中进行循环判断
            QDir dir(info.absoluteFilePath());

            auto entry_list = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
            for (const auto& entry : entry_list)
            {
                auto entry_path = dir.filePath(entry);
                entry_queue.push(entry_path);
            }

            auto dirpath = dir.absolutePath();
            auto dirname = dir.dirName();

            dir.cdUp();
            auto parent_dirpath = dir.absolutePath();

            if (!m_dictPath2Item.count(parent_dirpath))
            {
                continue;
            }

            auto* parent_item = m_dictPath2Item[parent_dirpath];
            auto* entry_item = new QTreeWidgetItem({dirname});
            entry_item->setIcon(0, QIcon(":/resources/folder-filled.svg"));
            m_dictPath2Item[dirpath] = entry_item;

            if (parent_item == nullptr)
            {
                addTopLevelItem(entry_item);
            }
            else
            {
                parent_item->addChild(entry_item);
            }
        }
    }
}