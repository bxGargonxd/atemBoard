#pragma once
#include <unordered_map>
#include <string>

class Mapping
{
public:
	/** 
	 * Constructor.
	*/
	Mapping() noexcept;


	std::unordered_map<int, std::string> buttons_odd;
	std::unordered_map<int, std::string> buttons_even;
	std::unordered_map<std::string, int> leds_odd;
	std::unordered_map<std::string, int> leds_even;

private:
};

