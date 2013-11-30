#ifndef FILEEXPLORERDOCKWIDGET_H
#define FILEEXPLORERDOCKWIDGET_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <QDateTime>
#include<QInputDialog>
#include "QtExtensions.h"

#include "PropertiesDialog.h"
#include "Xbdm.h"

#include <memory>

Q_DECLARE_METATYPE(XBDM::FileEntry)
Q_DECLARE_METATYPE(XBDM::Drive)

class FileExplorerDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit FileExplorerDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent = 0);

signals:

private slots:
    void on_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_btnBackClicked();
    void on_directoryEntered();
    void on_contextMenuRequested(QPoint pos);
    void on_search(QString text);

private:

    // widgets
    QTreeWidget *lstFiles;
    QLineEdit *txtPath;
    QLineEdit *txtSearch;
    QPushButton *btnBack;

    std::shared_ptr<XBDM::DevConsole> console;
    QString currentPath;

    void loadDirectoryIntoGUI(QString path);
    void loadVolumesIntoGUI();

    QString sizeToString(UINT64 size);
};

#endif // FILEEXPLORERDOCKWIDGET_H
