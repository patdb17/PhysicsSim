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
#include <queue>
#include <string>
#include <string_view>
#include <format>
#include <thread>
#include <atomic>
#include <print>

#include <GL/glew.h> // For GLubyte type

#include "Timing.h"

enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

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
        {
            // Lock the mutex when modifying the queue
            std::lock_guard queueUpdatelock(m_queueMutex);

            // Construct the message structure in the queue
            m_queue.emplace(level, sourceFile, lineNumber, msg);
        } // Unlock mutex before updating condition variable
    
        // Update condition variable
        m_cv.notify_one();
    }

private:
    // Structure to hold the log message
    struct msgStruct
    {
        LogLevel level;
        std::string sourceFile;
        unsigned int lineNumber;
        std::string message;

        // Constructor to initialize the message structure
        msgStruct(LogLevel lvl, std::string file, unsigned int line, std::string msg)
            : level(lvl), sourceFile(std::move(file)), lineNumber(line), message(msg) 
        {}
    };
    typedef msgStruct messageType;

    bool                    m_debugLogger = false; // Flag to indicate if debug prints in the logger itself should print
    size_t                  m_queueMaxSize = 0;
    std::queue<messageType> m_queue;
    std::mutex              m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool>       m_finishLogging = false;
    std::thread             m_logThread;

    // Function to run the logging thread
    // This function will continuously check for messages in the queue and log them.
    void RunLogger();

    // Convert LogLevel to string
    constexpr static inline std::string to_string(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            case LogLevel::DEBUG:   return "DEBUG";
            default:                return "UNKNOWN";
        }
    }

    // Helper function to extract base file name from __FILE__
    inline std::string_view GetBaseFileName(std::string_view file)
    {
        size_t lastSep = file.find_last_of("/\\");
        return (lastSep == std::string_view::npos) ? file : file.substr(lastSep + 1);
    }
};

// Pointer to a logging object for use by any file that includes this header.
// The actual object must be created in main.
inline Logger* loggerPtr = nullptr;

// Macro to log messages
// Usage: LOG(LogLevel::INFO, "This is a log message with value: {}", value);
// The macro automatically includes the function name and line number in the log message.
#define LOG(level, formatStr, ...) loggerPtr->Log(level, __FILE__, __LINE__, std::format(formatStr, ##__VA_ARGS__))

// Function to convert a GLEW string (GLubyte*) to a std::string_view
// This is useful for logging GLEW error messages or version strings.
inline std::string_view GlewStrToStrView(const GLubyte* glewStr)
{
    if (glewStr == nullptr) {
        return std::string_view();
    }
    return std::string_view(reinterpret_cast<const char*>(glewStr));
}

#endif // !LOGGER_H