#include "class_show.h"
#include "ui_class_show.h"
#include <QStringList>
#include "total.h"
#include <QFile>
class_show::class_show(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::class_show)
{
    ui->setupUi(this);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList i;
    i << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun" ;
    ui->table->setHorizontalHeaderLabels(i);
    QStringList j;
    j << "1" << "2" << "3" << "4" << "5";
    ui->table->setVerticalHeaderLabels(j);
    ui->table->horizontalHeader()->setStretchLastSection(true);

   // ui->table->setItem(1,1,new QTableWidgetItem("1,1"));
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
        ui->table->setItem(j-1,i-1,new QTableWidgetItem(line_classname));
    }
    file.close();
}

class_show::~class_show()
{
    delete ui;
}
