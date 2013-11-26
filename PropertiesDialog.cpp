#include "PropertiesDialog.h"
#include "ui_PropertiesDialog.h"

PropertiesDialog::PropertiesDialog(QString name, QPixmap icon, QList<QPair<QString, QString> > properties, QWidget *parent) :
    QDialog(parent), ui(new Ui::PropertiesDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    // setput the UI
    ui->imgIcon->setPixmap(icon);
    ui->txtName->setText(name);

    // load all the properties
    for (auto property : properties)
    {
        QLabel *lblName = new QLabel(property.first + ": ");
        QLabel *lblValue = new QLabel(property.second);

        ui->nameColumn1Layout->addWidget(lblName);
        ui->valueColumn1Layout->addWidget(lblValue);
    }

    setWindowTitle(name + " Properties");
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}
