/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Includes
#include "BasePCH.h"
#include "UnitTestsModule.h"

// ***********************************************************************
class MemTest
{
public:
	MemTest() {i=0;}
	MemTest(int a) {i=a;}
	int i;
};

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
	LOG_INFO(L"Thread ID: %d  | arg = %d | From thread()", threadId, i);
    Thread::Sleep(100);
	return (void*) i;
}

Event *event1 = NULL;

// ***********************************************************************
void *EventTestFunction(void * arg)
{
	LOG_INFO(L"EventTestFunction():  Pre-event");

	event1->Wait();

	LOG_INFO(L"EventTestFunction():  Wait 1 finished!");

	event1->Wait();

	LOG_INFO(L"EventTestFunction():  Wait 2 finished!");

	return 0;
}


// ***********************************************************************
void UnitTestsModule::TestSystem()
{
	Clear();
	WriteLine(LogLevel::System, L"Tests Starting");

	// ===================================================================
	// Test Logging
	{
		WriteLine(LogLevel::Info, L"Running: Logging Tests");

		// Enable verbose logging
		Log::MaxLevel = LogLevel::Verbose;

		// Write some logs
		LOG_VERBOSE(L"Verbose Log Test");
		LOG_INFO(L"Informational Log Test");
		LOG_WARN(L"Warning Log Test");
		LOG_ERROR(L"Error Log Test");

		// Test log formatting
		LOG_INFO(L"Log Format Test: str[%ls] int[%d] float[%f]", L"ABC", 123, 3.14f);

		// Test lower log levels
		Log::MaxLevel = LogLevel::Warning;
		LOG_VERBOSE(L"TEST FAIL - Loglevel pruning isnt working if you see this");
		LOG_INFO(L"TEST FAIL - Loglevel pruning isnt working if you see this");

		Log::MaxLevel = LogLevel::Verbose;

		// ===================================================================
		// Test Asserts

		int i = 0;

		// Test a valid assertion
		ASSERT(i == 0, L"TEST FAIL - Assertions are broken if you see this");

		// Test a failing assertion
		//ASSERT(i == 1, L"Plain assertion  (Press NO to continue testing)");
		//ASSERT(i == 1, L"Assertion with formating: str[%ls] int[%d] float[%f]", L"ABC", 123, 3.14f);

	}
	// ===================================================================
	// Test Memory
	{
		WriteLine(LogLevel::Info, L"Running: Memory Tests");

		// Test Single object / default constructor
		MemTest *memTest1 = GdkNew MemTest(1);
		ASSERT(memTest1 != NULL, L"TEST FAIL: Failed to create single instance");
		memTest1->i = 1;
		GdkDelete(memTest1);

		// Test: Single object, parameterized contructor
		MemTest *memTest2 = GdkNew MemTest(1);
		ASSERT(memTest2 != NULL, L"TEST FAIL: Failed to create single instance (parametric constructor)");
		memTest2->i = 2;
		GdkDelete(memTest2);

		// Test 1D object array
		MemTest *memTest3 = GdkNew1DArray<MemTest>(10);
		ASSERT(memTest3 != NULL, L"TEST FAIL: Failed to 1D array");
		memTest3[0].i = 321;
		memTest3[1].i = 123;
		GdkDelete1DArray(memTest3);

		// Test 2D object array
		MemTest **memTest4 = GdkNew2DArray<MemTest>(2, 3);
		ASSERT(memTest4 != NULL, L"TEST FAIL: Failed to 2D array");
		memTest4[0][0].i = 123;
		memTest4[1][2].i = 321;
		GdkDelete2DArray(memTest4);

		// Test Alloc / Free
		void* memTest5 = GdkAlloc(100);
		ASSERT(memTest5 != NULL, L"TEST FAIL: Failed to allocate 100 byte buffer");
		GdkFree(memTest5);
	}
	// ================================================================
	// Test StringHashmap<>
	{
		WriteLine(LogLevel::Info, L"Running: StringHashMap Tests");

		StringHashMap<int> hashMap;
		hashMap.Add("Foo", 3);
		hashMap.Add("Bar", 1);
		hashMap.Add("Gil", 4);
		hashMap.Add("Baz", 2);
		
		// Find
		StringHashMap<int>::Iterator iter1 = hashMap.Find("Gil");
		StringHashMap<int>::Iterator iter2 = hashMap.Find("Poo");
		ASSERT(iter1 != hashMap.End(), L"Failed to find an existing string hash");
		ASSERT(iter2 == hashMap.End(), L"Failed to not-find a missing string hash");

		// Iteration
		for(StringHashMap<int>::Iterator iter = hashMap.Begin(); iter != hashMap.End(); iter++)
		{
			UInt32 hash = iter->first;
			int val = iter->second;
		}

		// Removal
		hashMap.Remove("Baz");
		StringHashMap<int>::Iterator removalIter = hashMap.Find("Foo");
		hashMap.Remove(removalIter);
		
	}
	// ================================================================
	// Test SortedVector<>
	{
		WriteLine(LogLevel::Info, L"Running: SortedVector Tests");

		SortedVector<int> sv;

		sv.AddSorted(12);
		sv.AddSorted(4);
		sv.AddSorted(2);
		sv.AddSorted(7);
		sv.AddSorted(15);

		for(SortedVector<int>::iterator i = sv.begin(); i < sv.end(); i++)
		{
			int val = *i;
		}

		ASSERT(sv.front() == 2, L"Sorted vector is NOT sorted");
		ASSERT(sv.back() == 15, L"Sorted vector is NOT sorted");
	}

	// ================================================================
	{
		WriteLine(LogLevel::Info, L"Running: Threading Tests");

		// Simple thread test
		// ---------------------

		LOG_INFO(L"-- Test: Simple Thread");
		int thread1Input = 10;
		Thread thread1 = Thread::Create(ThreadTestFunction, (void*)thread1Input, false);
		UInt32 thread1Id = thread1.GetId();
		LOG_INFO(L"Thread ID: %d  | From main()", thread1Id);
		int thread1Output = (int) thread1.Join();
		ASSERT(thread1Input == thread1Output, L"thread input != output");

		// Event test
		// ---------------------

		LOG_INFO(L"-- Test: Simple Event");
		event1 = Event::Create();
		LOG_INFO(L"Creating thread");
		Thread thread2 = Thread::Create(EventTestFunction);

		LOG_INFO(L"Sleeping 100");
        Thread::Sleep(100);
		LOG_INFO(L"Signalling event");
		event1->Set();
        Thread::Sleep(10);

		LOG_INFO(L"Sleeping 100");
        Thread::Sleep(100);
		LOG_INFO(L"Signalling event");
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
	

	WriteLine(LogLevel::System, L"Tests Complete");

}
