#pragma once

#include <string>
#include <array>
#include <vector>
#include <atlbase.h> // CComPtr

#include "BMDSwitcherAPI_h.h"
#include "ButtonHandling.h"
#include "FaderHandling.h"

class Switcher
{
public:
	/**
	 * Constructor.
	 * \param ipAddress Ip address to look up for a device.
	*/
	Switcher(const std::string &ipAddress);

	/**
	 * Returns a string with the connected product name
	*/
	std::string get_product_name();

	// get functions
	CComPtr<IBMDSwitcher>& get_switcher() noexcept { return switcher; };

	/**
	 * Update switcher Inputs
	*/
	void update_switcher_inputs();

	/**
	 * Update switcher keys
	*/
	void update_switcher_keys();

	/**
	 * Update connected Hyperdecks
	*/
	void update_hyperdecks();

	/**
	 * \return array with short names if the input cameras
	*/
	std::array<std::string, 57> get_input_names_short();

	/**
	 * receives command and executes it
	 * \param cmd command to execute
	*/
	void execute_cmd(ButtonCommand cmd);
	void execute_cmd(FaderCommand cmd);

	/**
	 * Updates trasition frames. Has to be optimized
	*/
	void get_transition_frames();

	/**
	 * Set transition frames.
	*/
	void set_transition_frames();

	uint32_t get_ME1_trans() noexcept { return ME1_transition_frames;};
	uint32_t get_ME2_trans() noexcept { return ME2_transition_frames;};
	uint32_t get_FTB_trans() noexcept { return FTB_transition_frames;};

	CComPtr <IBMDSwitcherMixEffectBlock> get_mixblock1() noexcept { return switcherMixblock1; };
	CComPtr <IBMDSwitcherMixEffectBlock> get_mixblock2() noexcept { return switcherMixblock2; };

	void set_ME1_transition(double val) { switcherMixblock1->SetTransitionPosition(val); };
	void set_ME2_transition(double val) { switcherMixblock2->SetTransitionPosition(val); };

	void update_live_cams();
	void update_preview_cams();

	std::array<int, 2> get_live_cams() noexcept { return liveCams; };
	std::array<int, 2> get_preview_cams() noexcept { return previewCams; };

	CComPtr <IBMDSwitcherTransitionParameters> get_transitionParameters1() noexcept { return transitionParameters1; };
	CComPtr <IBMDSwitcherTransitionParameters> get_transitionParameters2() noexcept { return transitionParameters2; };

	CComPtr <IBMDSwitcherDownstreamKey> get_DSK1() noexcept { return switcherDSK1; };
	CComPtr <IBMDSwitcherDownstreamKey> get_DSK2() noexcept { return switcherDSK2; };

	std::vector<CComPtr<IBMDSwitcherKey>> get_switcherKeysME1() { return switcherKeysME1; };
	std::vector<CComPtr<IBMDSwitcherKey>> get_switcherKeysME2() { return switcherKeysME2; };

	CComPtr<IBMDSwitcherHyperDeck> get_hyperdeck() { return Hyperdecks.at(0); };

	BMDSwitcherHyperDeckStorageMediaState get_mediaState(int slot) { return MediaStates.at(slot); };
	void set_mediaState(int slot, BMDSwitcherHyperDeckStorageMediaState state) { MediaStates.at(slot) = state; };

	int get_activeStorageMedia() { return activeStorageMedia; };
	void set_activeStorageMedia(int num) { activeStorageMedia = num; };

	void update_switcherTime();
	std::array<uint8_t, 4> get_switcherTime() { return switcherTime; };

	BOOL get_recordingStatus() { return recordingStatus; };
	void set_recordingStatus(BOOL stat) { recordingStatus = stat; };

	bool get_cancel() { return cancel; };

private:
	CComPtr<IBMDSwitcherDiscovery> switcherDiscovery; // used to look up a switcher
	CComPtr<IBMDSwitcher> switcher; // the switcher object
	CComPtr <IBMDSwitcherMixEffectBlock> switcherMixblock1; // used to call mix effects for ME1
	CComPtr <IBMDSwitcherMixEffectBlock> switcherMixblock2; // used to call mix effects for ME2
	CComPtr <IBMDSwitcherDownstreamKey> switcherDSK1; // used to call dsk effects for DSK1
	CComPtr <IBMDSwitcherDownstreamKey> switcherDSK2; // used to call dsk effects for DSK2
	CComPtr <IBMDSwitcherTransitionMixParameters> switcherMixParameters1; // used to set up mix effects for ME1
	CComPtr <IBMDSwitcherTransitionMixParameters> switcherMixParameters2; // used to set up mix effects for ME2
	CComPtr <IBMDSwitcherTransitionParameters> transitionParameters1; // used to set up transitions for ME1
	CComPtr <IBMDSwitcherTransitionParameters> transitionParameters2; // used to set up transitions for ME2
	BMDSwitcherConnectToFailure connectToFailReason;
	std::vector<CComPtr<IBMDSwitcherInput>> switcherInputs;
	std::vector<CComPtr<IBMDSwitcherKey>> switcherKeysME1;
	std::vector<CComPtr<IBMDSwitcherKey>> switcherKeysME2;
	unsigned int ME1_transition_frames;
	unsigned int ME2_transition_frames;
	unsigned int FTB_transition_frames;
	std::array<int, 2> liveCams;
	std::array<int, 2> previewCams;
	std::vector<CComPtr<IBMDSwitcherHyperDeck>> Hyperdecks;
	std::array<BMDSwitcherHyperDeckStorageMediaState, 2> MediaStates;
	int activeStorageMedia;
	BOOL recordingStatus;

	std::array<uint8_t, 4> switcherTime;
	BOOL switcherTimeDroppedFrames;

	bool cancel;

	// Private functions
	
	/**
	 * change live cam
	 * \param me number of mix effect
	 * \param btn button number [1-19]
	 * \return returns HRESULT if the operation succeeded
	*/
	HRESULT set_cam_live(int me, int btn);

	/**
	 * change preview cam
	 * \param me number of mix effect
	 * \param btn button number [1-19]
	 * \return returns HRESULT if the operation succeeded
	*/
	HRESULT set_cam_prev(int me, int btn);

	/**
	 * roll through the predetermined number of transition frames
	 * \param frames number of frames of the transition
	*/
	void roll_trans_dur(unsigned int &frames);
};