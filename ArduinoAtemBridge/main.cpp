#include "mainwindow.h"

#include <QApplication>
#include "consts.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlag(Qt::FramelessWindowHint, true); // borderless window
    w.setGeometry(QRect(QPoint(0,0),MAINWINDOW_SIZE));
    w.show();
    return a.exec();
}
