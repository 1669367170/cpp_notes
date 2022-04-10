#pragma once

#include <chrono>

//using namespace std::chrono;

class Timer {
public:
    Timer(const std::string taskName) : taskName(taskName) {
        start = std::chrono::system_clock::now();
    }

    double fi() {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << taskName << " cost " << elapsed.count() * 1000 << "ms" << std::endl;
        return elapsed.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::string taskName;
};