/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


//
// DelegateX:			Encapsulation class for a function point or class method pointer
// MulticastDelegateX:  Encapsulates binding of multiple delegates
//
// Usage (Delegate):
//
//	    class Foo
//	    { 
//	    	string SomeMethod(int i) {return string("ima method ") + i;}
//	    };
//
//      string SomeFunction(int i) {return string("ima function ") + i;}
//      
//
//	    typedef Gdk::Delegate1<string, int> SimpleDelegate;
//
//      // Member Method
//      Foo instance;
//	    SimpleDelegate *myDelegate1 = SimpleDelegate::FromMethod(&instance, &Foo::SomeMethod);
//
//      // Function
//      SimpleDelegate *myDelegate2 = SimpleDelegate::FromFunction(&SomeFunction);
//
//      string result = myDelegate1->Invoke(123);
//      string result = myDelegate2->Invoke(123);
//
// Usage (MulticastDelegate):
//
//		// Define a Mulicast Delegate Type
//      typedef Gdk::MulticastDelegate1<bool, int> SimpleMCDelegate;
//
//		// Define an Event (with the MC delegate)
//      SimpleMCDelegate MyEvent;
//
//		// Bind a handler to the event
//		MyEvent.AddHandlerMethod(&foo, &Foo::Method);
//		MyEvent.AddHandlerFunction(&Func);
//
//		// Invoke the MC delegate
//      MyEvent.Invoke(2);      // Call all the handlers
//
//		// Un-Bind a handler
//		MyEvent.RemoveHandlerMethod(&foo, &Foo::Method);
//		MyEvent.RemoveHandlerFunction(&Func);
//

#pragma once


namespace Gdk
{
	// =========================================

	namespace DelegateType
	{
		enum Enum
		{
			Function,
			Method
		};
	}

	// ===============================================================================================================================================

	// This is some very dirty code to convert a generic function ptr to a UInt64 address value..  It's directly sub-verting the C++ type system..

	// This templated union will map any type to a UInt64
	template<typename T>
	union UnionTypeToUInt64
	{
		T Input;
		UInt64 Output;
	};

	template<typename T>
	UInt64 ConvertTypeToUInt64(T t)
	{
		UnionTypeToUInt64<T> converter;
		converter.Output = 0;
		converter.Input = t;
		return converter.Output;
	}

	// 0-Parameters
    // ===============================================================================================================================================

    template<typename TClass, typename TReturn> class DelegateMethod0;
    template<typename TReturn> class DelegateFunction0;

    // Delegate interface
    // --------------------------------------

    template<typename TReturn>
    class Delegate0
    {
    public:
        // Invokation Methods
		virtual TReturn operator()() = 0;
	    virtual TReturn Invoke() = 0;

	    // FromMethod
	    template<typename TClass>
	    static Delegate0<TReturn>* FromMethod(TClass *instance, TReturn (TClass::*method)())
	    {
		    return GdkNew DelegateMethod0<TClass, TReturn>(instance, method);
	    }

	    // FromFunction
	    static Delegate0<TReturn>* FromFunction(TReturn (*function)())
	    {
		    return GdkNew DelegateFunction0<TReturn>(function);
	    }

		virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress() { return 0; }
		virtual void* GetInstance() { return NULL; }
    };

    // Method Delegate implementation
    // --------------------------------------

    template <typename TClass, typename TReturn>
    class DelegateMethod0 : public Delegate0<TReturn>
    {
    private:
	    TReturn (TClass::*Method)();
	    TClass* Instance;

    public:
	    // Constructor
	    explicit DelegateMethod0(TClass *instance, TReturn (TClass::*method)())
	    {
		    Instance = instance;
		    Method = method;
	    }
	
	    // operator()
	    virtual TReturn operator()()
	    {
		    return (*Instance.*Method)();
	    }

	    // Invoke()
	    virtual TReturn Invoke()
	    {
		    return (*Instance.*Method)();
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
    };

    // Function Delegate implementation
    // --------------------------------------

