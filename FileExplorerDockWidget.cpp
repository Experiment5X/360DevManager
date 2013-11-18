#include "FileExplorerDockWidget.h"

using namespace XBDM;

FileExplorerDockWidget::FileExplorerDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent) :
    QDockWidget(parent)
{
    this->console = console;

    QWidget *dockWidgetContents = new QWidget(this);
    setWindowTitle("File Explorer");

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setStretch(0, 9);
    horizontalLayout->setStretch(1, 2);

    txtPath = new QLineEdit(dockWidgetContents);
    horizontalLayout->addWidget(txtPath);

    txtSearch = new QLineEdit(dockWidgetContents);
    txtSearch->setPlaceholderText("Search...");
    txtSearch->setMaximumWidth(200);
    horizontalLayout->addWidget(txtSearch);

    verticalLayout->addLayout(horizontalLayout);

    lstFiles = new QTreeWidget(dockWidgetContents);
    lstFiles->setIconSize(QSize(32, 32));
    lstFiles->setColumnWidth(0, 200);

    QStringList headerLabels;
    headerLabels << "Name" << "Size";
    lstFiles->setHeaderLabels(headerLabels);

    verticalLayout->addWidget(lstFiles);
    verticalLayout_2->addLayout(verticalLayout);

    setWidget(dockWidgetContents);

    // now let's load the volumes
    bool ok;
    for (Drive d : console->GetDrives(ok, true))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstFiles);

        // this will indicate whether or not the item is a drive/folder
        item->setData(0, Qt::UserRole, true);

        item->setIcon(0, QIcon(":/images/images/volume.png"));
        item->setText(0, qs(d.name));
    }

    connect(lstFiles, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemDoubleClicked(QTreeWidgetItem*,int)));
}

void FileExplorerDockWidget::on_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // first we need to make sure that this is a folder
    if (!item->data(0, Qt::UserRole).toBool())
        return;

    // if the user clicked on a volume, then stuffs a little bit different
    std::string path;
    if (txtPath->text().isEmpty())
        path = item->text(0).toStdString() + ":\\";
    else
        path = (txtPath->text() + item->text(0)).toStdString() + "\\";

    // request the directory contents
    bool ok;
    std::vector<FileEntry> contents = console->GetDirectoryContents(path, ok);

    // clear all of the items that are currently in the list
    while (lstFiles->topLevelItemCount() > 0)
        delete lstFiles->topLevelItem(0);

    // sort the contents so that the folders are first
    std::sort(contents.begin(), contents.end(), [](FileEntry f1, FileEntry f2) { return f1.directory && !f2.directory; });

    // load all of the files into the GUI
    for (FileEntry f : contents)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstFiles);
        item->setText(0, qs(f.name));

        // this will indicate whether or not the item is a drive/folder
        item->setData(0, Qt::UserRole, f.directory);

        if (f.directory)
            item->setIcon(0, QIcon(":/images/images/folder.png"));
        else
            item->setIcon(0, QIcon(":/images/images/file.png"));
    }

    txtPath->setText(qs(path));
}
