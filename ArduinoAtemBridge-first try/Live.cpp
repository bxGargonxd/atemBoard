#include "Live.h"
#include "TimeoutSerial.h"
#include "LEDHandling.h"
#include "MatrixHandling.h"
#include "ButtonHandling.h"
#include "FaderHandling.h"

#include <Windows.h>
#include <conio.h>

#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <bitset>



void Live(Configuration& config, Switcher& atem) {

	//
	// INITIALIZATION
	//
	bool cancel = false;
	int led_number = 0;

	SerialHandler serialHandler(config.get_port_odd(), config.get_port_even(), config.get_baudrate());

	// get input names
	std::array<std::string, 57> cameras = atem.get_input_names_short();

	// update some things
	atem.update_switcher_keys();
	atem.update_hyperdecks();

	// update camera matrices
	for (int i = 1; i < 7; i++) {
		serialHandler.get_buffer_odd().set_matrix_5(i - 1 + 6, cameras.at(i)); // take inputs 1-6 and write them to the corresponding matrices 7-12
		serialHandler.get_buffer_odd().set_matrix_5(i - 1 + 12, cameras.at(i)); // take inputs 1-6 and write them to the corresponding matrices 13-18
	}
	for (int i = 7; i < 20; i++) { // take inputs 7-19 and write them to the corresponding matrices
		serialHandler.get_buffer_even().set_matrix_5(i - 7 + 13, cameras.at(i));
		serialHandler.get_buffer_even().set_matrix_5(i - 7 + 26, cameras.at(i));
	}

	// get transition information
	atem.get_transition_frames();

	// update transition matrix buffers
	serialHandler.get_buffer_odd().set_matrix_6(9, std::to_string(atem.get_ME1_trans()));
	serialHandler.get_buffer_odd().set_matrix_6(4, std::to_string(atem.get_ME2_trans()));
	serialHandler.get_buffer_even().set_matrix_6(0, std::to_string(atem.get_FTB_trans()));

	// input queues from both arduinos
	std::queue<ButtonCommand> btn_cmds_odd{};
	std::queue<ButtonCommand> btn_cmds_even{};
	std::queue<FaderCommand> fdr_cmds_odd{};
	std::queue<FaderCommand> fdr_cmds_even{};



	// 
	// LOOPS
	//
	int count = 0;
	int dount = 0;

	int updateMatrices = 0;
	int updateLEDs = 0;

	while (cancel == false) {
		// Shutdown routine
		cancel = atem.get_cancel();
		if (cancel) {
			led_set_exit(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even());

			// concurrent update of the leds
			std::thread odd_leds(writeLEDs, &serialHandler, true);
			std::thread even_leds(writeLEDs, &serialHandler, false);
			odd_leds.join();
			even_leds.join();

			count = 0;
		}


		count += 1;
		dount += 1;

		atem.update_switcherTime();

		// concurrent reading of both arduinos
		std::thread serial_odd_read(manageInput, &serialHandler, true, &btn_cmds_odd, &fdr_cmds_odd);
		std::thread serial_even_read(manageInput, &serialHandler, false, &btn_cmds_even, &fdr_cmds_even);
		serial_odd_read.join();
		serial_even_read.join();

		// concurrent execution of all command queues
		std::thread odd_btn_commands(executeBtnCommands, &btn_cmds_odd, &atem, &updateMatrices, &updateLEDs);
		std::thread even_btn_commands(executeBtnCommands, &btn_cmds_even, &atem, &updateMatrices, &updateLEDs);
		std::thread odd_fdr_commands(executeFdrCommands, &fdr_cmds_odd, &atem, &serialHandler);
		std::thread even_fdr_commands(executeFdrCommands, &fdr_cmds_even, &atem, &serialHandler);
		odd_btn_commands.join();
		even_btn_commands.join();
		odd_fdr_commands.join();
		even_fdr_commands.join();

		

		// read relevant information from atem
		atem.get_transition_frames();
		atem.update_live_cams();
		atem.update_preview_cams();

		// update matrices on pressing ME.extra
		if (updateMatrices > 1) {
			cameras = atem.get_input_names_short();

			// update matrix buffers
			serialHandler.get_buffer_odd().set_matrix_6(9, std::to_string(atem.get_ME1_trans()));
			serialHandler.get_buffer_odd().set_matrix_6(4, std::to_string(atem.get_ME2_trans()));
			serialHandler.get_buffer_even().set_matrix_6(0, std::to_string(atem.get_FTB_trans()));

			// update camera matrices
			for (int i = 1; i < 7; i++) {
				serialHandler.get_buffer_odd().set_matrix_5(i - 1 + 6, cameras.at(i)); // take inputs 1-6 and write them to the corresponding matrices 7-12
				serialHandler.get_buffer_odd().set_matrix_5(i - 1 + 12, cameras.at(i)); // take inputs 1-6 and write them to the corresponding matrices 13-18
			}
			for (int i = 7; i < 20; i++) { // take inputs 7-19 and write them to the corresponding matrices
				serialHandler.get_buffer_even().set_matrix_5(i - 7 + 13, cameras.at(i));
				serialHandler.get_buffer_even().set_matrix_5(i - 7 + 26, cameras.at(i));
			}

			// concurrent updation of matrices
			std::thread odd_send_matrices_5(writeString, &serialHandler, true, 5);
			std::thread even_send_matrices_5(writeString, &serialHandler, false, 5);
			odd_send_matrices_5.join();
			even_send_matrices_5.join();

			std::thread odd_send_matrices_6(writeString, &serialHandler, true, 6);
			std::thread even_send_matrices_6(writeString, &serialHandler, false, 6);
			odd_send_matrices_6.join();
			even_send_matrices_6.join();

			updateMatrices = 0;
		}

		if (count == 30) {
			count = 0;
			updateLEDs = 2;
			//std::cout << "Updating LEDs..." << std::endl;
			//updateMatrices = 2;
		}

		if (dount == 100) {
			dount = 0;
			//updateMatrices = 2;
			//std::cout << "Updating Matrices..." << std::endl;
		}

		// update LEDs on pressing ME.2nd
		if (updateLEDs > 1) {

			led_set_cams(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even(), atem);
			led_set_hyperdeck(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even(), atem);
			led_set_transition(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even(), atem);
			led_set_dsk(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even(), atem);
			led_set_ftb(serialHandler.get_buffer_odd(), serialHandler.get_buffer_even(), atem);

			// concurrent update of the leds
			std::thread odd_leds(writeLEDs, &serialHandler, true);
			std::thread even_leds(writeLEDs, &serialHandler, false);
			odd_leds.join();
			even_leds.join();

			updateLEDs = 0;
		}
	}

	return;
}

