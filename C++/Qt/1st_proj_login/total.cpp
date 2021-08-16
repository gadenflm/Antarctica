#include <QApplication>
#include <QFile>
#include <QDebug>
#include "total.h"

 int num_student=0;
 int num_teacher=0;
 int num_administrator=1;
 int num_class=0;

 void num_exsit()
    {

         QFile file ("total");
          if ( !file.exists() )
          {
              file.open(QIODevice::ReadWrite | QIODevice::Text);
              QTextStream io(&file);
              io << num_student << endl << num_teacher << endl << num_administrator << endl <<num_class <<endl;
              file.close();
            }
    }



 void num_in()
{
    QFile file ("total");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
                QTextStream io(&file);
                io >> num_student; io >> num_teacher; io>>num_administrator>>num_class;
        }
    file.close();
}


 void num_out()
{
    QFile total ("total");
    total.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream io(&total);
    io << num_student << endl << num_teacher << endl << num_administrator << endl <<num_class << endl;
    total.close();
}


 void adm_exsit()
 {
     QFile file ("user");
      if ( !file.exists() )
      {
          file.open(QIODevice::ReadWrite | QIODevice::Text);
          QTextStream io(&file);
          io  << "admin" << "+" << "admin" << "+adm" << "1" <<"\r\n"  ;
          file.close();
        }

 }
