#include "qvideoboardview.h"

#include <QPushButton>

const QSize BUTTON_SIZE = QSize(20,25);

QVideoBoardView::QVideoBoardView(QWidget *parent)
    : QFrame(parent),
    m_me1_live(std::array<QPushButton *,20>()),
    m_me1_prev(std::array<QPushButton *,20>())
{
    // set up buttons
    for (size_t i = 0; i < m_me1_live.size(); i++) {
        QString name = QString(char(i));
        m_me1_live[i] = new QPushButton(name, this);
        m_me1_live[i]->resize(BUTTON_SIZE);
        m_me1_live[i]->move(i*30+10,545);
    }
    for (size_t i = 0; i < m_me1_prev.size(); i++) {
        QString name = QString(char(i));
        m_me1_prev[i] = new QPushButton(name, this);
        m_me1_prev[i]->resize(BUTTON_SIZE);
        m_me1_prev[i]->move(i*30+10,515);
    }
}
