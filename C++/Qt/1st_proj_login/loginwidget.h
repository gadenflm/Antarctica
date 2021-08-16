#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "total.h"
namespace Ui {
class loginWidget;
}

class loginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit loginWidget(QWidget *parent = 0);
    ~loginWidget();
    void layout ();

private slots:
    void on_zhuceButton_clicked();

    void on_cancel_Button_clicked();

    void on_login_pushButton_clicked();

private:
    Ui::loginWidget *ui;
};

#endif // LOGINWIDGET_H
