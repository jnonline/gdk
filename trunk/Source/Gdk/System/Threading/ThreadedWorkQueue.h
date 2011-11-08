/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

namespace Gdk
{
    /// @addtogroup System
    /// @{
    /// @addtogroup Threading
    /// @{
    
	// =================================================================================
    ///	@brief
    ///		Provides a queue of work items that are processed by background threads.
    /// @remarks
    ///     This template class gives you a queue of work items that you can enqueue to be
    ///     processed in a multi-threaded fashion.   Virtual methods are used to handle the
    ///     actual work item processing.
    // =================================================================================
    template<typename TWorkItem>
    class ThreadedWorkQueue
	{
    public:
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructor / Destructor
        /// @{

        // *****************************************************************
        /// @brief
        ///     Constructs a new threaded work queue
        /// @param numThreads
        ///     Number of threads to create for handling work items. (Typically 2-4)
        // *****************************************************************
        ThreadedWorkQueue(int numThreads)
        {
            // Create the thread sync objects
            queueMutex = Mutex::Create();
            signalEvent = Event::Create();
            shutdownRequest = false;
            
            // Create the worker threads
            for(int threadIndex=0; threadIndex < numThreads; threadIndex++)
            {
                // Create a worker thread
                threads.push_back(
                    Thread::Create(&ThreadedWorkQueue::Worker, (void*)this, false)
                    );
            }
        }
        
        // *****************************************************************
        /// @brief
        ///     Destructor
        // *****************************************************************
        ~ThreadedWorkQueue()
        {
            // Lock the work item queue mutex  (so the shutdown event doesnt get trampled)
            queueMutex->Lock();

            // Set the shutdown request
            shutdownRequest = true;
            
            // Signal to all the worker threads
            signalEvent->Set();
            
            // Unlock the work item queue mutex
            queueMutex->Unlock();
            
            // Join the worker threads
            for(size_t i=0; i<threads.size(); i++)
            {
                threads[i].Join();
            }	
            
            // Destroy the thread sync objects
            GdkDelete( queueMutex );
            GdkDelete( signalEvent );
        }

        /// @}
        // ---------------------------------
        /// @name Work Queue Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Adds a new work item to the queue
        /// @param data
        ///     The work item data that will be passed to the OnProcessWorkItem() method
        /// @param priority
        ///     The priority of the work item.. Higher priority work items will be processed sooner
        // *****************************************************************
        void Enqueue(TWorkItem data, short priority)
        {
            // Lock the mutex
            queueMutex->Lock();
            
            // Add the work item to the queue
            workItemQueue.push( QueueEntry(data, priority) );
            
            // Unlock the queue mutex
            queueMutex->Unlock();
            
            // Signal to the worker threads that there is work to be done!
            signalEvent->Set();
        }
        
        // *****************************************************************
        /// @brief
        ///     Gets the number of items in the work item queue
        // *****************************************************************
        size_t GetQueueCount()
        {
            // Lock the mutex
            queueMutex->Lock();
            
            // Add the work item to the queue
            size_t count = workItemQueue.size();
            
            // Unlock the queue mutex
            queueMutex->Unlock();
            
            return count;
        }
        
        /// @}
        
    protected:
        
        // Protected Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Virtuals for Work Handling
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     This virtual is called to process a work item
        /// @param workItem
        ///     The work item data to be processed
        /// @remarks
        ///     This method will be called from a worker thread, so make sure to 
        ///     syncronize if you are accesing external resources outside the thread.
        // *****************************************************************
        virtual void OnProcessWorkItem(TWorkItem workItem) = 0;
        
        /// @}
        
    private:
        
        // Private Methods
		// =====================================================
        
        // *****************************************************************
        /// @brief
        ///     Worker method for the processing threads
        // *****************************************************************
        
        static void* Worker(void* data)
        {
            // Get the ThreadedWorkQueue (passed in through the void*)
            ThreadedWorkQueue<TWorkItem>* workQueue = (ThreadedWorkQueue<TWorkItem>*)data;
            
            // Loop forever
            while(1)
            {
                // Wait for the 'work' signal
                workQueue->signalEvent->Wait(false);
                
                // Loop while we have work to do
                while(1)
                {
                    // Lock the work item queue mutex
                    workQueue->queueMutex->Lock();
                    
                    // Is there a shutdown request in order?
                    if(workQueue->shutdownRequest == true)
                    {
                        // Unlock the work item queue mutex
                        workQueue->queueMutex->Unlock();
                        
                        // Stop the thread
                        Thread::Exit(0);
                    }
                    
                    // Is the work item queue empty?
                    if(workQueue->workItemQueue.empty() == true)
                    {
                        // Reset the work item signal event
                        workQueue->signalEvent->Reset();
                        
                        // Unlock the work item queue mutex
                        workQueue->queueMutex->Unlock();
                        
                        // Stop working
                        break;
                    }
                    
                    // Get the next work item off the queue
                    QueueEntry queueEntry = workQueue->workItemQueue.top();
                    workQueue->workItemQueue.pop();
                    
                    // Unlock the work item queue mutex
                    workQueue->queueMutex->Unlock();
                    
                    // Do the work for this item
                    workQueue->OnProcessWorkItem(queueEntry.Data);
                    
                } // while(1) - working loop
                
            } // while(1) - forever loop

            return NULL;
        }
        
        // Private Types
		// =====================================================
        
		///	Container type for work items  (For Internal Use)
        struct QueueEntry
        {
            short Priority;
            TWorkItem Data;
            
            /// Constructor
            QueueEntry(TWorkItem data, short priority) 
            : Data(data), Priority(priority)
            {
            }
            
            /// Less-Than Operator
            inline bool operator< (const QueueEntry& input) const 
            { 
                // Higher priority = earlier sorting
                return this->Priority > input.Priority; 
            } 
        };
        
        /// A priority queue of QueueEntry items
        typedef priority_queue<QueueEntry> WorkItemPriorityQueue;
        
        // Private Properties
		// =====================================================
        
        // The work item queue
        WorkItemPriorityQueue workItemQueue;
        
        // Thread controls
        vector<Thread> threads;
        Mutex* queueMutex;
        Event* signalEvent;
        bool shutdownRequest;
        
        
	};
    
    /// @}
    /// @}
    
} // namespace Gdk
