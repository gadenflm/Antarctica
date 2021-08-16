#ifndef CLASS_ADD_H
#define CLASS_ADD_H

#include <QDialog>

namespace Ui {
class class_add;
}

class class_add : public QDialog
{
    Q_OBJECT

public:
    explicit class_add(QWidget *parent = 0);
    ~class_add();

private slots:
    void on_cancel_button_clicked();

    void on_ok_button_clicked();

private:
    Ui::class_add *ui;
};

#endif // CLASS_ADD_H
