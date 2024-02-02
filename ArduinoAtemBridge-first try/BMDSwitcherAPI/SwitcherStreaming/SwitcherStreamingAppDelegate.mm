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

#import <AppKit/AppKit.h>
#import "SwitcherStreamingAppDelegate.h"

@implementation SwitcherStreamingAppDelegate

static const NSDictionary* kStreamingQualityDict = @{
	  @((int)StreamingQuality::High) : @"Streaming High",
	@((int)StreamingQuality::Medium) : @"Streaming Medium",
	   @((int)StreamingQuality::Low) : @"Streaming Low",
};

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	try
	{
		m_switcherStreamingController = std::unique_ptr<SwitcherStreamingController>(std::move(new SwitcherStreamingController(self)));
	}
	catch (std::runtime_error)
	{
		[self showError:@"Switcher Discovery Error"
				 detail:@"Could not create Switcher Discovery Instance.\nATEM Switcher Software may not be installed.\n"];
	}

	for (id key in kStreamingQualityDict)
	{
		[m_streamQualityPopUpButton addItemWithTitle:kStreamingQualityDict[key]];
		[[m_streamQualityPopUpButton lastItem] setTag:[key integerValue]];
	}
	
	m_streamKeyHidden = YES;
	m_isConnected = NO;
	m_isStreaming = NO;
	[self enableSettingsBox:NO];
	[m_startStreamButton setEnabled:NO];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	if (m_isStreaming)
		m_switcherStreamingController->stopStreaming();
	
	if (m_isConnected)
		m_switcherStreamingController->disconnect();
	
	m_switcherStreamingController = nullptr;
}

- (IBAction)connectClicked:(id)sender
{
	if (m_isConnected)
	{
		m_switcherStreamingController->disconnect();
		[self switcherDisconnected];
	}
	else
	{
		if (m_switcherStreamingController->connect((__bridge CFStringRef)[m_ipAddressTextField stringValue]))
		{
			[m_connectButton setTitle:@"Disconnect"];
			[m_ipAddressTextField setEnabled:NO];
			[self enableSettingsBox:YES];
			[m_startStreamButton setEnabled:YES];
			m_isConnected = YES;
		}
	}
}

- (IBAction)serviceNameUpdated:(id)sender
{
	m_switcherStreamingController->setServiceName((__bridge CFStringRef)[m_serviceNameTextField stringValue]);
}

- (IBAction)serverUrlUpdated:(id)sender
{
	m_switcherStreamingController->setServerUrl((__bridge CFStringRef)[m_serverUrlTextField stringValue]);
}

- (IBAction)streamKeyUpdated:(id)sender
{
	NSString* streamKey = [m_streamKeyTextField stringValue];
	[m_streamKeySecureTextField setStringValue:streamKey];
	m_switcherStreamingController->setStreamingKey((__bridge CFStringRef)streamKey);
}

- (IBAction)secureStreamKeyUpdated:(id)sender
{
	NSString* secureStreamKey = [m_streamKeySecureTextField stringValue];
	[m_streamKeyTextField setStringValue:secureStreamKey];
	m_switcherStreamingController->setStreamingKey((__bridge CFStringRef)secureStreamKey);
}

- (IBAction)showHideKeyClicked:(id)sender
{
	m_streamKeyHidden = !m_streamKeyHidden;
	[m_streamKeyTabView selectTabViewItemAtIndex:(m_streamKeyHidden ? 0 : 1)];
	[m_streamKeyShowHideButton setTitle:(m_streamKeyHidden ? @"Show" : @"Hide")];
}

- (IBAction)lowLatencyClicked:(id)sender
{
	m_switcherStreamingController->setLowLatency([m_lowLatencyButton state] == NSControlStateValueOn);
}

- (IBAction)streamQualitySelected:(id)sender
{
	m_switcherStreamingController->setStreamQuality((StreamingQuality)[m_streamQualityPopUpButton selectedTag]);
}

