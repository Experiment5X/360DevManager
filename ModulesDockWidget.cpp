#include "ModulesDockWidget.h"

ModulesDockWidget::ModulesDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle("Modules");

    QTreeWidget *moduleTree = new QTreeWidget(this);

    QStringList headerLabels;
    headerLabels << "Name" << "Base Address" << "Size" << "Data Address" << "Data Size" << "Thread ID";
    moduleTree->setHeaderLabels(headerLabels);

    bool ok;
    for (const XBDM::Module &m : console->GetLoadedModules(ok, true))
    {
        QTreeWidgetItem *moduleItem = new QTreeWidgetItem(moduleTree);
        moduleItem->setText(0, qs(m.name));
        moduleItem->setText(1, qhex(m.baseAddress));
        moduleItem->setText(2, qhex(m.size));
        moduleItem->setText(3, qhex(m.dataAddress));
        moduleItem->setText(4, qhex(m.dataSize));
        moduleItem->setText(5, qhex(m.threadId));

        // load all of the module sections into the GUI
        for (const XBDM::ModuleSection &sect : m.sections)
        {
            QTreeWidgetItem *sectionItem = new QTreeWidgetItem(moduleItem);
            sectionItem->setText(0, qs(sect.name));
            sectionItem->setText(1, qhex(sect.baseAddress));
            sectionItem->setText(2, qhex(sect.size));
        }
    }

    setWidget(moduleTree);
}
