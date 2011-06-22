/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "Gdk.h"

using namespace Gdk;

// *******************************************************************
namespace ModuleCategory
{
	enum Enum
	{
		Samples,
		Tests
	};
}

// *******************************************************************
class ModuleRegistration
{
public:
	// Properties
	wstring					Name;
	ModuleCategory::Enum	Category;
    
	// Methods
	ModuleRegistration(const wchar_t* name, ModuleCategory::Enum category) : Name(name), Category(category) {}
	virtual class Module* CreateModule() = 0;
};

// *******************************************************************
template <typename TModule>
class ModuleRegistrationT : public ModuleRegistration
{
public:
	// CTor
	ModuleRegistrationT(const wchar_t* name, ModuleCategory::Enum category) : ModuleRegistration(name, category)
	{
	}
    
	// CreateModule
	virtual Module* CreateModule()
	{
		return GdkNew TModule();
	}
};


// A Module is an encapsulated chunk of game code that runs within the sample game.
// Modules are used to test functionality, demonstrate effects, and be general how-to samples

// *******************************************************************
class Module
{
public:
	Module() {}
	virtual ~Module() {}

	// =====================================================
	// Main overrides for all Modules

	virtual void OnUpdate(float elapsedSeconds) = 0;
	virtual void OnDraw() = 0;

	// =====================================================
    // Module registration access
    static void Init();
    static vector<class ModuleRegistration*>& GetRegistrations()
    {
        return registeredModules;
    }
    
private:

    // =====================================================
    // Internals
    
    // Properties
    static vector<class ModuleRegistration*> registeredModules;
    
    // Methods
    template <typename TModule>
    static void RegisterModule(const wchar_t* name, ModuleCategory::Enum category)
    {
        registeredModules.push_back(GdkNew ModuleRegistrationT<TModule>(name, category));
    }
    
	
};




