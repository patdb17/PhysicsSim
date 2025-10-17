#include "Timer.h"

#include <chrono> 
#include <format>

#include "Logger.h"

void Timer::Start()
{
    // Get the start time
    m_startTime = std::chrono::high_resolution_clock::now();
}

std::chrono::microseconds Timer::Stop(const int id)
{
    // Get the stop time
    MsTimer endTime = std::chrono::high_resolution_clock::now();

    // Calculate the time difference
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime);

    if (m_printTime)
    {
        // Print out the time difference
        LOG(LogLevel::INFO, "TimerID: {}, Time passed = {}us", id, duration.count());
    }

    // Return the time difference
    return duration;
}