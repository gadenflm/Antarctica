#include "admwidget.h"
#include "ui_admwidget.h"
#include "total.h"
#include "adm_zhuce_dialog.h"
#include "class_add.h"
admwidget::admwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admwidget)
{
    ui->setupUi(this);
}

admwidget::~admwidget()
{
    delete ui;
}

void admwidget::showinformation()
{
    QString numstu=QString::number(num_student,10);
    QString numtea=QString::number(num_teacher,10);
    QString numadm=QString::number(num_administrator,10);
    ui->adm_information->append("已注册学生数: "+numstu);
    ui->adm_information->append("已注册老师数量: "+numtea);
    ui->adm_information->append("已注册管理员: "+numadm);
}

void admwidget::on_zhuce_adm_clicked()
{
    adm_zhuce_dialog *dlg_zhuce= new adm_zhuce_dialog(this);
    dlg_zhuce->setWindowTitle(QObject::tr("注册界面"));
    dlg_zhuce->show();
}

void admwidget::on_exit_button_clicked()
{
    this->close();
}

void admwidget::on_class_add_button_clicked()
{
    class_add *dlg_add= new class_add(this);
    dlg_add->show();
}
