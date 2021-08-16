#include "widget.h"
#include "ui_widget.h"
#include <QStringListModel>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList num;

       num<<QString("1")<<QString("2")<<QString("3")<<QString("4");

       QStringListModel *model = new QStringListModel(num);

       ui->listView->setModel(model);

}

Widget::~Widget()
{
    delete ui;
}
