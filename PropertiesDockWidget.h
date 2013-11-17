#ifndef PROPERTIESDOCKWIDGET_H
#define PROPERTIESDOCKWIDGET_H

#include <QDockWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "QtExtensions.h"

#include "Xbdm.h"

#include <memory>

class PropertiesDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit PropertiesDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent = 0);

signals:

public slots:

private:

};

#endif // PROPERTIESDOCKWIDGET_H
