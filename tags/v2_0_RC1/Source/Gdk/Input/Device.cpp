/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Device.h"

using namespace Gdk;

// Static Instantiations
DeviceType::Enum                    Device::deviceType = DeviceType::Unknown;
PlatformType::Enum                  Device::platformType = PlatformType::Unknown;  
PlatformVersion                     Device::platformVersion;
DeviceOrientation::Enum             Device::deviceOrientation = DeviceOrientation::Unknown;
wstring                             Device::deviceDisplayName;
string                              Device::deviceUniqueId;

// Events
Device::MediaControlActionHandler	Device::MediaControlAction;
Device::DeviceOrientationHandler	Device::OrientationChanged;

// *****************************************************************
const char* DeviceType::ToString(DeviceType::Enum value)
{
	static const char* pc = "PC";
	static const char* web = "Web";
	static const char* iPad = "iPad";
	static const char* iPhone = "iPhone";
	static const char* iPodTouch = "iPodTouch";
	static const char* androidPhone = "AndroidPhone";
	static const char* androidTablet = "AndroidTablet";

	switch(value)
	{
		case DeviceType::PC:				return pc;
		case DeviceType::Web:				return web;
		case DeviceType::iPad:				return iPad;
		case DeviceType::iPhone:			return iPhone;
		case DeviceType::iPodTouch:			return iPodTouch;
		case DeviceType::AndroidPhone:		return androidPhone;
		case DeviceType::AndroidTablet:		return androidTablet;
        case DeviceType::Unknown:           return StringUtilities::Unknown;
	};

	return StringUtilities::Unknown;
}

// *****************************************************************
const char* PlatformType::ToString(PlatformType::Enum value)
{
	static const char* windows = "Windows";
	static const char* mac = "Mac";
	static const char* linux = "Linux";
	static const char* iOS = "iOS";
	static const char* android = "Android";
	static const char* naCl = "NaCl";

	switch(value)
	{
		case PlatformType::Windows:		return windows;
		case PlatformType::Mac:			return mac;
		case PlatformType::Linux:		return linux;
		case PlatformType::iOS:			return iOS;
		case PlatformType::Android:		return android;
		case PlatformType::NaCl:		return naCl;
        case PlatformType::Unknown:     return StringUtilities::Unknown;
	};

	return StringUtilities::Unknown;
}

// *****************************************************************
const char* DeviceOrientation::ToString(DeviceOrientation::Enum value)
{
	static const char* portrait = "Portrait";
	static const char* portraitUpsideDown = "PortraitUpsideDown";
	static const char* landscapeLeft = "LandscapeLeft";
	static const char* landscapeRight = "LandscapeRight";
    
	switch(value)
	{
		case DeviceOrientation::Portrait:               return portrait;
		case DeviceOrientation::PortraitUpsideDown:		return portraitUpsideDown;
		case DeviceOrientation::LandscapeLeft:          return landscapeLeft;
		case DeviceOrientation::LandscapeRight:			return landscapeRight;
        case DeviceOrientation::Unknown:                return StringUtilities::Unknown;
	};
    
	return StringUtilities::Unknown;
}

// *****************************************************************
const char* DeviceBatteryState::ToString(DeviceBatteryState::Enum value)
{
	static const char* charging = "Charging";
	static const char* unplugged = "Unplugged";
    
	switch(value)
	{
		case DeviceBatteryState::Charging:      return charging;
		case DeviceBatteryState::Unplugged:     return unplugged;
        case DeviceBatteryState::Unknown:       return StringUtilities::Unknown;
	};
    
	return StringUtilities::Unknown;
}

// *****************************************************************
const char* MediaControlActions::ToString(MediaControlActions::Enum value)
{
	static const char* play = "Play";
	static const char* pause = "Pause";
    static const char* stop = "Stop";
    static const char* togglePlayPause = "TogglePlayPause";
    static const char* nextTrack = "v";
    static const char* previousTrack = "PreviousTrack";
    static const char* beginSeekForward = "BeginSeekForward";
    static const char* endSeekForward = "EndSeekForward";
    static const char* beginSeekBackward = "BeginSeekBackward";
    static const char* endSeekBackward = "EndSeekBackward";
    
	switch(value)
	{
		case MediaControlActions::Play:                 return play;
		case MediaControlActions::Pause:                return pause;
        case MediaControlActions::Stop:                 return stop;
        case MediaControlActions::TogglePlayPause:      return togglePlayPause;
        case MediaControlActions::NextTrack:            return nextTrack;
        case MediaControlActions::PreviousTrack:        return previousTrack;
        case MediaControlActions::BeginSeekForward:     return beginSeekForward;
        case MediaControlActions::EndSeekForward:       return endSeekForward;
        case MediaControlActions::BeginSeekBackward:    return beginSeekBackward;
        case MediaControlActions::EndSeekBackward:      return endSeekBackward;
	};
    
	return StringUtilities::Unknown;
}

