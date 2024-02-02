#include "LEDHandling.h"

#include <unordered_map>
#include <iostream>


// Mapping table to convert int into char to represent hex values
std::unordered_map<int, char> hex_map = {
    {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'},
    {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
    {8, '8'}, {9, '9'}, {10, 'A'}, {11, 'B'},
    {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}
};

void led_start_up(std::array<uint8_t, 72>& array) noexcept {
    for (int i = 0; i < 72; ++i) { // switch on all leds
        //array.at(i) = 0B11111111; // each bit represents a led in an array
    }
    for (int i = 0; i < 72; ++i) { // switch off all leds
        array.at(i) = 0B00000000; // each bit represents a led in an array
    }
}

std::string led_string(std::array<uint8_t, 72> array) {
    std::string output;
    output.reserve(146); // 8 bit per char, 72*2 chars for the leds, 2 chars for the prefix and line feed
    output += "1"; // prefix for led string
    for (int i = 0; i < 72; i++) { // convert the int of each array into a hex of two chars
        const int exp0 = array.at(i) % 16;
        const int exp1 = (array.at(i) - exp0)/16;
        output += hex_map[exp1];
        output += hex_map[exp0];
    }
    output += "\n";
    //std::cout << output.length() << std::endl;
    return output;
}


void led_set_cams(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher) {
  /**
   * need variables
  */
  int live1 = switcher.get_live_cams().at(0);
  int live2 = switcher.get_live_cams().at(1);
  int preview1 = switcher.get_preview_cams().at(0);
  int preview2 = switcher.get_preview_cams().at(1);

  /**
   * Live, Preview preperation
  **/

  // set the corresponding bits of each array to 0
  
  for (int i = 2; i < 16; i++) { // preview, live 1-6 for ME1 and ME2
    buffer_odd.get_led_buffer().at(i) = buffer_odd.get_led_buffer().at(i) & 0b11111100;
  }
  for (int i = 0; i < 8; i++) { // preview 7-20 for ME1 and ME2
    buffer_even.get_led_buffer().at(i) = buffer_even.get_led_buffer().at(i) & 0b11110000;
  }
  for (int i = 9; i < 16; i++) { // live 7-20 for ME1 and ME2
    buffer_even.get_led_buffer().at(i) = buffer_even.get_led_buffer().at(i) & 0b11110000;
  }

  // ME1
  switch (live1) {
  case 1:
    buffer_odd.get_led_buffer().at(15) = buffer_odd.get_led_buffer().at(15) | 0b00000001;
    break;
  case 2:
    buffer_odd.get_led_buffer().at(14) = buffer_odd.get_led_buffer().at(14) | 0b00000001;
    break;
  case 3:
    buffer_odd.get_led_buffer().at(13) = buffer_odd.get_led_buffer().at(13) | 0b00000001;
    break;
  case 4:
    buffer_odd.get_led_buffer().at(12) = buffer_odd.get_led_buffer().at(12) | 0b00000001;
    break;
  case 5:
    buffer_odd.get_led_buffer().at(11) = buffer_odd.get_led_buffer().at(11) | 0b00000001;
    break;
  case 6:
    buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) | 0b00000001;
    break;
  case 7:
    buffer_even.get_led_buffer().at(15) = buffer_even.get_led_buffer().at(15) | 0b00000010;
    break;
  case 8:
    buffer_even.get_led_buffer().at(14) = buffer_even.get_led_buffer().at(14) | 0b00000010;
    break;
  case 9:
    buffer_even.get_led_buffer().at(13) = buffer_even.get_led_buffer().at(13) | 0b00000010;
    break;
  case 10:
    buffer_even.get_led_buffer().at(12) = buffer_even.get_led_buffer().at(12) | 0b00000010;
    break;
  case 11:
    buffer_even.get_led_buffer().at(11) = buffer_even.get_led_buffer().at(11) | 0b00000010;
    break;
  case 12:
    buffer_even.get_led_buffer().at(10) = buffer_even.get_led_buffer().at(10) | 0b00000010;
    break;
  case 13:
    buffer_even.get_led_buffer().at(9) = buffer_even.get_led_buffer().at(9) | 0b00000010;
    break;
  case 14:
    buffer_even.get_led_buffer().at(15) = buffer_even.get_led_buffer().at(15) | 0b00001000;
    break;
  case 15:
    buffer_even.get_led_buffer().at(14) = buffer_even.get_led_buffer().at(14) | 0b00001000;
    break;
  case 16:
    buffer_even.get_led_buffer().at(13) = buffer_even.get_led_buffer().at(13) | 0b00001000;
    break;
  case 17:
    buffer_even.get_led_buffer().at(12) = buffer_even.get_led_buffer().at(12) | 0b00001000;
    break;
  case 18:
    buffer_even.get_led_buffer().at(11) = buffer_even.get_led_buffer().at(11) | 0b00001000;
    break;
  case 19:
    buffer_even.get_led_buffer().at(10) = buffer_even.get_led_buffer().at(10) | 0b00001000;
    break;
  case 20:
    buffer_even.get_led_buffer().at(9) = buffer_even.get_led_buffer().at(9) | 0b00001000;
    break;
  default:
    break;
  }
  switch (preview1) {
  case 0:
    buffer_even.get_led_buffer().at(23) = buffer_even.get_led_buffer().at(23) | 0b00100000;
    break;
  case 1:
    buffer_odd.get_led_buffer().at(7) = buffer_odd.get_led_buffer().at(7) | 0b00000001;
    break;
  case 2:
    buffer_odd.get_led_buffer().at(6) = buffer_odd.get_led_buffer().at(6) | 0b00000001;
    break;
  case 3:
    buffer_odd.get_led_buffer().at(5) = buffer_odd.get_led_buffer().at(5) | 0b00000001;
    break;
  case 4:
    buffer_odd.get_led_buffer().at(4) = buffer_odd.get_led_buffer().at(4) | 0b00000001;
    break;
  case 5:
    buffer_odd.get_led_buffer().at(3) = buffer_odd.get_led_buffer().at(3) | 0b00000001;
    break;
  case 6:
    buffer_odd.get_led_buffer().at(2) = buffer_odd.get_led_buffer().at(2) | 0b00000001;
    break;
  case 7:
    buffer_even.get_led_buffer().at(7) = buffer_even.get_led_buffer().at(7) | 0b00000010;
    break;
  case 8:
    buffer_even.get_led_buffer().at(6) = buffer_even.get_led_buffer().at(6) | 0b00000010;
    break;
  case 9:
    buffer_even.get_led_buffer().at(5) = buffer_even.get_led_buffer().at(5) | 0b00000010;
    break;
  case 10:
    buffer_even.get_led_buffer().at(4) = buffer_even.get_led_buffer().at(4) | 0b00000010;
    break;
  case 11:
    buffer_even.get_led_buffer().at(3) = buffer_even.get_led_buffer().at(3) | 0b00000010;
    break;
  case 12:
    buffer_even.get_led_buffer().at(2) = buffer_even.get_led_buffer().at(2) | 0b00000010;
    break;
  case 13:
    buffer_even.get_led_buffer().at(1) = buffer_even.get_led_buffer().at(1) | 0b00000010;
    break;
  case 14:
    buffer_even.get_led_buffer().at(7) = buffer_even.get_led_buffer().at(7) | 0b00001000;
    break;
  case 15:
    buffer_even.get_led_buffer().at(6) = buffer_even.get_led_buffer().at(6) | 0b00001000;
    break;
  case 16:
    buffer_even.get_led_buffer().at(5) = buffer_even.get_led_buffer().at(5) | 0b00001000;
    break;
  case 17:
    buffer_even.get_led_buffer().at(4) = buffer_even.get_led_buffer().at(4) | 0b00001000;
    break;
  case 18:
    buffer_even.get_led_buffer().at(3) = buffer_even.get_led_buffer().at(3) | 0b00001000;
    break;
  case 19:
    buffer_even.get_led_buffer().at(2) = buffer_even.get_led_buffer().at(2) | 0b00001000;
    break;
  case 20:
    buffer_even.get_led_buffer().at(1) = buffer_even.get_led_buffer().at(1) | 0b00001000;
    break;
  case 1000: // Bars
    buffer_even.get_led_buffer().at(14) = buffer_even.get_led_buffer().at(14) | 0b00100000;
    break;
  case 2001: // Color1
    buffer_even.get_led_buffer().at(21) = buffer_even.get_led_buffer().at(21) | 0b00100000;
    break;
  case 2002: // Color2
    buffer_even.get_led_buffer().at(20) = buffer_even.get_led_buffer().at(20) | 0b00100000;
    break;
  case 3010: // MP1
    buffer_even.get_led_buffer().at(19) = buffer_even.get_led_buffer().at(19) | 0b00100000;
    break;
  case 3011: // MP1K
    buffer_even.get_led_buffer().at(18) = buffer_even.get_led_buffer().at(18) | 0b00100000;
    break;
  case 3020: // MP2
    buffer_even.get_led_buffer().at(17) = buffer_even.get_led_buffer().at(17) | 0b00100000;
    break;
  case 3021: // MP2K
    buffer_even.get_led_buffer().at(16) = buffer_even.get_led_buffer().at(16) | 0b00100000;
    break;
  case 6000: // SS1
    buffer_even.get_led_buffer().at(15) = buffer_even.get_led_buffer().at(15) | 0b00100000;
    break;
  default:
    break;
  }

  // ME2
  switch (live2) {
  case 1:
    buffer_odd.get_led_buffer().at(15) = buffer_odd.get_led_buffer().at(15) | 0b00000010;
    break;
  case 2:
    buffer_odd.get_led_buffer().at(14) = buffer_odd.get_led_buffer().at(14) | 0b00000010;
    break;
  case 3:
    buffer_odd.get_led_buffer().at(13) = buffer_odd.get_led_buffer().at(13) | 0b00000010;
    break;
  case 4:
    buffer_odd.get_led_buffer().at(12) = buffer_odd.get_led_buffer().at(12) | 0b00000010;
    break;
  case 5:
    buffer_odd.get_led_buffer().at(11) = buffer_odd.get_led_buffer().at(11) | 0b00000010;
    break;
  case 6:
    buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) | 0b00000010;
    break;
  case 7:
    buffer_even.get_led_buffer().at(15) = buffer_even.get_led_buffer().at(15) | 0b00000001;
    break;
  case 8:
    buffer_even.get_led_buffer().at(14) = buffer_even.get_led_buffer().at(14) | 0b00000001;
    break;
  case 9:
    buffer_even.get_led_buffer().at(13) = buffer_even.get_led_buffer().at(13) | 0b00000001;
    break;
  case 10:
    buffer_even.get_led_buffer().at(12) = buffer_even.get_led_buffer().at(12) | 0b00000001;
    break;
  case 11:
    buffer_even.get_led_buffer().at(11) = buffer_even.get_led_buffer().at(11) | 0b00000001;
    break;
  case 12:
    buffer_even.get_led_buffer().at(10) = buffer_even.get_led_buffer().at(10) | 0b00000001;
    break;
  case 13:
    buffer_even.get_led_buffer().at(9) = buffer_even.get_led_buffer().at(9) | 0b00000001;
    break;
  case 14:
    buffer_even.get_led_buffer().at(15) = buffer_even.get_led_buffer().at(15) | 0b00000100;
    break;
  case 15:
    buffer_even.get_led_buffer().at(14) = buffer_even.get_led_buffer().at(14) | 0b00000100;
    break;
  case 16:
    buffer_even.get_led_buffer().at(13) = buffer_even.get_led_buffer().at(13) | 0b00000100;
    break;
  case 17:
    buffer_even.get_led_buffer().at(12) = buffer_even.get_led_buffer().at(12) | 0b00000100;
    break;
  case 18:
    buffer_even.get_led_buffer().at(11) = buffer_even.get_led_buffer().at(11) | 0b00000100;
    break;
  case 19:
    buffer_even.get_led_buffer().at(10) = buffer_even.get_led_buffer().at(10) | 0b00000100;
    break;
  case 20:
    buffer_even.get_led_buffer().at(9) = buffer_even.get_led_buffer().at(9) | 0b00000100;
    break;
  default:
    break;
  }
  switch (preview2) {
  case 1:
    buffer_odd.get_led_buffer().at(7) = buffer_odd.get_led_buffer().at(7) | 0b00000010;
    break;
  case 2:
    buffer_odd.get_led_buffer().at(6) = buffer_odd.get_led_buffer().at(6) | 0b00000010;
    break;
  case 3:
    buffer_odd.get_led_buffer().at(5) = buffer_odd.get_led_buffer().at(5) | 0b00000010;
    break;
  case 4:
    buffer_odd.get_led_buffer().at(4) = buffer_odd.get_led_buffer().at(4) | 0b00000010;
    break;
  case 5:
    buffer_odd.get_led_buffer().at(3) = buffer_odd.get_led_buffer().at(3) | 0b00000010;
    break;
  case 6:
    buffer_odd.get_led_buffer().at(2) = buffer_odd.get_led_buffer().at(2) | 0b00000010;
    break;
  case 7:
    buffer_even.get_led_buffer().at(7) = buffer_even.get_led_buffer().at(7) | 0b00000001;
    break;
  case 8:
    buffer_even.get_led_buffer().at(6) = buffer_even.get_led_buffer().at(6) | 0b00000001;
    break;
  case 9:
    buffer_even.get_led_buffer().at(5) = buffer_even.get_led_buffer().at(5) | 0b00000001;
    break;
  case 10:
    buffer_even.get_led_buffer().at(4) = buffer_even.get_led_buffer().at(4) | 0b00000001;
    break;
  case 11:
    buffer_even.get_led_buffer().at(3) = buffer_even.get_led_buffer().at(3) | 0b00000001;
    break;
  case 12:
    buffer_even.get_led_buffer().at(2) = buffer_even.get_led_buffer().at(2) | 0b00000001;
    break;
  case 13:
    buffer_even.get_led_buffer().at(1) = buffer_even.get_led_buffer().at(1) | 0b00000001;
    break;
  case 14:
    buffer_even.get_led_buffer().at(7) = buffer_even.get_led_buffer().at(7) | 0b00000100;
    break;
  case 15:
    buffer_even.get_led_buffer().at(6) = buffer_even.get_led_buffer().at(6) | 0b00000100;
    break;
  case 16:
    buffer_even.get_led_buffer().at(5) = buffer_even.get_led_buffer().at(5) | 0b00000100;
    break;
  case 17:
    buffer_even.get_led_buffer().at(4) = buffer_even.get_led_buffer().at(4) | 0b00000100;
    break;
  case 18:
    buffer_even.get_led_buffer().at(3) = buffer_even.get_led_buffer().at(3) | 0b00000100;
    break;
  case 19:
    buffer_even.get_led_buffer().at(2) = buffer_even.get_led_buffer().at(2) | 0b00000100;
    break;
  case 20:
    buffer_even.get_led_buffer().at(1) = buffer_even.get_led_buffer().at(1) | 0b00000100;
    break;
  default:
    break;
  }

  
}

