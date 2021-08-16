#include "doc_information.h"
#include "ui_doc_information.h"
#include "total.h"
#include "doctor.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <xiugaidoc_wid.h>


doc_information::doc_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::doc_information)
{
    ui->setupUi(this);
    listrefresh();

}

void doc_information::listrefresh()
{
    ui->listWidget->setViewMode(QListView::ListMode);

    ui->listWidget->clear();

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No,name,work_no from doctor");

    while (query.next())
    {
        QString no=query.value(0).toString();
        QString name=query.value(1).toString();
        QString word_no=query.value(2).toString();

        QString str=no+"   "+name+"   "+word_no;
        ui->listWidget->addItem(str);

    }
}

doc_information::~doc_information()
{
    delete ui;
}


void doc_information::on_pushButton_xiugai_clicked()
{
    int i=ui->listWidget->currentRow()+1;
    xiugaidoc_wid *xiugaiwid=new xiugaidoc_wid();
    xiugaiwid->i_in(i,true);
    xiugaiwid->text_in();
    xiugaiwid->text_lock();
    xiugaiwid->setWindowTitle("");
    xiugaiwid->show();
    this->close();

}

void doc_information::on_pushButton_infor_clicked()
{
    int i=ui->listWidget->currentRow()+1;
    xiugaidoc_wid *inforwid=new xiugaidoc_wid();
    inforwid->i_in(i,false);
    inforwid->text_in();
    inforwid->text_lock();
    inforwid->setWindowTitle("");
    inforwid->show();
    this->close();
}

void doc_information::on_pushButton_del_clicked()
{
   num_doctor--;
   num_out();

   int i=ui->listWidget->currentRow()+1;
   QString i_str=QString::number(i,10);

   if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
   QSqlDatabase db=QSqlDatabase::database("admin");
   QSqlQuery query(db);
   query.exec("delete from doctor where No="+i_str);
   query.exec("UPDATE doctor SET No=No-1 WHERE No>"+i_str);

   listrefresh();

}
