
// Includes
#include "Module.h"

// *******************************************************************
vector<class ModuleRegistrationBase*>& Module::GetRegistrations()
{
	// Create the registrations vector on first use
	static vector<class ModuleRegistrationBase*>* registrations = new vector<class ModuleRegistrationBase*>();
	return *registrations;
}

