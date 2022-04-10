#include "Coccurrency.h"
#include <cmath>
#include <chrono>
#include <vector>

namespace {
    constexpr int MAX = 10e7;
}

namespace ConcurrencyNS {
    static double sum = 0;

    // 计算某个范围内所有自然数的平方根之和，例如[1, 10e8]。
    void workerFuncB(int min, int max) {
        for (int i = min; i <= max; ++i) {
            sum += sqrt(i); // 非原子操作, 实际是多条指令组合完成的
        }
    }

    void serial_task(int min, int max) {
        auto start_time = std::chrono::steady_clock::now();
        sum = 0;
        workerFuncB(0, MAX);
        auto end_time = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        std::cout << "thread-" << std::this_thread::get_id() << " Serail task finish, " << ms
                  << " ms consumed, Result: " << sum << std::endl;
    }

    void concurrent_task_simple(int min, int max) {
        // TODO 以下并发实现：性能没有明显提升，且结果是错误的
        auto start_time = std::chrono::steady_clock::now();
        // 根据处理器的情况决定线程的数量
        unsigned concurrent_count = std::thread::hardware_concurrency();
        std::cout << "hardware_concurrency: " << concurrent_count << std::endl;
        std::vector<std::thread> threads;
        min = 0;
        sum = 0;
        for (int t = 0; t < concurrent_count; ++t) {
            int range = max / concurrent_count * (t + 1);
            threads.push_back(std::thread(workerFuncB, min, range));
            min = range + 1;
        }
        for (auto &t : threads) {
            t.join();
        }
        auto end_time = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        std::cout << "thread-" << std::this_thread::get_id() << " Concurrent_task finish, " << ms
                  << " ms consumed, Result: " << sum << std::endl;
    }

    void test6() {
        serial_task(0, MAX);
        concurrent_task_simple(0, MAX);
    }
}