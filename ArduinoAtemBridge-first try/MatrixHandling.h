#pragma once

#include <array>
#include <string>

/**
* returns a command string for the arduinos
* \param array give reference to the corresponding buffer array
*/
std::string matrix_string(std::array<std::array<char, 4>, 39> array, bool odd, char prefix);


/**
* load matrix pattern
* \param array give reference to the corresponding buffer array
*/
void matrix_start_up(std::array<std::array<char, 4>, 39>& array) noexcept;