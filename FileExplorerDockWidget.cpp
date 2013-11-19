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

    btnBack = new QPushButton(dockWidgetContents);
    btnBack->setIconSize(QSize(24, 24));
    btnBack->setIcon(QIcon(":/images/images/back.png"));
    connect(btnBack, SIGNAL(clicked()), this, SLOT(on_btnBackClicked()));
    horizontalLayout->addWidget(btnBack);

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
    loadVolumesIntoGUI();

    connect(lstFiles, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemDoubleClicked(QTreeWidgetItem*,int)));
}

void FileExplorerDockWidget::on_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // first we need to make sure that this is a folder
    if (!item->data(0, Qt::UserRole).toBool())
        return;

    // if the user clicked on a volume, then stuffs a little bit different
    QString path;
    if (txtPath->text().isEmpty())
        path = item->text(0) + ":\\";
    else
        path = txtPath->text() + item->text(0) + "\\";

    loadDirectoryIntoGUI(path);
}

void FileExplorerDockWidget::on_btnBackClicked()
{
    QString fullPath = txtPath->text();
    QString temp = fullPath.mid(0, fullPath.length() - 2);

    if (temp.lastIndexOf('\\') < 0)
    {
        loadVolumesIntoGUI();
        txtPath->setText("");
    }
    else
        loadDirectoryIntoGUI(temp.mid(0, temp.lastIndexOf('\\') + 1));
}

void FileExplorerDockWidget::loadDirectoryIntoGUI(QString path)
{
    // request the directory contents
    bool ok;
    std::vector<FileEntry> contents = console->GetDirectoryContents(path.toStdString(), ok);

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

        QString fileName = qs(f.name);
        if (f.directory)
            item->setIcon(0, QIcon(":/images/images/folder.png"));
        else if (fileName.mid(fileName.lastIndexOf(".") + 1).toLower() == "xex")
            item->setIcon(0, QIcon(":/images/images/executable.png"));
        else
            item->setIcon(0, QIcon(":/images/images/file.png"));
    }

    txtPath->setText(path);
}

void FileExplorerDockWidget::loadVolumesIntoGUI()
{
    // clear all of the items that are currently in the list
    while (lstFiles->topLevelItemCount() > 0)
        delete lstFiles->topLevelItem(0);

    bool ok;
    for (Drive d : console->GetDrives(ok, true))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstFiles);

        // this will indicate whether or not the item is a drive/folder
        item->setData(0, Qt::UserRole, true);

        item->setIcon(0, QIcon(":/images/images/volume.png"));
        item->setText(0, qs(d.name));
    }
}
