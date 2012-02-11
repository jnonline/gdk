/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Includes
#include "BasePCH.h"
#include "UnitTestsModule.h"
 

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_System_Logging(TestExecutionContext *context)
{
    context->Log->WriteLine(LogLevel::Info, "Writing to the GDK log");
    
    // Enable verbose logging
    Log::MaxLevel = LogLevel::Verbose;
    
    // Write some logs
    LOG_VERBOSE("Verbose Log Test");
    LOG_INFO("Informational Log Test");
    LOG_WARN("Warning Log Test");
    LOG_ERROR("Error Log Test");
    
    // Test log formatting
    LOG_INFO("Log Format Test: str[%s] int[%d] float[%f]", "ABC", 123, 3.14f);
    
    // Test lower log levels
    Log::MaxLevel = LogLevel::Warning;
    LOG_VERBOSE("TEST FAIL - Loglevel pruning isnt working if you see this");
    LOG_INFO("TEST FAIL - Loglevel pruning isnt working if you see this");
    
    Log::MaxLevel = LogLevel::Verbose;
    
    // ===================================================================
    // Test Asserts
    
    int i = 0;
    
    // Test a valid assertion
    ASSERT(i == 0, "TEST FAIL - Assertions are broken if you see this");
    
    // Test a failing assertion
    //ASSERT(i == 1, "Plain assertion  (Press NO to continue testing)");
    //ASSERT(i == 1, "Assertion with formating: str[%s] int[%d] float[%f]", "ABC", 123, 3.14f);
    
    return TestStatus::Pass;
}


