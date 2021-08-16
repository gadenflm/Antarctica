#include "class_add.h"
#include "ui_class_add.h"
#include "QDebug"
#include "QFile"
#include "total.h"
#include "QMessageBox"
class_add::class_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::class_add)
{

    ui->setupUi(this);
     ui->spinBox_i->setRange(1,7);
     ui->spinBox_j->setRange(1,5);
}

class_add::~class_add()
{
    delete ui;
}

void class_add::on_cancel_button_clicked()
{
    this->close();
}

void class_add::on_ok_button_clicked()
{
    int i=ui->spinBox_i->value();
    int j=ui->spinBox_j->value();
    QString classname=ui->lineEdit->text();
    if (i && j && (classname!=""))
    {
        int cnum=(i-1)*5+j;
        num_class++;\
        num_out();
        QFile file ("class");
        file.open (QIODevice::ReadWrite | QIODevice::Append);
        QTextStream io(&file);
        io  << classname << "+" << cnum << "+" << num_class <<"\r\n"  ;
        file.close();
        this->close();
    }
    else
    {
        QMessageBox::warning (this,tr("老铁,扎心!"),tr("信息不完全!"),QMessageBox::Yes);
    }
}
