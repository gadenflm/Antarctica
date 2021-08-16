#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QWidget>
#include "qmovie.h"
#include <QLabel>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    lab_gif = new QLabel ("gif",this );
    lab_gif->setGeometry( QRect(0,0,400,320));
    QMovie movie ("../mydrawing3/123.gif");
    movie.setBackgroundColor(black);
    movie.unpause();
    lab_gif->setMovie (movie);
    lab_gif->show();
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