void led_set_hyperdeck(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher) {
    // check record state
    BMDSwitcherHyperDeckPlayerState state;
    switcher.get_hyperdeck()->GetPlayerState(&state);
    switch (state) {
    case bmdSwitcherHyperDeckStateRecord: {
        switcher.set_recordingStatus(true);
        buffer_even.get_led_buffer().at(28) = buffer_even.get_led_buffer().at(28) | 0b00000001;
        buffer_odd.get_led_buffer().at(3) = buffer_odd.get_led_buffer().at(3) & 0b01111111;
        buffer_odd.get_led_buffer().at(19) = buffer_odd.get_led_buffer().at(19) & 0b10111111;
        break;
    }
    case bmdSwitcherHyperDeckStateIdle: {
        switcher.set_recordingStatus(false);
        buffer_even.get_led_buffer().at(28) = buffer_even.get_led_buffer().at(28) & 0b11111110;
        buffer_odd.get_led_buffer().at(3) = buffer_odd.get_led_buffer().at(3) | 0b10000000;
        buffer_odd.get_led_buffer().at(19) = buffer_odd.get_led_buffer().at(19) & 0b10111111;
        break;
    }
    case bmdSwitcherHyperDeckStatePlay: {
        switcher.set_recordingStatus(false);
        buffer_even.get_led_buffer().at(28) = buffer_even.get_led_buffer().at(28) & 0b11111110;
        buffer_odd.get_led_buffer().at(3) = buffer_odd.get_led_buffer().at(3) & 0b01111111;
        buffer_odd.get_led_buffer().at(19) = buffer_odd.get_led_buffer().at(19) | 0b01000000;
        break;
    }
    default: {
        switcher.set_recordingStatus(false);
        buffer_even.get_led_buffer().at(28) = buffer_even.get_led_buffer().at(28) & 0b11111110;
    }
    }

    // check loop state
    BOOL loop;
    switcher.get_hyperdeck()->GetLoopedPlayback(&loop);
    if (loop) {
        buffer_odd.get_led_buffer().at(11) = buffer_odd.get_led_buffer().at(11) | 0b10000000;
    }
    else {
        buffer_odd.get_led_buffer().at(11) = buffer_odd.get_led_buffer().at(11) & 0b01111111;
    }

    // check SSD status
    BMDSwitcherHyperDeckStorageMediaState MediaState;
    switcher.get_hyperdeck()->GetStorageMediaState(0, &MediaState);
    switcher.set_mediaState(0, MediaState);
    switch (switcher.get_mediaState(0)) {
    case bmdSwitcherHyperDeckStorageMediaStateReady: {
        buffer_even.get_led_buffer().at(47) = buffer_even.get_led_buffer().at(47) | 0b00000001;
        break;
    }
    default: buffer_even.get_led_buffer().at(47) = buffer_even.get_led_buffer().at(47) & 0b11111110;
    }

    switcher.get_hyperdeck()->GetStorageMediaState(1, &MediaState);
    switcher.set_mediaState(1, MediaState);
    switch (switcher.get_mediaState(1)) {
    case bmdSwitcherHyperDeckStorageMediaStateReady: {
        buffer_even.get_led_buffer().at(39) = buffer_even.get_led_buffer().at(39) | 0b00000001;
        break;
    }
    default: buffer_even.get_led_buffer().at(39) = buffer_even.get_led_buffer().at(39) & 0b11111110;
    }

    int activeMediaSlot = -1;
    switcher.get_hyperdeck()->GetActiveStorageMedia(&activeMediaSlot);
    switcher.set_activeStorageMedia(activeMediaSlot);
    switch (switcher.get_activeStorageMedia()) {
    case 0:
        if (switcher.get_switcherTime().at(2) % 2 == 1 && switcher.get_recordingStatus()) {
            buffer_even.get_led_buffer().at(47) = buffer_even.get_led_buffer().at(47) & 0b11111110;
        }
        break;
    case 1:
        if (switcher.get_switcherTime().at(2) % 2 == 1 && switcher.get_recordingStatus()) {
            buffer_even.get_led_buffer().at(39) = buffer_even.get_led_buffer().at(39) & 0b11111110;
        }
        break;
    default:
        break;
    }

    // trying to switch some on
    for (int i = 18; i < 24; i++) {
        //buffer_even.get_led_buffer().at(i) = buffer_even.get_led_buffer().at(i) | 0b11111111;
        //buffer_odd.get_led_buffer().at(i) = buffer_odd.get_led_buffer().at(i) | 0b01000000;
    }
    constexpr int k = 19;
    //buffer_even.get_led_buffer().at(k) = buffer_even.get_led_buffer().at(k) | 0b1;
    //buffer_odd.get_led_buffer().at(k) = buffer_odd.get_led_buffer().at(k) | 0b01000000;

}

