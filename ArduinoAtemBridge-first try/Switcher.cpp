#include "Switcher.h"

#include <iostream>
#include <comutil.h> // _com_util::
#include <map>

#pragma comment(lib, "comsuppw.lib")

/**
 * MAPS NEEDED
*/

static const std::map<BMDSwitcherConnectToFailure, std::string> kConnectFailReasonCodes =
{
	{ bmdSwitcherConnectToFailureNoResponse,			"No response" },
	{ bmdSwitcherConnectToFailureIncompatibleFirmware,	"Incompatible firmware" },
	{ bmdSwitcherConnectToFailureCorruptData,			"Corrupt data" },
	{ bmdSwitcherConnectToFailureStateSync,				"State synchronisation failed" },
	{ bmdSwitcherConnectToFailureStateSyncTimedOut,		"State synchronisation timed-out" },
};

const std::unordered_map<std::string, int> StringToInt =
{
	{"0", 0},
	{"1", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
	{"10", 10},
	{"11", 11},
	{"12", 12},
	{"13", 13},
	{"14", 14},
	{"15", 15},
	{"16", 16},
	{"17", 17},
	{"18", 18},
	{"19", 19},
	{"20", 20},
	{"21", 21}
};


/**
 * FUNCTIONS
*/

/** 
 * Converts a BSTR to a CString
 * \param str Pointer to a BSTR
 * \returns retString C-style string
*/
const char* BSTRToCString(const CComBSTR& str)
{
	_bstr_t bstr(str);
	const size_t newSize = (static_cast<size_t>(bstr.length()) + 1) * 2;
	char* retString = new char[newSize];

	strcpy_s(retString, newSize, (char*)bstr);

	return retString;
}

/**
 * returns the corresponding string from a map
 * \param stringMap Map for Atem codes
 * \param index Expression to look up
*/
template<typename T>
std::string LookupString(const std::map<T, std::string>& stringMap, T index)
{
	std::string connectFailReasonStr;
	auto iter = stringMap.find(index);

	if (iter != stringMap.end())
		return iter->second;
	else
		return "Unknown";
}

/**
 * returns the product name of the connect switcher
 * \param switcher Connected switcher device
*/
std::string Switcher::get_product_name() {
	CComBSTR productNameString;

	// *** Print the product name of the Switcher
	if (switcher->GetProductName(&productNameString) != S_OK)
	{
		return std::string("N/A");
	}

	const int wlen = ::SysStringLen(productNameString);
	int mblen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)productNameString, wlen, NULL, 0, NULL, NULL);

	std::string productName(mblen, '\0');
	mblen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)productNameString, wlen, &productName.at(0), mblen, NULL, NULL);

	return productName;
}

