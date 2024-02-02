#pragma once

#include <array>
#include <string>

#include "Buffer.h"
#include "Switcher.h"


/**
* load standard led pattern
* \param array give reference to the corresponding buffer array
*/
void led_start_up(std::array<uint8_t, 72>& array) noexcept;

/**
* returns a command string for the arduinos
* \param array give reference to the corresponding buffer array
*/
std::string led_string(std::array<uint8_t, 72> array);

/**
 * set all camera leds off and turn the leds for live and preview on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_cams (Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher);

/**
 * set all camera leds off and turn the leds for live and preview on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_hyperdeck(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher);

/**
 * set all transition leds off and turn the leds for relevant information on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_transition(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher);

/**
 * set all dsk leds off and turn the leds for relevant information on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_dsk(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher);

/**
 * set all fade to black leds off and turn the leds for relevant information on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_ftb(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher);

/**
 * set all leds off, exit led on
 * \param buffer_odd buffer for the odd arduino
 * \param buffer_even buffer for the even arduino
 * \param switcher Atem switcher which is connected
*/
void led_set_exit(Buffer& buffer_odd, Buffer& buffer_even);