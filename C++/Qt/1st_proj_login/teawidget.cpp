#include "teawidget.h"
#include "ui_teawidget.h"
#include <teacher.h>
#include <QDebug>
#include <QFile>
teawidget::teawidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teawidget)
{
    ui->setupUi(this);
}

teawidget::~teawidget()
{
    delete ui;
}
void teawidget::inputnum(int a)
{
    num=a;
    tea.teafromfile(num);
    QString hello=tea.name+",Hello!";
    ui->label_hello->setText(hello);
    showinformation();
}

void teawidget::showinformation()
{
    QString information;
    ui->tea_information->append("Name:"+tea.name);
    ui->tea_information->append("Teacher ID:"+tea.id);
    ui->tea_information->append("Sex:"+tea.sex);
    ui->tea_information->append("Campus:"+tea.campus);
    ui->tea_information->append("Academy:"+tea.academy);
 }

void teawidget::on_exit_button_clicked()
{
    this->close();
}
