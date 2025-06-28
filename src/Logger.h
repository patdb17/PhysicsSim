#ifndef LOGGER_H
#define LOGGER_H
// Logger.h
// This file contains the definition of the Logger_t class.
// The Logger_t class is a singleton that provides a thread-safe logging mechanism.
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
#include <thread>
#include <atomic>

class Logger_t
{
public:
    // Pointer to a logging object for use by any file that includes this header.
    // Logger_t* Logger_t::logPtr = nullptr; // This should be declared above main,
    // and the actual object must be created in main.
    static Logger_t* logPtr;

    Logger_t();
    ~Logger_t();

    typedef std::string messageType;

    void Log(const messageType& msg);

private:
    std::queue<messageType> c_queue;
    std::mutex              c_queueMutex;
    std::condition_variable c_cv;
    std::atomic<bool>       c_finishLogging = false;
    std::thread             c_logThread;

    void RunLogger();
};

#endif // !LOGGER_H