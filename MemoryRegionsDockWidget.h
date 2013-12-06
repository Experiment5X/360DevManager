#ifndef MEMORYREGIONSDOCKWIDGET_H
#define MEMORYREGIONSDOCKWIDGET_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QMessageBox>
#include "QtExtensions.h"

#include "Xbdm.h"

#include <memory>

class MemoryRegionsDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    MemoryRegionsDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent = 0);
};

#endif // MEMORYREGIONSDOCKWIDGET_H
