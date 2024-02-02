#ifndef QVIDEOBOARDVIEW_H
#define QVIDEOBOARDVIEW_H

#include <QFrame>
#include <QPushButton>

class QVideoBoardView : public QFrame
{
    Q_OBJECT

public:
    QVideoBoardView(QWidget *parent = nullptr);

private:
    std::array<QPushButton *, 20> m_me1_live;
    std::array<QPushButton *, 20> m_me1_prev;
};

#endif // QVIDEOBOARDVIEW_H
