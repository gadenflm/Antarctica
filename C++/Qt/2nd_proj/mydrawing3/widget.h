#ifndef WIDGET_H
#include <QDebug>
#include <QWidget>
#include "qmovie.h"
#include <QLabel>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    QLabel *lab_gif;
    QMovie *movie;
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
