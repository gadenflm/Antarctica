#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include <QSize>
#include <QTime>
#include <QLabel>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    movie = new QMovie(this);
    movie->setFileName("D:/game/photo/deep/jineng1.gif");
    movie->setCacheMode(QMovie::CacheAll);
    QSize size = ui->label->size();
    movie->setScaledSize(size);
    QObject::connect(movie, &QMovie::frameChanged, [=](int frameNumber) {

          if (frameNumber == movie->frameCount() - 1) {
              this->close();
          }
      });

    ui->label->setMovie(movie);
        movie->start();


}

MainWindow::~MainWindow()
{
    delete ui;
}
