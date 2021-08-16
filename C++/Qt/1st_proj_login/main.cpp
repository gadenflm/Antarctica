#include "total.h"
#include "loginwidget.h"
#include <QApplication>

#include "zhucedialog.h"

#include <QFile>
#include <QDebug>





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   adm_exsit();
   num_exsit();
   num_in();

    loginWidget dlg_login;
    dlg_login.show();

    return a.exec();
}


