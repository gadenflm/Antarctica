#ifndef STUDENT_H
#define STUDENT_H
#include "total.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
class student
{
public:
    QString user_name;
    QString password;
    QString name;
    QString id;
    QString sex;
    QString campus;
    QString academy;
    bool single;
    QString classname[36];

bool IsPofei()
{
   if (user_name=="" || password =="" || name =="" || id  =="" || sex =="" || campus=="" ||  academy=="" ) return false ;
   else return true;
}
void InputStu()
{
    num_student++;
//total ++

    num_out();

//user ++
    QFile user ("user");
    user.open (QIODevice::ReadWrite | QIODevice::Append);
    QTextStream io(&user);
    io  << user_name << "+" << password << "+stu" << num_student <<"\r\n"  ;
    user.close();

//stu ++
    QString num=QString::number(num_student,10);
    QString filename="STU"+num;
    QFile file (filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
            QTextStream out(&file);
            out << user_name <<endl;
            out << password <<endl;
            out << name << endl;
            out << id <<endl;
            out << sex << endl;
            out << campus << endl;
            out << academy << endl;
            out << single << endl;

    }
    file.close();
}

void stufromfile(int num)
{
    QString numstr=QString::number(num,10);
    QString filename="STU"+numstr;
    QFile file (filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            QTextStream in (&file);
            user_name=in.readLine();
            password=in.readLine();
            name=in.readLine();
            id=in.readLine();
            sex=in.readLine();
            campus=in.readLine();
            academy=in.readLine();
            QString single_bool=in.readLine();
            if (single_bool=="1") single=true; else single =false;
    }
    file.close();
}


};


#endif // STUDENT_H
