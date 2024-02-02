#pragma once

#include "TimeoutSerial.h"
#include "Buffer.h"

class SerialHandler
{
public:
	/**
	 * Constructor.
	 * \param port COM port number
	 * \param rate baud rate
	*/
	SerialHandler(int port_odd, int port_even, int rate);


	/**
	* getter functions
	*/
	Buffer& get_buffer_odd() noexcept { return buffer_odd; };
	Buffer& get_buffer_even() noexcept { return buffer_even; };
	TimeoutSerial& get_serial_odd() noexcept { return serial_odd; };
	TimeoutSerial& get_serial_even() noexcept { return serial_even; };

private:
	TimeoutSerial serial_odd;
	TimeoutSerial serial_even;
	Buffer buffer_odd;
	Buffer buffer_even;
};