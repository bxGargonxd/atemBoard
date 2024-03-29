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

#include <stdexcept>
#include "SwitcherRecordAppDelegate.h"

@implementation SwitcherRecordAppDelegate

NSInteger const kInvalidRecordingTime = 0xffffffff;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	try
	{
		m_switcherRecordController = std::unique_ptr<SwitcherRecordController>(std::move(new SwitcherRecordController(self)));
	}
	catch (std::runtime_error)
	{
		[self showError:@"Switcher Discovery Error"
				 detail:@"Could not create Switcher Discovery Instance.\nATEM Switcher Software may not be installed.\n"];
	}
	
	m_isConnected = NO;
	m_isRecording = NO;
	[self enableRecordSettings:NO];
	[m_startRecordingButton setEnabled:NO];
	[m_switchDiskButton setEnabled:NO];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	if (m_isRecording)
		m_switcherRecordController->stopRecoding();
	
	if (m_isConnected)
		m_switcherRecordController->disconnect();
	
	m_switcherRecordController = nullptr;
}

- (IBAction)connectClicked:(id)sender
{
	if (m_isConnected)
	{
		m_switcherRecordController->disconnect();
		[self switcherDisconnected];
	}
	else
	{
		if (m_switcherRecordController->connect((__bridge CFStringRef)[m_ipAddressTextField stringValue]))
		{
			[m_connectButton setTitle:@"Disconnect"];
			[m_ipAddressTextField setEnabled:NO];
			[self enableRecordSettings:YES];
			[m_startRecordingButton setEnabled:([m_disk1PopUpButton numberOfItems] > 0)];
			m_isConnected = YES;
		}
	}
}

- (IBAction)baseFilenameUpdated:(id)sender
{
	m_switcherRecordController->setBaseFilename((__bridge CFStringRef)[m_baseFilenameTextField stringValue]);
}

- (IBAction)recordInAllCamerasClicked:(id)sender
{
	m_switcherRecordController->setRecordInAllCameras([m_recordInAllCamerasButton state] == NSControlStateValueOn);
}

- (IBAction)isoRecordingClicked:(id)sender
{
	m_switcherRecordController->setIsoRecording([m_isoRecordingButton state] == NSControlStateValueOn);
}

- (IBAction)disk1Selected:(id)sender
{
	m_switcherRecordController->setDisk1WorkingSet((uint32_t)[m_disk1PopUpButton selectedTag]);
}

- (IBAction)disk2Selected:(id)sender
{
	m_switcherRecordController->setDisk2WorkingSet((uint32_t)[m_disk2PopUpButton selectedTag]);
}

- (IBAction)switchDiskClicked:(id)sender
{
	m_switcherRecordController->switchDisk();
}

- (IBAction)startStopRecordClicked:(id)sender
{
	if (m_isRecording)
		m_switcherRecordController->stopRecoding();
	else
		m_switcherRecordController->startRecording();
}

- (void)setProductName:(NSString*)productName
{
	[m_productNameTextField setStringValue:productName];
}

- (void)setBaseFilename:(NSString*)fileBasename
{
	[m_baseFilenameTextField setStringValue:fileBasename];
}

- (void)setRecordInAllCameras:(BOOL)recordInAllCameras
{
	[m_recordInAllCamerasButton setState:(recordInAllCameras ? NSControlStateValueOn : NSControlStateValueOff)];
}

- (void)setIsoRecording:(BOOL)isoRecording
{
	[m_isoRecordingButton setState:(isoRecording ? NSControlStateValueOn : NSControlStateValueOff)];
}

- (void)setSupportIsoRecording:(BOOL)supportsIsoRecording
{
	[m_isoRecordingButton setEnabled:supportsIsoRecording];
	if (supportsIsoRecording)
		[m_isoRecordingButton setState:NSControlStateValueOff];
}

- (void)setDisk1Selection:(NSInteger)diskId
{
	if (diskId == kInvalidDiskId)
	{
		[m_disk1PopUpButton setEnabled:NO];
		[m_disk1PopUpButton selectItemAtIndex:-1];
		[m_startRecordingButton setEnabled:NO];
		[m_disk1StatusTextField setStringValue:@""];
		[m_disk1TimeAvailableTextField setStringValue:@""];
	}
	else
	{
		[m_disk1PopUpButton setEnabled:YES];
		[m_disk1PopUpButton selectItemWithTag:diskId];
		[m_startRecordingButton setEnabled:YES];
	}
}

- (void)setDisk2Selection:(NSInteger)diskId
{
	if (diskId == kInvalidDiskId)
	{
		[m_disk2PopUpButton setEnabled:NO];
		[m_disk2PopUpButton selectItemAtIndex:-1];
		[m_switchDiskButton setEnabled:NO];
		[m_disk2StatusTextField setStringValue:@""];
		[m_disk2TimeAvailableTextField setStringValue:@""];
	}
	else
	{
		[m_disk2PopUpButton setEnabled:YES];
		[m_disk2PopUpButton selectItemWithTag:diskId];
	}
}

- (void)setDiskStatus:(NSString*)status forDiskId:(NSInteger)diskId
{
	if ([m_disk1PopUpButton selectedTag] == diskId)
		[m_disk1StatusTextField setStringValue:status];
	else if ([m_disk2PopUpButton selectedTag] == diskId)
		[m_disk2StatusTextField setStringValue:status];
}

- (void)setDiskRecordingTimeAvailable:(NSInteger)seconds forDiskId:(NSInteger)diskId
{
	if ([m_disk1PopUpButton selectedTag] == diskId)
		[m_disk1TimeAvailableTextField setStringValue:[self recordingTimeToString:seconds]];
	else if ([m_disk2PopUpButton selectedTag] == diskId)
		[m_disk2TimeAvailableTextField setStringValue:[self recordingTimeToString:seconds]];
}

