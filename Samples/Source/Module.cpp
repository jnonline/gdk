/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Includes
#include "BasePCH.h"

#include "Samples/Tanks3D/Tanks3DModule.h"
#include "Samples/StarfieldSample.h"

#include "Tests/Input/InputTests.h"
#include "Tests/Graphics/Simple2DTests.h"
#include "Tests/Graphics/Simple3DTests.h"
#include "Tests/UnitTests/UnitTestsModule.h"


// *******************************************************************
// Static Instantiations

vector<class ModuleRegistration*> Module::registeredModules;

// *******************************************************************
void Module::Init()
{
    // Tests
    RegisterModule<UnitTestsModule>("Unit Tests", ModuleCategory::Tests);
	RegisterModule<InputTestsModule>("Input Tests", ModuleCategory::Tests);
    RegisterModule<Simple2DTests>("Simple 2D Tests", ModuleCategory::Tests);
    RegisterModule<Simple3DTests>("Simple 3D Tests", ModuleCategory::Tests);
 
    // Samples
    RegisterModule<Tanks3DModule>("Tanks 3D", ModuleCategory::Samples);
	RegisterModule<StarfieldSample>("Starfield", ModuleCategory::Samples);
}



