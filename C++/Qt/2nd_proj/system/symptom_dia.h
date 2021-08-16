#ifndef SYMPTOM_DIA_H
#define SYMPTOM_DIA_H

#include <QDialog>

namespace Ui {
class symptom_dia;
}

class symptom_dia : public QDialog
{
    Q_OBJECT

public:
    int pat_no;
    int doc_no;
    bool pat_is_fuzhen;
    QString pat_symptom;
    QString pat_medicine;
    void no_in(int a,int b);
    void text_in();
    explicit symptom_dia(QWidget *parent = 0);
    ~symptom_dia();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_OK_clicked();

private:
    Ui::symptom_dia *ui;
};

#endif // SYMPTOM_DIA_H
