#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QLabel>
namespace Ui {
class MainWindow;
}
class QMovie;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMovie *movie;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
