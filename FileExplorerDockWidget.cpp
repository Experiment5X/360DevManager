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
    connect(btnBack, SIGNAL(clicked()), this, SLOT(on_btnBackClicked()));
    horizontalLayout->addWidget(btnBack);

    txtPath = new QLineEdit(dockWidgetContents);

    btnBack->setMaximumHeight(txtPath->height());
#ifdef __WIN32
    btnBack->setIconSize(btnBack->height());
#else
    btnBack->setIconSize(QSize(16, 16));
#endif
    btnBack->setIcon(QIcon(":/images/images/back.png"));

    connect(txtPath, SIGNAL(returnPressed()), this, SLOT(on_directoryEntered()));
    horizontalLayout->addWidget(txtPath);

    txtSearch = new QLineEdit(dockWidgetContents);
    txtSearch->setPlaceholderText("Search...");
    txtSearch->setMaximumWidth(200);
    connect(txtSearch, SIGNAL(textChanged(QString)), this, SLOT(on_search(QString)));
    horizontalLayout->addWidget(txtSearch);

    verticalLayout->addLayout(horizontalLayout);

    lstFiles = new QTreeWidget(dockWidgetContents);
    lstFiles->setIconSize(QSize(32, 32));
    lstFiles->setColumnWidth(0, 200);
    lstFiles->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lstFiles, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_contextMenuRequested(QPoint)));

    QStringList headerLabels;
    headerLabels << "Name";
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
    if (item->data(0, Qt::UserRole).toBool() || item->data(1, Qt::UserRole).value<FileEntry>().directory)
    {
        // if the user clicked on a volume, then stuffs a little bit different
        QString path;
        if (currentPath.isEmpty())
            path = qs(item->data(1, Qt::UserRole).value<Drive>().name) + ":\\";
        else
            path = currentPath + item->text(0) + "\\";

        loadDirectoryIntoGUI(path);
    }
    else
    {
        QString fileName;
        if (item->data(0, Qt::UserRole).toBool())
            qs(item->data(1, Qt::UserRole).value<Drive>().name);
        else
            fileName = qs(item->data(1, Qt::UserRole).value<FileEntry>().name);

        // check if it's an exectuable
        if (fileName.mid(fileName.lastIndexOf(".") + 1).toLower() == "xex")
        {
            console->LaunchXEX((currentPath + item->text(0)).toStdString());
        }
    }
}

void FileExplorerDockWidget::on_btnBackClicked()
{
    QString fullPath = currentPath;
    QString temp = fullPath.mid(0, fullPath.length() - 2);

    if (temp.lastIndexOf('\\') < 0)
    {
        loadVolumesIntoGUI();
        currentPath = "";
        txtPath->setText("");
    }
    else
        loadDirectoryIntoGUI(temp.mid(0, temp.lastIndexOf('\\') + 1));
}

void FileExplorerDockWidget::on_directoryEntered()
{
    if (txtPath->text() == "")
        loadVolumesIntoGUI();
    else
        loadDirectoryIntoGUI(txtPath->text());
}

