#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <set>
#include <regex>


Configuration::Configuration(std::string file)
{
    // initialize all neccassary variables
    mode = "full";
    port_even = 2;
    port_odd = 1;
    baudrate = 1200;

    // parse the config file
    std::string line;
    std::ifstream config_file;
    config_file.open(file);

    // parse each line, check for '=', check for command and store value into the variable
    while (std::getline(config_file, line))
    {
        const size_t pos = line.find_first_of("=");
        if (pos != -1) { // if '=' was found, split string before and after '='
            std::string cmd(line, 0, pos); // copy from other string (line), start at, end at
            std::string value(line, pos + 1, line.length() - 1);

            // store the value in the respective command
            if (cmd == "MODE") {
                if (validate_mode(value)) {
                    mode = value;
                }
                else {
                    std::cout << "Received invalid mode value." << std::endl;
                }
            }
            else if (cmd == "PORT_EVEN") {
                if (validate_port_even(value)) {
                    port_even = std::stoi(value);
                }
                else {
                    std::cout << "Received invalid port_even value." << std::endl;
                }
            }
            else if (cmd == "PORT_ODD") {
                if (validate_port_odd(value)) {
                    port_odd = std::stoi(value);
                }
                else {
                    std::cout << "Received invalid port_odd value." << std::endl;
                }
            }
            else if (cmd == "BAUDRATE") {
                const int baud = std::stoi(value);
                if (baud > 0) {
                    baudrate = baud;
                }
                else {
                    std::cout << "Received invalid baud rate." << std::endl;
                }
            }
            else if (cmd == "IPADDRESS") {
                ipAddress = value;
            }
            else {
                std::cout << "Received invalid command " << cmd << "." << std::endl;
            }
        }
    }
    std::cout << "Config loaded." << std::endl;
}


bool Configuration::validate_mode(std::string value) {
    std::set<std::string> modes = { "arduino_test", "atem_test", "live"};
    if (modes.find(value) == modes.end()) {
        return false;
    }
    else {
        return true;
    }
}

bool Configuration::validate_port_even(std::string value) {
    return regex_match(value, std::regex("[1-9]"));
}

bool Configuration::validate_port_odd(std::string value) {
    return regex_match(value, std::regex("[1-9]"));
}