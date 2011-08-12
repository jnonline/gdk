/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Input/Device.h"
#include "../IO/Path.h"

// This file contains all the interfaces / function prototypes that MUST be implemented for each platform\

// IO
string _Gdk_Platform_IO_GetCommonPath(Gdk::CommonPaths::Enum commonPath);

// Window/Application
extern void _Gdk_Platform_SetTitle(const wchar_t* title);
extern void _Gdk_Platform_Resize(int width, int height);

// Mouse
extern void _Gdk_Platform_Mouse_SetPosition(int x, int y); // Sets the position of the mouse in pixel coordinates ( <0,0> at top left )
extern void _Gdk_Platform_Mouse_ShowCursor(bool show); 

// Device
extern Gdk::DeviceType::Enum			_Gdk_Platform_Device_GetDeviceType();
extern Gdk::PlatformType::Enum			_Gdk_Platform_Device_GetPlatformType();
extern Gdk::PlatformVersion				_Gdk_Platform_Device_GetPlatformVersion();
extern wstring                          _Gdk_Platform_Device_GetDeviceDisplayName();
extern string							_Gdk_Platform_Device_GetDeviceUniqueId();
extern Gdk::DeviceBatteryState::Enum	_Gdk_Platform_Device_GetBatteryState();
extern float							_Gdk_Platform_Device_GetBatteryLevel();
extern void                             _Gdk_Platform_Device_Vibrate();

