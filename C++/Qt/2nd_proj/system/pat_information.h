#ifndef PAT_INFORMATION_H
#define PAT_INFORMATION_H

#include <QWidget>

namespace Ui {
class pat_information;
}

class pat_information : public QWidget
{
    Q_OBJECT

public:
    void listrefresh();
    explicit pat_information(QWidget *parent = 0);
    ~pat_information();

private slots:
    void on_pushButton_del_clicked();

    void on_pushButton_infor_clicked();

private:
    Ui::pat_information *ui;
};

#endif // PAT_INFORMATION_H
