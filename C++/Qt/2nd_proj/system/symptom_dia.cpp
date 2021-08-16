#include "symptom_dia.h"
#include "ui_symptom_dia.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "total.h"
#include <QDate>
#include <QDateTime>
#include "zhenduan.h"

void symptom_dia::no_in(int a,int b)
{
    pat_no=a;
    doc_no=b;
    return;
}

void symptom_dia::text_in()
{
    QString no_str=QString::number(pat_no,10);
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No,name,symptom,medicine,isfuzhen from patient where No="+no_str);
    QString pat_name;

    int int_isfuzhen;
    while (query.next())
    {
         pat_name=query.value(1).toString();
         pat_symptom=query.value(2).toString();
         pat_medicine=query.value(3).toString();
         int_isfuzhen=query.value(4).toInt();
    }
    ui->label_name->setText(pat_name);
    if (int_isfuzhen==0) pat_is_fuzhen=false; else pat_is_fuzhen=true;

    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("\nyyyy-MM-dd hh:mm:ss\n");

    if (pat_is_fuzhen)
    {
        pat_symptom.append(strBuffer);
        pat_medicine.append(strBuffer);
    }
    else
    {
        pat_symptom=strBuffer;
        pat_medicine=strBuffer;
    }
    ui->textEdit_symptom->setText(pat_symptom);
    ui->textEdit_medicine->setText(pat_medicine);

}

symptom_dia::symptom_dia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::symptom_dia)
{
    ui->setupUi(this);
}

symptom_dia::~symptom_dia()
{
    delete ui;
}

void symptom_dia::on_pushButton_cancel_clicked()
{
    this->close();
}

void symptom_dia::on_pushButton_OK_clicked()
{
    QString no_str=QString::number(pat_no,10);
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);

    pat_symptom=ui->textEdit_symptom->toPlainText();
    pat_medicine=ui->textEdit_medicine->toPlainText();
    QString str="UPDATE patient SET symptom='"+pat_symptom+"',medicine='"+pat_medicine+"',zdg=1 where No="+no_str;
    query.exec(str);

    zhenduan *zdwid=new zhenduan();
    zdwid->no_in(doc_no);
    zdwid->list_refresh();
    zdwid->setWindowTitle("");
    zdwid->show();
    this->close();

    return;

}