    template <typename TReturn>
    class DelegateFunction0 : public Delegate0<TReturn>
    {
    private:
	    TReturn (*Function)();

    public:
	    // Constructor
	    explicit DelegateFunction0(TReturn (*function)())
	    {
		    Function = function;
	    }
	
	    // operator()
	    virtual TReturn operator()()
	    {
		    return (*Function)();
	    }

	    // Invoke()
	    virtual TReturn Invoke()
	    {
		    return (*Function)();
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
    };

    // Event
    // --------------------------------------

    template<typename TReturn>
    class MulticastDelegate0
    {
    public:

        // Types
        typedef Delegate0<TReturn>                  Delegate;
        typedef vector<Delegate*>                   DelegateVector;
        typedef typename DelegateVector::iterator   DelegateVectorIterator;

        // CTor
        MulticastDelegate0()
        {
        }

        // DTor
        ~MulticastDelegate0()
        {
            Clear(true);
        }

        // Add a new handler to the event
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // Removes a handler from the event  (The caller is thusly responsible for calling GdkDelete on the delegate)
        void RemoveHandler(Delegate* handler)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // Clears all the handlers from the event (and optionally deletes all the handlers : default=true)
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // Gets the number of handlers in this event
        int Count()
        {
            return delegates.size();
        }

        // Gets a handler at the specified index
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

		// Add a function as a new handler to the event
        Delegate* AddHandlerFunction(TReturn (*function)())
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

		// Add a method as a new handler to the event
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)())
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }

		// Removes a handler to the given function
        void RemoveHandlerFunction(TReturn (*function)())
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the function delegate to remove?
				Delegate* del = (*iter);
				if(del->GetType() == DelegateType::Function && del->GetFunctionAddress() == ConvertTypeToUInt64(function))
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

		// Removes a handler to the given method
		template<class TClass>
        void RemoveHandlerMethod(TClass *instance, TReturn (TClass::*method)())
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the method delegate to remove?
				Delegate* del = (*iter);
				if(del->GetType() == DelegateType::Method && del->GetInstance() == instance && del->GetFunctionAddress() == ConvertTypeToUInt64(method))
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

