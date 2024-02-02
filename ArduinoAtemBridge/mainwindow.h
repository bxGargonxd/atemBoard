#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QPushButton>


#include "qhomescreen.h"
#include "qarduinoups.h"
#include "qdigitalclock.h"
#include "qconfig.h"
#include "qserialview.h"
#include "qvideoboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // serial handling
    void readDataOdd();
    void handleBytesWrittenOdd();
    void readDataEven();
    void handleBytesWrittenEven();

    // button handling
    void handleButtonHome();
    void handleButtonConnect();
    void handleButtonDisconnect();
    void handleButtonTime();
    void handleButtonConfig();
    void handleButtonUps();
    void handleButtonSerialOdd();
    void handleButtonSerialEven();
    void hideMainWidgets();


private:
    // serials
    QSerialPort *m_serial_odd;
    QSerialPort *m_serial_even;

    // buttons
    QPushButton *m_button_home;
    QPushButton *m_button_connect;
    QPushButton *m_button_disconnect;
    QPushButton *m_button_time;
    QPushButton *m_button_config;
    QPushButton *m_button_ups;
    QPushButton *m_button_serial_odd;
    QPushButton *m_button_serial_even;
    QPushButton *m_button_clear;

    // main widgets
    QHomescreen *m_home_view;
    QDigitalClock *m_time_view;
    QConfig *m_config_view;
    QArduinoUps *m_ups_view;
    QSerialView *m_serial_odd_view;
    QSerialView *m_serial_even_view;

    // Video board
    QVideoBoard *m_video_board;
};
#endif // MAINWINDOW_H
