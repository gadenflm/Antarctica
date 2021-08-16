#ifndef LOGIN_ADM_H
#define LOGIN_ADM_H

#include <QDialog>

namespace Ui {
class login_adm;
}

class login_adm : public QDialog
{
    Q_OBJECT

public:
    explicit login_adm(QWidget *parent = 0);
    ~login_adm();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_OK_clicked();

private:
    Ui::login_adm *ui;
};

#endif // LOGIN_ADM_H