/**
 * Constructor.
 * \param ipAddress ip address to connect to
*/
Switcher::Switcher(const std::string& ipAddress)
	:
	ME1_transition_frames(),
	ME2_transition_frames(),
	FTB_transition_frames()
{
	HRESULT result{};
	connectToFailReason = BMDSwitcherConnectToFailure();

	// Initialize COM on this thread
	std::cout << "Initializing COM for Atem..." << std::endl;
	result = CoInitializeEx(NULL, NULL);
	if (FAILED(result)) {
		fprintf(stderr, "Initialization of COM failed - result = %08x\n", result);
		return;
	}

	// Create an IBMDSwitcherDiscovery object to access switcher device
	std::cout << "Creating an IBMDSwitcherDiscovery object to access switcher device..." << std::endl;
	result = switcherDiscovery.CoCreateInstance(CLSID_CBMDSwitcherDiscovery, NULL, CLSCTX_ALL);
	if (result != S_OK) {
		fprintf(stderr, "A Switcher Discovery instance could not be created.  The Switcher drivers may not be installed.\n");
		return;
	}

	// Connect to switcher with address provided by parameter
	std::cout << "Connecting to Switcher..." << std::endl;
	CComBSTR addressString = _com_util::ConvertStringToBSTR(ipAddress.c_str());
	result = switcherDiscovery->ConnectTo(addressString, &switcher, &connectToFailReason);
	if (result != S_OK)
	{
		std::string connectFailReasonStr = LookupString<BMDSwitcherConnectToFailure>(kConnectFailReasonCodes, connectToFailReason);
		fprintf(stderr, "Failed to connect to switcher at address %s, reason = %s\n", ipAddress.c_str(), connectFailReasonStr.c_str());
		return;
	}
	::SysFreeString(addressString);

	// Create first IBMDSwitcherMixEffectBlock object to send  mixing commands (ME1)
	std::cout << "Creating first IBMDSwitcherMixEffectBlock object to send mixing commands (ME1).." << std::endl;
	switcherMixblock1 = NULL;
	IBMDSwitcherMixEffectBlockIterator* iterator = NULL;
	switcher->CreateIterator(IID_IBMDSwitcherMixEffectBlockIterator, (void**)&iterator);
	iterator->Next(&switcherMixblock1);

	// Create second IBMDSwitcherMixEffectBlock object to send mixing commands (ME2)
	std::cout << "Creating second IBMDSwitcherMixEffectBlock object to send mixing commands (ME2)..." << std::endl;
	switcherMixblock2 = NULL;
	iterator -> Next(&switcherMixblock2);

	// Create first IBMDSwitcherDownstreamKey object to send commands (DSK 1)
	switcherDSK1 = NULL;
	IBMDSwitcherDownstreamKeyIterator* iteratord = NULL;
	switcher->CreateIterator(IID_IBMDSwitcherDownstreamKeyIterator, (void**)&iteratord);
	iteratord->Next(&switcherDSK1);

	// Create second IBMDSwitcherDownstreamKey object to send commands (DSK 2)
	switcherDSK2 = NULL;
	iteratord->Next(&switcherDSK2);


	////////////// Transition Parameters
	// Create first IBMDSwitcherTrasitionParameters object to set up the next transition
	transitionParameters1 = NULL;
	result = switcherMixblock1->QueryInterface(IID_IBMDSwitcherTransitionParameters, (void**)&transitionParameters1);

	// Create second IBMDSwitcherTrasitionParameters object to set up the next transition
	transitionParameters2 = NULL;
	result = switcherMixblock2->QueryInterface(IID_IBMDSwitcherTransitionParameters, (void**)&transitionParameters2);

	// Create first IBMDSwitcherTransitionMixParameters object to set up mixing effects for ME1
	switcherMixParameters1 = NULL;
	result = switcherMixblock1->QueryInterface(IID_IBMDSwitcherMixEffectBlock, (void**)&switcherMixParameters1);

	// Create second IBMDSwitcherTransitionMixParameters object to set up mixing effects for ME2
	switcherMixParameters2 = NULL;
	switcherMixblock2->QueryInterface(IID_IBMDSwitcherMixEffectBlock, (void**)&switcherMixParameters2);

	// Set transition frame rates
	get_transition_frames();

	// Set active media slot
	activeStorageMedia = -1;

	// Print switcher product name
	std::cout << "Switcher found at " << ipAddress << "." << std::endl;
	std::cout << get_product_name() << std::endl;
}

void Switcher::update_switcher_inputs() {
	switcherInputs.clear();
	CComPtr<IBMDSwitcherInputIterator> inputIterator;
	if (switcher->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator) == S_OK)
	{
		CComPtr<IBMDSwitcherInput> input;
		while (inputIterator->Next(&input) == S_OK)
			switcherInputs.push_back(std::move(input));
	}
}

void Switcher::update_switcher_keys() {
	switcherKeysME1.clear();
	switcherKeysME2.clear();
	CComPtr<IBMDSwitcherKeyIterator> keyIterator1;
	if (switcherMixblock1->CreateIterator(IID_IBMDSwitcherKeyIterator, (void**)&keyIterator1) == S_OK) {
		CComPtr<IBMDSwitcherKey> key;
		while (keyIterator1->Next(&key) == S_OK) {
			switcherKeysME1.push_back(std::move(key));
		}
	}
	CComPtr<IBMDSwitcherKeyIterator> keyIterator2;
	if (switcherMixblock2->CreateIterator(IID_IBMDSwitcherKeyIterator, (void**)&keyIterator2) == S_OK) {
		CComPtr<IBMDSwitcherKey> key;
		while (keyIterator2->Next(&key) == S_OK) {
			switcherKeysME2.push_back(std::move(key));
		}
	}
}

