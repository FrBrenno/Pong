#include "Timer.hpp"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
}

void Timer::reset()
{
    start = std::chrono::high_resolution_clock::now();
}

double Timer::elapsed()
{
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

void Timer::wait(double seconds)
{
    std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}
