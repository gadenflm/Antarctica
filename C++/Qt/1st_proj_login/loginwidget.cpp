#include "loginwidget.h"
#include "zhucedialog.h"
#include "ui_loginwidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "total.h"
#include <QMessageBox>
#include <QFile>
#include "stuwidget.h"
#include "teawidget.h"
#include "admwidget.h"
loginWidget::loginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginWidget)
{
    ui->setupUi(this);
}

loginWidget::~loginWidget()
{
    delete ui;
}


void loginWidget::on_zhuceButton_clicked()
{
     zhuceDialog *dlg_zhuce= new zhuceDialog(this);
     dlg_zhuce->setWindowTitle(QObject::tr("注册界面"));
     dlg_zhuce->show();

}

void loginWidget::on_cancel_Button_clicked()
{
    this->close();
}

void loginWidget::on_login_pushButton_clicked()
{
    QString username=ui->lineEdit_user_name->text();
    QString password=ui->lineEdit_password-> text();
    int stu=0;
    int tea=0;
    int adm=0;

    QFile user ("user");
    user.open (QIODevice::ReadWrite | QIODevice::Text);
    QTextStream io(&user);
    while (!io.atEnd())
    {
        QString line=io.readLine();
        QString line_username=line.section('+',0,0);
        QString line_password=line.section('+',1,1);
        QString line_kind=line.section('+',2,2);

        if ((username==line_username) && (password==line_password) )
        {
            QString left=line_kind.left(3);
            QString Strnum=line_kind.mid(3);
            bool ok;
            int num=Strnum.toInt(&ok,10);
            if (left=="stu") { stu=num;break;}
            if (left=="tea") { tea=num;break;}
            if (left=="adm") { adm=num;break;}

        }

    }
    user.close();


    if (stu)
    {
        stuwidget *stuwid=new stuwidget ();
        stuwid->inputnum(stu);
        stuwid->setWindowTitle(QObject::tr("学生界面"));
        stuwid->show();
        this->close();
    }

    if (tea)
    {
        teawidget *teawid=new teawidget ();
        teawid->inputnum(tea);
        teawid->setWindowTitle(QObject::tr("老师界面"));
        teawid->show();
        this->close();
    }
    if (adm)
    {
        admwidget *admwid=new admwidget ();
        admwid->showinformation();
        admwid->show();
        this->close();
    }
    if ((!stu) && (!tea )&& (!adm))
    {
        QMessageBox::warning (this,tr("老铁,扎心!"),tr("用户名或密码错误!"),QMessageBox::Yes);
        ui->lineEdit_user_name->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_user_name->setFocus();
    }

}
