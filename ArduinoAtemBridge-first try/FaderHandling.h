#pragma once

#include <array>
#include <vector>

#include "Fader.h"

class FaderCommand {
public:
	/**
	 * Constructor.
	 * \param val Fader value
	 * \param fdr First or second fader
	 * \param od Fader is on the odd or even numbered pcbs
	*/
	FaderCommand(double val, bool fdr, bool od) noexcept;

	/**
	 * Getter for the fader
	 * \return fader
  	*/
	bool get_fader() noexcept { return fader; };

	/**
	 * Getter for the value
	 * \return value
  	*/
	double get_value() noexcept { return value; };

	/**
	 * Getter for the odd/even state
	 * \return true for odd, false for even
	*/
	bool get_odd() noexcept { return odd; };

private:
	bool odd;
	double value;
	bool fader;
};

/**
 * Compares two fader buffers and determines the differences
 * \param oldStates old fader buffer
 * \param newStates new fader buffer
 * \odd odd or even numbered pcbs
 * \cmds vector containing commands to be executed
*/
void checkFaderChange(std::array<Fader, 2>& oldStates, const std::array<int, 2>& newStates, const bool odd, std::vector<FaderCommand>* cmds);