void Switcher::update_hyperdecks() {
	Hyperdecks.clear();
	CComPtr<IBMDSwitcherHyperDeckIterator> iterator;
	if (switcher->CreateIterator(IID_IBMDSwitcherHyperDeckIterator, (void**)&iterator) == S_OK) {
		CComPtr<IBMDSwitcherHyperDeck> deck;
		while (iterator->Next(&deck) == S_OK) {
			Hyperdecks.push_back(std::move(deck));
		}
	}
}

std::array<std::string, 57> Switcher::get_input_names_short() {
	update_switcher_inputs();
	std::array<std::string, 57> cams{};
	int i = 0;
	for (auto& input : switcherInputs) {
		if (i >= 57) {
			break;
		}
		BSTR sname[5];
		input->GetShortName(sname);
		cams.at(i) = BSTRToCString(*sname);
		i += 1;
	}

	return cams;
}

void Switcher::execute_cmd(ButtonCommand cmd) {
	//return if button released
	if (!cmd.get_state()) {
		return;
	}
	std::string name = cmd.get_name();
	const size_t split1 = name.find_first_of('.');
	size_t split2;
	std::string first = name.substr(0, split1);
	std::string second;
	name = name.substr(split1 + 1, name.length() - split1);

	// Display panel
	if (!first.compare("display")) {
		if (!name.compare("vtr_enable")) {
			if (Hyperdecks.at(0)->Record() == S_OK) {
				std::cout << "Recording..." << std::endl;
			}
		}
		else if (!name.compare("exit")) {
			cancel = true;
		}
	}

	// PP panel (ME1)
	else if (!name.compare("2nd") && !first.compare("PP")) {
		std::cout << "SECOOOOOOOOOOOOOND" << std::endl;
	}
	else if (!first.compare("PP")) {
		split2 = name.find_first_of('.');
		second = name.substr(0, split2);
		std::string button = name.substr(split2 + 1, name.length() - split2);
		const int btn = StringToInt.at(button);

		if (!second.compare("2")) { // PP row 2 => set live
			if (btn == 11) { // handle cam 11 as super source
				set_cam_live(1, 34);
			}
		
			else if (btn < 21) {
				set_cam_live(1, btn);
			}
			else if (btn == 21) {
				// Button right of ME1
			}
		}
		else if (!second.compare("3")) { // PP row 3 => set preview
			if (btn == 11) { // handle cam 11 as super source
				set_cam_prev(1, 34);
			}
			else if (btn < 21) {
				set_cam_prev(1, btn);
			}
			else if (btn == 21) {
				// Button right of ME1
			}
		}
	}
	// ME panel (ME2)
	else if (!name.compare("extra") && !first.compare("ME")) {
		std::cout << "EXTRAAAAAA" << std::endl;
	}
	else if (!name.compare("2nd") && !first.compare("ME")) {
		std::cout << "SECOOOOOOOOOOOOOND" << std::endl;
	}
	else if (!first.compare("ME")) {
		split2 = name.find_first_of('.');
		second = name.substr(0, split2);
		std::string button = name.substr(split2 + 1, name.length() - split2);
		const int btn = StringToInt.at(button);

		if (!second.compare("2")) { // ME row 2 => set live
			if (btn < 21) {
				set_cam_live(2, btn);
			}
			else if (btn == 21) {
				// Button right of ME1
			}
		}
		else if (!second.compare("3")) { // ME row 3 => set preview
			if (btn < 21) {
				set_cam_prev(2, btn);
			}
			else if (btn == 21) {
				// Button right of ME1
			}
		}
	}


	// Wipe
	
	else if (!first.compare("Wipe")) {
		if (!name.compare("U1")) {
			uint16_t hours;
			uint8_t minutes;
			uint8_t seconds;
			uint8_t frames;
			Hyperdecks.at(0)->GetCurrentClipTime(&hours, &minutes, &seconds, &frames);
			if (hours != 0 || minutes != 0 || seconds != 0 || frames != 0) {
				Hyperdecks.at(0)->SetCurrentClipTime(0, 0, 0, 0);
			}
			else {
				BMDSwitcherHyperDeckClipId clip;
				Hyperdecks.at(0)->GetCurrentClip(&clip);
				clip -= 1;
				Hyperdecks.at(0)->SetCurrentClip(clip);
			}
		}
		else if (!name.compare("U2")) {
			BOOL loop;
			// get loop status and invert it
			Hyperdecks.at(0)->GetLoopedPlayback(&loop);
			Hyperdecks.at(0)->SetLoopedPlayback(!loop);
			
		}
		else if (!name.compare("U3")) {
			if (Hyperdecks.at(0)->Stop() == S_OK) {
				std::cout << "Stopped recording!" << std::endl;
			}
		}
		else if (!name.compare("U4")) {
			Hyperdecks.at(0)->SetSingleClipPlayback(true);
			Hyperdecks.at(0)->Play();
		}
		else if (!name.compare("U5")) {
			BMDSwitcherHyperDeckClipId clip;
			Hyperdecks.at(0)->GetCurrentClip(&clip);
			clip += 1;
			Hyperdecks.at(0)->SetCurrentClip(clip);
		}
	}

	
	// tansition PP panel (ME1 transitions)
	else if (!first.compare("tPP")) {
		split2 = name.find_first_of('.');
		second = name.substr(0, split2);

		// select keys for next transition
		if (!second.compare("bgdl")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters1->GetNextTransitionSelection(&selection);
			transitionParameters1->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 2));
		}
		else if (!second.compare("bgdr")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters1->GetNextTransitionSelection(&selection);
			transitionParameters1->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 4));
		}
		else if (!second.compare("dsk1")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters1->GetNextTransitionSelection(&selection);
			transitionParameters1->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 8));
		}
		else if (!second.compare("dsk2")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters1->GetNextTransitionSelection(&selection);
			transitionParameters1->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 16));
		}
		else if (!second.compare("limit_set")) {
			BOOL active;
			switcherDSK1->GetTie(&active);
			switcherDSK1->SetTie(!active);
		}
		else if (!second.compare("limit_on")) {
			BOOL active;
			switcherDSK2->GetTie(&active);
			switcherDSK2->SetTie(!active);
		}
		else if (!second.compare("blk_pst")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters1->GetNextTransitionSelection(&selection);
			transitionParameters1->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 1));
		}
		// set transition style
		else if (!second.compare("add")) {
			transitionParameters1->SetNextTransitionStyle(bmdSwitcherTransitionStyleMix);
		}
		else if (!second.compare("dve")) {
			transitionParameters1->SetNextTransitionStyle(bmdSwitcherTransitionStyleDip);
		}
		else if (!second.compare("mix")) {
			transitionParameters1->SetNextTransitionStyle(bmdSwitcherTransitionStyleWipe);
		}
		else if (!second.compare("wipe1")) {
			transitionParameters1->SetNextTransitionStyle(bmdSwitcherTransitionStyleStinger);
		}
		else if (!second.compare("wipe2")) {
			transitionParameters1->SetNextTransitionStyle(bmdSwitcherTransitionStyleDVE);
		}
		// set transition duration
		else if (!second.compare("trans_dur")) {
			roll_trans_dur(ME1_transition_frames);
		}
		// set preview transition
		else if (!second.compare("trans_pvw")) {
			BOOL live;
			switcherMixblock1->GetPreviewTransition(&live);
			switcherMixblock1->SetPreviewTransition(!live);
	}
		// auto cut with transition time
		else if (!second.compare("auto")) {
			switcherMixblock1->PerformAutoTransition();
		}
		// hard cut
		else if (!second.compare("cut")) {
			switcherMixblock1->PerformCut();
		}
	}
	// transition ME panel (ME2 transitions)
	else if (!first.compare("tME")) {
		split2 = name.find_first_of('.');
		second = name.substr(0, split2);
		// select keys for next transition
		if (!second.compare("bgdl")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters2->GetNextTransitionSelection(&selection);
			transitionParameters2->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 2));
		}
		else if (!second.compare("bgdr")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters2->GetNextTransitionSelection(&selection);
			transitionParameters2->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 4));
		}
		else if (!second.compare("key1")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters2->GetNextTransitionSelection(&selection);
			transitionParameters2->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 8));
		}
		else if (!second.compare("key2")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters2->GetNextTransitionSelection(&selection);
			transitionParameters2->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 16));
		}
		else if (!second.compare("limit_set")) {
			/*
			BOOL active;
			switcherDSK1->GetTie(&active);
			switcherDSK1->SetTie(!active);
			*/
		}
		else if (!second.compare("limit_on")) {
			/*
			BOOL active;
			switcherDSK2->GetTie(&active);
			switcherDSK2->SetTie(!active);
			*/
		}
		else if (!second.compare("blk_pst")) {
			BMDSwitcherTransitionSelection selection;
			HRESULT result = transitionParameters2->GetNextTransitionSelection(&selection);
			transitionParameters2->SetNextTransitionSelection(static_cast<BMDSwitcherTransitionSelection>(selection ^ 1));
		}
		// set transition style
		else if (!second.compare("add")) {
			transitionParameters2->SetNextTransitionStyle(bmdSwitcherTransitionStyleMix);
		}
		else if (!second.compare("dve")) {
			transitionParameters2->SetNextTransitionStyle(bmdSwitcherTransitionStyleDip);
		}
		else if (!second.compare("mix")) {
			transitionParameters2->SetNextTransitionStyle(bmdSwitcherTransitionStyleWipe);
		}
		else if (!second.compare("wipe1")) {
			transitionParameters2->SetNextTransitionStyle(bmdSwitcherTransitionStyleStinger);
		}
		else if (!second.compare("wipe2")) {
			transitionParameters2->SetNextTransitionStyle(bmdSwitcherTransitionStyleDVE);
		}
		// set transition duration
		else if (!second.compare("trans_dur")) {
			roll_trans_dur(ME2_transition_frames);
		}
		// set preview transition
		else if (!second.compare("trans_pvw")) {
			BOOL live;
			switcherMixblock2->GetPreviewTransition(&live);
			switcherMixblock2->SetPreviewTransition(!live);
		}
		// auto cut with transition time
		else if (!second.compare("auto")) {
			switcherMixblock2->PerformAutoTransition();
		}
		// hard cut
		else if (!second.compare("cut")) {
			switcherMixblock2->PerformCut();
		}
	}
	// dsk panel (manage both down stream keyers)
	else if (!first.compare("dsk")) {
		split2 = name.find_first_of('.');
		second = name.substr(0, split2);
		// perform autotransition for dsk1
		if (!second.compare("auto_l")) {
			switcherDSK1->PerformAutoTransition();
		}
		// perform autotransition for dsk2
		else if (!second.compare("auto_r")) {
			switcherDSK2->PerformAutoTransition();
		}
		// perform hard cut for dsk1
		else if (!second.compare("cut_l")) {
			BOOL on_air = false;
			switcherDSK1->GetOnAir(&on_air);
			switcherDSK1->SetOnAir(!on_air);
		}
		// perform hard cut for dsk2
		else if (!second.compare("cut_r")) {
			BOOL on_air = false;
			switcherDSK2->GetOnAir(&on_air);
			switcherDSK2->SetOnAir(!on_air);
		}
	}
	// FTB panel
	else if (!first.compare("ftb")) {
		// set transition duration
		if (!name.compare("trans_dur")) {
			roll_trans_dur(FTB_transition_frames);
		}
		// perform auto fade to black on ME2
		else if (!name.compare("cut")) {
			switcherMixblock2->PerformFadeToBlack();
		}
		// perform auto fade to black on ME1
		else if (!name.compare("auto")) {
			switcherMixblock1->PerformFadeToBlack();
		}
	}
	else if (!first.compare("desti")) {
		if (!name.compare("7")) {
			set_cam_prev(1, 0);
		}
		else if (!name.compare("8")) {
			//set_cam_prev(1, 0);
		}
		else if (!name.compare("9")) {
			set_cam_prev(1, 22);
		}
		else if (!name.compare("10")) {
			set_cam_prev(1, 23);
		}
		else if (!name.compare("11")) {
			set_cam_prev(1, 24);
		}
		else if (!name.compare("12")) {
			set_cam_prev(1, 25);
		}
		else if (!name.compare("13")) {
			set_cam_prev(1, 26);
		}
		else if (!name.compare("14")) {
			set_cam_prev(1, 27);
		}
		else if (!name.compare("15")) {
			set_cam_prev(1, 38);
		}
		else if (!name.compare("16")) {
			set_cam_prev(1, 21);
		}
	}
}

