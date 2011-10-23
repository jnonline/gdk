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
    /// @addtogroup System
    /// @{
    
    /// @cond INTERNAL
    
    // =================================================================================
    ///	@brief
    ///		Types of delegate handlers
    /// @note 
    ///     GDK Internal Use Only
    // =================================================================================
	namespace DelegateType
	{
		enum Enum
		{
            /// Function or static class method
			Function,
            
            /// Instance class method
			Method
		};
	}

    // =================================================================================
    ///	@brief
    ///		Utility union that maps a template type to a UInt64
    /// @remarks
    ///     This is some very dirty code to convert a generic function ptr to a UInt64 
    ///     address value..  It's directly sub-verting the C++ type system..
    /// @param T
    ///     The Type to map to a UInt64
    /// @note
    ///     GDK Internal Use Only
    // =================================================================================
	template<typename T>
	union UnionTypeToUInt64
	{
        /// An instance of the type T
		T Input;
        
        /// UInt64 that shares the same memory space as the Input
		UInt64 Output;
	};

    // =================================================================================
    ///	@brief
    ///		Utility function that converts an instance of templated type to a UInt64
    /// @remarks
    ///     This is some very dirty code to convert a generic function ptr to a UInt64 
    ///     address value..  It's directly sub-verting the C++ type system..
    /// @param T
    ///     The Type to map to a UInt64
    /// @param t
    ///     The instance of type T to be converted
    /// @note
    ///     GDK Internal Use Only
    // =================================================================================
	template<typename T>
	UInt64 ConvertTypeToUInt64(T t)
	{
		UnionTypeToUInt64<T> converter;
		converter.Output = 0;
		converter.Input = t;
		return converter.Output;
	}
    
    /// @endcond

    // Template Prototypes
    
    template<typename TReturn> class Delegate0;
    template<typename TClass, typename TReturn> class DelegateMethod0;
    template<typename TReturn> class DelegateFunction0;
    template<typename TReturn> class MulticastDelegate0;

    template<typename TReturn, typename TParam1> class Delegate1;
    template<typename TClass, typename TReturn, typename TParam1> class DelegateMethod1;
    template<typename TReturn, typename TParam1> class DelegateFunction1;
    template<typename TReturn, typename TParam1> class MulticastDelegate1;
    
    template<typename TReturn, typename TParam1, typename TParam2> class Delegate2;
    template<typename TClass, typename TReturn, typename TParam1, typename TParam2> class DelegateMethod2;
    template<typename TReturn, typename TParam1, typename TParam2> class DelegateFunction2;
    template<typename TReturn, typename TParam1, typename TParam2> class MulticastDelegate2;
    
    
	// 0-Parameters
    // ===============================================================================================================================================

    // =================================================================================
    ///	@brief
    ///		A delegate with 0 parameters
    /// @remarks
    ///     Delegates are templated wrappers around a function or method with a matching 
    ///     signature, similar to .Net delegates.
    ///   @par
    ///     To create a delegate, use the FromMethod() or FromFunction() static methods.
    ///     To invoke a delegate, use either the Invoke() or the () operator directly.
    /// @param TReturn
    ///     The return Type of the delegate's signature
    // =================================================================================
    template<typename TReturn>
    class Delegate0
    {
    public:
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{

        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        // *****************************************************************
		virtual TReturn operator()() = 0;
        
        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        // *****************************************************************
	    virtual TReturn Invoke() = 0;

        // *****************************************************************
        /// @brief
        ///     Creates a delegate from a class instance method.
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name  needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
	    template<typename TClass>
	    static Delegate0<TReturn>* FromMethod(TClass *instance, TReturn (TClass::*method)())
	    {
		    return GdkNew DelegateMethod0<TClass, TReturn>(instance, method);
	    }

        // *****************************************************************
        /// @brief
        ///     Creates a delegate from a function or a static class method.
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
	    static Delegate0<TReturn>* FromFunction(TReturn (*function)())
	    {
		    return GdkNew DelegateFunction0<TReturn>(function);
	    }
        
        /// @}

    protected:
        
        // Protected Methods
		// =====================================================
        
        /// @cond INTERNAL
        
		virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress() { return 0; }
		virtual void* GetInstance() { return NULL; }
        
        /// @endcond
        
        friend class MulticastDelegate0<TReturn>;
    };

    /// @cond INTERNAL
    
    // =================================================================================
    ///	@brief
    ///		Binds a class instance method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TClass, typename TReturn>
    class DelegateMethod0 : public Delegate0<TReturn>
    {
    public:
        // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
        
    private:
        
        // Public Properties
		// =====================================================
        
	    TReturn (TClass::*Method)();
	    TClass* Instance;
    };

    // =================================================================================
    ///	@brief
    ///		Binds a function or static class method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TReturn>
    class DelegateFunction0 : public Delegate0<TReturn>
    {
    public:
        // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
        
    private:
        
        // Private Properties
		// =====================================================
        
	    TReturn (*Function)();
    };
    
    /// @endcond

    // =================================================================================
    ///	@brief
    ///		A multicast delegate with 0 parameters
    /// @remarks
    ///     A multicast delegate is collections of delegates that are all invoked in 
    ///     succession when the multicast delegate is invoked.  These are mostly useful
    ///     for creating events of a given signature for others to subscribe to.
    ///   @par
    ///     For the simplest usage:
    ///     To add a function or method to the multicast delegate, use the AddHandlerFunction() or AddHandlerMethod() utility methods.  
    ///     To remove an attached delegate, use the RemoveHandlerFunction() or RemoveHandlerMethod().
    ///     To invoke the multicast delegate, use Invoke()
    /// @param TReturn
    ///     The return Type of the multicast delegate's signature
    // =================================================================================
    template<typename TReturn>
    class MulticastDelegate0
    {
    public:

        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Types
        /// @{
        
        /// The Delegate type this multicast delegate uses.
        typedef Delegate0<TReturn>                  Delegate;
        
        /// A vector of Delegate instances
        typedef vector<Delegate*>                   DelegateVector;
        
        /// An iterator of the DelegateVector type.
        typedef typename DelegateVector::iterator   DelegateVectorIterator;

        /// @}
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Default constructor
        // *****************************************************************
        MulticastDelegate0()
        {
        }

        // *****************************************************************
        /// @brief
        ///     Default destructor
        // *****************************************************************
        ~MulticastDelegate0()
        {
            Clear(true);
        }

        // *****************************************************************
        /// @brief
        ///     Adds a handler delegate to this multicast delegate
        // *****************************************************************
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // *****************************************************************
        /// @brief
        ///     Removes a handler from this multicast delegate
        /// @remarks
        ///     The caller assumes responsibility for calling GdkDelete on the handler delegate
        // *****************************************************************
        void RemoveHandler(Delegate* handler)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // *****************************************************************
        /// @brief
        ///     Clears all the handlers from the multicast delegate
        /// @param deleteHandlers
        ///     If true (default) all the handlers will be deleted, otherwise the caller assumes 
        ///     responsibility for calling GdkDelete on the handler delegates
        // *****************************************************************
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the number of handler delegates bound to this multicast delegate
        // *****************************************************************
        int Count()
        {
            return delegates.size();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        
        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

        // *****************************************************************
        /// @brief
        ///     Adds a function or static class method as a handler to this multicast delegate
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
        Delegate* AddHandlerFunction(TReturn (*function)())
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

        // *****************************************************************
        /// @brief
        ///     Adds a class instance method as a handler to this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)())
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }
        
        // *****************************************************************
        /// @brief
        ///     Removes a function or static class method as a handler from this multicast delegate
        /// @param function
        ///     The function to be removed from the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
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

		// *****************************************************************
        /// @brief
        ///     Removes a class instance method as a handler from this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance
        /// @param method
        ///     The method to be removed from the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
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

        // *****************************************************************
        /// @brief
        ///     Invokes all the delegates in this multicast delegate
        // *****************************************************************
        void Invoke()
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke();
        }
        
        /// @}

    private:
        
        // Private Properties
		// =====================================================
        
        DelegateVector delegates;
    };



    // 1-Parameter
    // ===============================================================================================================================================

    // =================================================================================
    ///	@brief
    ///		A delegate with 1 parameter
    /// @remarks
    ///     Delegates are templated wrappers around a function or method with a matching 
    ///     signature, similar to .Net delegates.
    ///   @par
    ///     To create a delegate, use the FromMethod() or FromFunction() static methods.
    ///     To invoke a delegate, use either the Invoke() or the () operator directly.
    /// @param TReturn
    ///     The return type of the delegate's signature
    /// @param TParam1
    ///     The type of the delegate parameter
    // =================================================================================
    template<typename TReturn, typename TParam1>
    class Delegate1
    {
    public:
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        /// @param param1
        ///     The 1st parameter to pass to the delegate
        // *****************************************************************
	    virtual TReturn operator()(TParam1 param1) = 0;
        
        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        /// @param param1
        ///     The 1st parameter to pass to the delegate
        // *****************************************************************
	    virtual TReturn Invoke(TParam1 param1) = 0;

	    
        // *****************************************************************
        /// @brief
        ///     Creates a delegate from a class instance method.
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name  needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
	    template<typename TClass>
	    static Delegate1<TReturn, TParam1>* FromMethod(TClass *instance, TReturn (TClass::*method)(TParam1))
	    {
		    return GdkNew DelegateMethod1<TClass, TReturn, TParam1>(instance, method);
	    }

	    // *****************************************************************
        /// @brief
        ///     Creates a delegate from a function or a static class method.
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
	    static Delegate1<TReturn, TParam1>* FromFunction(TReturn (*function)(TParam1))
	    {
		    return GdkNew DelegateFunction1<TReturn, TParam1>(function);
	    }

        /// @}
        
    protected:
        
        // Protected Methods
		// =====================================================
        
        /// @cond INTERNAL
        
		virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress()	{ return 0;}
		virtual void* GetInstance()	{ return NULL;}
        
        /// @endcond
        
        friend class MulticastDelegate1<TReturn, TParam1>;
    };

    /// @cond INTERNAL
    
    // =================================================================================
    ///	@brief
    ///		Binds a class instance method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TClass, typename TReturn, typename TParam1>
    class DelegateMethod1 : public Delegate1<TReturn, TParam1>
    {
    public:
        // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
        
    private:
        
        // Public Properties
		// =====================================================
        
	    TReturn (TClass::*Method)(TParam1);
	    TClass* Instance;
    };

    // =================================================================================
    ///	@brief
    ///		Binds a function or static class method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TReturn, typename TParam1>
    class DelegateFunction1 : public Delegate1<TReturn, TParam1>
    {
    public:
	    // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
        
    private:
	    
        // Private Properties
		// =====================================================
        
        TReturn (*Function)(TParam1);
    };

    /// @endcond
    
    // =================================================================================
    ///	@brief
    ///		A multicast delegate with 1 parameter
    /// @remarks
    ///     A multicast delegate is collections of delegates that are all invoked in 
    ///     succession when the multicast delegate is invoked.  These are mostly useful
    ///     for creating events of a given signature for others to subscribe to.
    ///   @par
    ///     For the simplest usage:
    ///     To add a function or method to the multicast delegate, use the AddHandlerFunction() or AddHandlerMethod() utility methods.  
    ///     To remove an attached delegate, use the RemoveHandlerFunction() or RemoveHandlerMethod().
    ///     To invoke the multicast delegate, use Invoke()
    /// @param TReturn
    ///     The return type of the multicast delegate's signature
    /// @param TParam1
    ///     The type of the delegate parameter
    // =================================================================================
    template<class TReturn, class TParam1>
    class MulticastDelegate1
    {
    public:

        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Types
        /// @{

        /// The Delegate type this multicast delegate uses.
        typedef Delegate1<TReturn, TParam1>         Delegate;
        
        /// A vector of Delegate instances
        typedef vector<Delegate*>                   DelegateVector;
        
        /// An iterator of the DelegateVector type.
        typedef typename DelegateVector::iterator   DelegateVectorIterator;

        /// @}
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Default constructor
        // *****************************************************************
        MulticastDelegate1()
        {
        }

        // *****************************************************************
        /// @brief
        ///     Default destructor
        // *****************************************************************
        ~MulticastDelegate1()
        {
            Clear(true);
        }

        // *****************************************************************
        /// @brief
        ///     Adds a handler delegate to this multicast delegate
        // *****************************************************************
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // *****************************************************************
        /// @brief
        ///     Removes a handler from this multicast delegate
        /// @remarks
        ///     The caller assumes responsibility for calling GdkDelete on the handler delegate
        // *****************************************************************
        void RemoveHandler(Delegate* handler)
        {
            
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // *****************************************************************
        /// @brief
        ///     Clears all the handlers from the multicast delegate
        /// @param deleteHandlers
        ///     If true (default) all the handlers will be deleted, otherwise the caller assumes 
        ///     responsibility for calling GdkDelete on the handler delegates
        // *****************************************************************
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the number of handler delegates bound to this multicast delegate
        // *****************************************************************
        int Count()
        {
            return delegates.size();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        
        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

		// *****************************************************************
        /// @brief
        ///     Adds a function or static class method as a handler to this multicast delegate
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
        Delegate* AddHandlerFunction(TReturn (*function)(TParam1))
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

		// *****************************************************************
        /// @brief
        ///     Adds a class instance method as a handler to this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1))
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }

		// *****************************************************************
        /// @brief
        ///     Removes a function or static class method as a handler from this multicast delegate
        /// @param function
        ///     The function to be removed from the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
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

		// *****************************************************************
        /// @brief
        ///     Removes a class instance method as a handler from this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance
        /// @param method
        ///     The method to be removed from the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
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

        // *****************************************************************
        /// @brief
        ///     Invokes all the delegates in this multicast delegate
        /// @param param1
        ///     The parameter to pass to the handlers
        // *****************************************************************
        void Invoke(TParam1 param1)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke(param1);
        }

        /// @}
        
    private:
        
        // Private Properties
		// =====================================================
        
        DelegateVector delegates;
    };



    // 2-Parameters
    // ===============================================================================================================================================

    // =================================================================================
    ///	@brief
    ///		A delegate with 2 parameters
    /// @remarks
    ///     Delegates are templated wrappers around a function or method with a matching 
    ///     signature, similar to .Net delegates.
    ///   @par
    ///     To create a delegate, use the FromMethod() or FromFunction() static methods.
    ///     To invoke a delegate, use either the Invoke() or the () operator directly.
    /// @param TReturn
    ///     The return type of the delegate's signature
    /// @param TParam1
    ///     The type of the 1st delegate parameter
    /// @param TParam2
    ///     The type of the 2nd delegate parameter
    // =================================================================================
    template<typename TReturn, typename TParam1, typename TParam2>
    class Delegate2
    {
    public:
	    
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        /// @param param1
        ///     The 1st parameter to pass to the delegate
        /// @param param1
        ///     The 2nd parameter to pass to the delegate
        // *****************************************************************
	    virtual TReturn operator()(TParam1 param1, TParam2 param2) = 0;
        
        // *****************************************************************
        /// @brief
        ///     Invokes the delegate
        /// @param param1
        ///     The 1st parameter to pass to the delegate
        /// @param param1
        ///     The 2nd parameter to pass to the delegate
        // *****************************************************************
	    virtual TReturn Invoke(TParam1 param1, TParam2 param2) = 0;

        // *****************************************************************
        /// @brief
        ///     Creates a delegate from a class instance method.
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name  needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
	    template<typename TClass>
	    static Delegate2<TReturn, TParam1, TParam2>* FromMethod(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
	    {
		    return GdkNew DelegateMethod2<TClass, TReturn, TParam1, TParam2>(instance, method);
	    }

	    // *****************************************************************
        /// @brief
        ///     Creates a delegate from a function or a static class method.
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
	    static Delegate2<TReturn, TParam1, TParam2>* FromFunction(TReturn (*function)(TParam1, TParam2))
	    {
		    return GdkNew DelegateFunction2<TReturn, TParam1, TParam2>(function);
	    }

        /// @}
        
    protected:
        
        // Protected Methods
		// =====================================================
        
        /// @cond INTERNAL
		
        virtual DelegateType::Enum GetType() = 0;
		virtual UInt64 GetFunctionAddress() { return 0; }
		virtual void* GetInstance()	{ return NULL;}
        
        /// @endcond
        
        friend class MulticastDelegate2<TReturn, TParam1, TParam2>;
        
    };

    /// @cond INTERNAL
    
    // =================================================================================
    ///	@brief
    ///		Binds a class instance method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TClass, typename TReturn, typename TParam1, typename TParam2>
    class DelegateMethod2 : public Delegate2<TReturn, TParam1, TParam2>
    {
    public:
        // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Method; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Method); }
		virtual void* GetInstance() { return this->Instance; }
        
    private:
        
        // Public Properties
		// =====================================================
        

	    TReturn (TClass::*Method)(TParam1, TParam2);
	    TClass* Instance;
    };

    // =================================================================================
    ///	@brief
    ///		Binds a function or static class method to a delegate
    /// @remarks
    ///     GDK Internal Use Only
    // =================================================================================
    template <typename TReturn, typename TParam1, typename TParam2>
    class DelegateFunction2 : public Delegate2<TReturn, TParam1, TParam2>
    {
    public:
	    // Public Methods
		// =====================================================
        
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

    protected:
        
        // Protected Methods
		// =====================================================
        
		virtual DelegateType::Enum GetType() {return DelegateType::Function; }
		virtual UInt64 GetFunctionAddress() { return ConvertTypeToUInt64(this->Function); }
        
    private:
        
        // Private Properties
		// =====================================================
        
	    TReturn (*Function)(TParam1, TParam2);

    };

    /// @endcond
    
    // =================================================================================
    ///	@brief
    ///		A multicast delegate with 2 parameters
    /// @remarks
    ///     A multicast delegate is collections of delegates that are all invoked in 
    ///     succession when the multicast delegate is invoked.  These are mostly useful
    ///     for creating events of a given signature for others to subscribe to.
    ///   @par
    ///     For the simplest usage:
    ///     To add a function or method to the multicast delegate, use the AddHandlerFunction() or AddHandlerMethod() utility methods.  
    ///     To remove an attached delegate, use the RemoveHandlerFunction() or RemoveHandlerMethod().
    ///     To invoke the multicast delegate, use Invoke()
    /// @param TReturn
    ///     The return type of the multicast delegate's signature
    /// @param TParam1
    ///     The type of the 1st delegate parameter
    /// @param TParam2
    ///     The type of the 2nd delegate parameter
    // =================================================================================
    template<typename TReturn, typename TParam1, typename TParam2>
    class MulticastDelegate2
    {
    public:

        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Types
        /// @{
        
        /// The Delegate type this multicast delegate uses.
        typedef Delegate2<TReturn, TParam1, TParam2>    Delegate;
        
        /// A vector of Delegate instances
        typedef vector<Delegate*>                       DelegateVector;
        
        /// An iterator of the DelegateVector type.
        typedef typename DelegateVector::iterator       DelegateVectorIterator;

        /// @}
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Default constructor
        // *****************************************************************
        MulticastDelegate2()
        {
        }

        // *****************************************************************
        /// @brief
        ///     Default destructor
        // *****************************************************************
        ~MulticastDelegate2()
        {
            Clear(true);
        }

        // *****************************************************************
        /// @brief
        ///     Adds a handler delegate to this multicast delegate
        // *****************************************************************
        void AddHandler(Delegate* handler)
        {
            delegates.push_back(handler);
        }

        // *****************************************************************
        /// @brief
        ///     Removes a handler from this multicast delegate
        /// @remarks
        ///     The caller assumes responsibility for calling GdkDelete on the handler delegate
        // *****************************************************************
        void RemoveHandler(Delegate* handler)
        {
            
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                if(*iter == handler)
                {
                    delegates.erase(iter);
                    return;
                }
        }
        
        // *****************************************************************
        /// @brief
        ///     Clears all the handlers from the multicast delegate
        /// @param deleteHandlers
        ///     If true (default) all the handlers will be deleted, otherwise the caller assumes 
        ///     responsibility for calling GdkDelete on the handler delegates
        // *****************************************************************
        void Clear(bool deleteHandlers = true)
        {
            if(deleteHandlers)
                for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                    GdkDelete (*iter);

            delegates.clear();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the number of handler delegates bound to this multicast delegate
        // *****************************************************************
        int Count()
        {
            return delegates.size();
        }

        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* GetHandler(int index)
        {
            return delegates[index];
        }
        
        // *****************************************************************
        /// @brief
        ///     Gets the handler delegate at the specified index
        // *****************************************************************
        Delegate* operator[](int index)
	    {
		    return delegates[index];
	    }

		// *****************************************************************
        /// @brief
        ///     Adds a function or static class method as a handler to this multicast delegate
        /// @param function
        ///     The function to be invoked by the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
        Delegate* AddHandlerFunction(TReturn (*function)(TParam1, TParam2))
        {
			Delegate* del = Delegate::FromFunction(&function);
            AddHandler(del);
			return del;
        }

		// *****************************************************************
        /// @brief
        ///     Adds a class instance method as a handler to this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance to invoke the method on
        /// @param method
        ///     The method to be invoked by the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
		template<class TClass>
        Delegate* AddHandlerMethod(TClass *instance, TReturn (TClass::*method)(TParam1, TParam2))
        {
			Delegate* del = Delegate::FromMethod(instance, method);
            AddHandler(del);
			return del;
        }

		// *****************************************************************
        /// @brief
        ///     Removes a function or static class method as a handler from this multicast delegate
        /// @param function
        ///     The function to be removed from the delegate.
        ///     For static class methods, use the fully qualified name: (&TClass::Method)
        // *****************************************************************
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

		// *****************************************************************
        /// @brief
        ///     Removes a class instance method as a handler from this multicast delegate
        /// @param TClass
        ///     The type of the class that owns the instance method.
        /// @param instance
        ///     The class instance
        /// @param method
        ///     The method to be removed from the delegate.  
        ///     The method name needs to be fully qualified:  (&TClass::Method)
        // *****************************************************************
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

        // *****************************************************************
        /// @brief
        ///     Invokes all the delegates in this multicast delegate
        /// @param param1
        ///     The parameter to pass to the handlers
        // *****************************************************************
        void Invoke(TParam1 param1, TParam2 param2)
        {
            for(DelegateVectorIterator iter = delegates.begin(); iter != delegates.end(); iter++)
                (*iter)->Invoke(param1, param2);
        }
        /// @}
        
    private:
        
        // Private Properties
		// =====================================================
        
        DelegateVector delegates;
    };

    // ===============================================================================================================================================

    /// @}
    
}// namespace Gdk
