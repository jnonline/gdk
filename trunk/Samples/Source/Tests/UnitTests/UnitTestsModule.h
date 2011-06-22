/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
class UnitTestsModule : public Module
{
public:
	// CTor / DTor
    UnitTestsModule();
	~UnitTestsModule();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

protected:

	// Input handlers
	void OnTouchBegan(Touch* touch);
	void OnMouseDown(MouseButton::Enum button);

	// Output Display
	// -----------------------------

	class DisplayLine
	{
	public:
		wstring		Text;
		Gdk::Color	Color;

		DisplayLine(const wchar_t* text, Gdk::Color color) : Text(text), Color(color) {}
	};
	deque<DisplayLine> displayLines;
	float spinnerT;

	Mutex* logMutex;

	// Methods
	void WriteLine(Gdk::LogLevel::Enum logLevel, const wchar_t *format, ...);
	void Clear();

	// Tests
	// -----------------------------

	class UnitTestRegistration
	{
	public:
		wstring		Name;
		Gdk::Color	ButtonColor;
		Rectangle2	ButtonBounds;
		void (UnitTestsModule::*StartMethod)();
		
		UnitTestRegistration(const wchar_t* name, Gdk::Color buttonColor, void (UnitTestsModule::*startMethod)()) : Name(name), ButtonColor(buttonColor), StartMethod(startMethod) {}
	};

	vector<UnitTestRegistration> registeredUnitTests;

	void StartTest(UnitTestRegistration& test);

	// Test Methods
	void TestSystem(void);
	void TestMath(void);

};