void led_set_transition(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher) {
    /**
    * Transition ME1
    **/

    // check for ongoing auto cut
    BOOL trans_me1_auto = false;
    switcher.get_mixblock1()->GetInTransition(&trans_me1_auto);
    if (trans_me1_auto) {
        buffer_odd.get_led_buffer().at(31) = buffer_odd.get_led_buffer().at(31) | 0b00010000;
    }
    else {
        buffer_odd.get_led_buffer().at(31) = buffer_odd.get_led_buffer().at(31) & 0b11101111;
    }

    // check for active transition type
    BMDSwitcherTransitionStyle trans_me1_trans_style;
    switcher.get_transitionParameters1()->GetTransitionStyle(&trans_me1_trans_style);
    switch (trans_me1_trans_style) {
    case bmdSwitcherTransitionStyleMix: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) | 0b00010000;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11101111;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) & 0b11101111;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11101111;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11101111;
        break;
    }
    case bmdSwitcherTransitionStyleDip: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11101111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) | 0b00010000;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) & 0b11101111;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11101111;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11101111;
        break;
    }
    case bmdSwitcherTransitionStyleWipe: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11101111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11101111;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) | 0b00010000;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11101111;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11101111;
        break;
    }
    case bmdSwitcherTransitionStyleStinger: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11101111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11101111;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) & 0b11101111;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) | 0b00010000;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11101111;
        break;
    }
    case bmdSwitcherTransitionStyleDVE: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11101111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11101111;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) & 0b11101111;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11101111;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) | 0b00010000;
        break;
    }
    default: {
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11101111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11101111;
        buffer_odd.get_led_buffer().at(34) = buffer_odd.get_led_buffer().at(34) & 0b11101111;
        buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11101111;
        buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11101111;
        break;
    }
    }

    // check for active fader
    BOOL fader1 = buffer_odd.get_fdr1_status();
    if (fader1) {
        buffer_odd.get_led_buffer().at(40) = buffer_odd.get_led_buffer().at(40) | 0b10000000;
        buffer_odd.get_led_buffer().at(25) = buffer_odd.get_led_buffer().at(25) | 0b00001000;
    }
    else {
        buffer_odd.get_led_buffer().at(40) = buffer_odd.get_led_buffer().at(40) & 0b01111111;
        buffer_odd.get_led_buffer().at(25) = buffer_odd.get_led_buffer().at(25) & 0b11110111;
    }

    // check for active tie for next transition (USK)
    BMDSwitcherTransitionSelection trans_me1_usk_tie_selection;
    HRESULT result = switcher.get_transitionParameters1()->GetNextTransitionSelection(&trans_me1_usk_tie_selection);
    int me1_usk_tie_selection = trans_me1_usk_tie_selection;
    // background (blk pst)
    if (me1_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) & 0b11101111; }
    // key1 (bgd l)
    me1_usk_tie_selection = me1_usk_tie_selection >> 1;
    if (me1_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(25) = buffer_odd.get_led_buffer().at(25) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(25) = buffer_odd.get_led_buffer().at(25) & 0b11101111; }
    // key2 (bgd r)
    me1_usk_tie_selection = me1_usk_tie_selection >> 1;
    if (me1_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(39) = buffer_odd.get_led_buffer().at(39) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(39) = buffer_odd.get_led_buffer().at(39) & 0b11101111; }
    // key3 (dsk1)
    me1_usk_tie_selection = me1_usk_tie_selection >> 1;
    if (me1_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(38) = buffer_odd.get_led_buffer().at(38) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(38) = buffer_odd.get_led_buffer().at(38) & 0b11101111; }
    // key4 (dsk2)
    me1_usk_tie_selection = me1_usk_tie_selection >> 1;
    if (me1_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11101111; }

    // check for active tie for next transition (DSK)
    BOOL dsk1_active;
    switcher.get_DSK1()->GetTie(&dsk1_active);
    if (dsk1_active) { buffer_odd.get_led_buffer().at(47) = buffer_odd.get_led_buffer().at(47) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(47) = buffer_odd.get_led_buffer().at(47) & 0b11110111; }
    BOOL dsk2_active;
    switcher.get_DSK2()->GetTie(&dsk2_active);
    if (dsk2_active) { buffer_odd.get_led_buffer().at(46) = buffer_odd.get_led_buffer().at(46) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(46) = buffer_odd.get_led_buffer().at(46) & 0b11110111; }


    // check for on air keys
    std::vector<CComPtr<IBMDSwitcherKey>> keys = switcher.get_switcherKeysME1();
    std::array<BOOL, 4> on_air;
    for (int i = 0; (i < keys.size()) && (i < 4) ; i++) {
        keys.at(i)->GetOnAir(&on_air[i]);
    }
    if (on_air.at(0)) { buffer_odd.get_led_buffer().at(40) = buffer_odd.get_led_buffer().at(40) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(40) = buffer_odd.get_led_buffer().at(40) & 0b11110111; }
    if (on_air.at(1)) { buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11110111; }
    // missing led and strange led
    //if (on_air.at(2)) { buffer_odd.get_led_buffer().at(24) = buffer_odd.get_led_buffer().at(24) | 0b00010000; }
    //else { buffer_odd.get_led_buffer().at(24) = buffer_odd.get_led_buffer().at(24) & 0b11101111; }
    //if (on_air.at(3)) { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) | 0b00001000; }
    //else { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) & 0b11110111; }

    // check for preview transition
    BOOL prev_trans;
    switcher.get_mixblock1()->GetPreviewTransition(&prev_trans);
    if (prev_trans) { buffer_odd.get_led_buffer().at(26) = buffer_odd.get_led_buffer().at(26) | 0b00010000; }
    else { buffer_odd.get_led_buffer().at(26) = buffer_odd.get_led_buffer().at(26) & 0b11101111; }

    /**
    * Transition ME2
    **/

    // check for ongoing auto cut
    BOOL trans_me2_auto = false;
    switcher.get_mixblock2()->GetInTransition(&trans_me2_auto);
    if (trans_me2_auto) {
        buffer_odd.get_led_buffer().at(47) = buffer_odd.get_led_buffer().at(47) | 0b00100000;
    }
    else {
        buffer_odd.get_led_buffer().at(47) = buffer_odd.get_led_buffer().at(47) & 0b11011111;
    }

    // check for active transition type
    BMDSwitcherTransitionStyle trans_me2_trans_style;
    switcher.get_transitionParameters2()->GetTransitionStyle(&trans_me2_trans_style);
    switch (trans_me2_trans_style) {
    case bmdSwitcherTransitionStyleMix: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) | 0b00100000;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11011111;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11011111;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11011111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11011111;
        break;
    }
    case bmdSwitcherTransitionStyleDip: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) & 0b11011111;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) | 0b00100000;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11011111;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11011111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11011111;
        break;
    }
    case bmdSwitcherTransitionStyleWipe: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) & 0b11011111;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11011111;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) | 0b00100000;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11011111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11011111;
        break;
    }
    case bmdSwitcherTransitionStyleStinger: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) & 0b11011111;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11011111;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11011111;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) | 0b00100000;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11011111;
        break;
    }
    case bmdSwitcherTransitionStyleDVE: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) & 0b11011111;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11011111;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11011111;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11011111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) | 0b00100000;
        break;
    }
    default: {
        buffer_odd.get_led_buffer().at(43) = buffer_odd.get_led_buffer().at(43) & 0b11011111;
        buffer_odd.get_led_buffer().at(42) = buffer_odd.get_led_buffer().at(42) & 0b11011111;
        buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11011111;
        buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11011111;
        buffer_odd.get_led_buffer().at(35) = buffer_odd.get_led_buffer().at(35) & 0b11011111;
        break;
    }
    }

    // check for active fader
    BOOL fader2 = buffer_odd.get_fdr2_status();
    if (fader2) {
        buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) | 0b00100000;
        buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) | 0b00010000;
    }
    else {
        buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) & 0b11011111;
        buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) & 0b11101111;
    }

    // check for active tie for next transition (USK)
    BMDSwitcherTransitionSelection trans_me2_usk_tie_selection;
    result = switcher.get_transitionParameters2()->GetNextTransitionSelection(&trans_me2_usk_tie_selection);
    int me2_usk_tie_selection = trans_me2_usk_tie_selection;
    // background (blk pst)
    if (me2_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(41) = buffer_odd.get_led_buffer().at(41) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(41) = buffer_odd.get_led_buffer().at(41) & 0b11011111; }
    // key1 (bgd l)
    me2_usk_tie_selection = me2_usk_tie_selection >> 1;
    if (me2_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(44) = buffer_odd.get_led_buffer().at(44) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(44) = buffer_odd.get_led_buffer().at(44) & 0b11011111; }
    // key2 (bgd r)
    me2_usk_tie_selection = me2_usk_tie_selection >> 1;
    if (me2_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(38) = buffer_odd.get_led_buffer().at(38) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(38) = buffer_odd.get_led_buffer().at(38) & 0b11011111; }
    // key3 (dsk1)
    me2_usk_tie_selection = me2_usk_tie_selection >> 1;
    if (me2_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) & 0b11011111; }
    // key4 (dsk2)
    me2_usk_tie_selection = me2_usk_tie_selection >> 1;
    if (me2_usk_tie_selection % 2 == 1) { buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(10) = buffer_odd.get_led_buffer().at(10) & 0b11011111; }

    // check for on air keys
    keys = switcher.get_switcherKeysME2();
    for (int i = 0; (i < keys.size()) && (i < 4); i++) {
       keys.at(i)->GetOnAir(&on_air[i]);
    }
    if (on_air.at(0)) { buffer_odd.get_led_buffer().at(31) = buffer_odd.get_led_buffer().at(31) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(31) = buffer_odd.get_led_buffer().at(31) & 0b11011111; }
    if (on_air.at(1)) { buffer_odd.get_led_buffer().at(30) = buffer_odd.get_led_buffer().at(30) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(30) = buffer_odd.get_led_buffer().at(30) & 0b11011111; }
    // missing led and strange led
    //if (on_air.at(2)) { buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) | 0b00100000; }
    //else { buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11011111; }
    //if (on_air.at(3)) { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) | 0b00100000; }
    //else { buffer_odd.get_led_buffer().at(28) = buffer_odd.get_led_buffer().at(28) & 0b11011111; }

    // check for preview transition
    switcher.get_mixblock2()->GetPreviewTransition(&prev_trans);
    if (prev_trans) { buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) | 0b00100000; }
    else { buffer_odd.get_led_buffer().at(33) = buffer_odd.get_led_buffer().at(33) & 0b11011111; }
}

