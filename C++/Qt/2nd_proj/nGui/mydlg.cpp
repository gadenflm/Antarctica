#include "mydlg.h"
#include "ui_mydlg.h"

Mydlg::Mydlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mydlg)
{
    ui->setupUi(this);
}

Mydlg::~Mydlg()
{
    delete ui;
}
