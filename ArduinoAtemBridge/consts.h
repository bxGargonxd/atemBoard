#ifndef CONSTS_H
#define CONSTS_H

#include <QRect>

// Sizes
inline constexpr QSize MAINWINDOW_SIZE = QSize(1280,400);
inline constexpr QSize BUTTON_SIZE = QSize(128,50);

// Geometries
inline constexpr QRect MID_WIDGET_GEOMETRY = QRect(QPoint(15,60),QSize(1250,280));

// Serial settings
inline QString PORT_ODD = QString("/dev/tty.usbmodem1201");
inline QString PORT_EVEN = QString("");
inline constexpr qint32 BAUD_RATE = 115200;

#endif // CONSTS_H