        // Invokes all the delegates in the event
        void Invoke()
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke();
        }

    private:
        // Internal Properties
        DelegateVector delegates;
    };



    // 1-Parameter
    // ===============================================================================================================================================

    template<typename TClass, typename TReturn, typename TParam1> class DelegateMethod1;
    template<typename TReturn, typename TParam1> class DelegateFunction1;

    // Delegate interface
    // --------------------------------------

    template<typename TReturn, typename TParam1>
    class Delegate1
    {
    public:
        // Invokation Methods
	    virtual TReturn operator()(TParam1 param1) = 0;
	    virtual TReturn Invoke(TParam1 param1) = 0;

	    // FromMethod
	    template<typename TClass>
	    static Delegate1<TReturn, TParam1>* FromMethod(TClass *instance, TReturn (TClass::*method)(TParam1))
	    {
		    return GdkNew DelegateMethod1<TClass, TReturn, TParam1>(instance, method);
	    }

	    // FromFunction
	    static Delegate1<TReturn, TParam1>* FromFunction(TReturn (*function)(TParam1))
	    {
		    return GdkNew DelegateFunction1<TReturn, TParam1>(function);
	    }

		virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress()	{ return 0;}
		virtual void* GetInstance()	{ return NULL;}
    };

    // Method Delegate implementation
    // --------------------------------------

    template <typename TClass, typename TReturn, typename TParam1>
    class DelegateMethod1 : public Delegate1<TReturn, TParam1>
    {
    private:
	    TReturn (TClass::*Method)(TParam1);
	    TClass* Instance;

    public:
	    // Constructor
	    explicit DelegateMethod1(TClass *instance, TReturn (TClass::*method)(TParam1))
	    {
		    Instance = instance;
		    Method = method;
	    }
	
	    // operator()
	    virtual TReturn operator()(TParam1 param1)
	    {
		    return (*Instance.*Method)(param1);
	    }

	    // Invoke()
	    virtual TReturn Invoke(TParam1 param1)
	    {
		    return (*Instance.*Method)(param1);
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
    };

    // Function Delegate implementation
    // --------------------------------------

    template <typename TReturn, typename TParam1>
    class DelegateFunction1 : public Delegate1<TReturn, TParam1>
    {
    private:
	    TReturn (*Function)(TParam1);

    public:
	    // Constructor
	    explicit DelegateFunction1(TReturn (*function)(TParam1))
	    {
		    Function = function;
	    }
	
	    // operator()
	    virtual TReturn operator()(TParam1 param1)
	    {
		    return (*Function)(param1);
	    }

	    // Invoke()
	    virtual TReturn Invoke(TParam1 param1)
	    {
		    return (*Function)(param1);
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
    };

    // Event
    // --------------------------------------

    template<class TReturn, class TParam1>
    class MulticastDelegate1
    {
    public:

        // Types
        typedef Delegate1<TReturn, TParam1>         Delegate;
        typedef vector<Delegate*>                   DelegateVector;
        typedef typename DelegateVector::iterator   DelegateVectorIterator;

        // CTor
        MulticastDelegate1()
        {
			int i = 0;
			if(i == 0)
			{
				printf("i");
			}
        }

        // DTor
        ~MulticastDelegate1()
        {
            Clear(true);
        }

        // Add a new handler to the event
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // Removes a handler from the event  (The caller is thusly responsible for calling GdkDelete on the delegate)
        void RemoveHandler(Delegate* handler)
        {
            
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // Clears all the handlers from the event (and optionally deletes all the handlers : default=true)
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // Gets the number of handlers in this event
        int Count()
        {
            return delegates.size();
        }

        // Gets a handler at the specified index
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

		// Add a function as a new handler to the event
        Delegate* AddHandlerFunction(TReturn (*function)(TParam1))
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

		// Add a method as a new handler to the event
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1))
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }

		// Removes a handler to the given function
        void RemoveHandlerFunction(TReturn (*function)(TParam1))
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the function delegate to remove?
				Delegate* del = (*iter);
				if(del->GetType() == DelegateType::Function && del->GetFunctionAddress() == ConvertTypeToUInt64(function))
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

		// Removes a handler to the given method
		template<class TClass>
        void RemoveHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1))
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the method delegate to remove?
				Delegate* del = (*iter);
				TClass* delInst = (TClass*) del->GetInstance();
				UInt64 delMethodAddress = del->GetFunctionAddress();
				UInt64 methodAddress = ConvertTypeToUInt64(method);
				if(del->GetType() == DelegateType::Method && delInst == instance && delMethodAddress == methodAddress)
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

        // Invokes all the delegates in the event
        void Invoke(TParam1 param1)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke(param1);
        }

    private:
        // Internal Properties
        DelegateVector delegates;
    };



    // 2-Parameters
    // ===============================================================================================================================================

    template<typename TClass, typename TReturn, typename TParam1, typename TParam2> class DelegateMethod2;
    template<typename TReturn, typename TParam1, typename TParam2> class DelegateFunction2;

    // Delegate interface
    // --------------------------------------

    template<typename TReturn, typename TParam1, typename TParam2>
    class Delegate2
    {
    public:
	    virtual TReturn operator()(TParam1 param1, TParam2 param2) = 0;
	    virtual TReturn Invoke(TParam1 param1, TParam2 param2) = 0;

    public:
	
	    // FromMethod
	    template<typename TClass>
	    static Delegate2<TReturn, TParam1, TParam2>* FromMethod(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
	    {
		    return GdkNew DelegateMethod2<TClass, TReturn, TParam1, TParam2>(instance, method);
	    }

	    // FromFunction
	    static Delegate2<TReturn, TParam1, TParam2>* FromFunction(TReturn (*function)(TParam1, TParam2))
	    {
		    return GdkNew DelegateFunction2<TReturn, TParam1, TParam2>(function);
	    }

		virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress() { return 0; }
		virtual void* GetInstance()	{ return NULL;}
    };

    // Method Delegate implementation
    // --------------------------------------

    template <typename TClass, typename TReturn, typename TParam1, typename TParam2>
    class DelegateMethod2 : public Delegate2<TReturn, TParam1, TParam2>
    {
    private:
	    TReturn (TClass::*Method)(TParam1, TParam2);
	    TClass* Instance;

    public:
	    // Constructor
	    explicit DelegateMethod2(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
	    {
		    Instance = instance;
		    Method = method;
	    }
	
	    // operator()
	    virtual TReturn operator()(TParam1 param1, TParam2 param2)
	    {
		    return (*Instance.*Method)(param1, param2);
	    }

	    // Invoke()
	    virtual TReturn Invoke(TParam1 param1, TParam2 param2)
	    {
		    return (*Instance.*Method)(param1, param2);
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
    };

    // Function Delegate implementation
    // --------------------------------------

    template <typename TReturn, typename TParam1, typename TParam2>
    class DelegateFunction2 : public Delegate2<TReturn, TParam1, TParam2>
    {
    private:
	    TReturn (*Function)(TParam1, TParam2);

    public:
	    // Constructor
	    explicit DelegateFunction2(TReturn (*function)(TParam1, TParam2))
	    {
		    Function = function;
	    }
	
	    // operator()
	    virtual TReturn operator()(TParam1 param1, TParam2 param2)
	    {
		    return (*Function)(param1, param2);
	    }

	    // Invoke()
	    virtual TReturn Invoke(TParam1 param1, TParam2 param2)
	    {
		    return (*Function)(param1, param2);
	    }

		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
    };

    // Event
    // --------------------------------------

    template<typename TReturn, typename TParam1, typename TParam2>
    class MulticastDelegate2
    {
    public:

        // Types
        typedef Delegate2<TReturn, TParam1, TParam2>    Delegate;
        typedef vector<Delegate*>                       DelegateVector;
        typedef typename DelegateVector::iterator       DelegateVectorIterator;

        // CTor
        MulticastDelegate2()
        {
        }

        // DTor
        ~MulticastDelegate2()
        {
            Clear(true);
        }

        // Add a new handler to the event
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // Removes a handler from the event
        void RemoveHandler(Delegate* handler)
        {
            
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // Clears all the handlers from the event (and optionally deletes all the handlers : default=true)
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // Gets the number of handlers in this event
        int Count()
        {
            return delegates.size();
        }

        // Gets a handler at the specified index
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

		// Add a function as a new handler to the event
        Delegate* AddHandlerFunction(TReturn (*function)(TParam1, TParam2))
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

		// Add a method as a new handler to the event
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }

		// Removes a handler to the given function
        void RemoveHandlerFunction(TReturn (*function)(TParam1, TParam2))
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the function delegate to remove?
				Delegate* del = (*iter);
				if(del->GetType() == DelegateType::Function && del->GetFunctionAddress() == ConvertTypeToUInt64(function))
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

		// Removes a handler to the given method
		template<class TClass>
        void RemoveHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
        {
			// Loop through the handlers
			for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
			{
				// Is this the method delegate to remove?
				Delegate* del = (*iter);
				TClass* delInst = (TClass*) del->GetInstance();
				UInt64 delMethodAddress = del->GetFunctionAddress();
				UInt64 methodAddress = ConvertTypeToUInt64(method);
				if(del->GetType() == DelegateType::Method && delInst == instance && delMethodAddress == methodAddress)
				{
					// Erase & Delete the delegate
					delegates.erase(iter);
					GdkDelete(del);
					return;
				}
			}
        }

        // Invokes all the delegates in the event
        void Invoke(TParam1 param1, TParam2 param2)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke(param1, param2);
        }

    private:
        // Internal Properties
        DelegateVector delegates;
    };

    // ===============================================================================================================================================

}// namespace Gdk
