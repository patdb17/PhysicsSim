#ifndef LOGGER_H
#define LOGGER_H
// Logger.h
// This file contains the definition of the Logger class.
// The Logger class is a singleton that provides a thread-safe logging mechanism.
// It uses a separate thread to handle logging messages, allowing the main thread
// to continue executing without blocking.
// The logger uses a queue to store messages and a condition variable to signal
// the logging thread when there are messages to log.
// The logger can be used from any thread, and it is safe to call the LogMessage
// method from multiple threads simultaneously.

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <string_view>
#include <format>
#include <thread>
#include <atomic>
#include <iostream>

#include <GL/glew.h> // For GLubyte type

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
    // Pointer to a logging object for use by any file that includes this header.
    // Logger* Logger::m_logPtr = nullptr; // This should be declared above main,
    // and the actual object must be created in main.
    static Logger* m_logPtr;

    Logger();
    ~Logger();

    typedef std::string messageType;

    template<typename... Args>
    static inline void Log( const LogLevel level, 
                            const std::string_view sourceFile,
                            const unsigned int lineNumber,
                            const std::string_view msg,
                            Args&&... args)
    {
        // Create a formatted log message with the level, source file, line number, and message
        const std::string logMsg = std::vformat("{:<8} [{:<15}:{:<3}] {}", std::make_format_args(to_string(level), sourceFile, lineNumber,
                                                std::vformat(msg, std::make_format_args(std::forward<Args>(args)...))));
        
        // Check if the logger is initialized
        if (m_logPtr != nullptr)
        {
            // If the logger is initialized, push the message to the queue
            m_logPtr->PushMessage(logMsg);
        }
        else
        {
            // If the logger is not initialized, print to std::cout
            std::cout << logMsg << "\n";
        }
    }

private:
    std::queue<messageType> m_queue;
    std::mutex              m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool>       m_finishLogging = false;
    std::thread             m_logThread;

    void RunLogger();
    inline void PushMessage(const messageType& msg)
    {
        {
            std::lock_guard queueUpdatelock(m_queueMutex);
            m_queue.push(msg);
        } // Unlock mutex before updating condition variable
    
        // Update condition variable
        m_cv.notify_one();
    }

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
};

// Macro
#define LOG(level, ...) Logger::m_logPtr->Log(level, __func__, __LINE__, __VA_ARGS__)

inline std::string_view GlewStrToStrView(const GLubyte* glewStr)
{
    if (glewStr == nullptr) {
        return std::string_view();
    }
    return std::string_view(reinterpret_cast<const char*>(glewStr));
}

#endif // !LOGGER_H