#ifndef ADDADM_WID_H
#define ADDADM_WID_H

#include <QWidget>

namespace Ui {
class addadm_wid;
}

class addadm_wid : public QWidget
{
    Q_OBJECT

public:
    explicit addadm_wid(QWidget *parent = 0);
    ~addadm_wid();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::addadm_wid *ui;
};

#endif // ADDADM_WID_H
