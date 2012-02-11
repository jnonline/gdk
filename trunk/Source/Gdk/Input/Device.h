/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Types of physical hardware devices
    ///	@see 
    ///		Device::GetDeviceType()
    // =================================================================================
	namespace DeviceType
	{
		enum Enum
		{
			Unknown = 0, 

			/// General Desktop/Laptop computers
			PC = 1,
			
			/// The application is being hosted inside a web browser and cannot determine the physical device information.
			Web = 2,

			/// Apple iPad
			iPad = 101,
            
            /// Apple iPhone
			iPhone = 102,
            
            /// Apple iPod Touch
			iPodTouch = 103,
			
			/// Android-based Phone
			AndroidPhone = 201,
            
            /// Android-based Tablet
			AndroidTablet = 202
		};
        
        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
		const char* ToString(DeviceType::Enum value);
        
	}

	// =================================================================================
    ///	@brief
    ///		Types of run-time platforms.
    ///	@see 
    ///		Device::GetPlatformType()
    // =================================================================================
	namespace PlatformType
	{
		enum Enum
		{
			Unknown = 0,

            /// Microsoft Windows
			Windows = 1,
            
            /// Apple Mac OSX
			Mac = 2,
            
            /// Linux
			Linux = 3,
            
            /// Apple iOS
			iOS = 4,
            
            /// Android OS
			Android = 5,
            
            // Google Native Client
			NaCl = 6,
		};

        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
		const char* ToString(PlatformType::Enum value);
	}

    // =================================================================================
    ///	@brief
    ///		Contains run-time platform version information.
    ///	@see 
    ///		Device::GetPlatformVersion()
    // =================================================================================
	struct PlatformVersion
	{
        /// Major version of the platform OS
		UInt32	MajorVersion;
        
        /// Minor version of the platform OS
		UInt32	MinorVersion;
        
        /// Revision number of the platform OS
		UInt32	Revision;

        // ---------------------------------
        /// @name Constructors
        /// @{
        
		// *****************************************************************
        /// @brief
        ///     Default constructor
        // *****************************************************************
		PlatformVersion()
		{
			MajorVersion = 0;
			MinorVersion = 0;
			Revision = 0;
		}

		// *****************************************************************
        /// @brief
        ///     Parameterized constructor
        /// @param majorVersion
        ///     Major version of the platform OS
        /// @param minorVersion
        ///     Minor version of the platform OS
        /// @param revision
        ///     Revision number of the platform OS
        // *****************************************************************
		PlatformVersion(UInt32 majorVersion, UInt32 minorVersion, UInt32 revision)
		{
			MajorVersion = majorVersion;
			MinorVersion = minorVersion;
			Revision = revision;
		}
        
        /// @}
	};

	// =================================================================================
    ///	@brief
    ///		Enumeration of supported device orientation values.
    ///	@see 
    ///		Device::GetOrientation()
    // =================================================================================
	namespace DeviceOrientation
	{
		enum Enum
		{
            /// Unable to determine the device's current orientation.
			Unknown,
            
            /// Portrait orientation, with the top of the device facing up.
			Portrait,
            
            /// Portrait orientation, with the top of the device facing down.
			PortraitUpsideDown,
            
            /// Landscape orientation, with the top of the device on the left side
			LandscapeLeft,
            
            /// Landscape orientation, with the top of the device on the right side
			LandscapeRight
		};
        
        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
        const char* ToString(DeviceOrientation::Enum value);
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of the device battery charging states.
    ///	@see 
    ///		Device::GetBatteryState()
    // =================================================================================
	namespace DeviceBatteryState
	{
		enum Enum
		{
            /// Unable to determine the device's battery status
			Unknown,
            
            /// The battery is currently charging.
			Charging,
            
            /// The device is currently unplugged and not charging.
			Unplugged,
		};
        
        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
        const char* ToString(DeviceBatteryState::Enum value);
	}
    
    // =================================================================================
    ///	@brief
    ///		Types of media control actions.
    ///	@remarks 
    ///		Several devices support media control buttons on the device hardware or external 
    ///     peripherials, such as remotes, headsets, or headphones.  This enumeration contains
    ///     all the supported media control events that the GDK can detect.
    /// @see
    ///     Device::MediaControlAction
    // =================================================================================
	namespace MediaControlActions
	{
		enum Enum
		{
            /// The 'Play button
            Play,
            
            /// The 'Pause' button
            Pause,
            
            /// The 'Stop' button
            Stop,
            
            /// The 'Play/Pause' button has been toggled
            TogglePlayPause,
            
            /// The 'Next Track' button
            NextTrack,
            
            /// The 'Previous Track' button
            PreviousTrack,
            
            /// The 'Fast-Forward' button has been pressed
            BeginSeekForward,
            
            /// The 'Fast-Forward' button has been released
            EndSeekForward,
            
            /// The 'Rewind' button has been pressed
            BeginSeekBackward,
            
            /// The 'Rewind' button has been pressed
            EndSeekBackward,
		};
        
        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
        const char* ToString(MediaControlActions::Enum value);
	}
    
	// =================================================================================
    ///	@brief
    ///		Interface for accessing/controller device information.
    ///	@remarks 
    ///		This static-only class contains methods and properties for accessing device 
    ///     specific information, about the device hardware, platform, and operating system.
    // =================================================================================
	class Device
	{
	public: 

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Platform Methods
        /// @{

        static PlatformType::Enum GetPlatformType();
        static PlatformVersion GetPlatformVersion();

        /// @}
        
        // ---------------------------------
        /// @name Device & Hardware Methods
        /// @{
        
        static DeviceType::Enum GetDeviceType();
        static const string& GetDeviceDisplayName();
		static const string& GetDeviceUniqueId();
        static bool IsHandheld();
        static bool IsTablet();
        
        static DeviceOrientation::Enum GetOrientation();
        
        static DeviceBatteryState::Enum GetBatteryState();
        static float GetBatteryLevel();
        
        static void Vibrate();
        
        /// @}
        
        // Public Types
		// =====================================================
		
        // ---------------------------------
        /// @name Delegate Types
        /// @{
        
        /// Multicast delegate type for MediaControlActions enumeration events
        typedef Gdk::MulticastDelegate1<void, MediaControlActions::Enum>		MediaControlActionHandler;
        
        /// Multicast delegate type for DeviceOrientation enumeration events
        typedef Gdk::MulticastDelegate1<void, DeviceOrientation::Enum>			DeviceOrientationHandler;

        /// @}
        
        // Public Events
		// =====================================================
		
        /// @name Events
        /// @{
        
		/// @brief
		///     This event is raised when the device's orientation changes
		static DeviceOrientationHandler		OrientationChanged;
        
        /// @brief
		///     This event is raised when the device receives a media control action from a remote source
		static MediaControlActionHandler	MediaControlAction;
        
        /// @}
        
    public:
        
        // INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
        static void Platform_ProcessMediaControlAction(MediaControlActions::Enum action);
		static void Platform_SetDeviceOrientation(DeviceOrientation::Enum orientation);
        
        /// @}
        
        /// @endcond
        
	private:

		// Internal Properties
		// ================================

		static DeviceType::Enum			deviceType;
		static PlatformType::Enum		platformType;  
		static PlatformVersion			platformVersion;
		static DeviceOrientation::Enum	deviceOrientation;

		static string					deviceDisplayName;
		static string					deviceUniqueId;
        
        // Internal Methods
		// ================================

		// Init
		static void Init();
        
        friend class Application;
	};
    
    /// @} // Input


} // namespace Gdk
