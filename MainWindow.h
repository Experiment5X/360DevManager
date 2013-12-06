#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// qt
#include <QMainWindow>
#include <QInputDialog>
#include <QRegExp>
#include <QMessageBox>
#include <QTreeWidget>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include "QtExtensions.h"

// std
#include <memory>

// xbox
#include "Xbdm.h"

// widgets
#include "PropertiesDockWidget.h"
#include "ModulesDockWidget.h"
#include "FileExplorerDockWidget.h"
#include "MemoryRegionsDockWidget.h"

using namespace XBDM;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();

    void on_actionTo_Current_Title_triggered();
    void on_actionTo_XShell_triggered();
    void on_actionCold_triggered();


    void on_actionShow_Properties_triggered();
    void on_actionProperties_close(bool visible);

    void on_actionShow_Modules_triggered();
    void on_actionLoadedModules_close(bool visible);

    void on_actionShow_File_Explorer_triggered();
    void on_actionShow_File_Explorer_close(bool visible);

    void on_actionShow_Memory_Regions_triggered();
    void on_actionShow_Memory_Regions_close(bool visible);

private:
    Ui::MainWindow *ui;

    // why why why are some of these safe pointers while others are not?
    // well... qt will destroy all of the child objects when the parent is
    // destroyed. When all QObjects are created, I pass in a parent to it
    // so the memory will be taken care of. For non-QObjects it's my job
    // to deal with the memory, that's when I use safe pointers
    std::shared_ptr<DevConsole> console;

    ModulesDockWidget *moduleWindow;
    bool moduleWindowIsOpen;

    PropertiesDockWidget *propertiesWindow;
    bool propertiesWindowIsOpen;

    FileExplorerDockWidget *fileWindow;
    bool fileWindowIsOpen;

    MemoryRegionsDockWidget *memoryRegionsWindow;
    bool memoryRegionsWindowIsOpen;

    // im going to change this around and create a class for each of the windows
    void createModulesWindow();
    void createPropertiesWindow();
    void createFileWindow();
    void createMemoryRegionsWindow();
};

#endif // MAINWINDOW_H
