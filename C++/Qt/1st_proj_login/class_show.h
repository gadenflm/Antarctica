#ifndef CLASS_SHOW_H
#define CLASS_SHOW_H

#include <QDialog>

namespace Ui {
class class_show;
}

class class_show : public QDialog
{
    Q_OBJECT

public:
    explicit class_show(QWidget *parent = 0);
    ~class_show();

private:
    Ui::class_show *ui;
};

#endif // CLASS_SHOW_H
