#include "pat_information.h"
#include "ui_pat_information.h"
#include "total.h"
#include "patient.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "inforpat_wid.h"
#include <QDebug>

void pat_information::listrefresh()
{
    ui->listWidget->setViewMode(QListView::ListMode);

    ui->listWidget->clear();

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No,name,sex,keshi from patient");
    while (query.next())
    {
        QString no=query.value(0).toString();
        QString name=query.value(1).toString();
        QString sex=query.value(2).toString();
        QString keshi=query.value(3).toString();
        QString str=no+"   "+name+"   "+sex+"   "+keshi;

        ui->listWidget->addItem(str);

    }
}

pat_information::pat_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pat_information)
{
    ui->setupUi(this);
    listrefresh();
}

pat_information::~pat_information()
{
    delete ui;
}

void pat_information::on_pushButton_del_clicked()
{
    num_patient--;
    num_out();

    int i=ui->listWidget->currentRow()+1;
    QString i_str=QString::number(i,10);

    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("delete from patient where No="+i_str);
    query.exec("UPDATE patient SET No=No-1 WHERE No>"+i_str);

    listrefresh();

}

void pat_information::on_pushButton_infor_clicked()
{
    int i=ui->listWidget->currentRow()+1;
    inforpat_wid *inforwid=new inforpat_wid();
    inforwid->no_in(i);
    inforwid->text_in();
    inforwid->setWindowTitle("");
    inforwid->show();
    this->close();
}
