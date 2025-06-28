#include "Timing.h"

#include <chrono> 
#include <format>

#include "Logger.h"
//Timer_t:: Timer_t() {}
//Timer_t::~Timer_t(){}
//• Stop(const int id)

void Timer_t::Start()
{
    // Get the start time
    c_startTime = std::chrono::high_resolution_clock::now();
}

std::chrono::microseconds Timer_t::Stop(const int id)
{
    // Get the stop time
    MsTimer endTime = std::chrono::high_resolution_clock::now();

    // Calculate the time difference
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - c_startTime);
    // Print out the time difference
    PrintTime(duration, id);

    // Return the time difference
    return duration;
}

void Timer_t::PrintTime(const std::chrono::microseconds duration, const int id)
{
    Logger_t::logPtr->Log(std::format("ID; {}, Time passed = {}us", id, duration.count()));
}