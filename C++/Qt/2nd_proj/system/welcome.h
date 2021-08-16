#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

namespace Ui {
class welcome;
}

class welcome : public QWidget
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = 0);
    ~welcome();

private slots:
    void on_Button_guahao_clicked();

    void on_Button_login_admin_clicked();

    void on_Button_login_doctor_clicked();

private:
    Ui::welcome *ui;
};

#endif // WELCOME_H
