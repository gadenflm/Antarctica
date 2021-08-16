#ifndef DOC_INFORMATION_H
#define DOC_INFORMATION_H

#include <QWidget>

namespace Ui {
class doc_information;
}

class doc_information : public QWidget
{
    Q_OBJECT

public:
    explicit doc_information(QWidget *parent = 0);
    void listrefresh();
    ~doc_information();


private slots:
    void on_pushButton_xiugai_clicked();

    void on_pushButton_infor_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::doc_information *ui;
};

#endif // DOC_INFORMATION_H
