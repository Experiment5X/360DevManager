#ifndef FILEEXPLORERDOCKWIDGET_H
#define FILEEXPLORERDOCKWIDGET_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QLineEdit>
#include "QtExtensions.h"

#include "Xbdm.h"

#include <memory>

class FileExplorerDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit FileExplorerDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent = 0);

signals:

public slots:

};

#endif // FILEEXPLORERDOCKWIDGET_H
