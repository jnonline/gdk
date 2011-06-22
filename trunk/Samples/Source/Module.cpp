/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Includes
#include "Module.h"

#include "Samples/Tanks3D/Tanks3DModule.h"

#include "Tests/Test2D/Test2DModule.h"
#include "Tests/Input/InputTests.h"
#include "Tests/UnitTests/UnitTestsModule.h"


// *******************************************************************
// Static Instantiations

vector<class ModuleRegistration*> Module::registeredModules;

// *******************************************************************
void Module::Init()
{
    // Samples
    //RegisterModule<Tanks3DModule>(L"Tanks 3D", ModuleCategory::Samples);

    // Tests
    RegisterModule<InputTestsModule>(L"Input Tests", ModuleCategory::Tests);
    RegisterModule<Test2DModule>(L"2D Rendering Tests", ModuleCategory::Tests);
    RegisterModule<UnitTestsModule>(L"Unit Tests", ModuleCategory::Tests);
}



