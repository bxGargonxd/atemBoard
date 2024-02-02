#pragma once

#include <queue>

#include "Configuration.h"
#include "Switcher.h"
#include "Buffer.h"
#include "SerialHandler.h"

/**
	* Runs the real deal
	* \param config configuration for the running programm
	* \param atem Switcher object
*/
void Live(Configuration& config, Switcher& atem);

/**
 * runns the handle input function for a thread
*/
void manageInput(SerialHandler* handler, bool odd, std::queue<ButtonCommand>* btn_cmds, std::queue<FaderCommand>* fdr_cmds);

/**
 * runns the execute commands function for a thread
 * \param cmds Vector containing the button commands to execute
 * \param switcher Switcher object which should execute the commands
 * \param up Determines if the matrices should be updated
*/
void executeBtnCommands(std::queue<ButtonCommand>* cmds, Switcher* switcher, int* upMtx, int* upLEDs);

/**
 * runns the execute commands function for a thread
 * \param cmds Vector containing the fader commands to execute
 * \param switcher Switcher object which should execute the commands
 * \param serialHandler Handler object which contains the needed fader buffers
*/
void executeFdrCommands(std::queue<FaderCommand>* cmds, Switcher* switcher, SerialHandler* serialHandler);


void writeString(SerialHandler* handler, bool odd, int prefix);

void writeLEDs(SerialHandler* handler, bool odd);