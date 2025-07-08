#ifndef TIMING_H
#define TIMING_H

#include <chrono> 
#include <mutex>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> MsTimer;

class Timer
{
public:
    Timer(bool printTime = true)
        : m_printTime(printTime) {};

    void                      Start();
    std::chrono::microseconds Stop(const int id = -1);

private:    
    MsTimer m_startTime;
    bool    m_printTime = true;
};

#endif // !TIMING_H
