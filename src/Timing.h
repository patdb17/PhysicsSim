#ifndef TIMING_H
#define TIMING_H

#include <chrono> 
#include <mutex>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> MsTimer;

class Timer
{
public:
    //Timer();
    //~Timer();

    void                      Start();
    std::chrono::microseconds Stop(const int id = -1);

private:
    void PrintTime(const std::chrono::microseconds & duration, const int id);
    
    MsTimer c_startTime;
};

#endif // !TIMING_H
