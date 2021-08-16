#ifndef ZHUCEDIALOG_H
#define ZHUCEDIALOG_H

#include <QDialog>

namespace Ui {
class zhuceDialog;
}
class QErrorMessage;
class zhuceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zhuceDialog(QWidget *parent = 0);
    ~zhuceDialog();
    void layout();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::zhuceDialog *ui;
    QErrorMessage *errordlg;
};

#endif // ZHUCEDIALOG_H
