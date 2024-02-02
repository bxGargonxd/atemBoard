#include "FaderHandling.h"

FaderCommand::FaderCommand(double val, bool fdr, bool od) noexcept {
	odd = od;
	value = val;
	fader = fdr;
}

void checkFaderChange(std::array<Fader, 2>& oldStates, const std::array<int, 2>& newStates, const bool odd, std::vector<FaderCommand>* cmds) {

	if (cmds == NULL) { return; };

	// run this for both faders
	for (int i = 0; i < 2; i++) {
		// move value from int to double
		double value = newStates.at(i);

		// define dead ends
		value = value - 100; // [0;1023] -> [-100;923]
		if (value < 0) { value = 0; } // [-100;923] -> [0;923]
		else if (value > 823) { value = 823; }; // [0;923] -> [0;823]
		/* old shit
		value = value - 110; // [0;1023] -> [-110;913]
		if (value < 0) { value = 0; } // [-110;913] -> [0;913]
		else if (value > 800) { value = 800; }; // [0;913] -> [0;800]
		*/

		// move value from range [0-800] to [0-1]
		value = value / 823;

		// compare values
		if (oldStates.at(i).get_value() != value) {
			FaderCommand const cmd(value, i, odd);
			cmds->push_back(cmd);
			oldStates.at(i).set_value(value);
		}
	}

	return;
}