void Switcher::execute_cmd(FaderCommand cmd) {
	double const value = cmd.get_value();

	if (!cmd.get_odd()) {
		if (cmd.get_fader()) {

			switcherMixblock1->SetTransitionPosition(value);
		}
		else if (!cmd.get_fader()) {
			switcherMixblock2->SetTransitionPosition(value);
		}
	}
}


HRESULT Switcher::set_cam_live(int me, int btn) {
	HRESULT result{};
	BMDSwitcherInputId Id;
	switcherInputs.at(btn)->GetInputId(&Id);
	switch (me) {
		case 1: 
			switcherMixblock1->SetProgramInput(Id);
			result = S_OK;
			break;
		case 2:
			switcherMixblock2->SetProgramInput(Id);
			result = S_OK;
			break;
		default:
			result = E_NOTIMPL;
			break;
	}
	return result;
}

HRESULT Switcher::set_cam_prev(int me, int btn) {
	HRESULT result{};
	BMDSwitcherInputId Id;
	switcherInputs.at(btn)->GetInputId(&Id);
	switch (me) {
		case 1: 
			switcherMixblock1->SetPreviewInput(Id);
			result = S_OK;
			break;
		case 2:
			switcherMixblock2->SetPreviewInput(Id);
			result = S_OK;
			break;
		default:
			result = E_NOTIMPL;
			break;
	}
	return result;
}

