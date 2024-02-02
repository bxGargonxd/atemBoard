#include "qdigitalclock.h"

#include <QTimer>
#include <QTime>

QDigitalClock::QDigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Filled);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QDigitalClock::showTime);
    timer->start(1000);

    showTime();

    //setWindowTitle(tr("Digital Clock"));
    setDigitCount(8);
}

void QDigitalClock::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    //if ((time.second() % 2) == 0)
    //    text[2] = ' ';
    display(text);
}
