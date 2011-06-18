//
// PlatformImpl.mm
// 

#include "BasePCH.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <AudioToolbox/AudioServices.h>
#include <sys/sysctl.h>

using namespace Gdk;


bool iOS_battery_monitoring_enabled = false;

/// ===========================================================
///
/// IO
///
/// ===========================================================

string _Gdk_Platform_IO_GetCommonPath(CommonPaths::Enum commonPath)
{
	switch(commonPath)
    {
        case CommonPaths::ApplicationFolder:
        {
            // Get the application's bundle path
			NSString* nsBundlePath = [[NSBundle mainBundle] resourcePath];
			return string([nsBundlePath UTF8String]);
		}
            
        case CommonPaths::AssetFolder:
        {
			// Get the application's bundle path & append "Assets"
			NSString* nsBundlePath = [[NSBundle mainBundle] resourcePath];
			return Path::Combine([nsBundlePath UTF8String], "Assets");
        }
            
        case CommonPaths::WorkingFolder:
        {
			// Get the application's "Documents" path
			NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
			NSString* documentsFolder = [paths objectAtIndex:0];
			return string([documentsFolder UTF8String]);
        }
    }

	return string();
}

/// ===========================================================
///
///	Window / Application
///
/// ===========================================================


// ***************************************************************
void _Gdk_Platform_Resize(int width, int height)
{
    // Do Nothing, cant resize on an iOS device
}

// ***************************************************************
void _Gdk_Platform_SetTitle(wchar_t const*)
{
    // Do Nothing, no window title on an iOS device
}


/// ===========================================================
///
/// Mouse
///
/// ===========================================================


// ***************************************************************
void _Gdk_Platform_Mouse_ShowCursor(bool show)
{
    // Do Nothing, no mouse on an iOS device
}

// ***************************************************************
void _Gdk_Platform_Mouse_SetPosition(int x, int y)
{
    // Do Nothing, no mouse on an iOS device
}

/// ===========================================================
///
/// Device
///
/// ===========================================================

#define IPHONE_3_NAMESTRING             @"iPhone 3G S" 
#define IPHONE_4_NAMESTRING				@"iPhone 4" 
#define IPHONE_5_NAMESTRING				@"iPhone 5"

#define IPOD_3_NAMESTRING				@"iPod touch 3"
#define IPOD_4_NAMESTRING				@"iPod touch 4"

#define IPAD_1_NAMESTRING				@"iPad"
#define IPAD_2_NAMESTRING				@"iPad 2"

#define SIMULATOR_IPHONE_NAMESTRING     @"iPhone Simulator"
#define SIMULATOR_IPAD_NAMESTRING       @"iPad Simulator"


// ***************************************************************
Gdk::DeviceType::Enum _Gdk_Platform_Device_GetDeviceType()
{
	// Determine the device type!
	NSString* deviceModel = [[UIDevice currentDevice] model];
    
    // iPhones
    if([deviceModel isEqualToString:IPHONE_3_NAMESTRING])
        return DeviceType::iPhone3GS;
    else if([deviceModel isEqualToString:IPHONE_4_NAMESTRING])
        return DeviceType::iPhone4;
    
    // iPods
    else if([deviceModel isEqualToString:IPOD_3_NAMESTRING])
        return DeviceType::iPod3;
    else if([deviceModel isEqualToString:IPOD_4_NAMESTRING])
        return DeviceType::iPod4;
    
    // iPads
    else if([deviceModel isEqualToString:IPAD_1_NAMESTRING])
        return DeviceType::iPad1;
    else if([deviceModel isEqualToString:IPAD_2_NAMESTRING])
        return DeviceType::iPad2;
    
    // Simulators
    else if([deviceModel isEqualToString:SIMULATOR_IPHONE_NAMESTRING])
        return DeviceType::iPhone4;
    else if([deviceModel isEqualToString:SIMULATOR_IPAD_NAMESTRING])
        return DeviceType::iPad2;

 
    NSLog(@"GDK WARNING!!!!  Unrecognized iOS device %@.  [defaulting as iPhone 4]", deviceModel);
    return DeviceType::iPhone4;  // Fallback to iPhone 4
}

// ***************************************************************
Gdk::PlatformType::Enum _Gdk_Platform_Device_GetPlatformType()
{
	return PlatformType::iOS;
}


// ***************************************************************
Gdk::PlatformVersion _Gdk_Platform_Device_GetPlatformVersion()
{
	// Get the OS version
    NSString* version = [[UIDevice currentDevice] systemVersion];
    
    // Break version into fields (separated by '.')    
    NSArray* versionElements = [version componentsSeparatedByString:@"."];
    
    // Parse the fields into the platform version
    PlatformVersion result;
    if([versionElements count] > 0)
        result.MajorVersion = [[versionElements objectAtIndex:0] intValue];
    if([versionElements count] > 1)
        result.MinorVersion = [[versionElements objectAtIndex:1] intValue];
    if([versionElements count] > 2)
        result.Revision = [[versionElements objectAtIndex:2] intValue];
        
    return result;
}

// ***************************************************************
wstring _Gdk_Platform_Device_GetDeviceDisplayName()
{
	// Get the device name
    NSString* deviceName = [[UIDevice currentDevice] name];
    
    wchar_t* temp = (wchar_t*)[deviceName cStringUsingEncoding:NSUTF32LittleEndianStringEncoding];    
    return wstring(temp);
}

// ***************************************************************
string _Gdk_Platform_Device_GetDeviceUniqueId()
{
	// Get the device unique Id
    NSString* uniqueId = [[UIDevice currentDevice] uniqueIdentifier];
    return string([uniqueId UTF8String]);
}

// ---------------------------------------------------
void EnsureBatteryMonitoringEnabled()
{
    static bool batteryMonitoringEnabled = false;
    if(batteryMonitoringEnabled == false)
    {
        UIDevice *device = [UIDevice currentDevice];
        device.batteryMonitoringEnabled = YES;
        batteryMonitoringEnabled = true;
    }
}

// ---------------------------------------------------
Gdk::DeviceBatteryState::Enum _Gdk_Platform_Device_GetBatteryState()
{
    // Make sure battery monitoring is enabled
	EnsureBatteryMonitoringEnabled();
    
    // Get the Battery State
    UIDeviceBatteryState batteryState = [[UIDevice currentDevice] batteryState];
    if(batteryState == UIDeviceBatteryStateCharging || batteryState == UIDeviceBatteryStateFull)
        return Gdk::DeviceBatteryState::Charging;
    else if(batteryState == UIDeviceBatteryStateUnplugged)
        return Gdk::DeviceBatteryState::Unplugged;
    
	return Gdk::DeviceBatteryState::Unplugged;
}

// ---------------------------------------------------
float _Gdk_Platform_Device_GetBatteryLevel()
{    
    // Make sure battery monitoring is enabled
	EnsureBatteryMonitoringEnabled();
    
    // Get the battery level
	return [[UIDevice currentDevice] batteryLevel];
}

// ---------------------------------------------------
void _Gdk_Platform_Device_Vibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}