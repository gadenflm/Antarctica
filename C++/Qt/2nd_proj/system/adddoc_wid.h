#ifndef ADDDOC_WID_H
#define ADDDOC_WID_H

#include <QWidget>

namespace Ui {
class adddoc_wid;
}

class adddoc_wid : public QWidget
{
    Q_OBJECT

public:
    explicit adddoc_wid(QWidget *parent = 0);
    ~adddoc_wid();

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::adddoc_wid *ui;
};

#endif // ADDDOC_WID_H
