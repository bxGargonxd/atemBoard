#ifndef QDIGITALCLOCK_H
#define QDIGITALCLOCK_H

#include <QLCDNumber>

class QDigitalClock : public QLCDNumber
{
    Q_OBJECT

public:
    QDigitalClock(QWidget *parent = nullptr);

private slots:
    void showTime();
};

#endif // QDIGITALCLOCK_H
