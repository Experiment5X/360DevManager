#ifndef MODULESDOCKWIDGET_H
#define MODULESDOCKWIDGET_H

#include <QDockWidget>
#include <QTreeWidget>
#include "QtExtensions.h"

#include "Xbdm.h"

#include <memory>

class ModulesDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    ModulesDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent = 0);
};

#endif // MODULESDOCKWIDGET_H