void led_set_dsk(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher) {
    BOOL on;

    // DSK1

    // check if DSK1 is auto transitioning
    switcher.get_DSK1()->IsAutoTransitioning(&on);
    if (on) { buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(37) = buffer_odd.get_led_buffer().at(37) & 0b11110111; }

    // check if DSK1 is on air
    switcher.get_DSK1()->GetOnAir(&on);
    if (on) { buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) | 0b01000000; }
    else { buffer_odd.get_led_buffer().at(32) = buffer_odd.get_led_buffer().at(32) & 0b10111111; }


    // DSK2
    
    // check if DSK2 is auto transitioning
    switcher.get_DSK2()->IsAutoTransitioning(&on);
    if (on) { buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(36) = buffer_odd.get_led_buffer().at(36) & 0b11110111; }

    // check if DSK2 is on air
    switcher.get_DSK2()->GetOnAir(&on);
    if (on) { buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) | 0b00001000; }
    else { buffer_odd.get_led_buffer().at(29) = buffer_odd.get_led_buffer().at(29) & 0b11110111; } 
}


void led_set_ftb(Buffer& buffer_odd, Buffer& buffer_even, Switcher& switcher) {
    /**
    * Fade To Black
    **/

    BOOL ftb1 = false;
    BOOL ftb2 = false;
    switcher.get_mixblock1()->GetFadeToBlackFullyBlack(&ftb1);
    switcher.get_mixblock2()->GetFadeToBlackFullyBlack(&ftb2);
    if (ftb1) {
        buffer_even.get_led_buffer().at(45) = buffer_even.get_led_buffer().at(45) | 0b00100000;
    }
    else {
        buffer_even.get_led_buffer().at(45) = buffer_even.get_led_buffer().at(45) & 0b11011111;
    }
    if (ftb2) {
        buffer_even.get_led_buffer().at(46) = buffer_even.get_led_buffer().at(46) | 0b00100000;
    }
    else {
        buffer_even.get_led_buffer().at(46) = buffer_even.get_led_buffer().at(46) & 0b11011111;
    }
}

void led_set_exit(Buffer& buffer_odd, Buffer& buffer_even) {
    for (int i = 0; i < 72; i++) {
        buffer_even.get_led_buffer().at(i) = buffer_even.get_led_buffer().at(i) & 0b00000000;
        buffer_odd.get_led_buffer().at(i) = buffer_odd.get_led_buffer().at(i) & 0b00000000;
    }

    buffer_even.get_led_buffer().at(36) = buffer_even.get_led_buffer().at(36) | 0b00000010;
}