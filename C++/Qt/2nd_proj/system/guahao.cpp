#include "guahao.h"
#include "total.h"
#include "ui_guahao.h"
#include <patient.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include "QMessageBox"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLabel>
guahao::guahao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::guahao)
{
    ui->setupUi(this);
    ui->dateEdit_date->setDate(QDate::currentDate());
    QPixmap pixmap2("D:/sb/Program/system/guahao-1.png");
    QPixmap pixmap=pixmap2.scaled(591,91);
    ui->pic_1->setPixmap(pixmap);
}

guahao::~guahao()
{
    delete ui;
}


void guahao::on_OKButton_clicked()
{
    patient newpat;
    num_patient++;
    num_out();

    newpat.no=num_patient;
    newpat.name=ui->lineEdit_name->text();
    newpat.id_no=ui->lineEdit_id_no->text();
    newpat.tel=ui->lineEdit_tel->text();
    newpat.sex=ui->comboBox_sex->currentText();
    newpat.age=ui->lineEdit_age->text();
    newpat.zd_date=ui->dateEdit_date->date().toString();
    newpat.birth=ui->dateEdit_birth->date().toString();
    newpat.keshi=ui->comboBox_administrative->currentText();
    if (ui->radioButton_chuzhen->isChecked()) newpat.isfuzhen=false; else newpat.isfuzhen=true;
    if (ui->radioButton_fuzhen->isChecked()) newpat.isfuzhen=true; else newpat.isfuzhen=false;
    newpat.zdg=false;

    if (!newpat.isfuzhen)
    {
        newpat.symptom="病人还未诊断";
        newpat.medicine="并无药房";

        newpat.pat_in();
    }
    else
    {
        if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
        QSqlDatabase db=QSqlDatabase::database("admin");
        QSqlQuery query(db);
        num_patient--;
        num_out();
        QString str="UPDATE patient SET keshi='"+newpat.keshi+"',zd_date='"+newpat.zd_date+"' ,isfuzhen=1,zdg=0 where id_no="+newpat.id_no;
        query.exec(str);
    }

    QMessageBox::warning (this,QString::fromUtf8(""),QString::fromUtf8("挂号成功!"),QMessageBox::Yes);
    this->close();


}
