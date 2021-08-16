#ifndef ADM_H
#define ADM_H

#include <QWidget>

namespace Ui {
class adm;
}

class adm : public QWidget
{
    Q_OBJECT

public:
    int adm_no;
    void no_in(int a);
    void text_in();
    explicit adm(QWidget *parent = 0);
    ~adm();

private slots:
    void on_pushButton_adddoc_clicked();

    void on_pushButton_addadmin_clicked();

    void on_pushButton_showdoc_clicked();

    void on_pushButton_showpat_clicked();

private:
    Ui::adm *ui;
};

#endif // ADM_H