void FileExplorerDockWidget::on_contextMenuRequested(QPoint pos)
{
    QMenu contextMenu;

    // get the selected files
    // the options displayed will differ based on if one is selected or not
    QTreeWidgetItem *selectedDirent = ((lstFiles->selectedItems().size() == 0) ? nullptr : lstFiles->selectedItems().at(0));

    // if we're in the root, then we can't create a folder
    if (currentPath != "")
    {
        contextMenu.addAction(QPixmap(":/images/images/folder.png"), "New Folder");
        contextMenu.addAction(QPixmap(":/images/images/upload.png"), "Send File Here");
    }

    // make sure an item is selected
    if (selectedDirent != nullptr)
    {    
        // make sure it's not a folder
        if (!selectedDirent->data(1, Qt::UserRole).value<FileEntry>().directory)
        {
            // check if it's an exectuable
            QString fileName = qs(selectedDirent->data(1, Qt::UserRole).value<FileEntry>().name);
            if (fileName.mid(fileName.lastIndexOf(".") + 1).toLower() == "xex")
                contextMenu.addAction("Launch");
        }

        // we can't modify volumes
        if (currentPath != "")
        {
            contextMenu.addAction("Delete");
            contextMenu.addAction("Rename");
        }

        contextMenu.addAction(QPixmap(":/images/images/download.png"), "Transfer to PC");
        contextMenu.addAction("Properties");
    }

    // if there aren't any items in the menu, then don't show it
    if (contextMenu.actions().size() == 0)
        return;

    // show the context menu
    QAction *selectedItem = contextMenu.exec(lstFiles->mapToGlobal(pos));
    if (selectedItem == nullptr)
        return;

    if (selectedItem->text() == "Transfer to PC")
    {
        // we need to display a different dialog and call different functions for folders and files
        if (selectedDirent->data(1, Qt::UserRole).value<FileEntry>().directory)
        {
            // get a location to save the folder to
            QString saveLocation = QFileDialog::getExistingDirectory(this, "Pick a location to transfer the folder", DESKTOP_LOCATION);
            if (saveLocation.isEmpty())
                return;

            bool ok;
            console->ReceiveDirectory(currentPath.toStdString() + selectedDirent->data(1, Qt::UserRole).value<FileEntry>().name + "\\", saveLocation.toStdString() + "/", ok);

            if (!ok)
                QMessageBox::warning(this, "Error", "Couldn't transfer directory from devkit.");
        }
        else
        {
            // get a location to save the file to
            QString savePath = QFileDialog::getSaveFileName(this, "Pick a location to transfer the file to", DESKTOP_LOCATION + selectedDirent->text(0));
            if (savePath.isEmpty())
                return;

            bool ok;
            console->ReceiveFile((currentPath + selectedDirent->text(0)).toStdString(), savePath.toStdString(), ok);

            if (!ok)
                QMessageBox::warning(this, "Error", "Couldn't transfer file from devkit.");
        }
    }
    else if (selectedItem->text() == "Launch")
    {
        console->LaunchXEX((currentPath + selectedDirent->text(0)).toStdString());
    }
    else if (selectedItem->text() == "Properties")
    {
        // get a list of properties to show in the properties window
        QList<QPair<QString, QString> > properties;
        QString name;

        // we need to load different properties for volumes than folders and files
        if (!selectedDirent->data(0, Qt::UserRole).toBool())
        {
            FileEntry dirent = selectedDirent->data(1, Qt::UserRole).value<FileEntry>();

            name = qs(dirent.name);
            properties.push_back(QPair<QString, QString>("Path", currentPath));
            properties.push_back(QPair<QString, QString>("Size", ((dirent.directory) ? "N/A" : sizeToString(dirent.size))));
            properties.push_back(QPair<QString, QString>("Created", QDateTime::fromTime_t(dirent.creationTime).toString()));
            properties.push_back(QPair<QString, QString>("Modified", QDateTime::fromTime_t(dirent.modifiedTime).toString()));
        }
        else
        {
            Drive drive = selectedDirent->data(1, Qt::UserRole).value<Drive>();

            name = qs(drive.name);
            properties.push_back(QPair<QString, QString>("Free Space", sizeToString(drive.totalFreeBytes)));
            properties.push_back(QPair<QString, QString>("Total Space", sizeToString(drive.totalBytes)));
            properties.push_back(QPair<QString, QString>("Percentage Free", QString::number(drive.totalFreeBytes / (double)drive.totalBytes * 100, 'g', 2) + "%"));
        }

        PropertiesDialog *dialog = new PropertiesDialog(name, selectedDirent->icon(0).pixmap(32, 32), properties, this);
        dialog->exec();
    }
    else if (selectedItem->text() == "New Folder")
    {
        // get a valid name for a folder
        int maxFolder = 1;
        for (int i = 0; i < lstFiles->topLevelItemCount(); i++)
        {
            FileEntry dirent = lstFiles->topLevelItem(i)->data(1, Qt::UserRole).value<FileEntry>();
            if (!dirent.directory)
                continue;

            QRegExp folderRegex("New Folder (\\d+)", Qt::CaseInsensitive);
            if (folderRegex.indexIn(qs(dirent.name)) >= 0)
            {
                // get the folder number, so if the text was New Folder 5
                // it would get the 5
                int currentFolderNum = folderRegex.cap(1).toInt();

                // this looks like it shouldn't work, but...
                // the folders are going to be in sorted order, so it's all good
                if (currentFolderNum == maxFolder)
                    maxFolder++;
            }

        }

        QString folderName = QInputDialog::getText(this, "New Folder", "Folder Name", QLineEdit::Normal,
                                                   "New Folder " + QString::number(maxFolder), NULL, Qt::WindowCloseButtonHint);

        if (folderName.isEmpty())
            return;

        // tell the console to create a directory
        bool ok;
        console->MakeDirectory((currentPath + folderName).toStdString(), ok);

        // reload the current directory, so the new one is put in there
        loadDirectoryIntoGUI(currentPath);
    }
    else if (selectedItem->text() == "Delete")
    {
        FileEntry dirent = selectedDirent->data(1, Qt::UserRole).value<FileEntry>();

        // tell the console to delete the dirent
        bool ok;
        console->DeleteDirent(currentPath.toStdString() + dirent.name, dirent.directory, ok);

        // reload the current directory so the deleted dirent isn't there anymore
        loadDirectoryIntoGUI(currentPath);
    }
    else if (selectedItem->text() == "Rename")
    {
        QString newName = QInputDialog::getText(this, "Rename", "New Name", QLineEdit::Normal, selectedDirent->text(0), NULL, Qt::WindowCloseButtonHint);
        if (newName.isEmpty())
            return;

        // make sure that the name hasn't already been used
        for (int i = 0; i < lstFiles->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *item = lstFiles->topLevelItem(i);
            if (item->text(0).toLower() == newName.toLower())
            {
                QMessageBox::warning(this, "Rename Error", "The name " + newName + " is already in use.");
                return;
            }
        }

        // tell the console to rename the file
        bool ok;
        console->RenameFile((currentPath + selectedDirent->text(0)).toStdString(), (currentPath + newName).toStdString(), ok);

        // report any errors
        if (!ok)
        {
            FileEntry dirent = selectedDirent->data(1, Qt::UserRole).value<FileEntry>();

            QMessageBox::warning(this, "Rename Error", QString("The console could not rename the ") + ((dirent.directory) ? "folder" : "file") + ".");
            return;
        }

        // update the GUI
        loadDirectoryIntoGUI(currentPath);
    }
    else if (selectedItem->text() == "Send File Here")
    {
        QString fileToSend = QFileDialog::getOpenFileName(this, "Choose a file to send...", DESKTOP_LOCATION);
        if (fileToSend.isEmpty())
            return;

        // send the file to the console
        bool ok;
        QFileInfo fileInfo(fileToSend);
        console->SendFile(fileToSend.toStdString(), (currentPath + fileInfo.fileName()).toStdString(), ok);

        // reload the GUI
        loadDirectoryIntoGUI(currentPath);
    }
}

