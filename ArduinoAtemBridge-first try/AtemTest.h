#pragma once

#include "Configuration.h"

#include "Switcher.h"

/**
	* Runs a test programm to check the atem configurationn
	* \param config configuration for the running programm
	* \param switcher Swicher device to handle
*/
bool AtemTest(Configuration& config, Switcher &switcher);