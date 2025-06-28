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
        Timer_t timer;
        timer.Start();

        // Simulate some work
        std::cout << std::format("cout #{}\n", i);

        totalCoutTime += timer.Stop(static_cast<int>(i));
    }

    // Run the test for the logger and sum durations
    std::chrono::microseconds totalLogTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer_t timer;
        timer.Start();

        // Simulate some work
        Logger_t::logPtr->Log(std::format("Log #{}", i));

        totalLogTime += timer.Stop(static_cast<int>(i));
    }
    // Log the average time taken for cout and the logger
    Logger_t::logPtr->Log(std::format("\ncout avg = {}us", totalCoutTime.count() / numIterations));
    Logger_t::logPtr->Log(std::format("Log avg = {}us", totalLogTime.count() / numIterations));

    // Log a message to indicate that the testing has finished
    Logger_t::logPtr->Log("Unit tests finished.");
}