void FileExplorerDockWidget::on_search(QString text)
{
    for (int i = 0; i < lstFiles->topLevelItemCount(); i++)
    {
        // if the name contains the text, ignroing case, then display it
        if (!lstFiles->topLevelItem(i)->text(0).toLower().contains(text.toLower()))
            lstFiles->topLevelItem(i)->setHidden(true);
        else
            lstFiles->topLevelItem(i)->setHidden(false);
    }
}

void FileExplorerDockWidget::loadDirectoryIntoGUI(QString path)
{
    // request the directory contents
    bool ok;
    std::vector<FileEntry> contents = console->GetDirectoryContents(path.toStdString(), ok);

    if (!ok)
    {
        QMessageBox::warning(this, "Warning", "Couldn't load directory listing.\n");
        return;
    }

    // clear all of the items that are currently in the list
    while (lstFiles->topLevelItemCount() > 0)
        delete lstFiles->topLevelItem(0);

    // sort the contents so that the folders are first, and make it alpah betical too
    std::sort(contents.begin(), contents.end(), [](FileEntry f1, FileEntry f2)
              {
                    // if they are both files, or both directories then sort
                    // them alpabetically, and ignoring case
                    if (f1.directory == f2.directory)
                    {
                        // snagged this from stack overflow, written by LB--
                        return lexicographical_compare(f1.name.begin(), f1.name.end(), f2.name.begin(), f2.name.end(),
                            [](std::string::value_type a, std::string::value_type b)
                            {
                                return std::tolower(a) < std::tolower(b);
                            });
                    }
                    // if the first one is a directory then it must come first
                    else if (f1.directory && !f2.directory)
                        return true;
                    // if the second one is, but the first isn't, then f2 must come first
                    else if (f2.directory)
                        return false;
              });

    // load all of the files into the GUI
    for (FileEntry f : contents)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstFiles);
        item->setText(0, qs(f.name));

        // this will indicate whether the item is a volume or not
        item->setData(0, Qt::UserRole, false);
        item->setData(1, Qt::UserRole, QVariant::fromValue(f));

        QString fileName = qs(f.name);
        if (f.directory)
            item->setIcon(0, QIcon(":/images/images/folder.png"));
        else if (fileName.mid(fileName.lastIndexOf(".") + 1).toLower() == "xex")
            item->setIcon(0, QIcon(":/images/images/executable.png"));
        else
            item->setIcon(0, QIcon(":/images/images/file.png"));
    }

    currentPath = path;
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

        // this will indicate whether the item is a volume or not
        item->setData(0, Qt::UserRole, true);
        item->setData(1, Qt::UserRole, QVariant::fromValue(d));


        item->setIcon(0, QIcon(":/images/images/volume.png"));
        item->setText(0, qs(d.friendlyName));
    }

    currentPath = "";
}

QString FileExplorerDockWidget::sizeToString(UINT64 size)
{
    if (size < 0x1000)
        return QString::number(size) + " bytes";
    else if (size < 1048576)
        return QString::number(size / 1024.0, 'g', 3) + " KB";
    else if (size < 1073741824)
        return QString::number(size / 1048576.0, 'g', 3) + " MB";
    else
        return QString::number(size / 1073741824.0, 'g', 3) + " GB";
}
