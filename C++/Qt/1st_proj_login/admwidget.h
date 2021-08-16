#ifndef ADMWIDGET_H
#define ADMWIDGET_H

#include <QWidget>

namespace Ui {
class admwidget;
}

class admwidget : public QWidget
{
    Q_OBJECT

public:
    explicit admwidget(QWidget *parent = 0);
    ~admwidget();
    void showinformation();
private slots:
    void on_zhuce_adm_clicked();

    void on_exit_button_clicked();

    void on_class_add_button_clicked();

private:
    Ui::admwidget *ui;
};

#endif // ADMWIDGET_H