- (void)setDiskVolumeName:(NSString*)volumeName forDiskId:(NSInteger)diskId
{
	NSInteger diskIdIndex = [m_disk1PopUpButton indexOfItemWithTag:diskId];
	if (diskIdIndex >= 0)
		[[m_disk1PopUpButton itemAtIndex:diskIdIndex] setTitle:volumeName];

	diskIdIndex = [m_disk2PopUpButton indexOfItemWithTag:diskId];
	if (diskIdIndex >= 0)
		[[m_disk2PopUpButton itemAtIndex:diskIdIndex] setTitle:volumeName];
}

- (void)setRecordingStatus:(NSString*)recordingStatus
{
	[m_recordStatusTextField setStringValue:recordingStatus];
}

- (void)setTimecodeWithHours:(NSInteger)timecodeHours minutes:(NSInteger)timecodeMinutes seconds:(NSInteger)timecodeSeconds
{
	[m_recordTimecodeTextField setStringValue:[NSString stringWithFormat:@"%ld:%02ld:%02ld", timecodeHours, timecodeMinutes, timecodeSeconds]];
}

- (void)setTotalRecordingTimeAvailable:(NSInteger)seconds
{
	[m_recordTimeAvailableTextField setStringValue:[self recordingTimeToString:seconds]];
}

- (void)addDiskWithDiskId:(NSInteger)diskId volumeName:(NSString*)volumeName
{
	BOOL isDiskSelected = [m_disk1PopUpButton indexOfSelectedItem] == -1;
	[m_disk1PopUpButton addItemWithTitle:volumeName];
	[[m_disk1PopUpButton lastItem] setTag:diskId];
	if (isDiskSelected)
	{
		[m_disk1PopUpButton selectItemAtIndex:-1];
	}

	isDiskSelected = [m_disk2PopUpButton indexOfSelectedItem] == -1;
	[m_disk2PopUpButton addItemWithTitle:volumeName];
	[[m_disk2PopUpButton lastItem] setTag:diskId];
	if (isDiskSelected)
		[m_disk2PopUpButton selectItemAtIndex:-1];

	// Update disk and record buttons
	[m_startRecordingButton setEnabled:([m_disk1PopUpButton numberOfItems] > 0)];
}

- (void)removeDiskWithDiskId:(NSInteger)diskId
{
	NSInteger diskIdIndex = [m_disk1PopUpButton indexOfItemWithTag:diskId];
	if (diskIdIndex >= 0)
		[m_disk1PopUpButton removeItemAtIndex:diskIdIndex];
	
	diskIdIndex = [m_disk2PopUpButton indexOfItemWithTag:diskId];
	if (diskIdIndex >= 0)
		[m_disk2PopUpButton removeItemAtIndex:diskIdIndex];

	// Update disk and record buttons
	[m_startRecordingButton setEnabled:([m_disk1PopUpButton numberOfItems] > 0)];
}

- (void)switcherDisconnected
{
	[m_connectButton setTitle:@"Connect"];
	[m_ipAddressTextField setEnabled:YES];
	[m_productNameTextField setStringValue:@""];
	
	[m_baseFilenameTextField setStringValue:@""];
	[m_recordInAllCamerasButton setState:NSControlStateValueOff];
	[m_isoRecordingButton setState:NSControlStateValueOff];
	
	[m_disk1PopUpButton removeAllItems];
	[m_disk1PopUpButton setEnabled:NO];
	[m_disk1StatusTextField setStringValue:@""];
	[m_disk1TimeAvailableTextField setStringValue:@""];
	
	[m_disk2PopUpButton removeAllItems];
	[m_disk2PopUpButton setEnabled:NO];
	[m_disk2StatusTextField setStringValue:@""];
	[m_disk2TimeAvailableTextField setStringValue:@""];

	[m_recordStatusTextField setStringValue:@"Not Connected"];
	[m_recordTimecodeTextField setStringValue:@"00:00:00"];
	[m_recordTimeAvailableTextField setStringValue:@"0 hr 00 min"];
	
	[self enableRecordSettings:NO];
	
	[m_startRecordingButton setEnabled:NO];
	m_isConnected = NO;
	m_isRecording = NO;
}

- (void)setRecordingActive:(BOOL)active
{
	if (active)
	{
		[self enableRecordSettings:NO];
		[m_startRecordingButton setTitle:@"Stop Recording"];
		[m_switchDiskButton setEnabled:[m_disk2PopUpButton numberOfItems] > 1];
		m_isRecording = YES;
	}
	else
	{
		[self enableRecordSettings:YES];
		[m_startRecordingButton setTitle:@"Start Recording"];
		[m_switchDiskButton setEnabled:NO];
		m_isRecording = NO;
	}
}

- (void)enableRecordSettings:(BOOL)enable
{
	[m_baseFilenameTextField setEnabled:enable];
	[m_recordInAllCamerasButton setEnabled:enable];
	[m_isoRecordingButton setEnabled:enable && m_switcherRecordController->supportsIsoRecording()];
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

- (NSString*)recordingTimeToString:(NSInteger)recordingTimeSeconds
{
	if (recordingTimeSeconds == kInvalidRecordingTime)
		recordingTimeSeconds = 0;

	NSInteger recordingTimeMinutes = recordingTimeSeconds / 60;
	return [NSString stringWithFormat:@"%ld hr %02ld min", recordingTimeMinutes / 60, recordingTimeMinutes % 60];
}

@end
