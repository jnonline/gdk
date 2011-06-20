//
// WinMain & Window classes and code for a Windows based GDK application.
//
// WARNING: NO NOT EDIT this file directly for creating a game.
//


// Includes
#include "BasePCH.h"
#include "Gdk.h"

#include <windows.h>
#include <windowsx.h>
#include <exception>
#include <string>

using namespace std;

// Function Declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Globals
HWND g_hWnd;
const wchar_t* g_className = L"GDK Windows Application";
bool g_mouseInClient = false;

// ******************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	bool quit = false;
	wchar_t errorMsg[256];

	// Start up the GDK
	if(Gdk::Application::Platform_InitGdk() == false)
		return 0;

	// Create the window
	// ------------------------------------------------------

	// Register window class
	WNDCLASS wc;
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_className;
	RegisterClass( &wc );
	
	// Create main window
	g_hWnd = CreateWindow(
		g_className, Gdk::Application::GetTitle(), 
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_VISIBLE, /* use: WS_POPUPWINDOW for fullscreen */
		10, 10, Gdk::Application::GetWidth(), Gdk::Application::GetHeight(),
		NULL, NULL, hInstance, NULL );

	// Resize the window so the Client Rectangle is the appropriate size (not just the whole window)
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect(g_hWnd, &rcClient);
	GetWindowRect(g_hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(g_hWnd, rcWindow.left, rcWindow.top, 
		Gdk::Application::GetWidth() + ptDiff.x, 
		Gdk::Application::GetHeight() + ptDiff.y, 
		TRUE);
	
	// Show the window
	ShowWindow(g_hWnd, SW_SHOWNORMAL);

	// Enable OpenGL 
	// -----------------------------------------------------

	// Setup a pixel format struct
	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		32,																// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	// get the device context (DC)
	HDC hDC = GetDC( g_hWnd );

	// Choose our pixel format
	GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat == 0)
	{
		MessageBox(g_hWnd, L"No valid Pixel Formats available", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Set the pixel format for our window
	if (SetPixelFormat(hDC, pixelFormat, &pfd) == FALSE)
	{
		MessageBox(g_hWnd, L"Failed to set the pixel format", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Create a GL Context
	HGLRC hRC = wglCreateContext(hDC);
	if (hRC == 0)
	{
		MessageBox(g_hWnd, L"Failed to Create the OpenGL Context", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent(hDC, hRC) == FALSE)
	{
		MessageBox(g_hWnd, L"Failed to Set the OpenGL Rendering Context", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		wsprintf(errorMsg, L"Failed to initiliaze GLEW: %S", glewGetErrorString(err));
		MessageBox(g_hWnd, errorMsg, L"GLEW Init Failed", MB_OK);
		return 0;
	}

	// Verify GL 2.1 is supported
	if (glewIsSupported("GL_VERSION_2_1") == 0)
	{
		MessageBox(g_hWnd, L"OpenGL 2.1 Not Supported", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Verify the necessary GL extensions are supported
	if(!GLEW_EXT_framebuffer_object)
	{
		MessageBox(g_hWnd, L"OpenGL Framebuffers Extension Not Supported", L"OpenGL Init Failed", MB_OK);
		return 0;
	}

	// Tell the GDK about the default frame & render buffers
	Gdk::Graphics::Platform_SetOSFrameBuffers(0, 0, 0, 0);

	// -------------------------------------
	
	// Exception handle
	try
	{
		// Init the application
		// ----------------------------------------

		if(Gdk::Application::Platform_InitGame() == true)
		{
			// Set the application as active
			Gdk::Application::Platform_OnActive();

			// Main loop
			// ----------------------------------------
			
			while ( !quit )
			{
				
				// Process any messages in the queue
				while ( !quit && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
				{
					
					// Dispatch messages
					if ( msg.message == WM_QUIT ) 
					{
						quit = true;
					} 
					else 
					{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
				} 

				// Do a game loop
				if( !quit )
				{
					// Update the gdk host
					Gdk::Application::Platform_MainLoop();

					// Is there an exit request in the gdk host?
					if(Gdk::Application::IsExitRequest() == true)
					{
						// Set the quit flag
						quit = true;
					}
					else
					{
						// Swap the front/back buffers
						SwapBuffers( hDC );
					}
				}

			} // Main Loop

		} // if(init)

		// Shutdown the application
		Gdk::Application::Platform_ShutdownGame();
	}
	// Catch any standard C++ exceptions
	catch (exception& ex)
	{
		// Log the exception
		LOG_ERROR(L"C++ Exception caught!!");
		LOG_ERROR(L"C++ Exception: %S", ex.what());
	}
	catch(...)
	{
		// Log the exception
		LOG_ERROR(L"Unknown Exception caught!!");
	}

	// Shutdown GDK
	Gdk::Application::Platform_ShutdownGdk();
	
	// Shutdown OpenGL
	wglMakeCurrent(hDC, 0);
	wglDeleteContext(hRC);
	ReleaseDC(g_hWnd, hDC);

	// Destroy the window explicitly
	DestroyWindow( g_hWnd );
	
	return msg.wParam;
	
}


// **************************************************************************************

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		// Application Messages
		// -----------------------------

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
		
	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE)
			Gdk::Application::Platform_OnDeactive();
		else
			Gdk::Application::Platform_OnActive();
		break;

	case WM_SIZE:
		{
			// Tell the application about the resize event
			int newWidth = LOWORD(lParam);
			int newHeight = HIWORD(lParam);
			Gdk::Application::Platform_OnResize(newWidth, newHeight);
			
			// Use the window size to determine simulated device orientation
			if(newWidth >= newHeight)
				Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::LandscapeLeft);
			else
				Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::Portrait);
		}
		break;
		
		// Keyboard Processing
		// -----------------------------

	case WM_KEYDOWN:
		{
			// Get the Gdk::Keys values for this key
			Gdk::Keys::Enum key = Gdk::Keyboard::Platform_ConvertScanCodeToKey((UInt8)(wParam & 0xFF));

			// Send the KeyDown to the GDK
			Gdk::Keyboard::Platform_ProcessKeyDown(key);
		}
		break;

	case WM_KEYUP:
		{
			// Get the Gdk::Keys values for this key
			Gdk::Keys::Enum key = Gdk::Keyboard::Platform_ConvertScanCodeToKey((UInt8)(wParam & 0xFF));

			// Send the KeyUp to the GDK
			Gdk::Keyboard::Platform_ProcessKeyUp(key);
		}
		break;

	case WM_CHAR:
		Gdk::Keyboard::Platform_ProcessChar((wchar_t)wParam);
		break;
	
		// Mouse Processing
		// -----------------------------

	case WM_MOUSEMOVE:
		Gdk::Mouse::Platform_ProcessMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		// Track the mouse as now being in the client region
		if(g_mouseInClient = false)
		{
			g_mouseInClient = true;
			Gdk::Mouse::Platform_ProcessMouseEnterApp();
			TRACKMOUSEEVENT tme = {sizeof(tme)};
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
		}
		break;

	case WM_MOUSELEAVE:
		g_mouseInClient = false;
		Gdk::Mouse::Platform_ProcessMouseLeaveApp();
		break;
		
	case WM_MOUSEWHEEL:
		{
			// Convert the wheel scroll value to the device scroll units
			float deltaY = GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
			Gdk::Mouse::Platform_ProcessMouseWheelScroll(0.0f, deltaY);

			wchar_t temp[256];
			swprintf(temp, 256, L"Mouse Wheel: %f\n", deltaY);
			OutputDebugString(temp);
		}

		break;

	case WM_LBUTTONUP:
		Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Left);
		break;
	case WM_LBUTTONDOWN:
		Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Left);
		break;

	case WM_RBUTTONUP:
		Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Right);
		break;
	case WM_RBUTTONDOWN:
		Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Right);
		break;

	case WM_MBUTTONUP:
		Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Middle);
		break;
	case WM_MBUTTONDOWN:
		Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Middle);
		break;

	case WM_XBUTTONUP:
		if(GET_XBUTTON_WPARAM(wParam) == 1)
			Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Button4);
		else if(GET_XBUTTON_WPARAM(wParam) == 2)
			Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Button5);
		break;

	case WM_XBUTTONDOWN:
		if(GET_XBUTTON_WPARAM(wParam) == 1)
			Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Button4);
		else if(GET_XBUTTON_WPARAM(wParam) == 2)
			Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Button5);
		break;

		// -----------------------------
			
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
	
}
