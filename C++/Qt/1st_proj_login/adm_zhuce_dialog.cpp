#include "adm_zhuce_dialog.h"
#include "ui_adm_zhuce_dialog.h"
#include <total.h>
#include <QFile>
#include <QMessageBox>

adm_zhuce_dialog::adm_zhuce_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adm_zhuce_dialog)
{
    ui->setupUi(this);
}

adm_zhuce_dialog::~adm_zhuce_dialog()
{
    delete ui;
}

void adm_zhuce_dialog::on_pushButton_cancel_clicked()
{
    this->close();
}

void adm_zhuce_dialog::on_pushButton_ok_clicked()
{
    QString username=ui->lineEdit_username->text();
    QString password=ui->lineEdit_password->text();
    if( (username!="")&&(password!="") )
    {
        num_administrator++;

        num_out();
        QFile user ("user");
        user.open (QIODevice::ReadWrite | QIODevice::Append);
        QTextStream io(&user);
        io  << username << "+" << password << "+adm" << num_administrator <<"\r\n"  ;
        user.close();
        this->close();
    }
    else
    {
        QMessageBox::warning (this,tr("老铁,扎心!"),tr("没填完!重填!"),QMessageBox::Yes);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_username->setFocus();
    }

}
