#pragma once

#include <string>

/**
 * Class to set up the ArduinoAtemBridge
*/
class Configuration {
private:
    std::string mode;
    int port_even;
    int port_odd;
    int baudrate;
    std::string ipAddress;

    /**
     * validates if the given mode is implemented
     * \param value name of the given mode
     * \return true if the given mode is implemented
    */
    bool validate_mode(std::string value);

    /**
     * validates if the given com port is plausible
     * \param value name of the given com port
     * \return true if the given com port is plausible
    */
    bool validate_port_even(std::string value);

    /**
    * validates if the given com port is plausible
    * \param value name of the given com port
    * \return true if the given com port is plausible
    */
    bool validate_port_odd(std::string value);

public:
    /**
    * Constructor.
    * \param file file name including path
    * \throw incorrect command or value
    * reads the file line wise and parses every config to its variable
    */
    Configuration(std::string file);

    /**
     * get mode variable
     * \return programm mode
    */
    std::string get_mode() { return mode; };

    /**
     * get port_even variable
     * \return com port of the even numbered arduino
    */
    int get_port_even() noexcept { return port_even; };

    /**
     * get port_odd variable
     * \return com port of the odd numbered arduino
    */
    int get_port_odd() noexcept { return port_odd; };

    /**
     * get baudrate variable
     * \return baudrate for the arduinos
    */
    int get_baudrate() noexcept { return baudrate; };

    /**
     * get ipAddress variable
     * \return ip address of the atem
    */
    std::string get_ipaddress() { return ipAddress; };
};