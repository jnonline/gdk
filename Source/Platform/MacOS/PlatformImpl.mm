/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#import "BasePCH.h"
#import "GdkGLView.h"

#import <Cocoa/Cocoa.h>
#include <sys/sysctl.h>
#import <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/network/IONetworkInterface.h>
#include <IOKit/network/IOEthernetController.h>




using namespace Gdk;

bool        _Gdk_Mac_IsMouseVisible = true;
GdkGLView*  _Gdk_Mac_MainGLView = NULL;
NSWindow*   _Gdk_Mac_MainWindow = NULL;

// Prototypes
NSString* GetMACAddress();

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
			NSString* nsBundlePath = [[NSBundle mainBundle] bundlePath];
			return string([nsBundlePath UTF8String]);
		}
            
        case CommonPaths::AssetFolder:
        {
			// Get the application's bundle path & append "Assets"
			NSString* nsBundlePath = [[NSBundle mainBundle] bundlePath];
			return Path::Combine([nsBundlePath UTF8String], "Contents/Assets");
        }
            
        case CommonPaths::WorkingFolder:
        {
            
            // TODO(P1):   This ends up being the "Users/USER/Documents" folder, but should be a working folder in the app folder...
            
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
    // Resize the main window
    NSWindow* window = [[NSApplication sharedApplication] mainWindow];
    NSSize size;
    size.width = width;
    size.height = height;
    [window setContentSize:size];
}

// ***************************************************************
void _Gdk_Platform_SetTitle(const wchar_t* title)
{
    // Get the main Window
    NSWindow* window = [[NSApplication sharedApplication] mainWindow];
    
    // Turn the wchar_T to an NSString
    int numBytes = wcslen(title) * sizeof(wchar_t);
    NSString* newTitle = [[NSString alloc] initWithBytes:title length:numBytes encoding:NSUTF32LittleEndianStringEncoding];
    window.title = newTitle;
    
    // set the title
    [newTitle release];
}


/// ===========================================================
///
/// Mouse
///
/// ===========================================================


// ***************************************************************
void _Gdk_Platform_Mouse_ShowCursor(bool show)
{
    // Do we need to change the mouse cursor visibility state?
    if(show != _Gdk_Mac_IsMouseVisible)
    {
        if(show)
            [NSCursor unhide];
        else
            [NSCursor hide];
        _Gdk_Mac_IsMouseVisible = show;
    }
}

// ***************************************************************
void _Gdk_Platform_Mouse_SetPosition(int x, int y)
{
    // Do we have the view & window pointers?
    if(_Gdk_Mac_MainGLView != NULL && _Gdk_Mac_MainWindow != NULL)
    {
        // Convert the mouse coordinate to base coordinates
        NSPoint localPoint;
        localPoint.x = x;
        localPoint.y = y;
        NSPoint basePoint = [_Gdk_Mac_MainGLView convertPointToBase:localPoint];
        NSPoint screenPoint = [_Gdk_Mac_MainWindow convertBaseToScreen:basePoint];
        CGWarpMouseCursorPosition(NSPointToCGPoint(screenPoint));
    }
}

/// ===========================================================
///
/// Device
///
/// ===========================================================


// ***************************************************************
Gdk::DeviceType::Enum _Gdk_Platform_Device_GetDeviceType()
{
    return DeviceType::PC;
}

// ***************************************************************
Gdk::PlatformType::Enum _Gdk_Platform_Device_GetPlatformType()
{
	return PlatformType::Mac;
}


// ***************************************************************
Gdk::PlatformVersion _Gdk_Platform_Device_GetPlatformVersion()
{
    SInt32 versionMajor = 10;
    SInt32 versionMinor = 5;
    SInt32 versionBugFix = 0;

    // Get the OS Version info from Gestalt
    Gestalt(gestaltSystemVersionMajor, &versionMajor);
    Gestalt(gestaltSystemVersionMinor, &versionMinor);
    Gestalt(gestaltSystemVersionBugFix, &versionBugFix);
        
    return PlatformVersion(versionMajor, versionMinor, versionBugFix);
}

