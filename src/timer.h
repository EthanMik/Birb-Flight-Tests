// timer.h
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    // Reset the timer
    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    // Get elapsed time in seconds
    double elapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - start_time;
        return diff.count();
    }

    // Get elapsed time in milliseconds
    long long elapsedMilliseconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
        return diff.count();
    }

    // Get elapsed time in microseconds
    long long elapsedMicroseconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time);
        return diff.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

#endif // TIMER_H
