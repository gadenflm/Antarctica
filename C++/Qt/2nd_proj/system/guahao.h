#ifndef GUAHAO_H
#define GUAHAO_H

#include <QWidget>

namespace Ui {
class guahao;
}

class guahao : public QWidget
{
    Q_OBJECT

public:
    explicit guahao(QWidget *parent = 0);
    ~guahao();

private slots:


    void on_OKButton_clicked();

private:
    Ui::guahao *ui;
};

#endif // GUAHAO_H
