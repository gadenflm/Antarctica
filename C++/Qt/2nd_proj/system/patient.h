#ifndef PATIENT_H
#define PATIENT_H
#include <QDebug>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "total.h"
class patient
{
public:
    QString name;
    QString id_no;
    QString birth;
    QString zd_date;
    QString tel;
    QString age;
    int no;
    QString sex;
    bool isfuzhen;
    bool zdg;
    QString keshi;
    QString symptom;
    QString medicine;

    void pat_in()
    {
        QString int_is_fuzhen;
        QString int_zdg;
        if (isfuzhen) int_is_fuzhen="1"; else int_is_fuzhen="0";
        if (zdg) int_zdg="1"; else int_zdg="0";

        if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
        QSqlDatabase db=QSqlDatabase::database("admin");
        QSqlQuery query(db);
        QString no_str=QString::number(no,10);
        QString str="insert into patient values ("+no_str+",'"+name+"','"+id_no+"','"+sex+"','"+tel+"','"+birth+"','"+age+"','"+zd_date+"','"+keshi+"','"+symptom+"','"+medicine+"',"+int_is_fuzhen+","+int_zdg+")";

        query.exec(str);
        return;
    }

};

#endif // PATIENT_H
