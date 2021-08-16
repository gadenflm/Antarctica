#include "inforpat_wid.h"
#include "ui_inforpat_wid.h"
#include "total.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "patient.h"
#include <QDebug>

void inforpat_wid::no_in(int a)
{
    pat_no=a;
    return;
}

void inforpat_wid::text_in()
{

    QString no_str=QString::number(pat_no,10);
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No,name,id_no,sex,tel,birth,age,keshi,symptom,medicine from patient where No="+no_str);

    while (query.next())
    {
        pat.no=query.value(0).toInt();
        pat.name=query.value(1).toString();
        pat.id_no=query.value(2).toString();
        pat.sex=query.value(3).toString();
        pat.tel=query.value(4).toString();
        pat.birth=query.value(5).toString();
        pat.age=query.value(6).toString();
        pat.keshi=query.value(7).toString();
        pat.symptom=query.value(8).toString();
        pat.medicine=query.value(9).toString();
    }
    ui->label_name->setText(pat.name);
    ui->label_id_no->setText(pat.id_no);
    ui->label_sex->setText(pat.sex);
    ui->label_tel->setText(pat.tel);
    ui->label_birth->setText(pat.birth);
    ui->label_age->setText(pat.age);
    ui->label_keshi->setText(pat.keshi);
    ui->textBrowser_symptom->setText(pat.symptom);
    ui->textBrowser_medicine->setText(pat.medicine);

}

inforpat_wid::inforpat_wid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inforpat_wid)
{
    ui->setupUi(this);

}

inforpat_wid::~inforpat_wid()
{
    delete ui;
}

void inforpat_wid::on_pushButton_2_clicked()
{
    this->close();
}

void inforpat_wid::on_pushButton_clicked()
{
    this->close();
}
