/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "UnitTestsModule.h"


#define COMPARE_FLOAT(a, b) (((a) >= ((b) - Math::ZERO_TOLERANCE)) && ((a) <= ((b) + Math::ZERO_TOLERANCE)))

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Randoms(TestExecutionContext *context)
{
    #define RANDOM_SAMPLE_SIZE	100
    TestStatus::Enum result = TestStatus::Pass;

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

    context->Log->WriteLine(LogLevel::Info, L"Test: Random Int Range 100-200");
    context->Log->WriteLine(LogLevel::Info, L" - Min: %u   Max: %u   Avg: %u", intMin, intMax, intAvg);

    // Verify the min,max,avg
    if(intMin > 110) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random int 100-200 minimum %u > 110", intMin); 
        result = TestStatus::Warning;
    }
    if(intMax < 190) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random int 100-200 maximum %u < 190", intMax); 
        result = TestStatus::Warning;
    }
    if(intAvg < 140 || intAvg > 160) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random int 100-200 average %u is <140 or >160", intAvg); 
        result = TestStatus::Warning;
    }
    
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

    context->Log->WriteLine(LogLevel::Info, L"Test: Random Float Unit [0,1]");
    context->Log->WriteLine(LogLevel::Info, L" - Min: %f   Max: %f   Avg: %f", unitMin, unitMax, unitAvg);

    // Verify the min,max,avg
    if(unitMin > 0.1f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random unit minimum %f > 0.1", unitMin); 
        result = TestStatus::Warning;
    }
    if(unitMax < 0.9f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random unit maximum %f < 0.9", unitMax); 
        result = TestStatus::Warning;
    }
    if(unitAvg < 0.4f || unitAvg > 0.6f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random unit average %f is <0.4 or >0.6", unitAvg); 
        result = TestStatus::Warning;
    }
    

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
    context->Log->WriteLine(LogLevel::Info, L"Test: Random Float Symmetric [-1,1]");
    context->Log->WriteLine(LogLevel::Info, L" - Min: %f   Max: %f   Avg: %f", symMin, symMax, symAvg);

    // Verify the min,max,avg
    if(symMin > -0.9f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random symmetric minimum %f > -0.9", symMin); 
        result = TestStatus::Warning;
    }
    if(symMax < 0.9f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random symmetric maximum %f < 0.9", symMax); 
        result = TestStatus::Warning;
    }
    if(symAvg < -0.1f || symAvg > 0.1f) 
    {
        context->Log->WriteLine(LogLevel::Warning, L"Warning: Random symmetric average %f is <-0.1 or >0.1", symAvg); 
        result = TestStatus::Warning;
    }

    // Reproducable Seeds
    // ------------------------

    MersenneTwister random1(123456789);
    MersenneTwister random2(123456789);
    
    context->Log->WriteLine(LogLevel::Info, L"Test: Randoms from duplicate seeds");

    for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
    {
        float test1 = random1.GetUnit();
        float test2 = random2.GetUnit();
        
        if(test1 != test2)
        {
            context->Log->WriteLine(LogLevel::Error, L"ERROR: Two generators with the same seed produced mismatched results"); 
            result = TestStatus::Fail;
            break;
        }
    }

    // ---------------------------

    MersenneTwister randomA(1);
    MersenneTwister randomB(2);

    for(int i=0; i<RANDOM_SAMPLE_SIZE; i++)
    {
        float test1 = randomA.GetUnit();
        float test2 = randomB.GetUnit();
        
        if(test1 == test2)
        {
            context->Log->WriteLine(LogLevel::Error, L"ERROR: Two generators with differing seeds produced the same results"); 
            result = TestStatus::Fail;
            break;
        }
    }

    return result;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Vectors(TestExecutionContext *context)
{
    TestStatus::Enum result = TestStatus::Pass;

    // CTors (and assignment
    Vector2 a = Vector2(10,20);
    Vector2 b(5,5);
    Vector2 c(a);

    // Vector2 operator tests
    // --------------------------
    
    context->Log->WriteLine(LogLevel::Info, L"Test: Vector2 operator tests");
    
    // Comparison
    UNIT_TEST_ASSERT(!(a == b), L"Vector2 == operator failed");
    UNIT_TEST_ASSERT(a != b, L"Vector2 != operator failed");
    
    // Arithmetic operators
    UNIT_TEST_ASSERT((a+b) == Vector2(15,25), L"Vector2 + (Vector2) operator failed");
    UNIT_TEST_ASSERT((a-b) == Vector2(5,15), L"Vector2 - (Vector2) operator failed");
    UNIT_TEST_ASSERT((a*2) == Vector2(20,40), L"Vector2 * (scalar) operator failed");
    UNIT_TEST_ASSERT((2*a) == Vector2(20,40), L"(scalar) * Vector2 operator failed");
    UNIT_TEST_ASSERT((a/2) == Vector2(5,10), L"Vector2 / (scalar) operator failed");
    UNIT_TEST_ASSERT((-a) == Vector2(-10,-20), L"Vector2 unary negation operator failed");

    // Arithmeic update operators
    c = a; c += b;
    UNIT_TEST_ASSERT(c == Vector2(15,25), L"Vector2 += operator failed");
    c = a; c -= b;
    UNIT_TEST_ASSERT(c == Vector2(5,15), L"Vector2 -= operator failed");
    c = a; c *= 2;
    UNIT_TEST_ASSERT(c == Vector2(20,40), L"Vector2 *= operator failed");
    c = a; c /= 2;
    UNIT_TEST_ASSERT(c == Vector2(5,10), L"Vector2 /= operator failed");

    // Vector2 Method Tests
    // --------------------------
    
    context->Log->WriteLine(LogLevel::Info, L"Test: Vector2 method tests");
    
    // Vector functions
    float f = Vector2(3,4).Length();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 5), L"Vector2.Length() function failed");
    f = Vector2(3,4).LengthSquared();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 25), L"Vector2.LengthSquared() function failed");

    c = Vector2(-123,123);
    c.Normalize();
    f = c.Length();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 1.0f) && COMPARE_FLOAT(c.X,-c.Y), L"Vector2.Normalize() function failed");

    f = Vector2(0,1).Dot(Vector2(1,0));
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 0), L"Vector2.Dot() - failed on perpendicular vectors");
    f = Vector2(1,1).Dot(Vector2(1,1));
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 2), L"Vector2.Dot() - failed on parallel, equal-direction vectors");
    f = Vector2(1,1).Dot(Vector2(-1,-1));
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, -2), L"Vector2.Dot() - failed on parallel, negative-direction vectors");

    f = Vector2(1,0).GetRotationAngle();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, 0), L"Vector2.GetDirectionAngle() - failed on 0 degree vector");
    f = Vector2(0,1).GetRotationAngle();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, Math::HALF_PI), L"Vector2.GetDirectionAngle() - failed on 90 degree vector");
    f = Vector2(-1,-1).GetRotationAngle();
    UNIT_TEST_ASSERT(COMPARE_FLOAT(f, Math::PI * -0.75f), L"Vector2.GetDirectionAngle() - failed on 225 degree vector");
    
    c = Vector2::FromAngle(0);
    UNIT_TEST_ASSERT(COMPARE_FLOAT(c.X, 1), L"Vector2.FromAngle() - failed on 0 degree vector");
    UNIT_TEST_ASSERT(COMPARE_FLOAT(c.Y, 0), L"Vector2.FromAngle() - failed on 0 degree vector");
    c = Vector2::FromAngle(Math::HALF_PI);
    UNIT_TEST_ASSERT(COMPARE_FLOAT(c.X, 0), L"Vector2.FromAngle() - failed on 90 degree vector");
    UNIT_TEST_ASSERT(COMPARE_FLOAT(c.Y, 1), L"Vector2.FromAngle() - failed on 90 degree vector");

    // Interpolation Methods
    // TODO

    // String Methods
    // TODO
    
    return result;
}
