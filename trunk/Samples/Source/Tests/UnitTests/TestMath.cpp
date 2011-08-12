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
    UNIT_TEST_CHECK(!(a == b), L"Vector2 == operator");
    UNIT_TEST_CHECK(a != b, L"Vector2 != operator");
    
    // Arithmetic operators
    UNIT_TEST_CHECK((a+b) == Vector2(15,25), L"Vector2 + (Vector2) operator");
    UNIT_TEST_CHECK((a-b) == Vector2(5,15), L"Vector2 - (Vector2) operator");
    UNIT_TEST_CHECK((a*2) == Vector2(20,40), L"Vector2 * (scalar) operator");
    UNIT_TEST_CHECK((2*a) == Vector2(20,40), L"(scalar) * Vector2 operator");
    UNIT_TEST_CHECK((a/2) == Vector2(5,10), L"Vector2 / (scalar) operator");
    UNIT_TEST_CHECK((-a) == Vector2(-10,-20), L"Vector2 unary negation operator");

    // Arithmeic update operators
    c = a; c += b;
    UNIT_TEST_CHECK(c == Vector2(15,25), L"Vector2 += operator");
    c = a; c -= b;
    UNIT_TEST_CHECK(c == Vector2(5,15), L"Vector2 -= operator");
    c = a; c *= 2;
    UNIT_TEST_CHECK(c == Vector2(20,40), L"Vector2 *= operator");
    c = a; c /= 2;
    UNIT_TEST_CHECK(c == Vector2(5,10), L"Vector2 /= operator");

    // Vector2 Method Tests
    // --------------------------
    
    context->Log->WriteLine(LogLevel::Info, L"Test: Vector2 method tests");
    
    // Vector functions
    float f = Vector2(3,4).Length();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 5), L"Vector2.Length() function");
    f = Vector2(3,4).LengthSquared();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 25), L"Vector2.LengthSquared() function");

    c = Vector2(-123,123);
    c.Normalize();
    f = c.Length();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 1.0f) && COMPARE_FLOAT(c.X,-c.Y), L"Vector2.Normalize() function");

    f = Vector2(0,1).Dot(Vector2(1,0));
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 0), L"Vector2.Dot() - perpendicular vectors");
    f = Vector2(1,1).Dot(Vector2(1,1));
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 2), L"Vector2.Dot() - parallel, equal-direction vectors");
    f = Vector2(1,1).Dot(Vector2(-1,-1));
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, -2), L"Vector2.Dot() - parallel, negative-direction vectors");

    f = Vector2(1,0).GetRotationAngle();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, 0), L"Vector2.GetDirectionAngle() - 0 degree vector");
    f = Vector2(0,1).GetRotationAngle();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, Math::HALF_PI), L"Vector2.GetDirectionAngle() - 90 degree vector");
    f = Vector2(-1,-1).GetRotationAngle();
    UNIT_TEST_CHECK(COMPARE_FLOAT(f, Math::PI * -0.75f), L"Vector2.GetDirectionAngle() - 225 degree vector");
    
    c = Vector2::FromAngle(0);
    UNIT_TEST_CHECK(COMPARE_FLOAT(c.X, 1) && COMPARE_FLOAT(c.Y, 0), L"Vector2.FromAngle() - 0 degree vector");
    c = Vector2::FromAngle(Math::HALF_PI);
    UNIT_TEST_CHECK(COMPARE_FLOAT(c.X, 0) && COMPARE_FLOAT(c.Y, 1), L"Vector2.FromAngle() - 90 degree vector");

    // Interpolation Methods
    // TODO

    // String Methods
    // TODO
    
    return result;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Distance2D(TestExecutionContext *context)
{
    float distance;
    
    // Point - Ray
    Ray2 testRay(Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f));
    
    distance = Distance2::PointToRay(Vector2(8.5f, 0.5f), testRay);
    UNIT_TEST_CHECK(distance == 0.5f, L"Distance2::PointToRay() - point close to ray");
    distance = Distance2::PointToRay(Vector2(8.5f, 1.0f), testRay);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance2::PointToRay() - point on ray");
    distance = Distance2::PointToRay(Vector2(0.0f, 1.0f), testRay);
    UNIT_TEST_CHECK(distance == 1.0f, L"Distance2::PointToRay() - point behind ray");
    
    // Point - Line Segment
    LineSegment2 testLineSegment(Vector2(-1.0f, 1.0f), Vector2(1.0f, -1.0f));
    
    distance = Distance2::PointToLineSegment(Vector2(1.0f, 1.0f), testLineSegment);
    UNIT_TEST_CHECK(distance == Math::Sqrt(2.0f), L"Distance2::PointToLineSegment() - point close to segment");
    distance = Distance2::PointToLineSegment(Vector2(-0.5f, 0.5f), testLineSegment);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance2::PointToLineSegment() - point on segment");
    distance = Distance2::PointToLineSegment(Vector2(2.0f, -1.0f), testLineSegment);
    UNIT_TEST_CHECK(distance == 1.0f, L"Distance2::PointToLineSegment() - point diagonal to segment");
    
    // Point - Box    (using a box centered at (2,2) and oriented at (1,1)  (1,-1) with extents of (1,1)
    Box2 testBox(Vector2(2.0f, 2.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), 1.0f, 1.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    distance = Distance2::PointToBox(Vector2(0.0f, 2.0f), testBox);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 2.0f - Math::Sqrt(2.0f)), L"Distance2::PointToBox() - point diagonal to box");
    distance = Distance2::PointToBox(Vector2(0.0f, 4.0f), testBox);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, Math::Sqrt(8.0f) - 1.0f), L"Distance2::PointToBox() - point adjacent to box");
    distance = Distance2::PointToBox(Vector2(2.0f, 3.0f), testBox);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance2::PointToBox() - point inside box");
    distance = Distance2::PointToBox(Vector2(2.0f - Math::Sqrt(2.0f), 2.0f), testBox);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance2::PointToBox() - point on box corner");
    
    // Point - Rectangle
    // This internally just uses PointToBox
    
    // Point - Circle
    Circle2 testCircle(Vector2(2.0f, 2.0f), 2.0f);
    
    distance = Distance2::PointToCircle(Vector2(-2.0f, 2.0f), testCircle);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 2.0f), L"Distance2::PointToCircle() - point outside circle");
    distance = Distance2::PointToCircle(Vector2(1.0f, 1.0f), testCircle);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 0.0f), L"Distance2::PointToCircle() - point inside circle");
    distance = Distance2::PointToCircle(Vector2(2.0f, 0.0f), testCircle);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 0.0f), L"Distance2::PointToCircle() - point on circle edge");
    
    return TestStatus::Pass;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Intersection2D(TestExecutionContext *context)
{
    float distance;
    float expected;
    bool testResult;
    bool findResult;
    
    #define DO_TEST(prim1, prim2) \
        testResult = Intersection2::Test(prim1, prim2);
        
    #define DO_TEST_FIND(prim1, prim2) \
        testResult = Intersection2::Test(prim1, prim2); \
        findResult = Intersection2::Find(prim1, prim2, distance);
    
    
    // Ray -> Ray
    // -----------------
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 0.0f), Vector2(0.0f, 1.0f)),
                 Ray2(Vector2(0.0f, 2.0f), Vector2(1.0f, 0.0f)));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Ray) - intersecting rays" );
    UNIT_TEST_CHECK(findResult == true && distance == 2.0f, 
                    L"Find(Ray,Ray) - intersecting rays" );
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 0.0f), Vector2(0.0f, -1.0f)),
                 Ray2(Vector2(0.0f, 2.0f), Vector2(1.0f, 0.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Ray) - pointing away" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Ray) - pointing away" );
    
    DO_TEST_FIND(Ray2(Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f)),
                 Ray2(Vector2(2.0f, 1.0f), Vector2(1.0f, 0.0f)));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Ray) - parallel - co-linear" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f, 
                    L"Find(Ray,Ray) - parallel - co-linear" );
    
    DO_TEST_FIND(Ray2(Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f)),
                 Ray2(Vector2(1.0f, 2.0f), Vector2(1.0f, 0.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Ray) - parallel - non-co-linear" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Ray) - parallel - non-co-linear" );
    
    // Ray -> LineSegment
    // ----------------------
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 0.0f), Vector2(0.0f, 1.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,LineSegment) - ray intersects segment" );
    UNIT_TEST_CHECK(findResult == true && distance == 2.0f, 
                    L"Find(Ray,LineSegment) - ray intersects segment" );
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 0.0f), Vector2(0.0f, -1.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,LineSegment) - ray pointing away from segment" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,LineSegment) - ray pointing away from segment" );
    
    DO_TEST_FIND(Ray2(Vector2(3.0f, 0.0f), Vector2(0.0f, -1.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,LineSegment) - ray points outside the segment" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,LineSegment) - ray points outside the segment" );
    
    DO_TEST_FIND(Ray2(Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,LineSegment) - ray & segment are parallel, but not co-linear" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,LineSegment) - ray & segment are parallel, but not co-linear" );
    
    DO_TEST_FIND(Ray2(Vector2(-1.0f, 2.0f), Vector2(1.0f, 0.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,LineSegment) - ray & segment are parallel & co-linear" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,LineSegment) - ray & segment are parallel & co-linear" );
    
    DO_TEST_FIND(Ray2(Vector2(-1.0f, 2.0f), Vector2(-1.0f, 0.0f)),
                 LineSegment2(Vector2(1.0f, 2.0f), Vector2(2.5f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,LineSegment) - ray & segment are parallel & co-linear, but ray points away" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,LineSegment) - ray & segment are parallel & co-linear, but ray points away" );
    
    
    // Ray -> Circle
    // ----------------------
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 1.0f), Vector2(0.0f, 1.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Circle) - ray starts in circle" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f, 
                    L"Find(Ray,Circle) - ray starts in circle" );
    
    DO_TEST_FIND(Ray2(Vector2(-2.0f, 2.0f), Vector2(1.0f, 0.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Circle) - ray intersects circle" );
    UNIT_TEST_CHECK(findResult == true && distance == 2.0f, 
                    L"Find(Ray,Circle) - ray intersects circle" );
    
    DO_TEST_FIND(Ray2(Vector2(0.0f, 1.0f), Vector2(0.0f, 1.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Circle) - ray is tangent to circle" );
    UNIT_TEST_CHECK(findResult == true && distance == 1.0f, 
                    L"Find(Ray,Circle) - ray is tangent to circle" );
    
    DO_TEST_FIND(Ray2(Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Circle) - ray misses circle 1" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Circle) - ray misses circle 1" );
    
    DO_TEST_FIND(Ray2(Vector2(-2.0f, 5.0f), Vector2(1.0f, 0.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Circle) - ray misses circle 2" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Circle) - ray misses circle 2" );
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, -1.0f), Vector2(0.0f, -1.0f)),
                 Circle2(Vector2(2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Circle) - ray pointing away from circle" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Circle) - ray pointing away from circle" );
    
    // Ray -> Box
    // ----------------------
    
    Box2 testBox(Vector2(2.0f, 2.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    DO_TEST_FIND(Ray2(Vector2(2.0f, 1.0f), Vector2(0.0f, 1.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray starts in box" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f, 
                    L"Find(Ray,Box) - ray starts in box" );
    
    DO_TEST_FIND(Ray2(Vector2(-2.0f, 2.0f), Vector2(1.0f, 0.0f)),
                 testBox);
    expected = 4.0f - Math::Sqrt(8.0f);
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray intersects box" );
    UNIT_TEST_CHECK(findResult == true && COMPARE_FLOAT(distance, expected), 
                    L"Find(Ray,Box) - ray intersects box" );
    
    Vector2 upRight(1.0f,1.0f);
    upRight.Normalize();
    DO_TEST_FIND(Ray2(Vector2(2.99f, -1.0f), upRight),
                 Box2(Vector2(2.0f, 2.0f), Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f), 2.0f, 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray barely intersects the box" );
    UNIT_TEST_CHECK(findResult == true && COMPARE_FLOAT(distance, Math::Sqrt(2.0f)), 
                    L"Find(Ray,Box) - ray barely intersects the box" );
    
    DO_TEST_FIND(Ray2(Vector2(3.01f, -1.0f), upRight),
                 Box2(Vector2(2.0f, 2.0f), Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f), 2.0f, 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray barely misses the box" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray barely misses the box" );
    
    DO_TEST_FIND(Ray2(Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray misses box 1" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray misses box 1" );
    
    DO_TEST_FIND(Ray2(Vector2(-2.0f, 5.0f), Vector2(1.0f, 0.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray misses box 2" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray misses box 2" );
    
    DO_TEST_FIND(Ray2(Vector2(2.2f, -1.0f), Vector2(0.0f, -1.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray pointing away from box" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray pointing away from box" );

    
    // Circle -> Circle
    // -----------------
    
    DO_TEST(Circle2(Vector2(-2.0f, 3.0f), 2.2f),
            Circle2(Vector2(2.0f, 3.0f), 2.2f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Circle) - Circles intersect" );

    DO_TEST(Circle2(Vector2(-2.0f, 3.0f), 10.0f),
            Circle2(Vector2(2.0f, 3.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Circle) - Big Circle encapsulates small circle" );
    
    DO_TEST(Circle2(Vector2(-2.0f, 3.0f), 2.0f),
            Circle2(Vector2(2.0f, 3.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Circle) - Circles touch at edge" );
    
    DO_TEST(Circle2(Vector2(-2.0f, 3.0f), 1.9f),
            Circle2(Vector2(2.0f, 3.0f), 1.9f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Circle) - Circles dont touch 1" );
    
    DO_TEST(Circle2(Vector2(-3.0f, -3.0f), 4.0f),
            Circle2(Vector2(3.0f, 3.0f), 4.0f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Circle) - Circles dont touch 2" );
    
    
    // Circle -> Rect
    // -----------------    
    
    DO_TEST(Circle2(Vector2(-1.0f, 2.0f), 2.2f),
            Rectangle2(Vector2(1.0f, 1.0f), Vector2(2.0f, 2.0f)));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Rect) - Circle & Rect intersect" );
    
    DO_TEST(Circle2(Vector2(3.0f, 3.0f), 2.0f),
            Rectangle2(Vector2(2.5f, 2.5f), Vector2(1.0f, 1.0f)));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Rect) - Circle encapsulates rect" );
    
    DO_TEST(Circle2(Vector2(3.0f, 3.0f), 0.5f),
            Rectangle2(Vector2(2.0f, 2.0f), Vector2(2.0f, 2.0f)));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Rect) - Rect encapsulates circle" );
    
    DO_TEST(Circle2(Vector2(-2.0f, 2.0f), 2.0f),
            Rectangle2(Vector2(0.0f, 1.0f), Vector2(2.0f, 2.0f)));
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Rect) - Circle & Rect touch at edge" );
    
    DO_TEST(Circle2(Vector2(-2.0f, 2.0f), 1.9f),
            Rectangle2(Vector2(0.0f, 1.0f), Vector2(2.0f, 2.0f)));
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Rect) - Circle & Rect dont touch 1" );
    
    DO_TEST(Circle2(Vector2(-2.0f, -2.0f), 2.8f),
            Rectangle2(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f)));
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Rect) - Circle & Rect dont touch 2" );
    
    // Circle -> Box
    // -----------------
    
    testBox = Box2(Vector2(2.0f, 2.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    DO_TEST(Circle2(Vector2(-1.0f, 2.0f), 2.2f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Box) - Circle & Box intersect" );
    
    DO_TEST(Circle2(Vector2(3.0f, 3.0f), 6.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Box) - Circle encapsulates Box" );
    
    DO_TEST(Circle2(Vector2(2.5f, 2.5f), 0.5f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Circle,Box) - Box encapsulates circle" );
    
    DO_TEST(Circle2(Vector2(-2.0f, -2.0f), 1.4f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Box) - Circle & Box dont touch 1" );
    
    DO_TEST(Circle2(Vector2(-2.0f, -2.0f), 2.8f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Circle,Box) - Circle & Box dont touch 2" );
    
    
    // Circle -> Box
    // -----------------
    
    testBox = Box2(Vector2(2.0f, 2.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    Box2 testBox2(Vector2(5.0f, 5.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), 3.0f, 3.0f);
    testBox2.Axis[0].Normalize();
    testBox2.Axis[1].Normalize();
    
    DO_TEST(testBox, testBox2);
    UNIT_TEST_CHECK(testResult == true, L"Test(Box,Box) - Boxes overlap" );
    
    testBox2.Extent[0] = 10.0f;
    testBox2.Extent[1] = 10.0f;
    DO_TEST(testBox, testBox2);
    UNIT_TEST_CHECK(testResult == true, L"Test(Box,Box) - Big box encapsulates smaller box" );
    
    testBox2.Extent[0] = 1.0f;
    testBox2.Extent[1] = 1.0f;
    DO_TEST(testBox, testBox2);
    UNIT_TEST_CHECK(testResult == false, L"Test(Box,Box) - Boxes dont touch 1" );
    
    testBox2.Center = Vector2(2.0f, 7.0f);
    DO_TEST(testBox, testBox2);
    UNIT_TEST_CHECK(testResult == false, L"Test(Box,Box) - Boxes dont touch 2" );
    
    
    // ------------------
    
    #undef DO_TEST
    #undef DO_TEST_FIND
 
    return TestStatus::Pass;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Distance3D(TestExecutionContext *context)
{
    float distance;
    
    // Point - Ray
    Ray3 testRay(Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f));
    
    distance = Distance3::PointToRay(Vector3(8.5f, 1.0f, 1.0f), testRay);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, Math::Sqrt(2.0f)) , L"Distance3::PointToRay() - point close to ray");
    distance = Distance3::PointToRay(Vector3(8.5f, 2.0f, 2.0f), testRay);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance3::PointToRay() - point on ray");
    distance = Distance3::PointToRay(Vector3(0.0f, 2.0f, 2.0f), testRay);
    UNIT_TEST_CHECK(distance == 2.0f, L"Distance3::PointToRay() - point behind ray");
    
    // Point - Plane
    Plane3 testPlane(Vector3(1.0f, 0.0f, 0.0f), 2.0f);
    
    distance = Distance3::PointToPlane(Vector3(4.0f, 5.0f, 6.0f), testPlane);
    UNIT_TEST_CHECK(distance == 2.0f, L"Distance3::PointToPlane() - point close to plane");
    distance = Distance3::PointToPlane(Vector3(2.0f, 3.0f, 0.5f), testPlane);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance3::PointToPlane() - point on plane");
    
    // Point - Box    (using a box centered at (2,2,2) and oriented with up at (1,1,1)  with extents of (2,2,2)
    Vector3 boxAxis1(1.0f, 1.0f, 1.0f);
    boxAxis1.Normalize();
    Vector3 boxAxis2 = boxAxis1.Cross(Vector3::UNIT_Y);
    boxAxis2.Normalize();
    Vector3 boxAxis3 = boxAxis1.Cross(boxAxis2);
    Box3 testBox(Vector3(2.0f, 2.0f, 2.0f), boxAxis1, boxAxis2, boxAxis3, 2.0f, 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    testBox.Axis[2].Normalize();
    
    distance = Distance3::PointToBox(Vector3(8.0f, 8.0f, 8.0f), Box3(Vector3(3.0f, 3.0f, 3.0f), Vector3::UNIT_X, Vector3::UNIT_Y, Vector3::UNIT_Z, 2.0f, 3.0f, 4.0f));
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, Math::Sqrt(14.0f)), L"Distance3::PointToBox() - point diagonal to box");
    distance = Distance3::PointToBox(Vector3(-2.0f, -2.0f, -2.0f), testBox);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, Math::Sqrt(48.0f) - 2.0f), L"Distance3::PointToBox() - point adjacent to box");
    distance = Distance3::PointToBox(Vector3(3.0f, 3.0f, 3.0f), testBox);
    UNIT_TEST_CHECK(distance == 0.0f, L"Distance3::PointToBox() - point inside box");
    
    // Point - Sphere
    Sphere3 testSphere(Vector3(2.0f, 2.0f, 2.0f), 2.0f);
    
    distance = Distance3::PointToSphere(Vector3(-2.0f, 2.0f, 2.0f), testSphere);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 2.0f), L"Distance3::PointToSphere() - point outside sphere");
    distance = Distance3::PointToSphere(Vector3(1.0f, 1.0f, 1.6f), testSphere);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 0.0f), L"Distance3::PointToSphere() - point inside sphere");
    distance = Distance3::PointToSphere(Vector3(2.0f, 2.0f, 0.0f), testSphere);
    UNIT_TEST_CHECK(COMPARE_FLOAT(distance, 0.0f), L"Distance3::PointToSphere() - point on sphere edge");
    
    return TestStatus::Pass;

}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_Math_Intersection3D(TestExecutionContext *context)
{
    float distance;
    float expected;
    bool testResult;
    bool findResult;
    
    #define DO_TEST(prim1, prim2) \
        testResult = Intersection3::Test(prim1, prim2);
        
    #define DO_TEST_FIND(prim1, prim2) \
        testResult = Intersection3::Test(prim1, prim2); \
        findResult = Intersection3::Find(prim1, prim2, distance);
    
    
    // Ray -> Plane
    // -----------------
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 1.0f, 0.0f)),
                 Plane3(Vector3(0.0f, 1.0f, 0.0f), 4.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Plane) - ray intersects plane" );
    UNIT_TEST_CHECK(findResult == true && distance == 2.0f, 
                    L"Find(Ray,Plane) - ray intersects plane" );
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, -1.0f, 0.0f)),
                 Plane3(Vector3(0.0f, 1.0f, 0.0f), 4.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Plane) - ray pointing away from plane" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Plane) - ray pointing away from plane" );
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f)),
                 Plane3(Vector3(0.0f, 1.0f, 0.0f), 4.0f));    
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Plane) - ray & plane are parallel, but not co-linear" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Plane) - ray & plane are parallel, but not co-linear" );
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f)),
                 Plane3(Vector3(0.0f, 1.0f, 0.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Plane) - ray & plane are parallel & co-linear" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f,
                    L"Find(Ray,Plane) - ray & plane are parallel & co-linear" );

    
    // Ray -> Sphere
    // ----------------------
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 1.0f, 1.5f), Vector3(0.0f, 1.0f, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Sphere) - ray starts in sphere" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f, 
                    L"Find(Ray,Sphere) - ray starts in sphere" );
    
    DO_TEST_FIND(Ray3(Vector3(-2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Sphere) - ray intersects sphere" );
    UNIT_TEST_CHECK(findResult == true && distance == 2.0f, 
                    L"Find(Ray,Sphere) - ray intersects sphere" );
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 0.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Sphere) - ray is tangent to sphere" );
    UNIT_TEST_CHECK(findResult == true && distance == 1.5f, 
                    L"Find(Ray,Sphere) - ray is tangent to sphere" );
    
    DO_TEST_FIND(Ray3(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Sphere) - ray misses sphere 1" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Sphere) - ray misses sphere 1" );
    
    DO_TEST_FIND(Ray3(Vector3(-2.0f, 5.0f, 5.0f), Vector3(1.0f, 0.0, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Sphere) - ray misses sphere 2" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Sphere) - ray misses sphere 2" );
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, -1.0f, 2.0f), Vector3(0.0f, -1.0f, 0.0f)),
                 Sphere3(Vector3(2.0f, 2.0f, 2.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Sphere) - ray pointing away from sphere" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Sphere) - ray pointing away from sphere" );
    
    // Ray -> Box
    // ----------------------
    
    Box3 testBox(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,1,0), Vector3(1,-1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    DO_TEST_FIND(Ray3(Vector3(2.0f, 1.0f, 1.5f), Vector3(0.0f, 1.0f, 0.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray starts in box" );
    UNIT_TEST_CHECK(findResult == true && distance == 0.0f, 
                    L"Find(Ray,Box) - ray starts in box" );
    
    DO_TEST_FIND(Ray3(Vector3(-2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f)),
                 testBox);
    expected = 4.0f - Math::Sqrt(8.0f);
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray intersects box" );
    UNIT_TEST_CHECK(findResult == true && COMPARE_FLOAT(distance, expected), 
                    L"Find(Ray,Box) - ray intersects box" );
    
    Vector3 upRight3(1.0f, 1.0f, 0.0f);
    upRight3.Normalize();
    expected = Math::Sqrt(2.0f);
    DO_TEST_FIND(Ray3(Vector3(2.99f, -1.0f, 2.0f), upRight3),
                 Box3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f));
    UNIT_TEST_CHECK(testResult == true, 
                    L"Test(Ray,Box) - ray barely intersects the box" );
    UNIT_TEST_CHECK(findResult == true && COMPARE_FLOAT(distance, expected), 
                    L"Find(Ray,Box) - ray barely intersects the box" );
    
    DO_TEST_FIND(Ray3(Vector3(3.01f, -1.0f, 2.0f), upRight3),
                 Box3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f));
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray barely misses the box" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray barely misses the box" );
    
    DO_TEST_FIND(Ray3(Vector3(-1.0f, 2.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray misses box 1" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray misses box 1" );
    
    DO_TEST_FIND(Ray3(Vector3(-2.0f, 5.0f, 10.0f), Vector3(1.0f, 0.0f, 1.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray misses box 2" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray misses box 2" );
    
    DO_TEST_FIND(Ray3(Vector3(2.2f, -1.0f, 2.2f), Vector3(0.0f, -1.0f, 0.0f)),
                 testBox);
    UNIT_TEST_CHECK(testResult == false, 
                    L"Test(Ray,Box) - ray pointing away from box" );
    UNIT_TEST_CHECK(findResult == false, 
                    L"Find(Ray,Box) - ray pointing away from box" );
    
    // Plane -> Plane
    // -----------------
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 5.0f),
            Plane3(Vector3(1,-1,-1).GetNormalized(), 7.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,Plane) - Planes intersect" );
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 5.0f),
            Plane3(Vector3(1,1,1).GetNormalized(), 5.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,Plane) - Planes co-planar" );
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 5.0f),
            Plane3(Vector3(1,1,1).GetNormalized(), 7.0f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Plane,Plane) - Planes parallel but not co-planar" );
    
    // Plane -> Sphere
    // -----------------
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 3.0f),
            Sphere3(Vector3(2,2,2), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,Sphere) - Plane intersects sphere" );
    
    DO_TEST(Plane3(Vector3(1,0,0), 3.99f),
            Sphere3(Vector3(2,2,2), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,Sphere) - Plane barely touches sphere" );
    
    DO_TEST(Plane3(Vector3(1,0,0), 4.01f),
            Sphere3(Vector3(2,2,2), 2.0f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Plane,Sphere) - Plane barely misses sphere" );
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 6.0f),
            Sphere3(Vector3(2,2,2), 2.0f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Plane,Sphere) - Plane does not intersect sphere" );
    
    // Plane -> Box
    // -----------------
    
    testBox = Box3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,1,0), Vector3(1,-1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 3.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,box) - Plane intersects box" );
    
    DO_TEST(Plane3(Vector3(0,0,1).GetNormalized(), 3.99f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Plane,box) - Plane barely intersects box" );
    
    DO_TEST(Plane3(Vector3(0,0,1).GetNormalized(), 4.01f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Plane,box) - Plane barely misses box" );
    
    DO_TEST(Plane3(Vector3(1,1,1).GetNormalized(), 8.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Plane,box) - Plane does not intersect box" );

    
    // Sphere -> Sphere
    // -----------------
    
    DO_TEST(Sphere3(Vector3(-2.0f, 3.0f, 4.0f), 2.2f),
            Sphere3(Vector3(2.0f, 3.0f, 4.0f), 2.2f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Sphere) - Spheres intersect" );
    
    DO_TEST(Sphere3(Vector3(-2.0f, 3.0f, 4.0f), 10.0f),
            Sphere3(Vector3(2.0f, 3.0f, 4.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Sphere) - Big Sphere encapsulates small Sphere" );
    
    DO_TEST(Sphere3(Vector3(-2.0f, 3.0f, 4.0f), 2.0f),
            Sphere3(Vector3(2.0f, 3.0f, 4.0f), 2.0f));
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Sphere) - Spheres touch at edge" );
    
    DO_TEST(Sphere3(Vector3(-2.0f, 3.0f, 4.0f), 1.9f),
            Sphere3(Vector3(2.0f, 3.0f, 4.0f), 1.9f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Sphere,Sphere) - Spheres dont touch 1" );
    
    DO_TEST(Sphere3(Vector3(-3.0f, -3.0f, 4.0f), 4.0f),
            Sphere3(Vector3(3.0f, 3.0f, 4.0f), 4.0f));
    UNIT_TEST_CHECK(testResult == false, L"Test(Sphere,Sphere) - Spheres dont touch 2" );
    
    
    // Sphere -> Box
    // -----------------
    
    testBox = Box3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f);
    
    DO_TEST(Sphere3(Vector3(-2.0f, 2.0f, 2.0f), 2.01f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Box) - Sphere and box intersect" );
    
    DO_TEST(Sphere3(Vector3(-2.0f, 2.0f, 2.0f), 8.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Box) - Sphere encapsulates box" );

    DO_TEST(Sphere3(Vector3(1.5, 1.5f, 1.5f), 0.4f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Sphere,Box) - Box encapsulates sphere" );
    
    DO_TEST(Sphere3(Vector3(-2.0f, 2.0f, 2.0f), 1.99f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Sphere,Box) - Sphere and box dont touch" );

    // Box -> Box
    // -----------------
    
    testBox = Box3(Vector3(2.0f, 2.0f, 2.0f), Vector3(1,1,0), Vector3(1,-1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f);
    testBox.Axis[0].Normalize();
    testBox.Axis[1].Normalize();
    
    DO_TEST(Box3(Vector3(-2.82f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Box,Box) - Boxes barely intersect" );
    
    DO_TEST(Box3(Vector3(-2.83f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Box,Box) - Boxes barely miss" );
    
    DO_TEST(Box3(Vector3(-2.0f, 2.0f, 2.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 8.0f, 8.0f, 8.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == true, L"Test(Box,Box) - Big box encapsulates smaller box" );
    
    DO_TEST(Box3(Vector3(8.0f, 7.0f, 6.0f), Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), 2.0f, 2.0f, 2.0f),
            testBox);
    UNIT_TEST_CHECK(testResult == false, L"Test(Box,Box) - Boxes do not intersect" );
    
    // ------------------
    
    #undef DO_TEST
    #undef DO_TEST_FIND
    
    return TestStatus::Pass;
}
