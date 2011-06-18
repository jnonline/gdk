
#pragma once

// Includes
#include "Gdk.h"

using namespace Gdk;

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

	// *******************************************************************
	static vector<class ModuleRegistrationBase*>& GetRegistrations();
};




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
class ModuleRegistrationBase
{
public:
	// Properties
	wstring					Name;
	ModuleCategory::Enum	Category;

	// Methods
	ModuleRegistrationBase(const wchar_t* name, ModuleCategory::Enum category) : Name(name), Category(category) {}
	virtual class Module* CreateModule() = 0;
};

// *******************************************************************
template <typename TModule>
class ModuleRegistration : public ModuleRegistrationBase
{
public:
	// CTor
	ModuleRegistration(const wchar_t* name, ModuleCategory::Enum category) : ModuleRegistrationBase(name, category)
	{
		Module::GetRegistrations().push_back(this);
	}

	// CreateModule
	virtual Module* CreateModule()
	{
		return GdkNew TModule();
	}
};