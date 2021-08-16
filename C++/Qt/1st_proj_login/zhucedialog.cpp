#include "zhucedialog.h"
#include "ui_zhucedialog.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QDebug>
#include "student.h"
#include <QErrorMessage>
#include "teacher.h"
zhuceDialog::zhuceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::zhuceDialog)
{
    errordlg= new QErrorMessage(this);
    ui->setupUi(this);
    layout();
}

zhuceDialog::~zhuceDialog()
{
    delete ui;
}
void zhuceDialog::layout()
{
    QGridLayout * layout =new QGridLayout;
    layout->addWidget(ui->label_kind,0,0,1,2);
    layout->addWidget(ui->comboBox_kind,0,2,1,1);
    layout->addWidget(ui->label_user_name,1,0,1,1);
    layout->addWidget(ui->lineEdit_user_name,1,1,1,3);
    layout->addWidget(ui->label_password,2,0,1,1);
    layout->addWidget(ui->lineEdit_password,2,1,1,3);
    layout->addWidget(ui->label_password_1,3,0,1,1);
    layout->addWidget(ui->lineEdit_password_1,3,1,1,3);
    layout->addWidget(ui->label_name,4,0,1,1);
    layout->addWidget(ui->lineEdit_name,4,1,1,1);
    layout->addWidget(ui->label_id,4,2,1,1);
    layout->addWidget(ui->lineEdit_id,4,3,1,1);
    layout->addWidget(ui->label_sex,5,0,1,1);
    layout->addWidget(ui->comboBox_sex,5,1,1,1);
    layout->addWidget(ui->label_single,5,2,1,1);
    layout->addWidget(ui->comboBox_single,5,3,1,1);
    layout->addWidget(ui->label_campus,6,0,1,1);
    layout->addWidget(ui->comboBox_campus,6,1,1,1);
    layout->addWidget(ui->label_academy,6,2,1,1);
    layout->addWidget(ui->comboBox_academy,6,3,1,1);
    layout->addWidget(ui->okButton,7,1,1,1);
    layout->addWidget(ui->cancelButton,7,3,1,1);
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    layout->setContentsMargins(50,5,50,30);
    setLayout(layout);
}

void zhuceDialog::on_okButton_clicked()
{

    if ( ui->lineEdit_password_1->text() ==  ui->lineEdit_password->text() )
    {
        int kind=ui->comboBox_kind->currentIndex();
        if (kind ==0)
        {
            student newstu;
            newstu.user_name=ui->lineEdit_user_name->text();
            newstu.name=ui->lineEdit_name->text();
            newstu.password=ui->lineEdit_password->text();
            newstu.id=ui->lineEdit_id->text();
            newstu.sex=ui->comboBox_sex->currentText();
            newstu.academy=ui->comboBox_academy->currentText();
            newstu.campus=ui->comboBox_campus->currentText();
            if (ui->comboBox_single->currentIndex()==0) newstu.single=true; else newstu.single=false;
            if (newstu.IsPofei()) {newstu.InputStu();this->close();}
            else {errordlg->showMessage(tr("你的信息不完善！！！！！！！！！！！"));}
        }
        if (kind ==1)
        {
            teacher newtea;
            newtea.user_name=ui->lineEdit_user_name->text();
            newtea.name=ui->lineEdit_name->text();
            newtea.password=ui->lineEdit_password->text();
            newtea.id=ui->lineEdit_id->text();
            newtea.sex=ui->comboBox_sex->currentText();
            newtea.academy=ui->comboBox_academy->currentText();
            newtea.campus=ui->comboBox_campus->currentText();
            if (ui->comboBox_single->currentIndex()==0) newtea.single=true; else newtea.single=false;
            if (newtea.IsPofei()) {newtea.InputTea();this->close();}
            else {errordlg->showMessage(tr("你的信息不完善！！！！！！！！！！！"));}
        }
    }
    else
    {
        errordlg->setWindowTitle(tr("密码输两次都能输错，傻不傻？"));
        errordlg->showMessage(tr("两次密码不一致，咖喱给给"));
    }

}

void zhuceDialog::on_cancelButton_clicked()
{
    this->close();
}
