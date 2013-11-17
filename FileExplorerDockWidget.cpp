#include "FileExplorerDockWidget.h"

FileExplorerDockWidget::FileExplorerDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent) :
    QDockWidget(parent)
{
    QWidget *dockWidgetContents = new QWidget();
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

    QLineEdit *txtPath = new QLineEdit(dockWidgetContents);
    horizontalLayout->addWidget(txtPath);

    QLineEdit *txtSearch = new QLineEdit(dockWidgetContents);
    txtSearch->setPlaceholderText("Search...");
    txtSearch->setMaximumWidth(200);
    horizontalLayout->addWidget(txtSearch);

    verticalLayout->addLayout(horizontalLayout);

    QTreeWidget *lstFiles = new QTreeWidget(dockWidgetContents);
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
    for (XBDM::Drive d : console->GetDrives(ok, true))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstFiles);
        item->setIcon(0, QIcon(":/images/images/volume.png"));
        item->setText(0, qs(d.name));
    }
}
