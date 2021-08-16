#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file ("myfile.txt");
    if (! file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << file.errorString();
    file.write("helloQt!\n");
    file.close();
    QFileInfo info(file);
    qDebug() << QObject::tr("jue dui lu jing:") << info.absoluteFilePath() << endl
             << QObject::tr("wen jian ming:") << info.fileName() <<endl
             << QObject::tr("ji ben ming chen:") << info.baseName() << endl
             << QObject::tr("hou zhui:") << info.suffix() << endl
             << QObject::tr("chuang jian shi jian:") << info.created() <<endl
             << QObject::tr ("da xiao ") << info.size ();
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug ()<< file.errorString();
    qDebug () << QObject::tr("wen jian nei rong:") << endl << file.readAll();
    qDebug () << QObject::tr("dang qian wei zhi ") << endl <<file.pos ();
    file.seek (0);


    return a.exec();
}
