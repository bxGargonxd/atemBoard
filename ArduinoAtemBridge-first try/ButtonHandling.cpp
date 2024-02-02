#include "ButtonHandling.h"
#include <iostream>

ButtonCommand::ButtonCommand() noexcept{
	pressed = false;
	name = "";
}


ButtonCommand::ButtonCommand(std::string& str, bool state) {
	name = str;
	pressed = state;
}


void button_start_up(std::array<uint8_t, 72>& array) noexcept
{
	for (uint8_t& val : array) {
		val = 0b11111111; // set all buttons as released
	}
	return;
}


void checkButtonChange(std::array<uint8_t, 72>& oldStates, const std::array<uint8_t, 72>& newStates, const Mapping& maps, const bool odd, std::vector<ButtonCommand>* cmds) {
	if (cmds == NULL) { return; };

	for (size_t i = 0; i < oldStates.size(); i++) { // iterate through the registers
		if (oldStates.at(i) != newStates.at(i)) { // check if the register differs
			const uint16_t offset = static_cast<uint16_t>(i) * static_cast<uint16_t>(8); // offset for the numbers in the register (e.g. for i=2 => the first button got number 2*8+1=17
			for (int j = 0; j < 8; j++) { // iterate through the 8 buttons of the current register
				const uint8_t mask = 1 << j; // j = 3 => 00001000
				if ((oldStates.at(i) & mask) != (newStates.at(i) & mask)) {
					if (odd == true) {
						if (maps.buttons_odd.find(offset + j) != maps.buttons_odd.end()) { // if button number is mapped
							std::string button = maps.buttons_odd.find(offset + j)->second; // get the current buttons name
							const bool pressed = ((oldStates.at(i) & mask) >> j) == 1; // if the button is pressed the old state should be not pressed
							//std::cout << "Button: " << button << "     pressed: " << pressed << std::endl;
							ButtonCommand newCom(button, pressed);
							cmds->push_back(newCom);
						}
						else {
							//std::cout << "Pressed button not mapped." << std::endl;
							//std::cout << "Boards: odd" << std::endl;
							//std::cout << "Button number: " << offset + j << std::endl;
						}
					}
					else if (odd == false) {
						if (maps.buttons_even.find(offset + j) != maps.buttons_even.end()) { // if button number is mapped
							std::string button = maps.buttons_even.find(offset + j)->second; // get the current buttons name
							const bool pressed = ((oldStates.at(i) & mask) >> j) == 1; // if the button is pressed the old state should be not pressed
							//std::cout << "Button: " << button << "     pressed: " << pressed << std::endl;
							ButtonCommand newCom(button, pressed);
							cmds->push_back(newCom);
						}
						else {
							//std::cout << "Pressed button not mapped." << std::endl;
							//std::cout << "Boards: even" << std::endl;
							//std::cout << "Button number: " << offset + j << std::endl;
						}
					}
				}
			}
		}
		oldStates.at(i) = newStates.at(i);
	}
	return;
}