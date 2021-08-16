#include "adddoc_wid.h"
#include "ui_adddoc_wid.h"
#include "doctor.h"
#include "total.h"

adddoc_wid::adddoc_wid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adddoc_wid)
{
    ui->setupUi(this);
}

adddoc_wid::~adddoc_wid()
{
    delete ui;
}

void adddoc_wid::on_pushButton_OK_clicked()
{
    doctor newdoc;
    num_doctor++;
    num_out();
    newdoc.no=num_doctor;
    newdoc.name=ui->lineEdit_name->text();
    newdoc.work_no=ui->lineEdit_work_no->text();
    newdoc.id_no=ui->lineEdit_idno->text();
    newdoc.tel=ui->lineEdit_tel->text();
    newdoc.email=ui->lineEdit_email->text();
    newdoc.keshi=ui->comboBox_keshi->currentText();
    newdoc.doc_in();
    this->close();

}

void adddoc_wid::on_pushButton_cancel_clicked()
{
    this->close();
}
