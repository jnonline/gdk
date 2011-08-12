/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// Utility macros for Unit Test methods
#ifdef GDKPLATFORM_WINDOWS
    #define UNIT_TEST_CHECK(condition, format, ...)   \
        if(condition) {   \
            context->Log->WriteLine(LogLevel::Info, L"PASS: " format, __VA_ARGS__);   \
        } else {   \
            context->Log->WriteLine(LogLevel::Error, L"FAIL: " format, __VA_ARGS__);   \
            return TestStatus::Fail;   \
        }
#else
    #define UNIT_TEST_CHECK(condition, format, args...)   \
        if(condition) {   \
            context->Log->WriteLine(LogLevel::Info, L"PASS: " format, ##args);   \
        } else {   \
            context->Log->WriteLine(LogLevel::Error, L"FAIL: " format, ##args);   \
            return TestStatus::Fail;   \
        }

#endif

// ==============================================
namespace TestStatus
{
    enum Enum
    {
        Pass,
        Fail,
        Warning,
        NotRun,
        Running
    };
}

// ==============================================
class TestLogLine
{
public:
    // Properties
    wstring Text;
    LogLevel::Enum Level;
    
    // CTor
    TestLogLine(const wchar_t* text, LogLevel::Enum level)
        : Text(text), Level(level)
    {}
};

// ==============================================
class TestLog
{
public:
    // Properties
    vector<TestLogLine*> Lines;
    int Indent;
    
    // CTor / DTor
    TestLog();
    ~TestLog();
    
    // Write methods
    void WriteLine(Gdk::LogLevel::Enum logLevel, const wchar_t *format, ...);
	void Clear();
};

// ==============================================
class TestExecutionContext
{
public:
    // Properties
    class TestTreeNode* TreeNode;
    TestLog* Log;
    
    // Ctor
    TestExecutionContext(class TestTreeNode* treeNode, TestLog* log)
        : TreeNode(treeNode), Log(log)
    {
    }
};

// ==============================================
typedef Delegate1<TestStatus::Enum, TestExecutionContext*> TestDelegate;

// ==============================================
class TestTreeNode
{
public:
    // Properties
    wstring Name;
    TestStatus::Enum Status;
    TestDelegate* Method;
    
    TestTreeNode* Parent;
    list<TestTreeNode*> Children;
    
    // CTor/DTor
    TestTreeNode(const wchar_t* name, TestDelegate* testMethod = NULL);
    ~TestTreeNode();
    
    // Adding children
    TestTreeNode* AddChild(TestTreeNode* child);
    TestTreeNode* AddChild(const wchar_t* name, TestDelegate* testMethod = NULL);
};


// *******************************************************************
class UnitTestsModule : public Module
{
public:
	// CTor / DTor
    UnitTestsModule();
	~UnitTestsModule();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

protected:

	// Input handlers
	void OnTouchBegan(Touch* touch);
	void OnMouseDown(MouseButton::Enum button);
    void OnMouseWheelScroll(float deltaX, float deltaY);

	// Test Tree 
	// -----------------------------

	TestTreeNode* rootNode;

    int currentTestIndex;
    vector<TestTreeNode*> flatTestTree;
    
    float currentLogPosition;
    TestLog log;    
    
    // Methods
    void BuildTestTree();
    void RecurseFlattenTree(TestTreeNode* node);
    
	// Tests!!!
	// -----------------------------

    #define TESTMETHOD(method)  TestStatus::Enum method(TestExecutionContext* context);
    
    // System Tests
    TESTMETHOD(Test_System_Logging);
    TESTMETHOD(Test_System_Memory);
    TESTMETHOD(Test_System_Containers_StringHashMap);
    TESTMETHOD(Test_System_Containers_SortedVector);
    
    // Math Tests
    TESTMETHOD(Test_Math_Randoms);
    TESTMETHOD(Test_Math_Vectors);
    TESTMETHOD(Test_Math_Distance2D);
    TESTMETHOD(Test_Math_Intersection2D);
    TESTMETHOD(Test_Math_Distance3D);
    TESTMETHOD(Test_Math_Intersection3D);
    
    #undef TESTMETHOD
   
};
