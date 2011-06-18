
#pragma once

// Includes
#include "Gdk.h"

using namespace Gdk;

// *******************************************************************
class PROJECTNAMEGame : public Game
{
public:
	// CTor
    PROJECTNAMEGame();

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
};
