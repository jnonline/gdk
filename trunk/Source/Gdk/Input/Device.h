#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================

	namespace DeviceType
	{
		enum Enum
		{
			Unknown = 0, 

			// Desktop/Laptop computers:   Ex: Windows, Mac, Linux
			PC = 1,
			
			// Web Interface:  NaCl
			Web = 2,

			// iOS Devices
			iPad = 101,
			iPhone = 102,
			iPodTouch = 103,
			
			// Android Devices
			AndroidPhone = 201,
			AndroidTablet = 202
		};

		const char* ToString(DeviceType::Enum value);
	}

	// ============================================================================

	namespace PlatformType
	{
		enum Enum
		{
			Unknown = 0,

			Windows = 1,
			Mac = 2,
			Linux = 3,
			iOS = 4,
			Android = 5,
			NaCl = 6,
		};

		const char* ToString(PlatformType::Enum value);
	}

	// ============================================================================

	struct PlatformVersion
	{
		UInt32	MajorVersion;
		UInt32	MinorVersion;
		UInt32	Revision;

		// Default CTor
		PlatformVersion()
		{
			MajorVersion = 0;
			MinorVersion = 0;
			Revision = 0;
		}

		// CTor
		PlatformVersion(UInt32 majorVersion, UInt32 minorVersion, UInt32 revision)
		{
			MajorVersion = majorVersion;
			MinorVersion = minorVersion;
			Revision = revision;
		}
	};

	// ============================================================================

	namespace DeviceOrientation
	{
		enum Enum
		{
			Unknown,
			Portrait,
			PortraitUpsideDown,
			LandscapeLeft,			// Top of the device is on the left side
			LandscapeRight			// Top of the device is on the right side
		};
        
        const char* ToString(DeviceOrientation::Enum value);
	}

	// ============================================================================

	namespace DeviceBatteryState
	{
		enum Enum
		{
			Unknown,
			Charging,
			Unplugged,
		};
        
        const char* ToString(DeviceBatteryState::Enum value);
	}
    
	// ============================================================================
    
    // The Media Control Actions are special inputs from external 
    // accessories such as remotes, external buttons, headsets, etc.
    
	namespace MediaControlActions
	{
		enum Enum
		{
            Play,
            Pause,
            Stop,
            TogglePlayPause,
            NextTrack,
            PreviousTrack,
            BeginSeekForward,
            EndSeekForward,
            BeginSeekBackward,
            EndSeekBackward,
		};
        
        const char* ToString(MediaControlActions::Enum value);
	}
    
	// ============================================================================

	class Device
	{
	public: 

        // Types
        // ----------------------------------
        
        // Delegates
        typedef Gdk::MulticastDelegate1<void, MediaControlActions::Enum>		MediaControlActionHandler;
        typedef Gdk::MulticastDelegate1<void, DeviceOrientation::Enum>			DeviceOrientationHandler;
        
		// Platform / Device details
		// --------------------------------------------

		// Gets information about the physical device
		static DeviceType::Enum GetDeviceType()	{ return deviceType; }

		// Get the type of the platform / OS
		static PlatformType::Enum GetPlatformType()		{ return platformType; }
		
		// Get the platform version (IE:  Windows [7.0], iOS [4.3], MAC [10.6.6], etc)
		static PlatformVersion GetPlatformVersion()		{ return platformVersion; }

        // Name of the device (IE: Bob's iPad)
		static const wstring& GetDeviceDisplayName()		{ return deviceDisplayName; }

        // Unique ID of the device
		static const string& GetDeviceUniqueId()			{ return deviceUniqueId; }
        
		// Quick utility methods to determine platform functionality
		static bool IsHandheld();  //		- phones / ipods / etc
		static bool IsTablet();    //		- iPad / android tablet / etc

		// Orientation
		// --------------------------------------------

		static DeviceOrientation::Enum GetOrientation()		{ return deviceOrientation; }
		static DeviceOrientationHandler		OrientationChanged;
		
		// Battery
		// --------------------------------------------

		static DeviceBatteryState::Enum GetBatteryState();
		static float GetBatteryLevel();
        
        // Media Control
        // --------------------------------------------
        
		// This event is raised any time a Remote Control action is sent to the device
		static MediaControlActionHandler	MediaControlAction;
        
        // Vibration
        // --------------------------------------------
        
        // Causes the device to vibrate
        static void Vibrate();
        
    public:
        
        // Public Platform Interfaces
		// ----------------------------------
        
		// Input processing
        static void Platform_ProcessMediaControlAction(MediaControlActions::Enum action);
		static void Platform_SetDeviceOrientation(DeviceOrientation::Enum orientation);
        
	private:

		// Internals
		// --------------------------------------------

		static DeviceType::Enum			deviceType;
		static PlatformType::Enum		platformType;  
		static PlatformVersion			platformVersion;
		static DeviceOrientation::Enum	deviceOrientation;

		static wstring					deviceDisplayName;
		static string					deviceUniqueId;
        
		// Hidden CTor
		Device() {}

		// Init
		friend class Application;
		static void Init();
	};


} // namespace Gdk