// ***********************************************************************
class MemTest
{
public:
	MemTest() {i=0;}
	MemTest(int a) {i=a;}
	int i;
};

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_System_Memory(TestExecutionContext *context)
{
    // Test Single object / default constructor
    MemTest *memTest1 = GdkNew MemTest();
    UNIT_TEST_CHECK(memTest1 != NULL, "Single object, default constructor");
    memTest1->i = 1;
    GdkDelete(memTest1);
    
    // Test: Single object, parameterized constructor
    context->Log->WriteLine(LogLevel::Info, "Single object, parameterized constructor");
    MemTest *memTest2 = GdkNew MemTest(1);
    UNIT_TEST_CHECK(memTest2 != NULL, "Single object, parameterized constructor");
    memTest2->i = 2;
    GdkDelete(memTest2);
    
    // Test 1D object array
    MemTest *memTest3 = GdkNew1DArray<MemTest>(10);
    UNIT_TEST_CHECK(memTest3 != NULL, "1D object array");
    memTest3[0].i = 321;
    memTest3[1].i = 123;
    GdkDelete1DArray(memTest3);
    
    // Test 2D object array
    MemTest **memTest4 = GdkNew2DArray<MemTest>(2, 3);
    UNIT_TEST_CHECK(memTest4 != NULL, "2D object array");
    memTest4[0][0].i = 123;
    memTest4[1][2].i = 321;
    GdkDelete2DArray(memTest4);
    
    // Test Alloc / Free
    void* memTest5 = GdkAlloc(100);
    UNIT_TEST_CHECK(memTest5 != NULL, "GdkAlloc / GdkFree on 100 byte buffer");
    GdkFree(memTest5);
    
    return TestStatus::Pass;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_System_Containers_StringHashMap(TestExecutionContext *context)
{
    context->Log->WriteLine(LogLevel::Info, "Creating StringHashMap");
    StringHashMap<int> hashMap;
    hashMap.Add("Foo", 3);
    hashMap.Add("Bar", 1);
    hashMap.Add("Gi", 4);
    hashMap.Add("Baz", 2);
    
    // Find
    StringHashMap<int>::Iterator iter1 = hashMap.Find("Gi");
    StringHashMap<int>::Iterator iter2 = hashMap.Find("Poo");
    UNIT_TEST_CHECK(iter1 != hashMap.End(), "Find() existing value");
    UNIT_TEST_CHECK(iter2 == hashMap.End(), "Find() a missing string hash");
    
    // Iteration
    context->Log->WriteLine(LogLevel::Info, "Testing Iteration");
    for(StringHashMap<int>::Iterator iter = hashMap.Begin(); iter != hashMap.End(); iter++)
    {
        UInt32 hash = iter->first;
        int val = iter->second;
        
        GDK_NOT_USED(hash);
        GDK_NOT_USED(val);
    }
    
    // Removal
    context->Log->WriteLine(LogLevel::Info, "Testing Remove() method");
    hashMap.Remove("Baz");
    StringHashMap<int>::Iterator removalIter = hashMap.Find("Foo");
    hashMap.Remove(removalIter);
    
    return TestStatus::Pass;
}

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_System_Containers_SortedVector(TestExecutionContext *context)
{
    context->Log->WriteLine(LogLevel::Info, "Creating SortedVector");
    
    SortedVector<int> sv;
    
    sv.AddSorted(12);
    sv.AddSorted(4);
    sv.AddSorted(2);
    sv.AddSorted(7);
    sv.AddSorted(15);
    
    context->Log->WriteLine(LogLevel::Info, "Testing iteration");
    for(SortedVector<int>::iterator i = sv.begin(); i < sv.end(); i++)
    {
        int val = *i;
        
        GDK_NOT_USED(val);
    }
    
    UNIT_TEST_CHECK(sv.front() == 2, "Sorted vector front()");
    UNIT_TEST_CHECK(sv.back() == 15, "Sorted vector back()");
    
    return TestStatus::Pass;
}

// ##############################################################################################
// ##############################################################################################

// ***********************************************************************
struct TestWorkItem
{
public:
    int Value;
    TestWorkItem() {}
    TestWorkItem(int value) { Value = value; }
};


// ***********************************************************************
class TestWorkQueue : public ThreadedWorkQueue<TestWorkItem*>
{
public:
    TestWorkQueue() : ThreadedWorkQueue<TestWorkItem*>(4)
    {
    }
    
protected:
    virtual void OnProcessWorkItem(TestWorkItem* item)
    {
        item->Value++;
        Thread::Sleep(1);
    }
};

// ***********************************************************************
TestStatus::Enum UnitTestsModule::Test_System_Threading_ThreadedWorkQueue(TestExecutionContext *context)
{
    // Create a bunch of work items
    TestWorkItem items[1000];
    for(int i=0; i<1000; i++)
        items[i].Value = i;
    
    context->Log->WriteLine(LogLevel::Info, "Creating ThreadedWorkQueue");
    
    // Create a work queue
    TestWorkQueue* workQueue = GdkNew TestWorkQueue();
    
    context->Log->WriteLine(LogLevel::Info, "Loading queue with 1000 work items");
    
    // Loop through the work items
    for(int i=0; i<1000; i++)
    {
        // Queue up the work items (on cycling priorities)
        workQueue->Enqueue(&items[i], i % 8);
        
        // Every Nth queued item, sleep a bit
        if(i % 100 == 0)
            Thread::Sleep(5);
    }
    
    context->Log->WriteLine(LogLevel::Info, "Waiting for work Queue to finish");
    
    // Wait for the queue to finish
    while(workQueue->GetQueueCount() > 0)
        Thread::Sleep(5);

    context->Log->WriteLine(LogLevel::Info, "Verifying work");

    // Verify the work
    int numBadItems = 0;
    for(int i=0; i<1000; i++)
    {
        if(items[i].Value != i+1)
        {
            numBadItems++;
        }
    }
    
    if(numBadItems > 0)
        context->Log->WriteLine(LogLevel::Error, "%d of 1000 work items failed to be processed correctly!!", numBadItems);
    
    context->Log->WriteLine(LogLevel::Info, "Shutdown down work queue");
    
    // Shutdown & Delete the work queue
    GdkDelete(workQueue);
    
    if(numBadItems > 0)
        return TestStatus::Fail;
    return TestStatus::Pass;
}


/*

// ***********************************************************************
class ThreadTestClass
{
public:
	int i;
	ThreadTestClass(int i) { this->i = i;}
	void* ThreadTestMethod(void *arg)
	{
		int i2 = (int)arg + i;
        Thread::Sleep(100);
		return (void*)i2;
	}
};

// ***********************************************************************
void *ThreadTestFunction(void * arg)
{
	int i = (int) arg;
	UInt32 threadId = Thread::GetCurrentThreadId();
	LOG_INFO("Thread ID: %d  | arg = %d | From thread()", threadId, i);
    Thread::Sleep(100);
	return (void*) i;
}

Event *event1 = NULL;

// ***********************************************************************
void *EventTestFunction(void * arg)
{
	LOG_INFO("EventTestFunction():  Pre-event");

	event1->Wait();

	LOG_INFO("EventTestFunction():  Wait 1 finished!");

	event1->Wait();

	LOG_INFO("EventTestFunction():  Wait 2 finished!");

	return 0;
}

	// ================================================================
	{
		WriteLine(LogLevel::Info, "Running: Threading Tests");

		// Simple thread test
		// ---------------------

		LOG_INFO("-- Test: Simple Thread");
		int thread1Input = 10;
		Thread thread1 = Thread::Create(ThreadTestFunction, (void*)thread1Input, false);
		UInt32 thread1Id = thread1.GetId();
		LOG_INFO("Thread ID: %d  | From main()", thread1Id);
		int thread1Output = (int) thread1.Join();
		ASSERT(thread1Input == thread1Output, "thread input != output");

		// Event test
		// ---------------------

		LOG_INFO("-- Test: Simple Event");
		event1 = Event::Create();
		LOG_INFO("Creating thread");
		Thread thread2 = Thread::Create(EventTestFunction);

		LOG_INFO("Sleeping 100");
        Thread::Sleep(100);
		LOG_INFO("Signalling event");
		event1->Set();
        Thread::Sleep(10);

		LOG_INFO("Sleeping 100");
        Thread::Sleep(100);
		LOG_INFO("Signalling event");
		event1->Set();

		thread2.Join();
		GdkDelete(event1);

		// ---------------------

		//ThreadTest testObj(10);
		//Thread::Delegate* dele = Thread::Delegate::FromMethod(&testObj, &ThreadTest::Func);
		//Thread dThread = Thread::Create(dele, (void*)2, false);

		//void* dResult = dThread.Join();
		//int dIntResult = (int)dResult;
			
		//GdkDelete( dele );
	}
	

	WriteLine(LogLevel::System, "Tests Complete");

}
 */




