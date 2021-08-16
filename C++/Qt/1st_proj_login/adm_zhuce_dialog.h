#ifndef ADM_ZHUCE_DIALOG_H
#define ADM_ZHUCE_DIALOG_H

#include <QDialog>

namespace Ui {
class adm_zhuce_dialog;
}

class adm_zhuce_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit adm_zhuce_dialog(QWidget *parent = 0);
    ~adm_zhuce_dialog();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::adm_zhuce_dialog *ui;
};

#endif // ADM_ZHUCE_DIALOG_H
