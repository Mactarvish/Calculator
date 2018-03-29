#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "calculator.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    //Calculator* calculator;

private slots:
    void on_pushButton_clicked();

};

#endif // DIALOG_H
