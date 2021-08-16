#ifndef TOTAL_H
#define TOTAL_H
#include <QApplication>
#include <QFile>
#include <QDebug>

extern int num_student;
extern int num_teacher;
extern int num_administrator;
extern int num_class;

void num_exsit();
void num_in();
void num_out();
void adm_exsit();
#endif // TOTAL_H








/*class total
{
public:
    static int num_student;
    static int num_teacher;
    static int num_administrator;
  void num_exsit()
        {
         //    num_student=0;
        //    num_teacher=0;
         //    num_administrator=0;
              QFile file ("total");
              if ( !file.exists() )
              {
                  file.open(QIODevice::ReadWrite | QIODevice::Text);
                  QTextStream io(&file);
                  io << num_student << endl << num_teacher << endl << num_administrator << endl;
                  file.close();
              }
        }



     void num_in()
    {
        QFile file ("total");
        if (file.open(QIODevice::ReadWrite | QIODevice::Text))
            {
                    QTextStream io(&file);
                    io >> num_student; io >> num_teacher; io>>num_administrator;
                    qDebug () << num_student << num_teacher << num_administrator;
            }
        file.close();
    }


     void num_out()
    {
        QFile total ("total");
        total.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream io(&total);
        io << num_student << endl << num_teacher << endl << num_administrator << endl;
        total.close();
    }



};*/
