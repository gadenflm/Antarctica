#include "total.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QDate>
int num_administrator=1;
int num_doctor=0;
int num_patient=0;



bool createsqlconnection(QString sqlname)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",sqlname);
    db.setDatabaseName(sqlname+".db");
    if (!db.open()){
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
        return false;
    }
    return true;
}

void chushihua()
{
    QFile file ("total.ini");
     if ( !file.exists() )
     {
         file.open(QIODevice::ReadWrite | QIODevice::Text);
         QTextStream io(&file);
         io <<"[gadenflm]" <<endl;
         io << num_patient << endl << num_doctor << endl << num_administrator <<endl;
         file.close();

        if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
        QSqlDatabase db=QSqlDatabase::database("admin");
        QSqlQuery query(db);
        query.exec ("create table admin(No int primary key,username varchar(180),password varchar(180))");
        query.exec ("create table doctor(No int primary key,name varchar(180),work_no varchar(180),keshi varchar(180),id_no varchar(180),tel varchar(180),email varchar(180))");
        query.exec ("create table patient(No int primary key,name varchar(180),id_no varchar(180),sex varchar(90),tel varchar(180),birth varchar(180),age varchar(90),zd_date varchar(180),keshi varchar(180),symptom varchar(18000),medicine varchar(9000),isfuzhen bit,zdg bit)");
        query.exec ("insert into admin values (1,'gadenflm','gadenflm')");

     }
     else
    {
         file.open(QIODevice::ReadOnly | QIODevice::Text);
         QTextStream in (&file);
         QString gadenflm=in.readLine();
         QString num_pat=in.readLine();
         QString num_doc=in.readLine();
         QString num_adm=in.readLine();
         file.close();
         num_patient=num_pat.toInt();
         num_doctor=num_doc.toInt()        ;
         num_administrator=num_adm.toInt();
     }
}

void num_out()
{
    QFile total ("total.ini");
    total.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream io(&total);
    io <<"[gadenflm]" <<endl;
    io << num_patient << endl << num_doctor << endl << num_administrator <<endl;
    total.close();

}



