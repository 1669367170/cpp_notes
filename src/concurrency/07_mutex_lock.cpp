#include "Coccurrency.h"
#include <cmath>
#include <chrono>
#include <vector>

namespace {
    constexpr int MAX = 10e7;
}

namespace ConcurrencyNS {
    static double sum = 0;
    static std::mutex mtx;

    // 计算某个范围内所有自然数的平方根之和，例如[1, 10e8]。
    void workerFuncC(int min, int max) {
        for (int i = min; i <= max; ++i) {
            // 访问共享数据之前加锁，访问完成之后解锁；
            // TODO 缺陷：实际每次只能有一个线程串行执行，此外还多了锁处理的负担
            mtx.lock();
            sum += sqrt(i);
            mtx.unlock();
        }
    }

    void workerFuncD(int min, int max) {
        double tmp_sum = 0;
        for (int i = min; i <= max; ++i) {
            tmp_sum += sqrt(i);
        }
        // 局部变量保存每个线程的处理结果，最后汇总的时候进行锁保护
        mtx.lock();
        sum += tmp_sum;
        mtx.unlock();
    }

    void concurrent_task_mutex(int min, int max) {
        auto start_time = std::chrono::steady_clock::now();
        unsigned concurrent_count = std::thread::hardware_concurrency();
        std::cout << "hardware_concurrency: " << concurrent_count << std::endl;
        std::vector<std::thread> threads;
        min = 0;
        sum = 0;
        for (int t = 0; t < concurrent_count; ++t) {
            int range = max / concurrent_count * (t + 1);
            threads.push_back(std::thread(workerFuncC, min, range));
            min = range + 1;
        }
        for (auto &t : threads) {
            t.join();
        }
        auto end_time = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        std::cout << "thread-" << std::this_thread::get_id() << " concurrent_task_mutex finish, " << ms
                  << " ms consumed, Result: " << sum << std::endl;
    }

    void concurrent_task_mutex_optimized(int min, int max) {
        auto start_time = std::chrono::steady_clock::now();
        unsigned concurrent_count = std::thread::hardware_concurrency();
        std::cout << "hardware_concurrency: " << concurrent_count << std::endl;
        std::vector<std::thread> threads;
        min = 0;
        sum = 0;
        for (int t = 0; t < concurrent_count; ++t) {
            int range = max / concurrent_count * (t + 1);
            threads.push_back(std::thread(workerFuncD, min, range));
            min = range + 1;
        }
        for (auto &t : threads) {
            t.join();
        }
        auto end_time = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        std::cout << "thread-" << std::this_thread::get_id() << " concurrent_task_mutex_optimized finish, " << ms
                  << " ms consumed, Result: " << sum << std::endl;
    }

    void test7() {
        concurrent_task_mutex(0, MAX);
        concurrent_task_mutex_optimized(0, MAX);
    }
}