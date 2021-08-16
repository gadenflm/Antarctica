#ifndef DOCTOR_H
#define DOCTOR_H
#include "total.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

class doctor
{
public:
    QString name;
    QString work_no;
    QString keshi;
    QString id_no;
    QString tel;
    QString email;
    int no;

    void doc_in()
    {
        if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
        QSqlDatabase db=QSqlDatabase::database("admin");
        QSqlQuery query(db);
        QString no_str=QString::number(no,10);
        QString str="insert into doctor values ("+no_str+",'"+name+"','"+work_no+"','"+keshi+"','"+id_no+"','"+tel+"','"+email+"')";
        query.exec(str);
        return;
                //query.exec ("insert into admin values (1,'gadenflm','gadenflm')");
    }

};

#endif // DOCTOR_H
