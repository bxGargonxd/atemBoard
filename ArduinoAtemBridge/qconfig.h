#ifndef QCONFIG_H
#define QCONFIG_H

#include <QFrame>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

class QConfig : public QFrame
{
    Q_OBJECT
public:
    QConfig(QWidget *parent = nullptr);

private:
    // columns
    QGroupBox *m_col_atem;
    QGroupBox *m_col_arduino_odd;
    QGroupBox *m_col_arduino_even;
    QGroupBox *m_col_stuff;

    // atem column
    QVBoxLayout *m_layout_atem;

    // arduino odd column
    QVBoxLayout *m_layout_arduino_odd;
    QLabel *m_odd_status_label;
    QLineEdit *m_odd_status;

    // arduino even column
    QVBoxLayout *m_layout_arduino_even;
    QLabel *m_even_status_label;

    // stuff column
    QVBoxLayout *m_layout_stuff;
    QLabel *m_stuff_test;


    // methods
    void update_information();
};

#endif // QCONFIG_H
