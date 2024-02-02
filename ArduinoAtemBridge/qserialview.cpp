#include "qserialview.h"

QSerialView::QSerialView(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setLineWrapMode(NoWrap);
    setWordWrapMode(QTextOption::NoWrap);
    setReadOnly(true);
    setEnabled(false);
}

void QSerialView::handleInput(const QString & text) {
    QChar prefix = text.front();
    QString payload = text.sliced(1);

    if (prefix == 'P') {
        this->appendPlainText("Poti values: " + payload);
    }
    else if (prefix == 'U') {
        this->appendPlainText("UPS: " + payload);
    }
}
