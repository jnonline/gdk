/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "UnitTestsModule.h"

// ***********************************************************************
UnitTestsModule::UnitTestsModule ()
{
	this->spinnerT = 0.0f;

	// Bind to Touch & Mouse event handlers
	TouchInput::TouchBegan.AddHandlerMethod(this, &UnitTestsModule::OnTouchBegan);
	Mouse::MouseButtonDown.AddHandlerMethod(this, &UnitTestsModule::OnMouseDown);

	// Register all the tests
	registeredUnitTests.push_back(UnitTestRegistration(L"System",	Gdk::Color(64,64,64),	&UnitTestsModule::TestSystem));
	registeredUnitTests.push_back(UnitTestRegistration(L"Math",		Gdk::Color(64,64,64),	&UnitTestsModule::TestMath));

	logMutex = Mutex::Create();
}

// ***********************************************************************
UnitTestsModule::~UnitTestsModule()
{
	GdkDelete(logMutex);

	// Unbind the input event handlers
    TouchInput::TouchBegan.RemoveHandlerMethod(this, &UnitTestsModule::OnTouchBegan);
	Mouse::MouseButtonDown.RemoveHandlerMethod(this, &UnitTestsModule::OnMouseDown);
}

// ***********************************************************************
void UnitTestsModule::OnUpdate(float elapsedSeconds)
{
	spinnerT = Math::Mod(spinnerT + elapsedSeconds, 1.0f);

	// Update all the test buttons
	float y = 2, x = 2;
	for(vector<UnitTestRegistration>::iterator iter = registeredUnitTests.begin(); iter != registeredUnitTests.end(); iter++)
	{
		UnitTestRegistration& test = *iter;

		// Set the button's bounds
		test.ButtonBounds = Rectangle2(x, y, 100, 18);
		y += 20; 
	}
}

// ***********************************************************************
void UnitTestsModule::OnDraw()
{
	// Clear the background
	Graphics::Clear(
		(ClearBuffers::Enum) (ClearBuffers::Color), 
		Color::FromNormalized(0.1f, 0.5f, 0.4f, 1.0f),
		1.0f
		);
	
	// Setup a 2D projection matrix to draw in screen coordinates
    Matrix3D proj = Matrix3D::CreateOrthoOffCenter(
		0, (float) Application::GetWidth(),					// left / right
		(float) Application::GetHeight(), 0,				// bottom / top
		-1.0f, 1.0f											// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	Vector2 textScale(0.7f, 0.7f);
	float textHeight = 14.0f;

	// Draw the test buttons
	// -----------------------------------

	// Loop through all the tests
	for(vector<UnitTestRegistration>::iterator iter = registeredUnitTests.begin(); iter != registeredUnitTests.end(); iter++)
	{
		// Draw a button for this test
		UnitTestRegistration& test = *iter;
		Renderer2D::DrawRectangle(test.ButtonBounds, test.ButtonColor);
		Renderer2D::DrawRectangleOutline(test.ButtonBounds, Color(196,196,196));
		Renderer2D::DrawText(SharedAssets::Fonts.Arial20, test.Name.c_str(), test.ButtonBounds.Position + Vector2(2,2), Color(196,196,196), textScale);
	}

	// Draw the display log 
	// -----------------------------------

	// Lock the logging mutex
	logMutex->Lock();
	{
		// Loop through the display lines
		float y = (float)Application::GetHeight() - 40;
		for(deque<DisplayLine>::iterator iter = displayLines.begin(); y >= -textHeight && iter != displayLines.end(); iter++)
		{
			// Draw this display line
			DisplayLine& line = *iter;
			Renderer2D::DrawText(SharedAssets::Fonts.Arial20, line.Text.c_str(), Vector2(104, y), line.Color, textScale);

			y -= textHeight;
		}
	}
	logMutex->Unlock();

	// Flush the 2D renderer
	Renderer2D::Flush();
}

// ***********************************************************************
void UnitTestsModule::OnTouchBegan(Touch* touch)
{
	// Is this touch un-owned?
	if(touch->GetOwner() == NULL)
	{
		// Do any of the test buttons contain this touch?
		for(vector<UnitTestRegistration>::iterator iter = registeredUnitTests.begin(); iter != registeredUnitTests.end(); iter++)
		{
			UnitTestRegistration& test = *iter;
			if(test.ButtonBounds.Contains(touch->GetPosition()))
			{
				TouchInput::SetTouchOwner(*touch, this);
				StartTest(test);
				return;
			}
		}
	}
}

// ***********************************************************************
void UnitTestsModule::OnMouseDown(MouseButton::Enum button)
{
	// Is this the left button?
	if(button == MouseButton::Left)
	{
		// Do any of the test buttons contain the mouse?
		for(vector<UnitTestRegistration>::iterator iter = registeredUnitTests.begin(); iter != registeredUnitTests.end(); iter++)
		{
			UnitTestRegistration& test = *iter;
			if(test.ButtonBounds.Contains(Mouse::GetPosition()))
			{
				StartTest(test);
				return;
			}
		}
	}
}

// ***********************************************************************
void UnitTestsModule::WriteLine(Gdk::LogLevel::Enum logLevel, const wchar_t *format, ...)
{
	// Determine the log line color
	Gdk::Color color;
	switch(logLevel)
	{
		case LogLevel::System:		color = Gdk::Color(255,255,255);	break;
		case LogLevel::Warning:		color = Gdk::Color(255,224,24);		break;
		case LogLevel::Assert:		
		case LogLevel::Error:		color = Gdk::Color(255,115,15);		break;
		case LogLevel::Info:		color = Gdk::Color(224,224,200);	break;
		case LogLevel::Verbose:
		default:					color = Gdk::Color(200,200,200);	break;
	}

	wchar_t temp[2048];

	// Build the full string from the format and args
	va_list args;
	va_start (args, format);
	vswprintf(temp, 2048, format, args);
	va_end (args);

	// Lock the logging mutex
	logMutex->Lock();
	{
		// Do we need to pop any items off the queue?
		while(displayLines.size() > 100)
			displayLines.pop_back();

		// Push the new item onto the queue
		displayLines.push_front(DisplayLine(temp, color));
	}
	logMutex->Unlock();
}

// ***********************************************************************
void UnitTestsModule::Clear()
{
	displayLines.clear();
}

// ***********************************************************************
void UnitTestsModule::StartTest(UnitTestRegistration& test)
{
	// Get the start method
	void (UnitTestsModule::*StartMethod)() = test.StartMethod;

		return (*this.*StartMethod)();
}