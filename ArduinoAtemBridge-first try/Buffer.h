 #pragma once

#include "Mapping.h"

#include <array>
#include <string>
#include <vector>
#include "ButtonHandling.h"
#include "TimeoutSerial.h"
#include "Fader.h"
#include "FaderHandling.h"

class Buffer {
public:
	/**
	* Constructor.
	* \param board_odd bool to determine odd or even numbered pcbs.
	*/
	Buffer(bool board_odd);

	/**
	* interprets read string and stores data to local buffer
	* \param serial reference to the serial input
	* \return Tuple of vectors of the different command types
	*/
	std::tuple<std::vector<ButtonCommand>, std::vector<FaderCommand>> handleInput(TimeoutSerial& serial);

	/**
	 * sets the matrix buffer 5
	 * \param matrix number of the matrix to set up
	 * \param name name to write to the matrix
	*/
	void set_matrix_5(int matrix, std::string name);

	/**
	 * sets the matrix buffer 6
	 * \param matrix number
	 * \param name name to write to the matrix
	*/
	void set_matrix_6(int matrix, std::string name);

	/**
	 * returns the chosen matrix buffer
	 * \param number 5 or 6
	*/
	std::array<std::array<char, 4>, 39> get_matrix_buffer(int number) noexcept;

	/**
	 * getter for fader 1
	 * \return value of fader 1
	*/
	double get_fader_value_1() { return fdr_buffer.at(0).get_value(); };

	/**
	 * getter for fader 1
	 * \return value of fader 1
	*/
	double get_fader_value_2() { return fdr_buffer.at(1).get_value(); };

	/**
	 * getter for fader 1 start value
	 * \return start value of fader 1
  	*/
	double get_fdr1_start() { return fdr_buffer.at(0).get_start(); };

	/**
	 * setter for fader 1 start value
	 * \param val desired start value for fader 1
  	*/
	void set_fdr1_start(double val) { fdr_buffer.at(0).set_start(val); };

	/**
	 * set fader 1 active
  	*/
	void set_fdr1_active() { fdr_buffer.at(0).set_active(true); };

	/**
	 * set fader 1 inactive
	*/
	void set_fdr1_inactive() { fdr_buffer.at(0).set_active(false); };

	/**
	 * getter for fader 2 start value
	 * \return start value of fader 2
  	*/
	double get_fdr2_start() { return fdr_buffer.at(1).get_start(); };

	/**
	 * setter for fader 2 start value
	 * \param val desired start value for fader 2
  	*/
	void set_fdr2_start(double val) { fdr_buffer.at(1).set_start(val); };

	/**
	 * set fader 2active
  	*/
	void set_fdr2_active() { fdr_buffer.at(1).set_active(true); };

	/**
	 * set fader 2 inactive
  	*/
	void set_fdr2_inactive() { fdr_buffer.at(1).set_active(false); };

	/**
	 * getter for fader 1 status
	 * \return fader 1 status
	*/
	bool get_fdr1_status() { return fdr_buffer.at(0).get_active(); };

	/**
	 * getter for fader 2 status
	 * \return fader 2 status
	*/
	bool get_fdr2_status() { return fdr_buffer.at(1).get_active(); };

	/**
	 * getter for the led buffer
	 * \return reference to the led buffer array
  	*/
	std::array<uint8_t, 72>& get_led_buffer() noexcept { return led_buffer; };

	/**
	 * setter for the led buffer
	 * \param reg number of the led register to change
	 * \param num number to write to the chosen register
  	*/
	void set_led_buffer(int reg, int num) { led_buffer.at(reg) = num; };

	/**
	 * clear the whole led buffer
  	*/
	void led_clear_preview();

	/**
	 * switches a specified led on
	 * \param arr array number
	 * \param bit bit number
  	*/
	void led_set_on(int arr, int bit);

	/**
	 * getter for the button buffer
	 * \return reference to the button buffer array
	*/
	std::array<uint8_t, 72>& get_btn_buffer() noexcept { return btn_buffer; };

private:
	const bool odd;
	std::array<uint8_t, 72> led_buffer; // buffer containing all leds, sepperated in 72 parts, one bit per led
	std::array<std::array<char, 4>, 39> matrix_buffer_5; // buffer containing all matrices which are addressed with the prefix 5
	std::array<std::array<char, 4>, 39> matrix_buffer_6; // buffer containing all matrices which are addressed with the prefix 6
	std::array<uint8_t, 72> btn_buffer; // buffer containing all button states, sepperated in 72 parts, one bit per button
	std::array<Fader, 2> fdr_buffer; // buffer containing the current state of both faders

	const Mapping maps; // contains all needed maps

	/**
	 * Parses a data string to button commands
	 * \param data Data string
	 * \param cmds Vector accepting the parsed commands
	*/
	void handleButtonInput(std::string* data, std::vector<ButtonCommand>* cmds);

	/**
	 * Parses a data string to fader commands
	 * \param data Data string
	 * \param cmds Vector accepting the parsed commands
  	*/
	void handleFaderInput(const std::string* data, std::vector<FaderCommand>* cmds);
};