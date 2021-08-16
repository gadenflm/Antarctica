#include "adm.h"
#include "ui_adm.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "total.h"
#include "addadm_wid.h"
#include "adddoc_wid.h"
#include "doc_information.h"
#include "pat_information.h"

void adm::no_in(int a)
{
    adm_no=a;
    return ;
}

void adm::text_in()
{
    if (!createsqlconnection("admin")) qDebug() << "admin_create_gg";
    QSqlDatabase db=QSqlDatabase::database("admin");
    QSqlQuery query(db);
    QString admno=QString::number(adm_no,10);

    query.exec("select * from admin where No="+admno);
    QString name;
    while (query.next())
    {
       name=query.value(1).toString();
    }
    QString text_1=name+"";
    ui->label_text->setText(text_1);
}

adm::adm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adm)
{
    ui->setupUi(this);
    ui->textBrowser->append(QString::fromUtf8("���ã����ڲ��ˣ�ҽ��������Ա�����ֱ����£�"));
    QString str_pat=QString::fromUtf8("���ˣ�")+ QString::number(num_patient,10);
    QString str_doc=QString::fromUtf8("ҽ����")+QString::number(num_doctor,10);
    QString str_adm=QString::fromUtf8("����Ա��")+QString::number(num_administrator,10);
    ui->textBrowser->append(str_pat);
    ui->textBrowser->append(str_doc);
    ui->textBrowser->append(str_adm);
}

adm::~adm()
{
    delete ui;
}

void adm::on_pushButton_adddoc_clicked()
{
    adddoc_wid *adddocwid=new adddoc_wid();
    adddocwid->setWindowTitle(QString::fromUtf8("ҽ��ע��"));
    adddocwid->show();

    ui->textBrowser->clear();
    ui->textBrowser->append(QString::fromUtf8("���ã����ڲ��ˣ�ҽ��������Ա�����ֱ����£�"));
    QString str_pat=QString::fromUtf8("���ˣ�")+ QString::number(num_patient,10);
    QString str_doc=QString::fromUtf8("ҽ����")+QString::number(num_doctor+1,10);
    QString str_adm=QString::fromUtf8("����Ա��")+QString::number(num_administrator,10);
    ui->textBrowser->append(str_pat);
    ui->textBrowser->append(str_doc);
    ui->textBrowser->append(str_adm);
}

void adm::on_pushButton_addadmin_clicked()
{
    addadm_wid *addadmwid=new addadm_wid();
    addadmwid->setWindowTitle(QString::fromUtf8(""));
    addadmwid->show();

    ui->textBrowser->clear();
    ui->textBrowser->append(QString::fromUtf8("���ã����ڲ��ˣ�ҽ��������Ա�����ֱ����£�"));
    QString str_pat=QString::fromUtf8("���ˣ�")+ QString::number(num_patient,10);
    QString str_doc=QString::fromUtf8("ҽ����")+QString::number(num_doctor,10);
    QString str_adm=QString::fromUtf8("����Ա��")+QString::number(num_administrator+1,10);
    ui->textBrowser->append(str_pat);
    ui->textBrowser->append(str_doc);
    ui->textBrowser->append(str_adm);
}

void adm::on_pushButton_showdoc_clicked()
{
    doc_information *docwid=new doc_information();
    docwid->setWindowTitle("");
    docwid->show();
}

void adm::on_pushButton_showpat_clicked()
{
    pat_information *patwid=new pat_information();
    patwid->setWindowTitle("");

    patwid->show();
}
