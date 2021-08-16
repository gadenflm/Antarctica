#include "stuwidget.h"
#include "ui_stuwidget.h"
#include <student.h>
#include <QDebug>
#include <QFile>
#include "class_choose.h"
#include "class_show.h"
stuwidget::stuwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stuwidget)
{

    ui->setupUi(this);

}

stuwidget::~stuwidget()
{
    delete ui;
}

void stuwidget::inputnum(int a)
{
    num=a;
    stu.stufromfile(num);
    QString hello=stu.name+",Hello!";
    ui->label_hello->setText(hello);
    showinformation();
}

void stuwidget::showinformation()
{
    QString information;
    ui->stu_information->append("Name:"+stu.name);
    ui->stu_information->append("Student ID:"+stu.id);
    ui->stu_information->append("Sex:"+stu.sex);
    ui->stu_information->append("campus:"+stu.campus);
    ui->stu_information->append("academy:"+stu.academy);
 }

void stuwidget::on_exit_button_clicked()
{
    this->close();
}

void stuwidget::on_class_choose_button_clicked()
{
    class_choose *dlg_choose= new class_choose(this);
    dlg_choose->num=num;
    dlg_choose->show();
}

void stuwidget::on_class_show_button_clicked()
{
    class_show *dlg_show= new class_show(this);
    dlg_show->show();
}
