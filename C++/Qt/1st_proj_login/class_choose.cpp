#include "class_choose.h"
#include "ui_class_choose.h"
#include "total.h"
#include <QFile>
#include <QDebug>
#include <QStringListModel>

class_choose::class_choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::class_choose)
{
    ui->setupUi(this);
    QStringList classlist;

    QFile file ("class");
    file.open (QIODevice::ReadWrite | QIODevice::Text);
    QTextStream io(&file);

    while (!io.atEnd())
    {
        QString line=io.readLine();
        QString line_classname=line.section('+',0,0);
        QString line_cnum=line.section('+',1,1);
        QString line_number=line.section('+',2,2);

        bool ok;
        int cnum=line_cnum.toInt(&ok,10);
        int i=(cnum-1)/5+1;
        int j=(cnum-1)%5+1;

        QString str_i;
        if (i==1)  str_i="Monday";
        if (i==2) str_i="Tuesday";
        if (i==3)   str_i="Wednesday";
        if (i==4) str_i="Thursday";
        if (i==5) str_i="Friday";
        if  (i==6) str_i="Saturday";
        if (i==7) str_i="Sunday";

        QString str_j=QString::number(j,10);


        QString appstr=line_classname+" "+str_i+" "+str_j;
        classlist.append(appstr);

    }

    QStringListModel *model = new QStringListModel(classlist);
    ui->listView_class->setModel(model);
    file.close();

}

class_choose::~class_choose()
{
    delete ui;
}

void class_choose::on_pushButton_cancel_clicked()
{
    this->close();
}

void class_choose::on_pushButton_ok_clicked()
{
    this->close();
}
