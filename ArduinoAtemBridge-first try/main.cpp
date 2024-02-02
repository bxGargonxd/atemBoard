#include <iostream>
#include <span>
#include <vector>
#include "TimeoutSerial.h"
#include "Configuration.h"
#include "ArduinoTest.h"
#include "AtemTest.h"
#include "Live.h"
#include "Switcher.h"

/**
	* Argument1: config file
*/

int main(int argc, const char* argv[]) {
	/**
	 * check if only one argument is given
	*/
	if (argc != 2) {
		std::cout << "Error: You need to provide exactly one config file." << std::endl;
		return 1;
	}

	std::vector<std::string> arguments;
	for (auto i : std::span(argv, argc)) {
		arguments.push_back(i);
	}

	Configuration config(arguments.at(1));

	if (config.get_mode() == "arduino_test") {
		try {
			ArduinoTest(config);
			return 0;
		}
		catch (const boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			return 1;
		}
	}

	Switcher atem(config.get_ipaddress());

	if (config.get_mode() == "atem_test") {
		std::cout << "Atem test completed." << std::endl;
		return 0;
	}

	if (config.get_mode() == "live") {
		Live(config, atem);
	}

	return 0;
}