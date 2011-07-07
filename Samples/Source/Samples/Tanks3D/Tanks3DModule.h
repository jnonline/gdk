/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
class Tanks3DModule : public Module
{
public:
	// CTor / DTor
    Tanks3DModule();
	~Tanks3DModule();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

protected:
    
    // Event Handlers
    void OnMouseMove(MouseMoveArgs* args);
    
	// Gameplay Assets
	// --------------------

	Model* tank;
	Model* ground;

	// Pool for our assets
	AssetPool assetsPool;

	// 3D Game Objects 
	// --------------------

	// Player
	Vector2 playerPosition;
	float playerDirection;

	// Enemy tanks
	struct EnemyTank
	{
		Vector2 Position;
		float Direction;
	};
	vector<EnemyTank> enemies;

	// 2D Overlay objects
	Sprite* chopper;
};
