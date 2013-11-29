#include "MainWindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), moduleWindowIsOpen(false), propertiesWindowIsOpen(false), fileWindowIsOpen(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    // get the console's ip address from the user
    QString ip = QInputDialog::getText(this, "Console IP Address", "IP Address:", QLineEdit::Normal, "169.254.66.42", NULL, Qt::WindowCloseButtonHint);

    // make sure it's a valid ip address
    QRegExp exp("\\d+\\.\\d+\\.\\d+\\.\\d+");
    if (ip == "")
    {
        return;
    }
    else if (!exp.exactMatch(ip))
    {
        QMessageBox::warning(this, "Invalid IP", "The IP address entered is not valid.");
        return;
    }


    // if it's valid, then try and connect to the console
    console = std::make_shared<DevConsole>(ip.toStdString());
    if (!console->OpenConnection())
    {
        QMessageBox::warning(this, "Error", "Could not establish a connection with the console " + ip);
        return;
    }

    // enable all of the controls
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    ui->actionShow_Modules->setEnabled(true);
    ui->actionShow_Properties->setEnabled(true);
    ui->actionShow_File_Explorer->setEnabled(true);
    ui->menuReboot->setEnabled(true);

    createPropertiesWindow();
    createFileWindow();
}

void MainWindow::on_actionShow_Modules_triggered()
{
    if (moduleWindowIsOpen)
    {
        // this will "close" it
        if (moduleWindow != nullptr)
        {
            delete moduleWindow;
            moduleWindow = nullptr;
        }

        moduleWindowIsOpen = false;
        return;
    }

    createModulesWindow();
}

void MainWindow::on_actionLoadedModules_close(bool visible)
{
    if (!visible)
    {
        ui->actionShow_Modules->setChecked(false);
        moduleWindowIsOpen = false;
    }
}

void MainWindow::on_actionTo_Current_Title_triggered()
{
    console->RebootToCurrentTitle();
}

void MainWindow::on_actionTo_XShell_triggered()
{
    console->RebootToXShell();
}

void MainWindow::on_actionCold_triggered()
{
    console->ColdReboot();
}

void MainWindow::createPropertiesWindow()
{
    propertiesWindow = new PropertiesDockWidget(console, this);

    propertiesWindowIsOpen = true;
    addDockWidget(static_cast<Qt::DockWidgetArea>(1), propertiesWindow);

    connect(propertiesWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(on_actionProperties_close(bool)));
}

void MainWindow::createFileWindow()
{
    fileWindow = new FileExplorerDockWidget(console, this);

    fileWindowIsOpen = true;
    addDockWidget(static_cast<Qt::DockWidgetArea>(2), fileWindow);

    connect(fileWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(on_actionShow_File_Explorer_close(bool)));
}

void MainWindow::on_actionShow_Properties_triggered()
{
    if (propertiesWindowIsOpen)
    {
        // this will "close" it
        if (propertiesWindow != nullptr)
        {
            delete propertiesWindow;
            propertiesWindow = nullptr;
        }

        propertiesWindow = nullptr;
        propertiesWindowIsOpen = false;
        return;
    }

    createPropertiesWindow();
}

void MainWindow::on_actionProperties_close(bool visible)
{
    if (!visible)
    {
        ui->actionShow_Properties->setChecked(false);
        propertiesWindowIsOpen = false;
    }
}

void MainWindow::createModulesWindow()
{  
    moduleWindow = new ModulesDockWidget(console, this);

    addDockWidget(Qt::BottomDockWidgetArea, moduleWindow);
    moduleWindowIsOpen = true;

    connect(moduleWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(on_actionLoadedModules_close(bool)));
}

void MainWindow::on_actionShow_File_Explorer_triggered()
{
    if (fileWindowIsOpen)
    {
        // this will "close" it
        if (fileWindow != nullptr)
        {
            delete fileWindow;
            fileWindow = nullptr;
        }

        fileWindow = nullptr;
        fileWindowIsOpen = false;
        return;
    }

    createFileWindow();
}

void MainWindow::on_actionShow_File_Explorer_close(bool visible)
{
    if (!visible)
    {
        ui->actionShow_File_Explorer->setChecked(false);
        fileWindowIsOpen = false;
    }
}
