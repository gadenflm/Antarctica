#include "addadm_wid.h"
#include "ui_addadm_wid.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "total.h"
addadm_wid::addadm_wid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addadm_wid)
{
    ui->setupUi(this);
}

addadm_wid::~addadm_wid()
{
    delete ui;
}

void addadm_wid::on_pushButton_OK_clicked()
{
    QString uname=ui->lineEdit_user_name->text();
    QString pword=ui->lineEdit_password->text();
    num_administrator++;
    num_out();
    QString no_str=QString::number(num_administrator,10);

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    QString str="insert into admin values ("+no_str+",'"+uname+"','"+pword+"')";
    query.exec(str);
    this->close();
}
