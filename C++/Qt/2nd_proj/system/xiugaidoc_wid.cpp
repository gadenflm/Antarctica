#include "xiugaidoc_wid.h"
#include "ui_xiugaidoc_wid.h"
#include "total.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <doctor.h>
#include "doc_information.h"
#include <QDebug>

void xiugaidoc_wid::i_in(int a,bool t)
{
    i=a;
    islock=t;

}

void xiugaidoc_wid::text_in()
{
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    QString i_str=QString::number(i,10);
    query.exec("select * from doctor where No="+i_str);

    qDebug () << i_str;
    while (query.next())
    {
        doc.no=i;
        doc.name=query.value(1).toString();
        doc.work_no=query.value(2).toString();
        doc.keshi=query.value(3).toString();
        doc.id_no=query.value(4).toString();
        doc.tel=query.value(5).toString();
        doc.email=query.value(6).toString();
    }


    ui->lineEdit_name->setText(doc.name);
    ui->lineEdit_work_no->setText(doc.work_no);
    ui->lineEdit_idno->setText(doc.id_no);
    ui->lineEdit_tel->setText(doc.tel);
    ui->lineEdit_email->setText(doc.email);
    ui->comboBox_keshi->setCurrentText(doc.keshi);

}

void xiugaidoc_wid::text_lock()
{
    ui->lineEdit_name->setEnabled(islock);
    ui->lineEdit_work_no->setEnabled(islock);
    ui->lineEdit_idno->setEnabled(islock);
    ui->lineEdit_tel->setEnabled(islock);
    ui->lineEdit_email->setEnabled(islock);
    ui->comboBox_keshi->setEnabled(islock);
}

xiugaidoc_wid::xiugaidoc_wid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::xiugaidoc_wid)
{
    ui->setupUi(this);
    text_in();
    text_lock();
}

xiugaidoc_wid::~xiugaidoc_wid()
{
    delete ui;
}

void xiugaidoc_wid::on_pushButton_cancel_clicked()
{
    doc_information *docwid=new doc_information();
    docwid->setWindowTitle("");
    docwid->show();

   this->close();
}

void xiugaidoc_wid::on_pushButton_OK_clicked()
{
    QString name=ui->lineEdit_name->text();
    QString work_no=ui->lineEdit_work_no->text();
    QString keshi=ui->comboBox_keshi->currentText();
    QString id_no=ui->lineEdit_idno->text();
    QString tel=ui->lineEdit_tel->text();
    QString email=ui->lineEdit_email->text();

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    QString i_str=QString::number(i,10);
    QString str="UPDATE doctor SET name='"+name+"',work_no='"+work_no+"',keshi='"+keshi+"',id_no='"+id_no+"',tel='"+tel+"',email='"+email+"' where No="+i_str;
    query.exec(str);

    doc_information *docwid=new doc_information();
    docwid->setWindowTitle("");
    docwid->show();

    this->close();
}
