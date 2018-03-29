#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //ui->lineEdit->setText("8+/0.0000001");
    //calculator =new Calculator();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString input = ui->lineEdit->text();
    Calculator calculator;
    double result = calculator.Parse(input);
    if (calculator.ExpIsValid() == Calculator::SYNERROR)
    {
        ui->label->setText("Syntax ERROR");
        qWarning("LCDRange::setRange0\n"
                 "\tRange must be 0..99\n"
                 "\tand minVal must not be greater than maxVal"
                 );
    }
    else if (calculator.ExpIsValid() == Calculator::MATHERROR)
    {
        ui->label->setText("Math ERROR");
    }
    else
    {
        ui->label->setText(QString::number(result));
    }
}
