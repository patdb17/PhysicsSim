#include "UnitTests.h"

#include <iostream>
#include <format>
#include <print>

#include "Logger.h"
#include "Timing.h"

void LogTimerTest()
{
    const unsigned int numIterations = 1000;

    // Run the test for cout durations
    std::chrono::microseconds totalCoutTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer timer;
        timer.Start();

        // Simulate some work
        std::cout << std::format("cout #{}\n", i);

        totalCoutTime += timer.Stop(static_cast<int>(i));
    }

    // Run the test for print durations
    std::chrono::microseconds totalPrintTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer timer;
        timer.Start();

        // Simulate some work
        std::println("println #{}", i);

        totalPrintTime += timer.Stop(static_cast<int>(i));
    }

    // Run the test for the logger durations
    std::chrono::microseconds totalLogTime = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < numIterations; ++i)
    {
        Timer timer;
        timer.Start();

        // Simulate some work
        LOG(LogLevel::INFO, "LOG #{}", i);

        totalLogTime += timer.Stop(static_cast<int>(i));
    }
    // Log the average time taken for cout and the logger
    LOG(LogLevel::WARNING, "-----------------------------");
    LOG(LogLevel::WARNING, "Cout avg = {}us",  totalCoutTime.count()  / numIterations);
    LOG(LogLevel::WARNING, "Print avg = {}us", totalPrintTime.count() / numIterations);
    LOG(LogLevel::WARNING, "Log avg = {}us",   totalLogTime.count()   / numIterations);
    LOG(LogLevel::WARNING, "-----------------------------");


    // Log a message to indicate that the testing has finished
    LOG(LogLevel::WARNING, "Unit tests finished.");
    LOG(LogLevel::WARNING, "-----------------------------");

}
