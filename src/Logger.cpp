#include "Logger.h"

#include <thread>
#include <mutex>
#include <print> 
#include <format>
#include <string_view>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger::Logger()
{
    std::ios_base::sync_with_stdio(false); // Disable synchronization with C-style I/O for performance
    m_logThread = std::thread(&Logger::RunLogger, this);
}

Logger::~Logger()
{
    LOG(LogLevel::WARNING, "Finishing Logging...");
    if (m_debugLogger)
    {
        LOG(LogLevel::DEBUG, "Max queue size during logging: {}", m_queueMaxSize);
    }

    // Tell the logging thread to finish up and update condition variable
    m_finishLogging = true;
    m_cv.notify_one();

    // Wait for the thread to finish logging
    m_logThread.join();
}

void Logger::RunLogger()
{
    // Get the current date
    const std::chrono::time_point now{ std::chrono::system_clock::now() };
    const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};

    // Output the current date in YYYY-MM-DD format
    const std::string dateString = std::format("{}-{:02}-{:02}", 
                static_cast<int>     (ymd.year()),
                static_cast<unsigned>(ymd.month()),
                static_cast<unsigned>(ymd.day()));

    // Get the time of day in hh:mm:ss format
    const std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
    const std::tm localTime = *std::localtime(&nowTimeT);
    const std::chrono::milliseconds ms {std::chrono::duration_cast<std::chrono::milliseconds>(now 
                                        - std::chrono::floor<std::chrono::seconds>(now)).count() / 10};

    // Format time to hundredths of a second (HH:MM:SS.hh)
    const std::string timeString = std::format("{:02}:{:02}:{:02}.{:<2}",
        localTime.tm_hour, localTime.tm_min, localTime.tm_sec, ms.count());

    // Output initial message indicating the logger has started
    std::println("----------------------------------------------------------------------------------------");
    std::println("{}  {:<8} [{:<20}:{:<3}] Started Logger... {}", 
        timeString, to_string(LogLevel::INFO), GetBaseFileName(__FILE__), __LINE__, dateString);
    std::println("----------------------------------------------------------------------------------------");

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

        if (m_queue.size() > m_queueMaxSize)
        {
            m_queueMaxSize = m_queue.size();
        }

        // Get the message and remove it from the queue
        const messageType msg = std::move(m_queue.front());
        m_queue.pop();
        // Since we're done messing with the queue, unlock the mutex
        queueReadLock.unlock();

        const std::time_t messageTimeT = std::chrono::system_clock::to_time_t(msg.messageTime);
        const std::tm     messageLocalTime = *std::localtime(&messageTimeT);
        const std::chrono::milliseconds ms {std::chrono::duration_cast<std::chrono::milliseconds>(msg.messageTime 
                                        - std::chrono::floor<std::chrono::seconds>(msg.messageTime)).count() / 10};

        // Format time to hundredths of a second (HH:MM:SS.hh)
        const std::string timeString = std::format("{:02}:{:02}:{:02}.{:<2}",
            messageLocalTime.tm_hour, messageLocalTime.tm_min, messageLocalTime.tm_sec, ms.count());

        // Output the message
        std::println("{}  {:<8} [{:<20}:{:<3}] {}", timeString, to_string(msg.level), GetBaseFileName(msg.sourceFile), msg.lineNumber, msg.message);
    }

    // End the logging with two empty lines
    std::print("\n\n");
}
