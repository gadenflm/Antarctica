//#include <QtGui>
//#include <QtGui/QApplication>
#include <QTextCodec>
#include <QWidget>
#include <ActiveQt/QAxWidget>
#include <QTime>

void gifplay(QLabel *label)
{
    label->show();
}

void gifclose(QLabel *label)
{
    label->close();
}

void flashplay(QAxWidget *flash)
{
    flash->show();
}

void flashclose(QAxWidget *flash)
{
    flash->close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("GB2312"); //文本为GB2312编码
    QTextCodec::setCodecForLocale(codec);       //设置本地编码
    QTextCodec::setCodecForCStrings(codec);     //设置字符串编码
    QTextCodec::setCodecForTr(codec);

    QAxWidget *flash = new QAxWidget(0,0);      //QAxWidget使用的是ActiveX插件
    flash->resize(800,600);                    //设置该控件的初始大小
    flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}")); //设定控制器
    flash->dynamicCall("LoadMovie(long,string)",0,"D:\\flash1.swf");   //
    flash->setWindowFlags(Qt::FramelessWindowHint);   //去除边框
    flashplay(flash);
    QTime t;
    t.start();
    while(t.elapsed()<5000)
    {
        QApplication::processEvents();
    }
    flashclose(flash);

    /*
    QMovie *movie;
    movie = new QMovie("D:\\gif1.gif");
    QLabel *label=new QLabel ("", 0);
    label->setGeometry( 300, 300, 500, 500 );
    label->setMovie(movie);
    label->setScaledContents(true);
    movie->start();
    gifplay(label);
    t.start();
    while(t.elapsed()<5000)
    {
        QApplication::processEvents();
    }
    gifclose(label);
    */

    return a.exec();
}
