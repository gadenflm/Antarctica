#include "mydlg.h"
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Mydlg my1;
    if (my1.exec()== Mydlg::Accepted)
    {
        w.show();
        return a.exec();
    }
    else return 0;
}
