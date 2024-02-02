#include "Buffer.h"

#include <iostream>
#include <unordered_map>

#include "LEDHandling.h"
#include "MatrixHandling.h"
#include "ButtonHandling.h"

// map to convert a hex char to int
const std::unordered_map<char, int> hexToInt =
{
	{'0', 0},
	{'1', 1},
	{'2', 2},
	{'3', 3},
	{'4', 4},
	{'5', 5},
	{'6', 6},
	{'7', 7},
	{'8', 8},
	{'9', 9},
	{'A', 10},
	{'B', 11},
	{'C', 12},
	{'D', 13},
	{'E', 14},
	{'F', 15}
};

Buffer::Buffer(bool board_odd) :
	odd(board_odd),
	maps()
	{
	led_start_up(led_buffer); // load the start up pattern for the leds
	matrix_start_up(matrix_buffer_5); // load the start up pattern for the matrices
	matrix_start_up(matrix_buffer_6); // load the start up pattern for the matrices
	button_start_up(btn_buffer); // load the start up pattern for the matrices
	fdr_buffer.at(0) = Fader();
	fdr_buffer.at(1) = Fader();
}

std::tuple<std::vector<ButtonCommand>, std::vector<FaderCommand>> Buffer::handleInput(TimeoutSerial& serial) {

  	// create empty command vectors and read serial input as a string
	std::vector<ButtonCommand> btn_cmds{};
	std::vector<FaderCommand> fdr_cmds{};
	std::string line = serial.readStringUntil();

	// 1. char of string determines if the data represents buttons or levers
	const char prefix = line.front();
	std::string data = line.substr(1); // datastring, line string minus the first char 

	switch (prefix) {
	case 'B':
		handleButtonInput(&data, &btn_cmds);
		break;

	case 'P':
		handleFaderInput(&data, &fdr_cmds);
		break;
	case 'U':
		//std::cout << "Arduino Ups: " << data << std::endl;
		break;
	default:
		std::cout << "Could not handle input string." << std::endl;
		std::cout << "Prefix: " << prefix << std::endl;
		std::cout << "Data: " << data << std::endl;
		break;
	}

	return { btn_cmds, fdr_cmds };
}

void Buffer::set_matrix_5(int matrix, std::string name) {
	for (int i = 0; i < 4; i++) {
		if (name.empty() != true) {
			matrix_buffer_5.at(matrix).at(i) = name.front();
			name.erase(name.begin());
		}
		else {
			matrix_buffer_5.at(matrix).at(i) = ' ';
		}
	}
}

void Buffer::set_matrix_6(int matrix, std::string name) {
	for (int i = 0; i < 4; i++) {
		if (name.empty() != true) {
			matrix_buffer_6.at(matrix).at(i) = name.front();
			name.erase(name.begin());
		}
		else {
			matrix_buffer_6.at(matrix).at(i) = ' ';
		}
	}
}

std::array<std::array<char, 4>, 39> Buffer::get_matrix_buffer(int number) noexcept {
	switch (number){
		case 5: {
			return matrix_buffer_5;
		}
		case 6: {
			return matrix_buffer_6;
		}
		default: {}
	}
	return std::array<std::array<char, 4>, 39> {};
}

void Buffer::led_clear_preview()
{
	for (int i = 0; i < 72; i++) {
		led_buffer.at(i) = 0;
	}
}

void Buffer::led_set_on(int arr, int bit) {
	int current = led_buffer.at(arr);
	int mask = 2 * bit + 1;
	int next = current | mask;
	led_buffer.at(arr) = next;
}

void Buffer::handleButtonInput(std::string* data, std::vector<ButtonCommand>* cmds) {
	if (data == NULL) { return; };

	// Button data string has have a length off 72
	const auto len = data->length() / 2;
	if (len != 72) {
		return;
	}

	std::array<uint8_t, 72> new_btn_buffer{};

	// load two chars into each array element by converting them from hex to int
	// e.g.: AB = 10*16+11; A=10, B=11
	for (size_t i = 0; i < len; i++) {
		new_btn_buffer.at(i) = hexToInt.at(data->at(2 * i));
		new_btn_buffer.at(i) *= 16;
		new_btn_buffer.at(i) += hexToInt.at(data->at(2 * i + 1));
	}

	checkButtonChange(btn_buffer, new_btn_buffer, maps, odd, cmds);

	return;
}

void Buffer::handleFaderInput(const std::string* data, std::vector<FaderCommand>* cmds) {
	if (data == NULL) { return; };

	// split the data string into the two values representing two faders
	const auto split = data->find_first_of(';');
	std::string val_str_1 = data->substr(0, split);
	std::string val_str_2 = data->substr(split + 1, data->size() - split - 2);

	// convert the first value from hex to int, range [0-1023]
	int val1 = 0;
	int exp = 0;
	while (!val_str_1.empty()) {
		val1 = val1 + hexToInt.at(val_str_1.back()) * static_cast<int>(pow(10, exp));
		val_str_1.pop_back();
		exp++;
	}

	// convert the second value from hex to int, range [0-1023]
	int val2 = 0;
	exp = 0;
	while (!val_str_2.empty()) {
		val2 = val2 + hexToInt.at(val_str_2.back()) * static_cast<int>(pow(10, exp));
		val_str_2.pop_back();
		exp++;
	}

	// create a new fader buffer
	std::array<int, 2> const new_fdr_buffer{ val1, val2 };

	checkFaderChange(fdr_buffer, new_fdr_buffer, odd, cmds);

	return;
}