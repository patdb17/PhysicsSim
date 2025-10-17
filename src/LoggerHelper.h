#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H

#include <string>
#include <chrono>
#include <format>
#include <print>

enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

// Convert LogLevel to string
constexpr inline std::string LogLevelToString(const LogLevel level)
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

// Function to get the time of day in hh:mm:ss format
inline std::string GetTimeString(const std::chrono::time_point<std::chrono::system_clock> now)
{
    // Get the time of day in hh:mm:ss format
    const std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
    const std::tm localTime = *std::localtime(&nowTimeT);
    const std::chrono::milliseconds ms {std::chrono::duration_cast<std::chrono::milliseconds>(now 
                                        - std::chrono::floor<std::chrono::seconds>(now)).count() / 10};

    // Format time to hundredths of a second (HH:MM:SS.hh)
    return std::format("{:02}:{:02}:{:02}.{:<2}",
        localTime.tm_hour, localTime.tm_min, localTime.tm_sec, ms.count());
}

// Function to extract base file name from __FILE__
constexpr static inline std::string_view GetBaseFileName(std::string_view file)
{
    const size_t lastSep = file.find_last_of("/\\");
    return (lastSep == std::string_view::npos) ? file : file.substr(lastSep + 1);
}

// Function to print a message to the console with the correct format
inline void PrintMessage(const std::string& timeString,
                         const LogLevel     level,
                         const unsigned int lineNumber,
                         const std::string& sourceFile,
                         const std::string& message)
{
    // Output the message
    std::println("{}  {:<8} [{:<20}:{:<3}] {}", timeString, LogLevelToString(level), GetBaseFileName(sourceFile), lineNumber, message);
}

// Function to immediately print a message to the console. 
// Useful when the program is crashing and you want to log a message before exiting or for logging from places where the logger instance is not available.
inline void PrintMessageNow(const LogLevel     level,
                            const unsigned int lineNumber,
                            const std::string& sourceFile,
                            const std::string& message)
{
    // Output the message
    PrintMessage(GetTimeString(std::chrono::system_clock::now()), level, lineNumber, sourceFile, message);
}

#endif // !LOGGER_TYPES_H