#include "SerialHandler.h"

SerialHandler::SerialHandler(int port_odd, int port_even, int rate) :
buffer_odd(true),
buffer_even(false)
{
	serial_odd.open("COM" + std::to_string(port_odd), rate);
	serial_even.open("COM" + std::to_string(port_even), rate);
}