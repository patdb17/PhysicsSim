#ifndef LOGGER_H
#define LOGGER_H
// Logger.h
// This file contains the definition of the Logger class.
// The Logger class provides a thread-safe logging mechanism.
// It uses a separate thread to handle logging messages, allowing the main thread
// to continue executing without blocking.
// The logger uses a queue to store messages and a condition variable to signal
// the logging thread when there are messages to log.
// The logger can be used from any thread, and it is safe to call the LOG
// macro from multiple threads simultaneously.

#include <mutex>
#include <condition_variable>
#include <string>
#include <string_view>
#include <format>
#include <thread>
#include <atomic>
#include <chrono> // For time-related functions

#include <GL/glew.h> // For GLubyte type

#include "LoggerHelper.h"
#include "Timing.h"
#include "CircularBuffer.h"

class Logger
{
public:
    Logger();
    ~Logger();

    // Method to log messages with a specific log level, source file, line number, and message
    // Accessed via the LOG macro, which automatically includes the source file and line number.
    // Example usage: LOG(LogLevel::INFO, "This is a log message with value: {}", value);
    // The macro automatically includes the function name and line number in the log message
    inline void Log(const LogLevel level, 
                    const std::string sourceFile,
                    const unsigned int lineNumber,
                    const std::string& msg)
    {
        // Get the current date
        const std::chrono::time_point msgTime{ std::chrono::system_clock::now() };
        {
            // Lock the mutex when modifying the queue
            std::lock_guard queueUpdatelock(m_queueMutex);

            // Construct the message structure in the queue
            m_queue.emplace(msgTime, level, sourceFile, lineNumber, msg);
        } // Unlock mutex before updating condition variable
    
        // Update condition variable
        m_cv.notify_one();
    }

private:
    // Structure to hold the log message
    struct msgStruct
    {
        std::chrono::time_point<std::chrono::system_clock> messageTime; // Time of day when the message was logged
        LogLevel level;
        std::string sourceFile;
        unsigned int lineNumber;
        std::string message;

        // Constructor to initialize the message structure
        msgStruct(std::chrono::time_point<std::chrono::system_clock> time, LogLevel lvl, std::string file, unsigned int line, std::string msg)
            : messageTime(time), level(lvl), sourceFile(std::move(file)), lineNumber(line), message(msg) 
        {}
    };
    typedef msgStruct messageType;

    bool                    m_debugLogger = true; // Flag to indicate if debug prints in the logger itself should print
    size_t                  m_queueMaxSize = 0;
    CircularBuffer<messageType> m_queue; // Circular buffer to hold messages
    std::mutex              m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool>       m_finishLogging = false;
    std::thread             m_logThread;

    // Function to run the logging thread
    // This function will continuously check for messages in the queue and log them.
    void RunLogger();
};

// Pointer to a logging object for use by any file that includes this header.
// The actual object must be created in main.
inline Logger* loggerPtr = nullptr;

// Macro to log messages
// Usage: LOG(LogLevel::INFO, "This is a log message with value: {}", value);
// The macro automatically includes the function name and line number in the log message.
#define LOG(level, formatStr, ...) loggerPtr->Log(level, __FILE__, __LINE__, std::format(formatStr, ##__VA_ARGS__))

#endif // !LOGGER_H