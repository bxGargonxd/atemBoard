/* -LICENSE-START-
** Copyright (c) 2021 Blackmagic Design
**
** Permission is hereby granted, free of charge, to any person or organization
** obtaining a copy of the software and accompanying documentation covered by
** this license (the "Software") to use, reproduce, display, distribute,
** execute, and transmit the Software, and to prepare derivative works of the
** Software, and to permit third-parties to whom the Software is furnished to
** do so, all subject to the following:
**
** The copyright notices in the Software and this entire statement, including
** the above license grant, this restriction and the following disclaimer,
** must be included in all copies of the Software, in whole or in part, and
** all derivative works of the Software, unless such copies or derivative
** works are solely in the form of machine-executable object code generated by
** a source language processor.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
** -LICENSE-END-
*/

#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "com_ptr.h"
#include "SwitcherMonitor.h"
#include "SwitcherStreamMonitor.h"
#include "BMDSwitcherAPI.h"

@class SwitcherStreamingAppDelegate;

enum class StreamingQuality : int
{
	High	= 0,
	Medium	= 1,
	Low		= 2
};

class SwitcherStreamingController
{
public:
	explicit SwitcherStreamingController(SwitcherStreamingAppDelegate* appDelegate);
	virtual ~SwitcherStreamingController();
	
	bool	connect(CFStringRef ipAddress);
	void	disconnect();

	void	startStreaming();
	void	stopStreaming();
	
	void	setServiceName(CFStringRef serviceName);
	void	setServerUrl(CFStringRef serverUrl);
	void	setStreamingKey(CFStringRef streamingKey);
	void	setStreamQuality(StreamingQuality quality);
	void	setLowLatency(bool lowLatency);

private:
	SwitcherStreamingAppDelegate*		m_appDelegate;
	
	com_ptr<IBMDSwitcherDiscovery>		m_switcherDiscovery;
	com_ptr<IBMDSwitcher>				m_switcher;
	com_ptr<IBMDSwitcherStreamRTMP>		m_switcherStreamRTMP;
	
	com_ptr<SwitcherMonitor>			m_switcherMonitor;
	com_ptr<SwitcherStreamMonitor>		m_switcherStreamMonitor;
	
	std::thread							m_streamingThread;
	std::condition_variable				m_conditionVariable;
	std::mutex							m_mutex;
	bool								m_stopStreaming;
	
	void	startStreamingThread(void);
	void	stopStreamingThread(void);
	void	disconnected(void);
	
	void	updateServiceName(void);
	void	updateServerUrl(void);
	void	updateStreamKey(void);
	void	updateStreamQuality(void);
	void	updateLowLatency(void);

	void	updateStreamingStatus(BMDSwitcherStreamRTMPState state);
	void	updateTimecode(void);
	void	updateCacheUsed(void);
	void	updateEncodingBitrate(void);

	void	displayError(const CFStringRef& error);
};
