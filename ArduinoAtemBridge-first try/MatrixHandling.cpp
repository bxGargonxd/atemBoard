#include "MatrixHandling.h"

#include <iostream>


// helper functions
void string_to_matrix(std::array<char, 4>& mat, std::string const& str) noexcept{
	for (int i = 0; i < 4; ++i) {
		mat.at(i) = str.at(i);
	}
}

// public functions
std::string matrix_string(std::array<std::array<char, 4>, 39> array, bool odd, char prefix) {
	std::string out;
	out += prefix;
	
	switch (odd) {
		case (true): {
			switch (prefix) {
				case ('5'): {
					// matrix 1-6
					for (int i = 0; i < 6; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					// empty space
					for (int i = 0; i < 26; i++) {
						out += "    ";
					}
					// matrix 13-18
					for (int i = 12; i < 18; i++) {
						for (int j = 0; j < 4; ++j) {
							out += array.at(i).at(j);
						}
					}
					// empty space
					for (int i = 0; i < 2; i++) {
						out += "    ";
					}
					// matrix 7-12
					for (int i = 6; i < 12; i++) {
						for (int j = 0; j < 4; ++j) {
							out += array.at(i).at(j);
						}
					}
					//empty space
					for (int i = 0; i < 15; i++) {
						out += "    ";
					}
					break;
				}
				case ('6'): {
					// matrix 1-4 (Transition ME)
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					// matrix 1-4 (Transition PP)
					for (int i = 5; i < 9; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					// matrix 1-2 (DSK)
					for (int i = 10; i < 12; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					// matrix 5 (Transition ME)
					for (int i = 4; i < 5; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					// matrix 5 (Transition PP)
					for (int i = 9; i < 10; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					//empty space
					for (int i = 0; i < 8; i++) {
						out += "          ";
					}
					// matrix 1-8 (Wipe down up)
					for (int i = 12; i < 20; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					//empty space
					for (int i = 0; i < 21; i++) {
						out += "    ";
					}
					break;
				}
				default: break;
			}
			break;
		}
		case (false): {
			switch (prefix) {
				case('5'):{
					//matrix 1-13
					for (int i = 0; i < 13; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}

					//empty space
					for (int i = 0; i < 19; i++) {
						out += "    ";
					}

					//matrix 14-26
					for (int i = 13; i < 26; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}

					//empty space
					for (int i = 0; i < 3; i++) {
						out += "    ";
					}

					//matrix 27-39
					for (int i = 26; i < 39; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					break;
				}
				case('6'): {
					//matrix 1 ftb trans dur
					for (int i = 0; i < 1; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}//extra trans dur
					for (int i = 1; i < 2; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}//extra 1-4
					for (int i = 2; i < 6; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					//empty space
					for (int i = 0; i < 13; i++) {
						out += "    ";
					}
					//positioner 1
					for (int i = 6; i < 7; i++) {
						for (int j = 0; j < 4; j++) {
							out += array.at(i).at(j);
						}
					}
					//empty space
					for (int i = 0; i < 41; i++) {
						out += "    ";
					}
					break;
				}
				default: break;
			}
		}
	}
	out += "\n";
	//std::cout << "Generated Matrix String length: " << out.length() << " " << odd << std::endl;
	return out;
}

void matrix_start_up(std::array<std::array<char, 4>, 39>& array) noexcept{
	for (int i = 0; i < array.size(); i++) {
		string_to_matrix(array.at(i), std::string("    "));
	}
}