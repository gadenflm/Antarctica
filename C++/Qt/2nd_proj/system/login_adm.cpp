#include "login_adm.h"
#include "ui_login_adm.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <adm.h>
#include "total.h"
#include <QLineEdit>
login_adm::login_adm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_adm)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

login_adm::~login_adm()
{
    delete ui;
}

void login_adm::on_pushButton_cancel_clicked()
{
    this->close();
}

void login_adm::on_pushButton_OK_clicked()
{
    QString username=ui->lineEdit_username->text();
    QString password=ui->lineEdit_password->text();

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select * from admin where username='"+username+"'");
    bool t=false;
    int i=-1;
    while (query.next())
    {
        int no=query.value(0).toInt();
        QString uname=query.value(1).toString();
        QString pword=query.value(2).toString();
        if ((uname==username) && (pword==password) )
        {
            t=true;i=no;
        }
    }
    if (!t)
    {
        QMessageBox::warning (this,QString::fromUtf8("老铁,扎心!"),QString::fromUtf8("用户名或密码错误!"),QMessageBox::Yes);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_username->setFocus();
    }
    else
    {
        adm *admwid= new adm();
        admwid->no_in(i);
        admwid->text_in();
        admwid->setWindowTitle(QString::fromLocal8Bit(""));
        admwid->show();
        this->close();

    }

}
