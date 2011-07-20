/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "Module.h"

using namespace Gdk;

// *******************************************************************
class SampleGame : public Game
{
public:
	// CTor
    SampleGame();

	// =====================================================
	// Game Events : interface methods
	//		Event methods that are called to notify derived applications.
	//		Custom applications should override these methods.
	// =====================================================

	// Main execution events
	virtual bool OnLoadSettings(ApplicationSettings &settings);
	virtual bool OnInit();
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw(float elapsedSeconds);
	virtual void OnShutdown();

	// Input handlers
	void OnTouchBegan(Touch* touch);
	void OnMouseDown(MouseButton::Enum button);

private:
	// Internals
	Module* activeModule;
	int activeModuleIndex;
	int changeToModuleIndex;
	Vector2 nameTextPosition;
	Rectangle2 nextButtonRect;
	Rectangle2 prevButtonRect;

};
