#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
static bool createConnection ()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","connection");
    db.setDatabaseName("my1.db");
    if (!db.open()){
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
        return false;
    }
    return true;
}

#endif // CONNECTION_H
