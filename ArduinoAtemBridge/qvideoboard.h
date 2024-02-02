#ifndef QVIDEOBOARD_H
#define QVIDEOBOARD_H

#include <QMainWindow>
#include "qvideoboardview.h"

class QVideoBoard : public QMainWindow
{
    Q_OBJECT

public:
    QVideoBoard(const QString &title = "", QWidget *parent = nullptr);

private:
    QVideoBoardView *m_board_view;
};

#endif // QVIDEOBOARD_H
