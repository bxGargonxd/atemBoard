#include "qarduinoups.h"
#include "consts.h"

#include <QLineSeries>
#include <QTime>
#include <QTimer>
#include <QDebug>

QArduinoUps::QArduinoUps(QWidget *parent)
    : QFrame(parent)
{
    starttime = QTime::currentTime().msecsSinceStartOfDay();
    last_ups_odd = std::queue<int>();
    m_chart = new QChart;
    m_series_odd = new QLineSeries;
    m_series_even = new QLineSeries;
    m_chart_view = new QChartView(this);

    m_series_odd->setName("Odd");
    m_series_even->setName("Even");
    m_chart->setTitle("UPS of both Arduinos");
    m_chart->createDefaultAxes();
    m_chart->addSeries(m_series_odd);
    m_chart->addSeries(m_series_even);


    m_chart_view->setChart(m_chart);
    m_chart_view->setGeometry(MID_WIDGET_GEOMETRY);
    m_chart_view->move(0,0);
    m_chart_view->show();
}

void QArduinoUps::refresh() {
    //m_chart->removeAllSeries();
    //m_chart->addSeries(m_series_odd);
    //repaint();
}


void QArduinoUps::newValue(QString data, bool odd) {
    int time = QTime::currentTime().msecsSinceStartOfDay() - starttime;
    if (odd) {
        last_ups_odd.push(data.toInt());
        if (last_ups_odd.size() > 99) {
            float sum = 0;
            while (!last_ups_odd.empty()){
                sum += last_ups_odd.front();
                last_ups_odd.pop();
            }
            last_ups_odd = std::queue<int>();
            m_series_odd->append((time/1000),sum/100);
            m_chart->removeSeries(m_series_odd);
            m_chart->addSeries(m_series_odd);
            m_chart->createDefaultAxes();
        }
    }
}
