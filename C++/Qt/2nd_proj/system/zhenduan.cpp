#include "zhenduan.h"
#include "ui_zhenduan.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "inforpat_wid.h"
#include <QDebug>
#include "symptom_dia.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>
void zhenduan::no_in(int a)
{
    doc_no=a;
    QString no_str=QString::number(doc_no,10);
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select name,keshi from doctor where No="+no_str);
    while (query.next())
    {
        name=query.value(0).toString();
        keshi=query.value(1).toString();
        qDebug () << "name:" << name << "  keshi:" << keshi << endl;
    }
    return;
}

void zhenduan::list_refresh()
{
    ui->listWidget->setViewMode(QListView::ListMode);

    ui->listWidget->clear();

    qDebug() << "select out";
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    query.exec("select No,name,sex,age,zd_date from patient where keshi='"+keshi+"' AND zdg=0");
    while (query.next())
    {

        qDebug() << "select in";
        QString pat_no=query.value(0).toString();
        QString pat_name=query.value(1).toString();
        QString pat_sex=query.value(2).toString();
        QString pat_age=query.value(3).toString();
        QString pat_zd_date=query.value(4).toString();
        QString str=pat_no+"   "+pat_name+"   "+pat_sex+"   "+pat_age+"   "+pat_zd_date;
        ui->listWidget->addItem(str);
    }
    return;
}

zhenduan::zhenduan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zhenduan)
{
    ui->setupUi(this);
    QPixmap pixmap2("D:/sb/Program/system/zhenduan-1.png");
    QPixmap pixmap=pixmap2.scaled(571,71);
    ui->pic_1->setPixmap(pixmap);
}

zhenduan::~zhenduan()
{
    delete ui;
}

void zhenduan::on_pushButton_infor_clicked()
{
    QString pat_str=ui->listWidget->currentItem()->text();
    int pat_no=pat_str.left(4).toInt();
    inforpat_wid *inforwid=new inforpat_wid();
    inforwid->no_in(pat_no);
    inforwid->text_in();
    inforwid->setWindowTitle("");
    inforwid->show();

}

void zhenduan::on_pushButton_symptom_clicked()
{
    QString pat_str=ui->listWidget->currentItem()->text();
    int pat_no=pat_str.left(4).toInt();
    symptom_dia *sym_dialog=new symptom_dia();
    sym_dialog->no_in(pat_no,doc_no);
    sym_dialog->text_in();
    sym_dialog->setWindowTitle("");
    sym_dialog->show();
    this->close();

}
