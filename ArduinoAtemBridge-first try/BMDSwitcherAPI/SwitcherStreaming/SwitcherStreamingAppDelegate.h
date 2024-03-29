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

#import <Cocoa/Cocoa.h>
#import <memory>
#import "SwitcherStreamingController.h"

@interface SwitcherStreamingAppDelegate : NSObject <NSApplicationDelegate>
{
	IBOutlet NSWindow*			m_window;

	IBOutlet NSTextField*		m_ipAddressTextField;
	IBOutlet NSButton*			m_connectButton;
	IBOutlet NSTextField*		m_productNameTextField;
	
	IBOutlet NSTextField*		m_serviceNameTextField;
	IBOutlet NSTextField*		m_serverUrlTextField;
	IBOutlet NSTabView*			m_streamKeyTabView;
	IBOutlet NSTextField*		m_streamKeyTextField;
	IBOutlet NSSecureTextField*	m_streamKeySecureTextField;
	IBOutlet NSButton*			m_streamKeyShowHideButton;
	IBOutlet NSPopUpButton*		m_streamQualityPopUpButton;
	IBOutlet NSButton*			m_lowLatencyButton;
	
	IBOutlet NSButton*			m_startStreamButton;
	
	IBOutlet NSTextField*		m_streamStatusTextField;
	IBOutlet NSTextField*		m_streamTimecodeTextField;
	IBOutlet NSLevelIndicator*	m_cacheUsedLevelIndicator;
	IBOutlet NSTextField*		m_encodingBitrateTextField;
	
	std::unique_ptr<SwitcherStreamingController>	m_switcherStreamingController;
	
	BOOL						m_streamKeyHidden;
	BOOL						m_isConnected;
	BOOL						m_isStreaming;
}

@property (assign) IBOutlet NSWindow *window;

// UI Controls
- (IBAction)connectClicked:(id)sender;
- (IBAction)serviceNameUpdated:(id)sender;
- (IBAction)serverUrlUpdated:(id)sender;
- (IBAction)streamKeyUpdated:(id)sender;
- (IBAction)secureStreamKeyUpdated:(id)sender;
- (IBAction)showHideKeyClicked:(id)sender;
- (IBAction)lowLatencyClicked:(id)sender;
- (IBAction)streamQualitySelected:(id)sender;
- (IBAction)startStopClicked:(id)sender;

// Update UI
- (void)setProductName:(NSString*)productName;

- (void)setServiceName:(NSString*)serviceName;
- (void)setServerUrl:(NSString*)serverUrl;
- (void)setStreamKey:(NSString*)streamKey;
- (void)setStreamQuality:(NSInteger)streamQualityId;
- (void)setLowLatency:(BOOL)lowLatency;

- (void)setStreamingStatus:(NSString*)streamingStatus;
- (void)setTimecodeWithHours:(NSInteger)timecodeHours minutes:(NSInteger)timecodeMinutes seconds:(NSInteger)timecodeSeconds;
- (void)setCacheUsed:(NSNumber*)cachedUsed;
- (void)setEncodingBitrate:(NSInteger)encodingBitrate;

- (void)switcherDisconnected;

- (void)setStreamingActive:(BOOL)active;

- (void)enableSettingsBox:(BOOL)enable;
- (void)showError:(NSString*)title detail:(NSString*)detail;

@end

