#include "logindialog.h"
#include "ui_logindialog.h"
#include <QApplication>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    if (ui->usrLineEdit->text().trimmed() == tr("gadenflm") && ui->pwdLineEdit -> text().trimmed() == tr("jdgsb"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning (this,tr("老铁,扎心!"),tr("用户名或密码错误!"),QMessageBox::Yes);
        ui->usrLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->usrLineEdit->setFocus();

     }

}
