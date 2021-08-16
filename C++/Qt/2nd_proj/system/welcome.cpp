#include "welcome.h"
#include "ui_welcome.h"
#include "total.h"
#include <guahao.h>
#include <login_adm.h>
#include <login_doc.h>

welcome::welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::welcome)
{

    ui->setupUi(this);
    ui->textBrowser->append(QString::fromUtf8("您好，欢迎使用本医院系统！"));
    ui->textBrowser->append(QString::fromUtf8("注意事项："));
    QString text1=QString::number(num_doctor,10);
    text1=QString::fromUtf8("1.现在共有医生")+text1+QString::fromUtf8("位。");
    ui->textBrowser->append(text1);
    ui->textBrowser->append(QString::fromUtf8("2.挂号时信息要完整，缺失信息无法成功挂号。"));
    ui->textBrowser->append(QString::fromUtf8("3.挂号成功后请找对应科室耐心等待。"));
    ui->textBrowser->append(QString::fromUtf8("4.有问题请联系管理员：gadenflm"));
}

welcome::~welcome()
{
    delete ui;
}

void welcome::on_Button_guahao_clicked()
{
    guahao *guahaowid=new guahao();
    guahaowid->setWindowTitle(QString::fromUtf8("挂号-请完善信息"));
    guahaowid->show();
    this->close();
}

void welcome::on_Button_login_admin_clicked()
{
    login_adm *login_adm_dialog=new login_adm();
    login_adm_dialog->setWindowTitle(QString::fromUtf8("管理员登陆入口"));
    login_adm_dialog->show();
    this->close();
}

void welcome::on_Button_login_doctor_clicked()
{
    login_doc *login_doc_wid=new login_doc();
    login_doc_wid->setWindowTitle("");
    login_doc_wid->show();
    this->close();
}
