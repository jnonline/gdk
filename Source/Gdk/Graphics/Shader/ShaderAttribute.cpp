
#include "BasePCH.h"
#include "ShaderAttribute.h"
#include "../../System/StringUtilities.h"

using namespace Gdk;

// *******************************************************************************************
ShaderAttribute::ShaderAttribute(const char* name, ShaderAttributeType::Enum type, GLint size, GLint location)
	: Name(name), Type(type), Size(size), Location(location)
{
}