// *****************************************************************
/// @brief
///     Internal method for initializing the Device class static data.
/// @note 
///     Internal Use Only
// *****************************************************************
void Device::Init()
{
	// Call the external platform interfaces to get the device/plaform details
	deviceType = _Gdk_Platform_Device_GetDeviceType();
	platformType = _Gdk_Platform_Device_GetPlatformType();
	platformVersion = _Gdk_Platform_Device_GetPlatformVersion();
    
	// Get the device name & unique id
	deviceDisplayName = _Gdk_Platform_Device_GetDeviceDisplayName();
    deviceUniqueId = _Gdk_Platform_Device_GetDeviceUniqueId();
    
	// Verify we got platform details..
	ASSERT(deviceType != DeviceType::Unknown, L"The device type for the platform could not been determined");
	ASSERT(platformType != PlatformType::Unknown, L"The platform type could not been determined");
    
	// Log the device details
	LOG_SYSTEM(
               L"Device Type: %hs", 
               DeviceType::ToString(deviceType)
               );
    
	// Log the platform details
	LOG_SYSTEM(
               L"Platform Type: %hs  [Version=%d.%d.%d]", 
               PlatformType::ToString(platformType), 
               platformVersion.MajorVersion,
               platformVersion.MinorVersion,
               platformVersion.Revision
               );
    
    LOG_SYSTEM(
               L"Device Name: %ls",
               GetDeviceDisplayName().c_str()
               );
    
	LOG_SYSTEM(
               L"Device UniqueId: %hs",
               GetDeviceUniqueId().c_str()
               );
}


// *****************************************************************
/// @brief
///     Gets the hardware device type of the current device
/// @remarks
///     The device type is the actual type of hardware the application is running on.
// *****************************************************************
DeviceType::Enum Device::GetDeviceType()        
{ 
    return deviceType; 
}


// *****************************************************************
/// @brief
///     Gets the platform type the application is running on
/// @remarks
///     The platform type is a combination of the current operating system and runtime environment 
///     on the device, in which the GDK is being hosted.
// *****************************************************************
PlatformType::Enum Device::GetPlatformType()		
{ 
    return platformType; 
}

// *****************************************************************
/// @brief
///     Gets the version information of the current platform.
/// @remarks
///     This is typically the operating system version.  Common examples are:
///     @li [7.0.0]  for Windows 7
///     @li [10.7.1] for Mac OSX Lion
///     @li [4.2.1]  for iOS
// *****************************************************************
PlatformVersion Device::GetPlatformVersion()		
{ 
    return platformVersion; 
}

// *****************************************************************
/// @brief
///     Gets the name of the current device or computer.
/// @remarks
///     This is the user assigned name of the device.  Exampled "Bob's iPad"
// *****************************************************************
const wstring& Device::GetDeviceDisplayName()		
{ 
    return deviceDisplayName; 
}

// *****************************************************************
/// @brief
///     Gets a unique string ID for the current device. 
/// @remarks
///     The returned string id is guaranteed to be unique across devices, computers, platforms, etc.  
///     The ID itself does not have a guaranteed format, as it may be a MAC Address, a Serial Number, 
///     or some other unique string determined from the device hardware
// *****************************************************************
const string& Device::GetDeviceUniqueId()			
{ 
    return deviceUniqueId; 
}


// *****************************************************************
/// @brief
///     Returns true if this device is a hand-held device. 
/// @remarks
///     The definition of a hand-held device, is a small, one-hand usable device, such as a phone, mp3 player, or PDA
// *****************************************************************
bool Device::IsHandheld()
{
	return
		deviceType == DeviceType::iPhone ||
		deviceType == DeviceType::iPodTouch ||
		deviceType == DeviceType::AndroidPhone;
}

// *****************************************************************
/// @brief
///     Returns true if this device is a tablet-sized device. 
/// @remarks
///     The definition of a tablet device, is a large (bigger than 5" screen) device which is intended to 
///     be used with 2 hands, such as an iPad or an Android Tablet.
// *****************************************************************
bool Device::IsTablet()
{
	return 
		deviceType == DeviceType::iPad ||
		deviceType == DeviceType::AndroidTablet;
}

// *****************************************************************
/// @brief
///     Gets the current physical orientation of the device
// *****************************************************************
DeviceOrientation::Enum Device::GetOrientation()
{ 
    return deviceOrientation; 
}

// *****************************************************************
/// @brief
///     Gets the charging state of the device battery
// *****************************************************************
DeviceBatteryState::Enum Device::GetBatteryState()
{
	return _Gdk_Platform_Device_GetBatteryState();
}

// *****************************************************************
/// @brief
///     Gets the charge level of the device battery.  (0=Dead, 1=Fully Charged)
// *****************************************************************
float Device::GetBatteryLevel()
{
	return _Gdk_Platform_Device_GetBatteryLevel();
}

// *****************************************************************
/// @brief
///     Tells the device to vibrate, if supported.
// *****************************************************************
void Device::Vibrate()
{
	return _Gdk_Platform_Device_Vibrate();
}

// *****************************************************************
/// @brief
///     Tells the Device class about a new media control event
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Device::Platform_ProcessMediaControlAction(MediaControlActions::Enum action)
{
    // Fire the MediaControlAction event
    MediaControlAction.Invoke(action);
}

// *****************************************************************
/// @brief
///     Tells the Device class that the device orientation has changed
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Device::Platform_SetDeviceOrientation(DeviceOrientation::Enum orientation)
{
	// Is this orientation different then what we already had?
	if(orientation != deviceOrientation)
	{
		// Update the orientation
		deviceOrientation = orientation;

		// Fire an event for the orientation change
		OrientationChanged.Invoke(orientation);
	}
}