void Switcher::get_transition_frames() {
	HRESULT result;
	result = switcherMixParameters1->GetRate(&ME1_transition_frames);
	switcherMixParameters2->GetRate(&ME2_transition_frames);
	switcherMixblock1->GetFadeToBlackRate(&FTB_transition_frames);
}

void Switcher::set_transition_frames() {
	HRESULT result;
	std::cout << "sending: " << ME1_transition_frames << " " << ME2_transition_frames << " " << FTB_transition_frames << " " << std::endl;
	result = switcherMixParameters1->SetRate(ME1_transition_frames);
	if (result == S_OK) { std::cout << "S_OK" << std::endl; }
	else if (result == E_FAIL) { std::cout << "E_FAIL" << std::endl; }
	else if (result == E_INVALIDARG) {std::cout << "E_INVALIDARG" << std::endl;}
	else { std::cout << "FUCK" << std::endl; }
	
	result = switcherMixParameters2->SetRate(ME2_transition_frames);
	std::cout << result << std::endl;
	result = switcherMixblock1->SetFadeToBlackRate(FTB_transition_frames);
	std::cout << result << std::endl;
	result = switcherMixblock2->SetFadeToBlackRate(FTB_transition_frames);
	std::cout << result << std::endl;
}

void Switcher::update_live_cams()
{
	BMDSwitcherInputId in = 0;
	switcherMixblock1->GetProgramInput(&in);
	liveCams.at(0) = in;
	switcherMixblock2->GetProgramInput(&in);
	liveCams.at(1) = in;
}

void Switcher::update_preview_cams()
{
	BMDSwitcherInputId in = 0;
	switcherMixblock1->GetPreviewInput(&in);
	previewCams.at(0) = in;
	switcherMixblock2->GetPreviewInput(&in);
	previewCams.at(1) = in;
}

void Switcher::roll_trans_dur(unsigned int &frames) {
	if (frames == 25) { frames = 50; }
	else if (frames == 50) { frames = 100; }
	else { frames = 25; };
	set_transition_frames();
}

void Switcher::update_switcherTime() {
	switcher->GetTimeCode(&switcherTime.at(0), &switcherTime.at(1), &switcherTime.at(2), &switcherTime.at(3), &switcherTimeDroppedFrames);
}