// ***************************************************************
wstring _Gdk_Platform_Device_GetDeviceDisplayName()
{
	// Get the device name
    NSString* deviceName = [[NSHost currentHost]  localizedName];

    wchar_t* temp = (wchar_t*)[deviceName cStringUsingEncoding:NSUTF32LittleEndianStringEncoding];    
    return wstring(temp);
}

// ***************************************************************
string _Gdk_Platform_Device_GetDeviceUniqueId()
{
	// Get the device unique Id
    NSString* uniqueId = GetMACAddress();
    return string([uniqueId UTF8String]);
}

// ***************************************************************
Gdk::DeviceBatteryState::Enum _Gdk_Platform_Device_GetBatteryState()
{
    // TODO(P2):  See IOPowerSources API
    return Gdk::DeviceBatteryState::Charging;
}

// ***************************************************************
float _Gdk_Platform_Device_GetBatteryLevel()
{    
    // TODO(P2):  See IOPowerSources API
    return 1.0f;
}

// ***************************************************************
void _Gdk_Platform_Device_Vibrate()
{
    // No Vibration on Mac :)
}





// -===================================================================================================================


// code adapted from Apple sample code GetPrimaryMACAddress.c
// http://developer.apple.com/samplecode/GetPrimaryMACAddress/listing1.html

// ***************************************************************
// Returns an iterator containing the primary (built-in) Ethernet interface.
// The caller is responsible for
// releasing the iterator after the caller is done with it.
static kern_return_t FindEthernetInterfaces(io_iterator_t *matchingServices) {
    kern_return_t		kernResult;
    CFMutableDictionaryRef	matchingDict;
    CFMutableDictionaryRef	propertyMatchDict;
    
    // Ethernet interfaces are instances of class kIOEthernetInterfaceClass.
    // IOServiceMatching is a convenience function to create a dictionary with
    // the key kIOProviderClassKey and the specified value.
    matchingDict = IOServiceMatching(kIOEthernetInterfaceClass);
    
    // Note that another option here would be:
    // matchingDict = IOBSDMatching("en0");
    
    if (NULL == matchingDict) 
    {
        NSLog(@"IOServiceMatching returned a NULL dictionary.\n");
    }
    else 
    {
        // Each IONetworkInterface object has a Boolean property with the key
        // kIOPrimaryInterface.
        // Only the primary (built-in) interface has this property set to TRUE.
        
        // IOServiceGetMatchingServices uses the default matching criteria
        // defined by IOService. This considers only the following properties
        // plus any family-specific matching in this order of precedence
        // (see IOService::passiveMatch):
        //
        // kIOProviderClassKey (IOServiceMatching)
        // kIONameMatchKey (IOServiceNameMatching)
        // kIOPropertyMatchKey
        // kIOPathMatchKey
        // kIOMatchedServiceCountKey
        // family-specific matching
        // kIOBSDNameKey (IOBSDNameMatching)
        // kIOLocationMatchKey
        
        // The IONetworkingFamily does not define any family-specific matching.
        // This means that in order to have IOServiceGetMatchingServices consider
        // the kIOPrimaryInterface property, we must add that property
        // to a separate dictionary and then add that to our matching dictionary
        // specifying kIOPropertyMatchKey.
        
        propertyMatchDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
													  &kCFTypeDictionaryKeyCallBacks,
													  &kCFTypeDictionaryValueCallBacks);
        
        if (NULL == propertyMatchDict) 
        {
            NSLog(@"CFDictionaryCreateMutable returned a NULL dictionary.\n");
        }
        else 
        {
            // Set the value in the dictionary of the property with the
            // given key, or add the key to the dictionary if it doesn't exist.
            // This call retains the value object passed in.
            CFDictionarySetValue(propertyMatchDict, CFSTR(kIOPrimaryInterface),
                                 kCFBooleanTrue);
            
            // Now add the dictionary containing the matching value for
            // kIOPrimaryInterface to our main matching dictionary.
            // This call will retain propertyMatchDict, so we can release our
            // reference on propertyMatchDict after adding it to matchingDict.
            CFDictionarySetValue(matchingDict, CFSTR(kIOPropertyMatchKey),
                                 propertyMatchDict);
            CFRelease(propertyMatchDict);
        }
    }
    
    // IOServiceGetMatchingServices retains the returned iterator, so release
    // the iterator when we're done with it.
    // IOServiceGetMatchingServices also consumes a reference on the matching
    // dictionary so we don't need to release the dictionary explicitly.
    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict,
                                              matchingServices);
    if (KERN_SUCCESS != kernResult) 
    {
        NSLog(@"IOServiceGetMatchingServices returned 0x%08x\n", kernResult);
    }
    
    return kernResult;
}

