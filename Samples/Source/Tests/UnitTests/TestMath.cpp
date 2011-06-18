
// Includes
#include "UnitTestsModule.h"

#define COMPARE_FLOAT(a, b) (((a) >= ((b) - Math::ZERO_TOLERANCE)) && ((a) <= ((b) + Math::ZERO_TOLERANCE)))

// ***********************************************************************
void UnitTestsModule::TestMath()
{
	Clear();
	WriteLine(LogLevel::System, L"Tests Starting");

	// ===================================================================
	// Randoms
	{
		WriteLine(LogLevel::Info, L"Running: Random Number Tests");

		#define RANDOM_SAMPLE_SIZE	100

		// Integer range
		// -----------------------

		int intArray[RANDOM_SAMPLE_SIZE];
		int intTotal = 0;
		int intMin = 200;
		int intMax = 100;
		for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
		{
			intArray[i] = Random::FromInterval(100, 200);
			intTotal += intArray[i];
			intMin = Math::Min(intMin, intArray[i]);
			intMax = Math::Max(intMax, intArray[i]);
		}
		int intAvg = intTotal / RANDOM_SAMPLE_SIZE;

		LOG_INFO(L"Test: Random Int Range 100-200");
		LOG_INFO(L" - Min: %u   Max: %u   Avg: %u", intMin, intMax, intAvg);

		ASSERT(intMin < 110, L"%u < 110", intMin); 
		ASSERT(intMax > 190, L"%u > 190", intMax); 
		ASSERT(intAvg > 145 && intAvg < 155, L"145 < %u < 155", intAvg); 

		// Unit [0-1] Floats
		// --------------------------

		float unitArray[RANDOM_SAMPLE_SIZE];
		float unitTotal = 0.0f;
		float unitMin = 10.0f;
		float unitMax = -10.0f;
		for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
		{
			unitArray[i] = Random::GetUnit();
			unitTotal += unitArray[i];
			unitMin = Math::Min(unitMin, unitArray[i]);
			unitMax = Math::Max(unitMax, unitArray[i]);
		}
		float unitAvg = unitTotal / RANDOM_SAMPLE_SIZE;

		LOG_INFO(L"Test: Random Float Unit [0,1]");
		LOG_INFO(L" - Min: %f   Max: %f   Avg: %f", unitMin, unitMax, unitAvg);

		ASSERT(unitMin < 0.1f, L"%f < 0.1", unitMin); 
		ASSERT(unitMax > 0.9f, L"%f > 0.9", unitMax); 
		ASSERT(unitAvg > 0.45f && unitAvg < 0.55f, L"0.45 < %f < 0.55", unitAvg); 

		// Symmetric [0-1] Floats
		// --------------------------

		float symArray[RANDOM_SAMPLE_SIZE];
		float symTotal = 0.0f;
		float symMin = 10.0f;
		float symMax = -10.0f;
		for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
		{
			symArray[i] = Random::GetSymmetric();
			symTotal += symArray[i];
			symMin = Math::Min(symMin, symArray[i]);
			symMax = Math::Max(symMax, symArray[i]);
		}
		float symAvg = symTotal / RANDOM_SAMPLE_SIZE;
		LOG_INFO(L"Test: Random Float Symmetric [-1,1]");
		LOG_INFO(L" - Min: %f   Max: %f   Avg: %f", symMin, symMax, symAvg);

		ASSERT(symMin < -0.9f, L"%f < -0.9", symMin); 
		ASSERT(symMax > 0.9f, L"%f > 0.9", symMax); 
		ASSERT(symAvg > -0.1f && symAvg < 0.1f, L"-0.1 < %f < 0.1", symAvg); 

		// Reproducable Seeds
		// ------------------------

		MersenneTwister random1(123456789);
		MersenneTwister random2(123456789);

		for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
		{
			float test1 = random1.GetUnit();
			float test2 = random2.GetUnit();
			ASSERT(test1 == test2, L"Random generator reproducable seed test failed");
		}

		LOG_INFO(L"Test: Two Random instances using the same seed, produce the same results");


		// ---------------------------

		MersenneTwister randomA(1);
		MersenneTwister randomB(2);

		for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
		{
			float test1 = randomA.GetUnit();
			float test2 = randomB.GetUnit();
			ASSERT(test1 != test2, L"Random generator reproducable seed test failed");
		}

		LOG_INFO(L"Test: Two Random instances using similar seeds, produce different results");
	}

	// ===================================================================
	// Vectors
	{
		WriteLine(LogLevel::Info, L"Running: Vector Tests");

		LOG_INFO(L"Test: Vector2 operator & Method tests");

		// CTors (and assignment
		Vector2 a = Vector2(10,20);
		Vector2 b(5,5);
		Vector2 c(a);

		// Comparison
		ASSERT(!(a == b), L"Vector2 == operator failed");
		ASSERT(a != b, L"Vector2 != operator failed");
		
		// Arithmetic operators
		ASSERT((a+b) == Vector2(15,25), L"Vector2 + (Vector2) operator failed");
		ASSERT((a-b) == Vector2(5,15), L"Vector2 - (Vector2) operator failed");
		ASSERT((a*2) == Vector2(20,40), L"Vector2 * (scalar) operator failed");
		ASSERT((2*a) == Vector2(20,40), L"(scalar) * Vector2 operator failed");
		ASSERT((a/2) == Vector2(5,10), L"Vector2 / (scalar) operator failed");
		ASSERT((-a) == Vector2(-10,-20), L"Vector2 unary negation operator failed");

		// Arithmeic update operators
		c = a; c += b;
		ASSERT(c == Vector2(15,25), L"Vector2 += operator failed");
		c = a; c -= b;
		ASSERT(c == Vector2(5,15), L"Vector2 -= operator failed");
		c = a; c *= 2;
		ASSERT(c == Vector2(20,40), L"Vector2 *= operator failed");
		c = a; c /= 2;
		ASSERT(c == Vector2(5,10), L"Vector2 /= operator failed");

		// Vector functions
		float f = Vector2(3,4).Length();
		ASSERT(COMPARE_FLOAT(f, 5), L"Vector2.Length() function failed");
		f = Vector2(3,4).LengthSquared();
		ASSERT(COMPARE_FLOAT(f, 25), L"Vector2.LengthSquared() function failed");

		c = Vector2(-123,123);
		c.Normalize();
		f = c.Length();
		ASSERT(COMPARE_FLOAT(f, 1.0f) && COMPARE_FLOAT(c.X,-c.Y), L"Vector2.Normalize() function failed");

		f = Vector2(0,1).Dot(Vector2(1,0));
		ASSERT(COMPARE_FLOAT(f, 0), L"Vector2.Dot() - failed on perpendicular vectors");
		f = Vector2(1,1).Dot(Vector2(1,1));
		ASSERT(COMPARE_FLOAT(f, 2), L"Vector2.Dot() - failed on parallel, equal-direction vectors");
		f = Vector2(1,1).Dot(Vector2(-1,-1));
		ASSERT(COMPARE_FLOAT(f, -2), L"Vector2.Dot() - failed on parallel, negative-direction vectors");

		f = Vector2(1,0).GetRotationAngle();
		ASSERT(COMPARE_FLOAT(f, 0), L"Vector2.GetDirectionAngle() - failed on 0 degree vector");
		f = Vector2(0,1).GetRotationAngle();
		ASSERT(COMPARE_FLOAT(f, Math::HALF_PI), L"Vector2.GetDirectionAngle() - failed on 90 degree vector");
		f = Vector2(-1,-1).GetRotationAngle();
		ASSERT(COMPARE_FLOAT(f, Math::PI * -0.75f), L"Vector2.GetDirectionAngle() - failed on 225 degree vector");
		
		c = Vector2::FromAngle(0);
		ASSERT(COMPARE_FLOAT(c.X, 1), L"Vector2.FromAngle() - failed on 0 degree vector");
		ASSERT(COMPARE_FLOAT(c.Y, 0), L"Vector2.FromAngle() - failed on 0 degree vector");
		c = Vector2::FromAngle(Math::HALF_PI);
		ASSERT(COMPARE_FLOAT(c.X, 0), L"Vector2.FromAngle() - failed on 90 degree vector");
		ASSERT(COMPARE_FLOAT(c.Y, 1), L"Vector2.FromAngle() - failed on 90 degree vector");

		// Interpolation Methods
		// TODO

		// String Methods
		// TODO
		
	}

	WriteLine(LogLevel::System, L"Tests Complete");
}

