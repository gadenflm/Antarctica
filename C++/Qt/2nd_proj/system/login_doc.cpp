#include "login_doc.h"
#include "ui_login_doc.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "total.h"
#include "zhenduan.h"

login_doc::login_doc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login_doc)
{
    ui->setupUi(this);
}

login_doc::~login_doc()
{
    delete ui;
}

void login_doc::on_pushButton_clicked()
{
    QString work_no_str=ui->lineEdit->text();
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No from doctor where work_no='"+work_no_str+"'");
    bool t=false;
    int i=-1;
    while (query.next())
    {
        int no=query.value(0).toInt();
        t=true;
        i=no;
    }
    if (!t)
    {
        QMessageBox::warning (this,QString::fromUtf8("老铁,扎心!"),QString::fromUtf8("工号不存在!"),QMessageBox::Yes);
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
    else
    {
        zhenduan *zdwid=new zhenduan();
        zdwid->no_in(i);
        zdwid->list_refresh();
        zdwid->setWindowTitle("");
        zdwid->show();
        this->close();
    }
}
