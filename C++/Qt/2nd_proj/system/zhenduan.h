#ifndef ZHENDUAN_H
#define ZHENDUAN_H

#include <QWidget>

namespace Ui {
class zhenduan;
}

class zhenduan : public QWidget
{
    Q_OBJECT

public:
    int doc_no;
    QString name;
    QString keshi;
    void no_in(int a);
    void list_refresh();
    explicit zhenduan(QWidget *parent = 0);
    ~zhenduan();

private slots:
    void on_pushButton_infor_clicked();

    void on_pushButton_symptom_clicked();

private:
    Ui::zhenduan *ui;
};

#endif // ZHENDUAN_H
