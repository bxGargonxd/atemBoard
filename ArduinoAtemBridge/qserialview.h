#ifndef QSERIALVIEW_H
#define QSERIALVIEW_H

#include <QPlainTextEdit>

class QSerialView : public QPlainTextEdit
{
    Q_OBJECT

public:
    QSerialView(QWidget *parent = nullptr);
    void handleInput(const QString & text);
};

#endif // QSERIALVIEW_H
