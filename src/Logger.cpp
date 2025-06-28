#include "Logger.h"

#include <thread>
#include <mutex>
#include <iostream> 


Logger_t::Logger_t()
{
    logPtr = this;
    std::ios_base::sync_with_stdio(false);
    c_logThread = std::thread(&Logger_t::RunLogger, this);
}

Logger_t::~Logger_t()
{
    Log("Finishing Logging...");

    // Tell the logging thread to finish up and update condition variable
    c_finishLogging = true;
    c_cv.notify_one();

    // Wait for the thread to finish logging
    c_logThread.join();
}

void Logger_t::Log(const messageType& msg)
{
    {
        std::lock_guard queueUpdatelock(c_queueMutex);
        c_queue.push(msg);
    } // Unlock mutex before updating condition variable

    // Update condition variable
    c_cv.notify_one();
}

void Logger_t::RunLogger()
{
    // Run loop until no more messages to log and the class is being destroyed
    while (!c_queue.empty() || !c_finishLogging)
    {
        // Mutex lock and wait
        std::unique_lock queueReadLock(c_queueMutex);
        // Wait for a message to be available in the queue or for the logging to finish
        c_cv.wait(queueReadLock, [this] {return !c_queue.empty() || c_finishLogging;});

        // If the queue is empty and we're not finishing, wait for the condition variable
        if (c_queue.empty())
        {
            continue;
        }

        // Get the message and remove it from the queue
        const messageType msg = c_queue.front();
        c_queue.pop();
        // Since we're done messing with the queue, unlock the mutex
        queueReadLock.unlock();

        // Output the message
        std::cout << msg << "\n";
    }
}
