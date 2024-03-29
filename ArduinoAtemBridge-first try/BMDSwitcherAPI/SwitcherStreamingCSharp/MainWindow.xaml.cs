﻿/* -LICENSE-START-
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;

using BMDSwitcherAPI;

namespace SwitcherStreamingCSharp
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private const int StreamStartMillisecondsTimeout = 30000;

		private SwitcherStreamDevice	switcherStreamDevice = null;
		private SwitcherMonitor			switcherMonitor = null;
		private SwitcherStreamMonitor	switcherStreamMonitor = null;
		private bool					streamKeyHidden = true;
		private bool					userStreamQualitySelection = false;

		private CancellationTokenSource streamStoppingCancel;
		private CancellationTokenSource switcherDisconnectedCancel;
		private CancellationTokenSource windowClosingCancel;

		// Dictionary defining streaming quality against low and high video bitrates, refer to ATEM Mini manual
		private readonly Dictionary<string, VideoBitrate> VideoBitrateDict = new Dictionary<string, VideoBitrate>
		{
			{ "Streaming High",     new VideoBitrate(6000000, 9000000) },
			{ "Streaming Medium",   new VideoBitrate(4500000, 7000000) },
			{ "Streaming Low",      new VideoBitrate(3000000, 4500000) },
		};

		private readonly IReadOnlyDictionary<_BMDSwitcherStreamRTMPState, string> StreamStateString = new Dictionary<_BMDSwitcherStreamRTMPState, string>()
		{
			{ _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateIdle,       "Idle" },
			{ _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateConnecting, "Connecting" },
			{ _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateStreaming,  "Streaming" },
			{ _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateStopping,   "Stopping" },
		};

		private void DispatchInMainWindow(Action action)
		{
			// Helper to ensure UI and switcher commands occur on MainWindow thread
			if (!Dispatcher.CheckAccess())
				Dispatcher.Invoke(action);
			else
				action.Invoke();
		}

		private static void EnableChildControls(DependencyObject parent, bool enable)
		{
			// Recursively find child elements and enable or disable
			for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
			{
				var child = VisualTreeHelper.GetChild(parent, i);

				if (child is UIElement)
					((UIElement)child).IsEnabled = enable;

				EnableChildControls(child, enable);
			}
		}

		public MainWindow()
		{
			InitializeComponent();
		}

		// Commences task to monitor stream status
		private async void StartStreamMonitoring()
		{
			if (switcherStreamDevice != null)
			{
				try
				{
					buttonStartStopStreaming.Content = "Stop Streaming";
					EnableChildControls(groupBoxStreamingSettings, false);

					streamStoppingCancel = new CancellationTokenSource();
					switcherDisconnectedCancel = new CancellationTokenSource();
					windowClosingCancel = new CancellationTokenSource();

					await switcherStreamDevice.TaskStream(streamStoppingCancel.Token, switcherDisconnectedCancel.Token, windowClosingCancel.Token);
				}
				catch (TaskCanceledException)
				{
				}
				finally
				{
					streamStoppingCancel.Dispose();
					streamStoppingCancel = null;

					switcherDisconnectedCancel.Dispose();
					switcherDisconnectedCancel = null;

					windowClosingCancel.Dispose();
					windowClosingCancel = null;

					buttonStartStopStreaming.Content = "Start Streaming";
					EnableChildControls(groupBoxStreamingSettings, true);
				}
			}
		}

		private void SetIPAddressTextBoxEnabled(bool enabled)
		{
			textBoxIPAddr1.IsEnabled = enabled;
			textBoxIPAddr2.IsEnabled = enabled;
			textBoxIPAddr3.IsEnabled = enabled;
			textBoxIPAddr4.IsEnabled = enabled;
		}

		private bool HandleIPAddressEntry(string ipAddressString, bool moveFocusOnPeriod)
		{
			bool enteredPeriod = moveFocusOnPeriod && ipAddressString.EndsWith(".");
			bool ipAddressIsValid = IsIpAddressByteValid(enteredPeriod ? ipAddressString.TrimEnd('.') : ipAddressString);

			if (enteredPeriod && ipAddressIsValid)
			{
				FocusNavigationDirection focusDirection = FocusNavigationDirection.Next;

				// MoveFocus takes a TraveralReqest as its argument.
				TraversalRequest request = new TraversalRequest(focusDirection);

				// Gets the element with keyboard focus.
				UIElement elementWithFocus = Keyboard.FocusedElement as UIElement;

				// Change keyboard focus.
				return elementWithFocus?.MoveFocus(request) ?? false;
			}

			return !ipAddressIsValid;
		}

		private bool HandleConnectEvent()
		{
			// Validate IP Address fields
			if (!IsIpAddressByteValid(textBoxIPAddr1.Text) ||
				!IsIpAddressByteValid(textBoxIPAddr2.Text) ||
				!IsIpAddressByteValid(textBoxIPAddr3.Text) ||
				!IsIpAddressByteValid(textBoxIPAddr4.Text))
				return true;

			string ipAddress = textBoxIPAddr1.Text + '.' + textBoxIPAddr2.Text + '.' + textBoxIPAddr3.Text + '.' + textBoxIPAddr4.Text;

			try
			{
				switcherStreamDevice = new SwitcherStreamDevice(ipAddress, switcherStreamMonitor, switcherMonitor);
			}
			catch (SwitcherConnectException e)
			{
				MessageBox.Show(e.Message, "Switcher Connection Error");
				return true;
			}
			catch (SwitcherStreamInvalidException e)
			{
				MessageBox.Show(e.Message, "Switcher Stream Invalid Error");
				return true;
			}
			// Update UI with values from connected switcher
			textBoxProductName.Text = switcherStreamDevice.ProductName;
			textBoxServiceName.Text = switcherStreamDevice.ServiceName;
			textBoxStreamURL.Text = switcherStreamDevice.StreamURL;
			textBoxStreamKey.Text = switcherStreamDevice.StreamKey;
			passwordBoxStreamKey.Password = textBoxStreamKey.Text;
			checkBoxLowLatency.IsChecked = switcherStreamDevice.LowLatency;

			SelectClosestQualityLevel();

			UpdateStreamStatus(switcherStreamDevice.StreamStatus);

			SetIPAddressTextBoxEnabled(false);
			buttonConnect.Content = "Disconnect";

			EnableChildControls(groupBoxStreamingSettings, true);
			buttonStartStopStreaming.Content = "Start Streaming";
			buttonStartStopStreaming.IsEnabled = true;

			if (switcherStreamDevice.IsStreaming)
			{
				// Switcher is already streaming, start stream monitor task
				StartStreamMonitoring();
			}

			return false;
		}

		private void UpdateStreamStatus(_BMDSwitcherStreamRTMPState state)
		{
			DispatchInMainWindow(() =>
			{
				string statusString;
				try
				{
					statusString = StreamStateString[state];
				}
				catch (KeyNotFoundException)
				{
					statusString = "Invalid";
				}
				statusBarItemStreamStatus.Text = statusString;
			});
		}

		private static bool IsIpAddressByteValid(string str)
		{
			return int.TryParse(str, out int i) && i >= 0 && i <= 255;
		}

		private void HandleDisconnectEvent()
		{
			switcherStreamDevice?.Disable();
			switcherStreamDevice = null;

			SetIPAddressTextBoxEnabled(true);
			EnableChildControls(groupBoxStreamingSettings, false);
			buttonStartStopStreaming.IsEnabled = false;
			textBoxProductName.Clear();
			buttonConnect.Content = "Connect";

			statusBarItemStreamStatus.Text = "Not connected";
		}

		private void UpdateStreamKey()
		{
			if (switcherStreamDevice?.StreamKey != textBoxServiceName.Text)
			{
				switcherStreamDevice.ServiceName = textBoxServiceName.Text;
			}
		}

		private void SelectClosestQualityLevel()
		{
			long? qualityDifference = null;
			var videoBitrates = switcherStreamDevice.VideoBitrates;
			int candidateIndex = 0;

			for (int idx = 0; idx < VideoBitrateDict.Count; idx++)
			{
				var value = VideoBitrateDict.ElementAt(idx).Value;
				long diff = Math.Abs(videoBitrates.Low - value.Low) +
					Math.Abs(videoBitrates.High - value.High);
				if (!qualityDifference.HasValue || diff < qualityDifference)
				{
					qualityDifference = diff;
					candidateIndex = idx;
				}
			}

			comboBoxStreamQuality.SelectedIndex = candidateIndex;
		}

		#region switcher_stream_monitor_events
		private void SwitcherStreamMonitor_ServiceNameUpdated(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { textBoxServiceName.Text = switcherStreamDevice.ServiceName; });
		}

		private void SwitcherStreamMonitor_StreamURLUpdated(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { textBoxStreamURL.Text = switcherStreamDevice.StreamURL; });
		}

		private void SwitcherStreamMonitor_StreamKeyChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() =>
			{
				textBoxStreamKey.Text = switcherStreamDevice.StreamKey;
				passwordBoxStreamKey.Password = switcherStreamDevice.StreamKey;
			});
		}

		private void SwitcherStreamMonitor_VideoBitratesChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { SelectClosestQualityLevel(); });
		}

		private void SwitcherStreamMonitor_LowLatencyChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { checkBoxLowLatency.IsChecked = switcherStreamDevice.LowLatency; });
		}

		private void SwitcherStreamMonitor_TimecodeChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { statusBarItemTimecode.Text = switcherStreamDevice.TimecodeHMSString; });
		}

		private void SwitcherStreamMonitor_CachedUsedChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { statusBarItemCachedUsedBar.Value = switcherStreamDevice.CacheUsed; });
		}

		private void SwitcherStreamMonitor_EncodingBitrateChanged(object sender, EventArgs e)
		{
			DispatchInMainWindow(() => { statusBarItemEncodingBitrate.Text = String.Format("{0:F1} Mb/s", switcherStreamDevice.EncodingBitrate / 1000000.0); });
		}

		private void SwitcherStreamMonitor_StatusUpdated(object sender, StatusUpdatedEventArgs e)
		{
			if (e.StateType == _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateStreaming)
			{
				// Streaming has started, start stream monitoring task
				DispatchInMainWindow(() => { StartStreamMonitoring(); });
			}
			else if (e.StateType == _BMDSwitcherStreamRTMPState.bmdSwitcherStreamRTMPStateStopping)
			{
				// Streaming is stopping, stop monitoring task
				streamStoppingCancel?.Cancel();
			}

			UpdateStreamStatus(e.StateType);
		}
		#endregion

		#region switcher_monitor_events
		private void SwitcherMonitor_SwitcherDisconnected(object sender, EventArgs e)
		{
			switcherDisconnectedCancel?.Cancel();
			DispatchInMainWindow(() => { HandleDisconnectEvent(); });
		}
		#endregion

		#region uievents
		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			comboBoxStreamQuality.ItemsSource = VideoBitrateDict;
			comboBoxStreamQuality.DisplayMemberPath = "Key";
			comboBoxStreamQuality.SelectedValuePath = "Value";
			comboBoxStreamQuality.SelectedIndex = 0;

			switcherMonitor = new SwitcherMonitor();
			switcherMonitor.SwitcherDisconnected += SwitcherMonitor_SwitcherDisconnected;

			switcherStreamMonitor = new SwitcherStreamMonitor();
			switcherStreamMonitor.ServiceNameChanged += SwitcherStreamMonitor_ServiceNameUpdated;
			switcherStreamMonitor.StreamURLChanged += SwitcherStreamMonitor_StreamURLUpdated;
			switcherStreamMonitor.StreamKeyChanged += SwitcherStreamMonitor_StreamKeyChanged;
			switcherStreamMonitor.VideoBitratesChanged += SwitcherStreamMonitor_VideoBitratesChanged;
			switcherStreamMonitor.LowLatencyChanged += SwitcherStreamMonitor_LowLatencyChanged;
			switcherStreamMonitor.TimecodeChanged += SwitcherStreamMonitor_TimecodeChanged;
			switcherStreamMonitor.CachedUsedChanged += SwitcherStreamMonitor_CachedUsedChanged;
			switcherStreamMonitor.EncodingBitrateChanged += SwitcherStreamMonitor_EncodingBitrateChanged;

			switcherStreamMonitor.StatusUpdated += SwitcherStreamMonitor_StatusUpdated;

			// Keep settings button inactive until connection
			EnableChildControls(groupBoxStreamingSettings, false);
			buttonStartStopStreaming.IsEnabled = false;
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			windowClosingCancel?.Cancel();
			switcherStreamDevice = null;

			switcherStreamMonitor.ServiceNameChanged -= SwitcherStreamMonitor_ServiceNameUpdated;
			switcherStreamMonitor.StreamURLChanged -= SwitcherStreamMonitor_StreamURLUpdated;
			switcherStreamMonitor.StreamKeyChanged -= SwitcherStreamMonitor_StreamKeyChanged;
			switcherStreamMonitor.LowLatencyChanged -= SwitcherStreamMonitor_LowLatencyChanged;
			switcherStreamMonitor.TimecodeChanged -= SwitcherStreamMonitor_TimecodeChanged;
			switcherStreamMonitor.CachedUsedChanged -= SwitcherStreamMonitor_CachedUsedChanged;
			switcherStreamMonitor.EncodingBitrateChanged -= SwitcherStreamMonitor_EncodingBitrateChanged;
			switcherStreamMonitor.StatusUpdated -= SwitcherStreamMonitor_StatusUpdated;

			switcherMonitor.SwitcherDisconnected -= SwitcherMonitor_SwitcherDisconnected;
		}

		private void TextBoxIPAddr1_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			e.Handled = HandleIPAddressEntry(((TextBox)sender).Text + e.Text, true);
		}

		private void TextBoxIPAddr2_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			e.Handled = HandleIPAddressEntry(((TextBox)sender).Text + e.Text, true);
		}

		private void TextBoxIPAddr3_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			e.Handled = HandleIPAddressEntry(((TextBox)sender).Text + e.Text, true);
		}

		private void TextBoxIPAddr4_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			if (e.Text == "\r")
			{
				e.Handled = HandleConnectEvent();
			}
			else
			{
				e.Handled = HandleIPAddressEntry(((TextBox)sender).Text + e.Text, false);
			}
		}

		private void BorderIPAddr_MouseEnter(object sender, MouseEventArgs e)
		{
			borderIPAddr.BorderBrush = new SolidColorBrush(Color.FromArgb(255, 126, 180, 234)); ;
		}

		private void BorderIPAddr_MouseLeave(object sender, MouseEventArgs e)
		{
			borderIPAddr.BorderBrush = new SolidColorBrush(Color.FromArgb(255, 171, 173, 179)); ;
		}

		private void ButtonConnect_Click(object sender, RoutedEventArgs e)
		{
			if (switcherStreamDevice == null)
			{
				HandleConnectEvent();
			}
			else
			{
				HandleDisconnectEvent();
			}
		}

		private void ButtonStartStopStreaming_Click(object sender, RoutedEventArgs e)
		{
			if (switcherStreamDevice == null)
				return;

			if (switcherStreamDevice.IsStreaming)
			{
				switcherStreamDevice.StopStreaming();
			}
			else
			{
				switcherStreamDevice.StartStreaming();
			}
		}

		private void TextBoxServiceName_LostFocus(object sender, RoutedEventArgs e)
		{
			if (switcherStreamDevice?.ServiceName != textBoxServiceName.Text)
			{
				switcherStreamDevice.ServiceName = textBoxServiceName.Text;
			}
		}

		private void TextBoxStreamURL_LostFocus(object sender, RoutedEventArgs e)
		{
			if (switcherStreamDevice?.StreamURL != textBoxStreamURL.Text)
			{
				switcherStreamDevice.StreamURL = textBoxStreamURL.Text;
			}
		}

		private void TextBoxStreamKey_LostFocus(object sender, RoutedEventArgs e)
		{
			passwordBoxStreamKey.Password = textBoxStreamKey.Text;
			UpdateStreamKey();
		}

		private void PasswordBoxStreamKey_LostFocus(object sender, RoutedEventArgs e)
		{
			textBoxStreamKey.Text = passwordBoxStreamKey.Password;
			UpdateStreamKey();
		}

		private void ButtonStreamKeyHide_Click(object sender, RoutedEventArgs e)
		{
			if (streamKeyHidden)
			{
				buttonStreamKeyHide.Content = "Hide";
				passwordBoxStreamKey.Visibility = Visibility.Hidden;
				textBoxStreamKey.Visibility = Visibility.Visible;
				streamKeyHidden = false;
			}
			else
			{
				buttonStreamKeyHide.Content = "Show";
				passwordBoxStreamKey.Visibility = Visibility.Visible;
				textBoxStreamKey.Visibility = Visibility.Hidden;
				streamKeyHidden = true;
			}
		}

		private void ComboBoxStreamQuality_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (switcherStreamDevice == null || !userStreamQualitySelection)
				// Ignore event when stream quality is updated externally
				return;

			var selectedValue = (VideoBitrate)comboBoxStreamQuality.SelectedValue;
			switcherStreamDevice.VideoBitrates = selectedValue;
		}

		private void CheckBoxLowLatency_Click(object sender, RoutedEventArgs e)
		{
			switcherStreamDevice.LowLatency = checkBoxLowLatency.IsChecked ?? false;
		}

		private void ComboBoxStreamQuality_DropDownOpened(object sender, EventArgs e)
		{
			userStreamQualitySelection = true;
		}

		private void ComboBoxStreamQuality_DropDownClosed(object sender, EventArgs e)
		{
			userStreamQualitySelection = false;
		}
		#endregion
	}
}
