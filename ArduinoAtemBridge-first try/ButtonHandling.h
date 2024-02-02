#pragma once

#include <array>
#include <string>
#include <vector>

#include "Mapping.h"


class ButtonCommand {
public:
	/**
	 * Constructor.
	*/
	ButtonCommand() noexcept;

	/**
	 * Constructor.
	 * \param str button name
	 * \param state if the button was pressed or released
	*/
	ButtonCommand(std::string& str, bool state);

	/**
	 * getter for name
	 * \return name of the current button command
  	*/
	std::string get_name() { return name; };
	/**
	 * getter for pressed state
	 * \return true if button is pressed
  	*/
	bool get_state() noexcept { return pressed; };
	/**
	 * getter for odd
	 * \return true if pcb number is odd
  	*/
	bool get_odd() noexcept { return odd; };

private:
	bool odd; // if pcb number is odd
	bool pressed; // if button is pressed
	std::string name; // current command name
};

/**
* load button states at start
* \param array give reference to the corresponding buffer array
*/
void button_start_up(std::array<uint8_t, 72>& array) noexcept;

/**
 * Compares two button buffers, determines the differences and updates commands to be triggered
 * \param oldStates old button buffer
 * \param newStates new button buffer
 * \param maps Maps containg all needed commands
 * \odd odd or even numbered pcbs
 * \cmds vector containing commands to be executed
*/
void checkButtonChange(std::array<uint8_t, 72>& oldStates, const std::array<uint8_t, 72>& newStates, const Mapping& maps, const bool odd, std::vector<ButtonCommand>* cmds);
