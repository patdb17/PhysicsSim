#include "UnitTests.h"

#include <iostream>
#include <format>

#include "Logger.h"
#include "Timing.h"

void LogTimerTest()
{
    using std::format;

    const unsigned int numIterations = 100;

    // Run the test for cout and sum durations
    std::chrono::microseconds totalCoutTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer timer;
        timer.Start();

        // Simulate some work
        std::cout << format("cout #{}\n", i);

        totalCoutTime += timer.Stop(static_cast<int>(i));
    }

    // Run the test for the logger and sum durations
    std::chrono::microseconds totalLogTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer timer;
        timer.Start();

        // Simulate some work
        LOG(LogLevel::INFO, "Log #{}", i);

        totalLogTime += timer.Stop(static_cast<int>(i));
    }
    // Log the average time taken for cout and the logger
    LOG(LogLevel::INFO, "\ncout avg = {}us", totalCoutTime.count() / numIterations);
    LOG(LogLevel::INFO, "Log avg = {}us", totalLogTime.count() / numIterations);

    // Log a message to indicate that the testing has finished
    LOG(LogLevel::INFO, "Unit tests finished.");
}
