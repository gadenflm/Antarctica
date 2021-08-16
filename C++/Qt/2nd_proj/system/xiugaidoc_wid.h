#ifndef XIUGAIDOC_WID_H
#define XIUGAIDOC_WID_H

#include <QWidget>
#include <doctor.h>

namespace Ui {
class xiugaidoc_wid;
}

class xiugaidoc_wid : public QWidget
{
    Q_OBJECT

public:
    int i;
    bool islock;
    doctor doc;
    void i_in(int a,bool t);
    void text_in();
    void text_lock();
    explicit xiugaidoc_wid(QWidget *parent = 0);
    ~xiugaidoc_wid();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_OK_clicked();

private:
    Ui::xiugaidoc_wid *ui;
};

#endif // XIUGAIDOC_WID_H
