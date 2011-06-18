
#include "BasePCH.h"
#include "Random.h"
#include "MersenneTwister.h"

using namespace Gdk;

// instantiations of static constants
MersenneTwister Random::Generator(0x19791979);

// ***********************************************************************
void Random::Seed(UInt32 seed)
{
	Generator.Seed(seed);
}

// ***********************************************************************
UInt32 Random::GetUInt32()
{
    return Generator.GetUInt32();
}

// ***********************************************************************
Int32 Random::GetInt32()
{
	return Generator.GetInt32();
}

// ***********************************************************************
Int32 Random::FromInterval(Int32 min, Int32 max)
{
	return Generator.FromInterval(min, max);
}

// ***********************************************************************
float Random::GetUnit()
{
	return Generator.GetUnit();
}

// ***********************************************************************
float Random::GetSymmetric()
{
	return Generator.GetSymmetric();
}

// ***********************************************************************
float Random::FromInterval(float min, float max)
{
	return Generator.FromInterval(min, max);
}