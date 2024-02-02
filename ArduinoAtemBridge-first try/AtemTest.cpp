#include "AtemTest.h"
#include "BMDSwitcherAPI_h.h"
#include "BMDSwitcherHelpers.h"

#include <iostream>

#include <atlbase.h> // CComPtr
#include <comutil.h> // _com_util::

#pragma comment(lib, "comsuppw.lib")


bool AtemTest(Configuration& config, Switcher &switcher) {

	// Print current and MultiView video modes
	BMDSwitcherVideoMode currentVideoMode;
	if (switcher.get_switcher()->GetVideoMode(&currentVideoMode) == S_OK)
	{
		std::string currentVideoModeStr = LookupString<BMDSwitcherVideoMode>(kSwitcherVideoModes, currentVideoMode);
		std::cout << "Current Video Mode: " << currentVideoModeStr << std::endl;

		BMDSwitcherVideoMode multiViewVideoMode;
		if (switcher.get_switcher()->GetMultiViewVideoMode(currentVideoMode, &multiViewVideoMode) == S_OK)
		{
			std::string multiViewVideoModeStr = LookupString<BMDSwitcherVideoMode>(kSwitcherVideoModes, multiViewVideoMode);
			std::cout << "MultiView Video Mode: " << multiViewVideoModeStr << std::endl;
		}
	}

	// Print the power status of switcher
	BMDSwitcherPowerStatus powerStatus;
	if (switcher.get_switcher()->GetPowerStatus(&powerStatus) == S_OK)
	{
		std::cout << "Power Supply 1: " << (powerStatus & bmdSwitcherPowerStatusSupply1 ? "Powered" : "Not powered") << std::endl;
		std::cout << "Power Supply 2: " << (powerStatus & bmdSwitcherPowerStatusSupply2 ? "Powered" : "Not powered") << std::endl;
	}

    return true;
}