// ***************************************************************
// Given an iterator across a set of Ethernet interfaces, return the MAC address
// of the last one.
// If no interfaces are found the MAC address is set to an empty string.
// In this sample the iterator should contain just the primary interface.
static kern_return_t GetMACAddress( io_iterator_t intfIterator,
                                   UInt8         *MACAddress,
                                   UInt8         bufferSize) {
    io_object_t		intfService;
    io_object_t		controllerService;
    kern_return_t	kernResult = KERN_FAILURE;
    
    // Make sure the caller provided enough buffer space. Protect against buffer
    // overflow problems.
	if (bufferSize < kIOEthernetAddressSize) 
    {
		return kernResult;
	}
    
	// Initialize the returned address
    bzero(MACAddress, bufferSize);
    
    // IOIteratorNext retains the returned object,
    // so release it when we're done with it.
    while ((intfService = IOIteratorNext(intfIterator))) 
    {
        CFTypeRef	MACAddressAsCFData;
        
        // IONetworkControllers can't be found directly by the
        // IOServiceGetMatchingServices call, since they are hardware nubs
        // and do not participate in driver matching. In other words,
        // registerService() is never called on them. So we've found the
        // IONetworkInterface and will get its parent controller
        // by asking for it specifically.
        
        // IORegistryEntryGetParentEntry retains the returned object,
        // so release it when we're done with it.
        kernResult = IORegistryEntryGetParentEntry(intfService,
												   kIOServicePlane,
												   &controllerService);
        
        if (KERN_SUCCESS != kernResult) 
        {
            NSLog(@"IORegistryEntryGetParentEntry returned 0x%08x\n",
                   kernResult);
        }
        else 
        {
            // Retrieve the MAC address property from the I/O Registry in
            // the form of a CFData
            MACAddressAsCFData = IORegistryEntryCreateCFProperty(
                                                                 controllerService,
                                                                 CFSTR(kIOMACAddress),
                                                                 kCFAllocatorDefault,
                                                                 0);
            
            if (MACAddressAsCFData) 
            {
                // Get the raw bytes of the MAC address from the CFData
                CFDataGetBytes((CFDataRef)MACAddressAsCFData,
                               CFRangeMake(0, kIOEthernetAddressSize), MACAddress);
                CFRelease(MACAddressAsCFData);
            }
            
            // Done with the parent Ethernet controller object so we release it.
            IOObjectRelease(controllerService);
        }
        
        // Done with the Ethernet interface object so we release it.
        IOObjectRelease(intfService);
    }
    
    return kernResult;
}

// ***************************************************************
NSString* GetMACAddress()
{
    NSString*       result;
    kern_return_t	kernResult = KERN_SUCCESS;
    io_iterator_t	intfIterator;
    UInt8			MACAddress[kIOEthernetAddressSize];
    
    // Get an interator of the ethernet interfaces
    kernResult = FindEthernetInterfaces(&intfIterator);
    if (KERN_SUCCESS != kernResult) 
    {
        NSLog(@"FindEthernetInterfaces returned 0x%08x\n", kernResult);
    }
    else 
    {
        // Get the mac address from the ethernet interfaces
        kernResult = GetMACAddress(intfIterator, MACAddress, sizeof(MACAddress));
        
        if (KERN_SUCCESS != kernResult) 
        {
            NSLog(@"GetMACAddress returned 0x%08x\n", kernResult);
        }
		else 
        {
            // Turn the MAC address into a properly formated string
            result = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x",
                     MACAddress[0], MACAddress[1], MACAddress[2], MACAddress[3], MACAddress[4], MACAddress[5]];
        }
    }
    
    // Release the iterator
    (void) IOObjectRelease(intfIterator);	// Release the iterator.
    
    return result;
}