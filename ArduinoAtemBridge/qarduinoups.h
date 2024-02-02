#ifndef QARDUINOUPS_H
#define QARDUINOUPS_H

#include <queue>
#include <QFrame>
#include <QChartView>
#include <QLineSeries>

class QArduinoUps : public QFrame
{
    Q_OBJECT

public:
    QArduinoUps(QWidget *parent = nullptr);
    void newValue(QString value, bool odd);

private slots:
    void refresh();

private:
    int starttime;
    std::queue<int> last_ups_odd;
    QChart *m_chart;
    QLineSeries *m_series_odd;
    QLineSeries *m_series_even;
    QChartView *m_chart_view;

};
#endif // QARDUINOUPS_H
