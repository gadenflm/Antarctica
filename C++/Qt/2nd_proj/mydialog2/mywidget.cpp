#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::red,this,tr("颜色"),QColorDialog::ShowAlphaChannel);
    qDebug () << "color:" << color ;
}



void MyWidget::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName (this,tr("文本"),"D:",tr("图片文件( * png * jpg)"));
    qDebug () << "fileName:" << fileName;
}

void MyWidget::on_pushButton_3_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont (&ok,this);
    if (ok) ui ->pushButton_3->setFont (font);
    else qDebug() << tr ("no Font!");

}

void MyWidget::on_pushButton_4_clicked()
{
    bool ok;
    QString string = QInputDialog::getText(this,tr("输入字符串:"),tr("输入用户名:"),QLineEdit::Normal,tr("admin"),&ok);
    if (ok) qDebug() << "string:" << string;
    int value = QInputDialog::getInt(this,tr("输入整数:"),tr("请输入 -1000到1000:"),100,-1000,1000,10,&ok);
    if (ok) qDebug() << "integer:" << value;
    QStringList items;
    items << tr ("条目1") << tr ("条目2");
    QString item = QInputDialog::getItem(this,tr("输入条目对话框:"),tr("选择以一个条目:"),items,0,true,&ok);
    if (ok) qDebug () << "item:" <<item;
}
