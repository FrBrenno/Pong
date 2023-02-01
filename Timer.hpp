#include <chrono>
#include <thread>

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    Timer();
    void reset();
    double elapsed();
    void wait(double seconds);
};