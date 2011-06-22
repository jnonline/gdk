
#include "BasePCH.h"
#include "Device.h"

using namespace Gdk;

// Static Instantiations
DeviceType::Enum			Device::deviceType = DeviceType::Unknown;
PlatformType::Enum			Device::platformType = PlatformType::Unknown;  
PlatformVersion				Device::platformVersion;
DeviceOrientation::Enum		Device::deviceOrientation = DeviceOrientation::Unknown;
wstring						Device::deviceDisplayName;
string						Device::deviceUniqueId;

// Events
Device::MediaControlActionHandler	Device::MediaControlAction;
Device::DeviceOrientationHandler	Device::OrientationChanged;

// ===================================================================================
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
	};

	return StringUtilities::Unknown;
}

// ===================================================================================
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
	};

	return StringUtilities::Unknown;
}

// ===================================================================================
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
	};
    
	return StringUtilities::Unknown;
}

// ===================================================================================
const char* DeviceBatteryState::ToString(DeviceBatteryState::Enum value)
{
	static const char* charging = "Charging";
	static const char* unplugged = "Unplugged";
    
	switch(value)
	{
		case DeviceBatteryState::Charging:      return charging;
		case DeviceBatteryState::Unplugged:     return unplugged;
	};
    
	return StringUtilities::Unknown;
}

// ===================================================================================
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

// ===================================================================================
bool Device::IsHandheld()
{
	return
		deviceType == DeviceType::iPhone ||
		deviceType == DeviceType::iPodTouch ||
		deviceType == DeviceType::AndroidPhone;
}

// ===================================================================================
bool Device::IsTablet()
{
	return 
		deviceType == DeviceType::iPad ||
		deviceType == DeviceType::AndroidTablet;
}

// ===================================================================================
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

// ===================================================================================
DeviceBatteryState::Enum Device::GetBatteryState()
{
	return _Gdk_Platform_Device_GetBatteryState();
}

// ===================================================================================
float Device::GetBatteryLevel()
{
	return _Gdk_Platform_Device_GetBatteryLevel();
}

// ===================================================================================
void Device::Vibrate()
{
	return _Gdk_Platform_Device_Vibrate();
}

// ===================================================================================
void Device::Platform_ProcessMediaControlAction(MediaControlActions::Enum action)
{
    // Fire the MediaControlAction event
    MediaControlAction.Invoke(action);
}

// ===================================================================================
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
