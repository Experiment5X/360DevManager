#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(QString name, QPixmap icon, QList<QPair<QString, QString> > properties, QWidget *parent = 0);
    ~PropertiesDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PropertiesDialog *ui;
};

#endif // PROPERTIESDIALOG_H
