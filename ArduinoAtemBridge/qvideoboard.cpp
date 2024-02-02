#include "qvideoboard.h"

#include <QPushButton>

QVideoBoard::QVideoBoard(const QString &title, QWidget *parent)
    : QMainWindow(parent),
    m_board_view(new QVideoBoardView(this))
{
    setWindowTitle(title);
    setWindowFlag(Qt::WindowStaysOnTopHint, true); // stay on
    resize(1024,576);
    setCentralWidget(m_board_view);

}
