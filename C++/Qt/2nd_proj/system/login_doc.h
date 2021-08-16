#ifndef LOGIN_DOC_H
#define LOGIN_DOC_H

#include <QWidget>

namespace Ui {
class login_doc;
}

class login_doc : public QWidget
{
    Q_OBJECT

public:
    explicit login_doc(QWidget *parent = 0);
    ~login_doc();

private slots:
    void on_pushButton_clicked();

private:
    Ui::login_doc *ui;
};

#endif // LOGIN_DOC_H