void manageInput(SerialHandler* handler, bool odd, std::queue<ButtonCommand>* btn_cmds, std::queue<FaderCommand>* fdr_cmds) {
	if ((handler == NULL) || (btn_cmds == NULL) || (fdr_cmds == NULL)) { return; };

	std::tuple<std::vector<ButtonCommand>, std::vector<FaderCommand>> tup{};
	if (odd) {
		tup = handler->get_buffer_odd().handleInput(handler->get_serial_odd());
	}
	else {
		tup = handler->get_buffer_even().handleInput(handler->get_serial_even());
	}

	
	std::vector<ButtonCommand> newBtnCmds = std::get<0>(tup);
	std::vector<FaderCommand> newFdrCmds = std::get<1>(tup);
	// handle button commands
	for (ButtonCommand cmd : newBtnCmds) {
		btn_cmds->push(cmd);
	}
	// handle fader commands
	for (const FaderCommand& cmd : newFdrCmds) {
		fdr_cmds->push(cmd);
	}
	
}

void executeBtnCommands(std::queue<ButtonCommand>* cmds, Switcher* switcher, int* upMtx, int* upLEDs) {
	if ((cmds == NULL) || (switcher == NULL) || (upMtx == NULL) || (upLEDs == NULL)) { return; };

	while (!cmds->empty()) {
		switcher->execute_cmd(cmds->front());
		if (!(cmds->front().get_name()).compare("ME.extra")) {
			*upMtx += 1;
		}
		else if (!(cmds->front().get_name()).compare("ME.2nd")) {
			*upLEDs += 1;
		}
		cmds->pop();
	}
	return;
}

