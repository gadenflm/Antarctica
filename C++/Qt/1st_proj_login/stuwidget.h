#ifndef STUWIDGET_H
#define STUWIDGET_H

#include <QWidget>
#include "total.h"
#include <QDebug>
#include <student.h>

namespace Ui {
class stuwidget;
}

class stuwidget : public QWidget
{
    Q_OBJECT

public:
    explicit stuwidget(QWidget *parent = 0);
    ~stuwidget();
    void inputnum(int a);
    void showinformation();
    student stu;
private slots:
    void on_exit_button_clicked();

    void on_class_choose_button_clicked();

    void on_class_show_button_clicked();

private:
    int num;
    Ui::stuwidget *ui;
};

#endif // STUWIDGET_H
