#include "Coccurrency.h"
#include "../common/Timer.h"
#include <cmath>
#include <vector>
#include <future>


namespace ConcurrencyNS {
    static const double MAX = 10e7;

    double concurrent_worker(int min, int max) {
        double sum = 0;
        for (int i = min; i <= max; ++i) {
            sum += sqrt(i);
        }
        return sum;
    }

    double concurrent_task(int min, int max) {
        std::vector<std::future<double>> results; // 集合，存储获取任务的结果
        unsigned concurrent_count = std::thread::hardware_concurrency();
        min = 0;
        for (int i = 0; i < concurrent_count; ++i) {
            // 将任务包装成packaged_task。请注意，由于concurrent_worker被包装成了任务，我们无法直接获取它的return值。而是要通过future对象来获取
            std::packaged_task<double(int, int)> task(concurrent_worker);
            // 获取任务关联的future对象，并将其存入集合中
            results.push_back(task.get_future());
            int range = max / concurrent_count * (i + 1);
            // 通过一个新的线程来执行任务
            std::thread t(std::move(task), min, range);
            t.detach();

            min = range + 1;
        }

        std::cout << "threads create finish" << std::endl;
        double sum = 0;
        // 通过future集合，逐个获取每个任务的计算结果，将其累加
        for (auto &r : results) {
            sum += r.get();
        }
        return sum;
    }

    // TODO 不再直接返回计算结果，而是增加了一个promise对象来存放结果
    void concurrent_task_promise(int min, int max, std::promise<double> *result) {
        std::vector<std::future<double>> results; // 集合，存储获取任务的结果
        unsigned concurrent_count = std::thread::hardware_concurrency();
        min = 0;
        for (int i = 0; i < concurrent_count; ++i) {
            // 将任务包装成packaged_task。请注意，由于concurrent_worker被包装成了任务，我们无法直接获取它的return值。而是要通过future对象来获取
            std::packaged_task<double(int, int)> task(concurrent_worker);
            // 获取任务关联的future对象，并将其存入集合中
            results.push_back(task.get_future());
            int range = max / concurrent_count * (i + 1);
            // 通过一个新的线程来执行任务
            std::thread t(std::move(task), min, range);
            t.detach();

            min = range + 1;
        }

        std::cout << "threads create finish" << std::endl;
        double sum = 0;
        // 通过future集合，逐个获取每个任务的计算结果，将其累加
        for (auto &r : results) {
            sum += r.get();
        }
        result->set_value(sum); // 一旦调用set_value，其相关联的future对象就会就绪
        std::cout << "concurrent_task_promise finish" << std::endl;
    }

    void test13() {
        auto tw = Timer("packaged_task_concurrent");
        // TODO 实际工程中，通常借助于packaged_task将任务组装成队列，然后通过线程池的方式进行调度：
        double r = concurrent_task(0, MAX);
        tw.fi();
        std::cout << "result = " << r << std::endl;
        std::cout << std::endl;

        auto tw1 = Timer("concurrent_task_promise");
        std::promise<double> result;
        concurrent_task_promise(0, MAX, &result);
        tw.fi();
        // TODO future对象只有被一个线程获取值，调用get()以后，就没有可获取的值了
        // 需要多个线程中获取future结果，使用shared_future
        std::cout << "result = " << result.get_future().get() << std::endl;
    }
}