#include "handleserialdata.h"

void handleSerialData(QString data, bool odd, QArduinoUps *chartview) {
    if (data.front() == 'U') {
        data = data.sliced(1);
        chartview->newValue(data, odd);
    }

    /*
    QChar prefix = text.front();
    QString payload = text.sliced(1);

    if (prefix == 'P') {
        this->appendPlainText("Poti values: " + payload);
    }
    else if (prefix == 'U') {
        this->appendPlainText("UPS: " + payload);
    }
    */
}
