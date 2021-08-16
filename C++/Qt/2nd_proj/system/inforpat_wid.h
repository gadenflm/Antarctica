#ifndef INFORPAT_WID_H
#define INFORPAT_WID_H

#include <QWidget>
#include <patient.h>

namespace Ui {
class inforpat_wid;
}

class inforpat_wid : public QWidget
{
    Q_OBJECT

public:
    int pat_no;
    patient pat;
    void no_in(int a);
    void text_in();
    explicit inforpat_wid(QWidget *parent = 0);
    ~inforpat_wid();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::inforpat_wid *ui;
};

#endif // INFORPAT_WID_H
