#include "mainwindow.h"
#include "handleserialdata.h"
#include "qarduinoups.h"
#include "consts.h"

#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_serial_odd(new QSerialPort(this)),
    m_serial_even(new QSerialPort(this))
{
    // create video board
    m_video_board = new QVideoBoard("THE BOARD!!!");
    //m_video_board->setWindowFlag(Qt::FramelessWindowHint, true);
    //m_video_board->setGeometry(QRect(QPoint(0,0), MAINWINDOW_SIZE));
    m_video_board->show();

    // Create buttons
    m_button_home = new QPushButton("Home", this);
    m_button_connect = new QPushButton("Connect", this);
    m_button_disconnect = new QPushButton("Disconnect", this);
    m_button_time = new QPushButton("Time", this);
    m_button_config = new QPushButton("Config", this);
    m_button_ups = new QPushButton("UPS", this);
    m_button_serial_odd = new QPushButton("Serial odd", this);
    m_button_serial_even = new QPushButton("Serial even", this);
    m_button_clear = new QPushButton("Clear", this);

    // set sizes and locations of the buttons
    m_button_home->setGeometry(QRect(QPoint(0,0), BUTTON_SIZE));
    m_button_connect->setGeometry(QRect(QPoint(1024,0), BUTTON_SIZE));
    m_button_disconnect->setGeometry(QRect(QPoint(1152,0), BUTTON_SIZE));
    m_button_time->setGeometry(QRect(QPoint(0,350),BUTTON_SIZE));
    m_button_config->setGeometry(QRect(QPoint(128,350),BUTTON_SIZE));
    m_button_ups->setGeometry(QRect(QPoint(640,350),BUTTON_SIZE));
    m_button_serial_odd->setGeometry(QRect(QPoint(768,350), BUTTON_SIZE));
    m_button_serial_even->setGeometry(QRect(QPoint(896,350), BUTTON_SIZE));
    m_button_clear->setGeometry(QRect(QPoint(1152,350),BUTTON_SIZE));

    // connect serial ports to slots
    connect(m_serial_odd, &QSerialPort::readyRead, this, &MainWindow::readDataOdd);
    connect(m_serial_odd, &QSerialPort::bytesWritten, this, &MainWindow::handleBytesWrittenOdd);
    connect(m_serial_even, &QSerialPort::readyRead, this, &MainWindow::readDataEven);
    connect(m_serial_even, &QSerialPort::bytesWritten, this, &MainWindow::handleBytesWrittenEven);


    // connect button signals to slots
    connect(m_button_home, &QPushButton::released, this, &MainWindow::handleButtonHome);
    connect(m_button_connect, &QPushButton::released, this, &MainWindow::handleButtonConnect);
    connect(m_button_disconnect, &QPushButton::released, this, &MainWindow::handleButtonDisconnect);
    connect(m_button_time, &QPushButton::released, this, &MainWindow::handleButtonTime);
    connect(m_button_config, &QPushButton::released, this, &MainWindow::handleButtonConfig);
    connect(m_button_ups, &QPushButton::released, this, &MainWindow::handleButtonUps);
    connect(m_button_serial_odd, &QPushButton::released, this, &MainWindow::handleButtonSerialOdd);
    connect(m_button_serial_even, &QPushButton::released, this, &MainWindow::handleButtonSerialEven);
    connect(m_button_clear, &QPushButton::released, this, &MainWindow::hideMainWidgets);

    // create main widgets
    m_home_view = new QHomescreen(this);
    m_time_view = new QDigitalClock(this);
    m_config_view = new QConfig(this);
    m_ups_view = new QArduinoUps(this);
    m_serial_odd_view = new QSerialView(this);
    m_serial_even_view = new QSerialView(this);


    // hide all main widgets
    hideMainWidgets();
    m_home_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_home_view->show();
}

MainWindow::~MainWindow() {}

void MainWindow::handleButtonHome() {
    hideMainWidgets();
    m_home_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_home_view->show();
}

void MainWindow::handleButtonConnect() {
    hideMainWidgets();

    // connect ports
    m_serial_odd->setPortName(PORT_ODD);
    m_serial_odd->setBaudRate(BAUD_RATE);
    m_serial_even->setPortName(PORT_EVEN);
    m_serial_even->setBaudRate(BAUD_RATE);
    if (m_serial_odd->open(QIODevice::ReadWrite)) {
        m_serial_odd_view->setEnabled(true);
    }
    else {
        QMessageBox::critical(this, tr("Error"), "PORT_ODD: " + m_serial_odd->errorString());
    }
    if (m_serial_even->open(QIODevice::ReadWrite)) {
        m_serial_even_view->setEnabled(true);
    }
    else {
        QMessageBox::critical(this, tr("Error"), "PORT_EVEN: " + m_serial_even->errorString());
    }
}

void MainWindow::handleButtonDisconnect() {
    if (m_serial_odd->isOpen()) {
        m_serial_odd->close();
    }
    if (m_serial_even->isOpen()) {
        m_serial_even->close();
    }
    m_serial_odd_view->setEnabled(false);
    m_serial_even_view->setEnabled(false);
}

void MainWindow::handleButtonTime() {
    hideMainWidgets();
    m_time_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_time_view->show();
}

void MainWindow::handleButtonConfig() {
    hideMainWidgets();
    m_config_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_config_view->show();
}

void MainWindow::handleButtonUps()
{
    hideMainWidgets();
    m_ups_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_ups_view->show();
}

void MainWindow::handleButtonSerialOdd() {
    hideMainWidgets();
    m_serial_odd_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_serial_odd_view->show();
}

void MainWindow::handleButtonSerialEven() {
    hideMainWidgets();
    m_serial_even_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_serial_even_view->show();
}

void MainWindow::hideMainWidgets() {
    m_time_view->hide();
    m_config_view->hide();
    m_ups_view->hide();
    m_serial_odd_view->hide();
    m_serial_even_view->hide();
}

void MainWindow::readDataOdd() {
    const QString data = m_serial_odd->readLine();
    handleSerialData(data, true, m_ups_view);
    m_serial_odd_view->appendPlainText(data);
}

void MainWindow::handleBytesWrittenOdd() {

}

void MainWindow::readDataEven() {

}

void MainWindow::handleBytesWrittenEven() {

}