- (IBAction)startStopClicked:(id)sender
{
	if (m_isStreaming)
		m_switcherStreamingController->stopStreaming();
	else
		m_switcherStreamingController->startStreaming();
}

// Update UI
- (void)setProductName:(NSString*)productName
{
	[m_productNameTextField setStringValue:productName];
}

- (void)setServiceName:(NSString*)serviceName
{
	[m_serviceNameTextField setStringValue:serviceName];
}

- (void)setServerUrl:(NSString*)serverUrl
{
	[m_serverUrlTextField setStringValue:serverUrl];
}

- (void)setStreamKey:(NSString*)streamKey
{
	[m_streamKeyTextField setStringValue:streamKey];
	[m_streamKeySecureTextField setStringValue:streamKey];
}

- (void)setStreamQuality:(NSInteger)streamQualityId
{
	[m_streamQualityPopUpButton selectItemWithTag:streamQualityId];
}

- (void)setLowLatency:(BOOL)lowLatency
{
	[m_lowLatencyButton setState:(lowLatency ? NSControlStateValueOn : NSControlStateValueOff)];
}

- (void)setStreamingStatus:(NSString*)streamingStatus
{
	[m_streamStatusTextField setStringValue:streamingStatus];
}

- (void)setTimecodeWithHours:(NSInteger)timecodeHours minutes:(NSInteger)timecodeMinutes seconds:(NSInteger)timecodeSeconds
{
	[m_streamTimecodeTextField setStringValue:[NSString stringWithFormat:@"%ld:%02ld:%02ld", timecodeHours, timecodeMinutes, timecodeSeconds]];
}

- (void)setCacheUsed:(NSNumber*)cachedUsed
{
	[m_cacheUsedLevelIndicator setDoubleValue:[cachedUsed doubleValue]];
}

- (void)setEncodingBitrate:(NSInteger)encodingBitrate
{
	[m_encodingBitrateTextField setStringValue:[NSString stringWithFormat:@"%.1f Mb/s", [[NSNumber numberWithFloat:(encodingBitrate/1000000.0)] floatValue]]];
}

- (void)switcherDisconnected
{
	[m_ipAddressTextField setEditable:YES];
	[m_startStreamButton setEnabled:NO];
	[m_productNameTextField setStringValue:@""];
	[m_connectButton setTitle:@"Connect"];

	[self enableSettingsBox:NO];
	[m_serviceNameTextField setStringValue:@""];
	[m_serverUrlTextField setStringValue:@""];
	[m_streamKeyTextField setStringValue:@""];
	[m_streamKeySecureTextField setStringValue:@""];
	[m_lowLatencyButton setState:NSControlStateValueOff];

	[m_streamStatusTextField setStringValue:@"Not connected"];
	[m_streamTimecodeTextField setStringValue:@"00:00:00"];
	[m_cacheUsedLevelIndicator setDoubleValue:0.0];
	[m_encodingBitrateTextField setStringValue:@"00.0 Mb/s"];
	
	m_isConnected = NO;
	m_isStreaming = NO;
}

- (void)setStreamingActive:(BOOL)active
{
	[self enableSettingsBox:(!active)];
	[m_startStreamButton setTitle:(active ? @"Stop Streaming" : @"Start Streaming")];
	m_isStreaming = active;
}

- (void)enableSettingsBox:(BOOL)enable
{
	[m_serviceNameTextField setEnabled:enable];
	[m_serverUrlTextField setEnabled:enable];
	[m_streamKeyTextField setEnabled:enable];
	[m_streamKeySecureTextField setEnabled:enable];
	[m_streamKeyShowHideButton setEnabled:enable];
	[m_streamQualityPopUpButton setEnabled:enable];
	[m_lowLatencyButton setEnabled:enable];
}

- (void)showError:(NSString*)title detail:(NSString*)detail
{
	NSAlert *alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"OK"];
	[alert setMessageText:title];
	[alert setInformativeText:detail];
	[alert setAlertStyle:NSAlertStyleCritical];
	[alert beginSheetModalForWindow:m_window completionHandler:nil];
}

@end
