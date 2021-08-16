#ifndef TEAWIDGET_H
#define TEAWIDGET_H

#include <QWidget>
#include "total.h"
#include <QDebug>
#include <teacher.h>
namespace Ui {
class teawidget;
}

class teawidget : public QWidget
{
    Q_OBJECT

public:
    explicit teawidget(QWidget *parent = 0);
    ~teawidget();
    void inputnum(int a);
    void showinformation();
    teacher tea;
private slots:
    void on_exit_button_clicked();

private:
    int num;
    Ui::teawidget *ui;
};

#endif // TEAWIDGET_H
