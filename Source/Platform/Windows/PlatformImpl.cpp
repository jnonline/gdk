/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Includes
#include "BasePCH.h"
#include "Gdk.h"

#include <windows.h>
#include <lm.h>
#include <windowsx.h>
#include <exception>
#include <string>

#include <iphlpapi.h>
#include <iptypes.h>

using namespace std;
using namespace Gdk;

// Globals
extern HWND g_hWnd;

// Prototypes
string GetMACAddress();

/// ===========================================================
///
/// IO
///
/// ===========================================================

string _Gdk_Platform_IO_GetCommonPath(CommonPaths::Enum commonPath)
{
	// Get the full path of the exe file
	char exeFilePath[MAX_PATH + 1];
	GetModuleFileNameA(NULL, exeFilePath, MAX_PATH);

	// Get the directory of the Exe
	string exeFolder = Path::GetDirectory(exeFilePath);

	// Which path is the user asking for?
	switch(commonPath)
	{
        case CommonPaths::ApplicationFolder:
			return exeFolder;

		case CommonPaths::AssetFolder:
			return Path::Combine(exeFolder.c_str(), "Assets");

		case CommonPaths::WorkingFolder:
			return Path::Combine(exeFolder.c_str(), "Data");
	}

	return string();
}


/// ===========================================================
///
/// Window / Application Methods
///
/// ===========================================================

void _Gdk_Platform_SetTitle(const wchar_t* title)
{
	SetWindowTextW(g_hWnd, title);
}

// ---------------------------------------------------
void _Gdk_Platform_Resize(int width, int height)
{
	// Resize the window so the Client Rectangle is the appropriate size (not just the whole window)
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect(g_hWnd, &rcClient);
	GetWindowRect(g_hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(g_hWnd, rcWindow.left, rcWindow.top, 
		width + ptDiff.x, 
		height + ptDiff.y,
		TRUE);
}

/// ===========================================================
///
/// Mouse Methods
///
/// ===========================================================

// ---------------------------------------------------
void _Gdk_Platform_Mouse_SetPosition(int x, int y)
{
	POINT point;
	point.x = x;
	point.y = y;

	// Convert the window coordinate to screen coordinates
	ClientToScreen(g_hWnd, &point);

	// Set the cursor position
	SetCursorPos(point.x, point.y);
}

// ---------------------------------------------------
void _Gdk_Platform_Mouse_ShowCursor(bool show)
{
	// We use the static to manage the ShowCursor() internal counter
	static bool mouseIsShowing = true;
 	if(show == true && mouseIsShowing == false)
	{
		ShowCursor(TRUE);
		mouseIsShowing = true;
	}
	else if(show == false && mouseIsShowing == true)
	{
		ShowCursor(FALSE);
		mouseIsShowing = false;
	}
}

/// ===========================================================
///
/// Device
///
/// ===========================================================

// ---------------------------------------------------
Gdk::DeviceType::Enum _Gdk_Platform_Device_GetDeviceType()
{
	return DeviceType::PC;
}

// ---------------------------------------------------
Gdk::PlatformType::Enum _Gdk_Platform_Device_GetPlatformType()
{
	return PlatformType::Windows;
}

// ---------------------------------------------------
Gdk::PlatformVersion _Gdk_Platform_Device_GetPlatformVersion()
{
	// Get the OS version
	OSVERSIONINFO osVersionInfo;
    ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFO));
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osVersionInfo);

	return PlatformVersion(
		osVersionInfo.dwMajorVersion,
		osVersionInfo.dwMinorVersion,
		osVersionInfo.dwBuildNumber
		);
}

// ---------------------------------------------------
wstring _Gdk_Platform_Device_GetDeviceDisplayName()
{
	// Get the server name information
	wstring displayName(StringUtilities::WUnknown);
	LPSERVER_INFO_101 pBuf = NULL;
	if (NetServerGetInfo(NULL, 101, (LPBYTE*)&pBuf) == NERR_Success)
	{
		// Get the system 'comment'  (which is the friendly display name)
		if(pBuf->sv101_comment != NULL && pBuf->sv101_comment[0] != 0)
			displayName = wstring(pBuf->sv101_comment);
		// Fallback to the Netbios name
		else if(pBuf->sv101_name != NULL && pBuf->sv101_name[0] != 0)
			displayName = wstring(pBuf->sv101_name);

		// Release the buffer from the OS call
		NetApiBufferFree(pBuf);
	}

	return displayName;
}

// ---------------------------------------------------
string _Gdk_Platform_Device_GetDeviceUniqueId()
{
	return GetMACAddress();
}

// ---------------------------------------------------
Gdk::DeviceBatteryState::Enum _Gdk_Platform_Device_GetBatteryState()
{
	// Get the Win32 Power Status
	SYSTEM_POWER_STATUS systemPowerStatus;
	if(GetSystemPowerStatus(&systemPowerStatus) == 0)
		return Gdk::DeviceBatteryState::Unknown;

	// Is the Battery charging or the AC power is enabled?
	if((systemPowerStatus.BatteryFlag & 8) == 8 || (systemPowerStatus.ACLineStatus & 1) == 1)
		return Gdk::DeviceBatteryState::Charging;
	return Gdk::DeviceBatteryState::Unplugged;
}

// ---------------------------------------------------
float _Gdk_Platform_Device_GetBatteryLevel()
{
	// Get the Win32 Power Status
	SYSTEM_POWER_STATUS systemPowerStatus;
	if(GetSystemPowerStatus(&systemPowerStatus) == 0)
		return 1.0f;

	if(systemPowerStatus.BatteryLifePercent == 255)
		return 1.0f;

	// Convert the 0-100 BYTE value to a 0.0 - 1.0 float value
	return systemPowerStatus.BatteryLifePercent / 100.0f;
}

// ---------------------------------------------------
void _Gdk_Platform_Device_Vibrate()
{
	// No vibration on PCs :)
}






// =====================================================================================
string GetMACAddress()
{
	string result(StringUtilities::Empty);

	// Get the size of the buffer we need for the adapter info
	DWORD dwBufLen = 0;
	if(GetAdaptersInfo(NULL, &dwBufLen) != ERROR_BUFFER_OVERFLOW)
		return StringUtilities::Empty;

	// Allocate a buffer to hold the adapter info
	IP_ADAPTER_INFO* buffer = (IP_ADAPTER_INFO*)malloc(dwBufLen);
	IP_ADAPTER_INFO* adapterInfo = buffer;

	// Get the adapter infos
	if(GetAdaptersInfo(adapterInfo, &dwBufLen) == ERROR_SUCCESS)
	{
		// Loop through the adapters
		while(adapterInfo != NULL)
		{
			// Is this an ethernet or wifi adapter?
			if(adapterInfo->Type == MIB_IF_TYPE_ETHERNET || adapterInfo->Type == IF_TYPE_IEEE80211)
			{
				result.reserve(adapterInfo->AddressLength * 3);

				// Convert the address to a string
				for(size_t i = 0; i < adapterInfo->AddressLength; i++)
				{
					char temp[16];
					sprintf(temp, "%02x", adapterInfo->Address[i]);
					result.append(temp);
					if(i < adapterInfo->AddressLength-1)
						result.append(":");
				}

				// Stop looping through the adapters
				break;
			}

			// Next adapter
			adapterInfo = adapterInfo->Next;
		}
	}

	// Free the buffer
	free(buffer);

	return result;
}