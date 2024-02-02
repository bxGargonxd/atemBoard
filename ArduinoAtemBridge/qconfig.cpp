#include "qconfig.h"

#include <QLabel>
#include <QVBoxLayout>


QConfig::QConfig(QWidget *parent)
    : QFrame{parent}
{
    // left column
    m_col_atem = new QGroupBox("Atem",this);
    m_col_atem->setGeometry(0,0,200,280);
    m_layout_atem = new QVBoxLayout(m_col_atem);
    auto first = new QLabel("Test");
    m_layout_atem->addWidget(first);


    // midleft column
    m_col_arduino_odd = new QGroupBox("Arduino Odd",this);
    m_col_arduino_odd->setGeometry(250,0,200,280);
    m_layout_arduino_odd = new QVBoxLayout(m_col_arduino_odd);
    m_odd_status_label = new QLabel("Status:");
    m_odd_status = new QLineEdit();
    m_odd_status->setReadOnly(true);

    m_layout_arduino_odd->addWidget(m_odd_status_label);
    m_layout_arduino_odd->addWidget(m_odd_status);

    // midright column
    m_col_arduino_even = new QGroupBox("Arduino Even",this);
    m_col_arduino_even->setGeometry(500,0,200,280);
    m_layout_arduino_even = new QVBoxLayout(m_col_arduino_even);
    m_even_status_label = new QLabel("Status:");

    m_layout_arduino_even->addWidget(m_even_status_label);

    // right column
    m_col_stuff = new QGroupBox("Other Stuff",this);
    m_col_stuff->setGeometry(750,0,200,280);
    m_layout_stuff = new QVBoxLayout(m_col_stuff);
    m_stuff_test = new QLabel("Test");

    m_layout_stuff->addWidget(m_stuff_test);

    update_information();
}

void QConfig::update_information() {
    // Atem

    // Arduino odd
    m_odd_status->setText("Test");

    // Arduino even

    // Stuff
}
