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

// std
#include <memory>

// xbox
#include "Xbdm.h"

using namespace XBDM;

#define qs(s) (QString::fromStdString(s))
#define qhex(n) ("0x" + QString::number(n, 16).toUpper())

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

private:
    Ui::MainWindow *ui;

    // why why why are some of these safe pointers while others are not?
    // well... qt will destroy all of the child objects when the parent is
    // destroyed. When all QObjects are created, I pass in a parent to it
    // so the memory will be taken care of. For non-QObjects it's my job
    // to deal with the memory, that's when I use safe pointers
    std::unique_ptr<DevConsole> console;

    QDockWidget *moduleWindow;
    bool moduleWindowIsOpen;

    QDockWidget *propertiesWindow;
    bool propertiesWindowIsOpen;

    QDockWidget *fileWindow;
    bool fileWindowIsOpen;

    // im going to change this around and create a class for each of the windows
    void createModulesWindow();
    void createPropertiesWindow();
    void createFileWindow();
};

#endif // MAINWINDOW_H