void executeFdrCommands(std::queue<FaderCommand>* cmds, Switcher* switcher, SerialHandler* serialHandler) {
	if ((cmds == NULL) || (switcher == NULL) || (serialHandler == NULL)) { return; };
	
	while (!cmds->empty()) {
		cmds->pop();
	}

	while (!cmds->empty()) {
		const bool odd = cmds->front().get_odd();
		const bool fdr = cmds->front().get_fader();
		const double val = cmds->front().get_value();
		double start;
		cmds->pop();

		if (!odd) { // even or odd pcbs
			if (fdr) { // fader 1 or 2
				start = serialHandler->get_buffer_odd().get_fdr1_start();
				if ((start == 0.0) && (val == 0.0)) {
					serialHandler->get_buffer_odd().set_fdr1_inactive();
					switcher->set_ME1_transition(0.0);
					return;
				}
				else if ((start == 0.0) && (val == 1.0)) {
					serialHandler->get_buffer_odd().set_fdr1_inactive();
					serialHandler->get_buffer_odd().set_fdr1_start(1.0);
					switcher->set_ME1_transition(1.0);
					return;
				}
				else if (start == 0.0) {
					serialHandler->get_buffer_odd().set_fdr1_active();
					switcher->set_ME1_transition(val);
				}
				else if ((start == 1.0) && (val == 1.0)) {
					serialHandler->get_buffer_odd().set_fdr1_inactive();
					switcher->set_ME1_transition(0.0);
					return;
				}
				else if ((start == 1.0) && (val == 0.0)) {
					serialHandler->get_buffer_odd().set_fdr1_inactive();
					serialHandler->get_buffer_odd().set_fdr1_start(0.0);
					switcher->set_ME1_transition(1.0);
					return;
				}
				else if (start == 1.0) {
					serialHandler->get_buffer_odd().set_fdr1_active();
					switcher->set_ME1_transition(1.0 - val);
				}
			}
			else if (!fdr) { // fader 1 or 2
				start = serialHandler->get_buffer_odd().get_fdr2_start();
				if ((start == 0.0) && (val == 0.0)) {
					serialHandler->get_buffer_odd().set_fdr2_inactive();
					switcher->set_ME2_transition(0.0);
					return;
				}
				else if ((start == 0.0) && (val == 1.0)) {
					serialHandler->get_buffer_odd().set_fdr2_inactive();
					serialHandler->get_buffer_odd().set_fdr2_start(1.0);
					switcher->set_ME2_transition(1.0);
					return;
				}
				else if (start == 0.0) {
					serialHandler->get_buffer_odd().set_fdr2_active();
					switcher->set_ME2_transition(val);
				}
				else if ((start == 1.0) && (val == 1.0)) {
					serialHandler->get_buffer_odd().set_fdr2_inactive();
					switcher->set_ME2_transition(0.0);
					return;
				}
				else if ((start == 1.0) && (val == 0.0)) {
					serialHandler->get_buffer_odd().set_fdr2_inactive();
					serialHandler->get_buffer_odd().set_fdr2_start(0.0);
					switcher->set_ME2_transition(1.0);
					return;
				}
				else if (start == 1.0) {
					serialHandler->get_buffer_odd().set_fdr2_active();
					switcher->set_ME2_transition(1.0 - val);
				}
			}
		}
	}
}

void writeString(SerialHandler* handler, bool odd, int prefix) {
	if (handler == NULL) { return; };

	if (odd && (prefix == 5)) {
		handler->get_serial_odd().writeString(matrix_string(handler->get_buffer_odd().get_matrix_buffer(prefix), odd, '5'));
	}
	else if (odd && (prefix == 6)) {
		handler->get_serial_odd().writeString(matrix_string(handler->get_buffer_odd().get_matrix_buffer(prefix), odd, '6'));
	}
	else if (!odd && (prefix == 5)) {
		handler->get_serial_even().writeString(matrix_string(handler->get_buffer_even().get_matrix_buffer(prefix), odd, '5'));
	}
	else if (!odd && (prefix == 6)) {
		handler->get_serial_even().writeString(matrix_string(handler->get_buffer_even().get_matrix_buffer(prefix), odd, '6'));
	}
}

void writeLEDs(SerialHandler* handler, bool odd) {
	if (handler == NULL) { return; }

	if (odd) {
		handler->get_serial_odd().writeString(led_string(handler->get_buffer_odd().get_led_buffer()));
	}
	else {
		handler->get_serial_even().writeString(led_string(handler->get_buffer_even().get_led_buffer()));
	}
}