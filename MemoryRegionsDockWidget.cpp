#include "MemoryRegionsDockWidget.h"

using namespace XBDM;
using namespace std;

MemoryRegionsDockWidget::MemoryRegionsDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle("Memory Regions");

    QTreeWidget *lstRegions = new QTreeWidget(this);

    QStringList headerLabels;
    headerLabels << "Address" << "Size" << "Protection";
    lstRegions->setHeaderLabels(headerLabels);

    // load the memory regions from the console
    bool ok;
    auto memoryRegions = console->GetMemoryRegions(ok, true);

    if (!ok)
    {
        QMessageBox::warning(this, "Error", "Couldn't load memory regions from the console.");
        return;
    }

    for (MemoryRegion m : memoryRegions)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lstRegions);
        item->setText(0, qhex(m.baseAddress));
        item->setText(1, qhex(m.size));
        item->setText(2, qs(m.protection));
    }

    setWidget(lstRegions);
}
