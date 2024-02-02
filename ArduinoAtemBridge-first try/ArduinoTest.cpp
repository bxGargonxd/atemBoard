#include "ArduinoTest.h"
#include "TimeoutSerial.h"
#include <iostream>

void ArduinoTest(Configuration& config) {
    /**
    /* test serial port for odd numbered PCBs
    **/
    TimeoutSerial serial_odd("COM" + std::to_string(config.get_port_odd()), config.get_baudrate()); // arduino with odd numbered PCBs (1,3,5,7)
    serial_odd.writeString("9\n");
    //serial_odd.writeString(startup_led(1));
    //serial_odd.writeString(startup_matrices(1));
    std::cout << "COM" << config.get_port_odd() << " connected." << std::endl;

    /**
    /* test serial port for even numbered PCBs
    **/
    TimeoutSerial serial_even("COM" + std::to_string(config.get_port_even()), config.get_baudrate()); // arduino with even numbered PCBs (1,3,5,7)
    serial_even.writeString("9\n");
    //serial_even.writeString(startup_led(1));
    //serial_even.writeString(startup_matrices(1));
    std::cout << "COM" << config.get_port_even() << " connected." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));
    serial_odd.writeString("9\n");
    serial_even.writeString("9\n");

    return;
}