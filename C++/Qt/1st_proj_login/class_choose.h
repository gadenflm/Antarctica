#ifndef CLASS_CHOOSE_H
#define CLASS_CHOOSE_H

#include <QDialog>

namespace Ui {
class class_choose;
}

class class_choose : public QDialog
{
    Q_OBJECT

public:
    explicit class_choose(QWidget *parent = 0);
    ~class_choose();
    int num;

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::class_choose *ui;

};

#endif // CLASS_CHOOSE_H
