#include "guahao.h"
#include "welcome.h"
#include <QApplication>
#include <total.h>
#include <QTextCodec>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    chushihua();
    welcome w;
    w.setWindowTitle(QString::fromUtf8("医院系统 Beta v1.0"));
    w.show();

    return a.exec();
}
