#include "Logger.h"

#include <thread>
#include <mutex>
#include <iostream> 
#include <format>
#include <string_view>


Logger::Logger()
{
    m_logPtr = this;
    std::ios_base::sync_with_stdio(false);
    m_logThread = std::thread(&Logger::RunLogger, this);
    LOG(LogLevel::WARNING, "Starting Logging...");
}

Logger::~Logger()
{
    LOG(LogLevel::WARNING, "Finishing Logging...");

    // Tell the logging thread to finish up and update condition variable
    m_finishLogging = true;
    m_cv.notify_one();

    // Wait for the thread to finish logging
    m_logThread.join();
}

void Logger::RunLogger()
{
    // Run loop until no more messages to log and the class is being destroyed
    while (!m_queue.empty() || !m_finishLogging)
    {
        // Mutex lock and wait
        std::unique_lock queueReadLock(m_queueMutex);
        // Wait for a message to be available in the queue or for the logging to finish
        m_cv.wait(queueReadLock, [this] {return !m_queue.empty() || m_finishLogging;});

        // If the queue is empty and we're not finishing, wait for the condition variable
        if (m_queue.empty())
        {
            continue;
        }

        // Get the message and remove it from the queue
        const messageType msg = m_queue.front();
        m_queue.pop();
        // Since we're done messing with the queue, unlock the mutex
        queueReadLock.unlock();

        // Output the message
        std::cout << msg << "\n";
    }
}
