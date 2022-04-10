#include "Coccurrency.h"
#include "../common/Timer.h"
#include <cmath>
#include <future>

namespace ConcurrencyNS {
    static const int MAX = 10e7;
    static double sum = 0;
    std::mutex coutMtx;

    void workerFuncE(int min, int max) {
        {
            std::lock_guard<std::mutex> lck(coutMtx);
            std::cout << "Asyc task runs in thread: " << std::this_thread::get_id() << std::endl;
        }
        for (int i = 0; i <= max; ++i) {
            sum += sqrt(i);
        }
    }

    void test12_1() {
        sum = 0;
        auto tw = Timer("asyncTask_test12_1");
        auto f1 = std::async(workerFuncE, 0, MAX);
        {
            std::lock_guard<std::mutex> lck(coutMtx);
            std::cout << "Asyc task triggered, thread: " << std::this_thread::get_id() << std::endl;
        }
        f1.wait();
        tw.fi();
        std::cout << "Aync task finish, result: " << sum << std::endl;
    }

    void test12_2() {
        double result = 0;
        auto tw = Timer("asyncTask_test12_2");
        {
            std::lock_guard<std::mutex> lck(coutMtx);
            std::cout << "Asyc task with lambda triggered, thread: " << std::this_thread::get_id() << std::endl;
        }
        // 默认情况下，async是启动一个新的线程，还是以同步的方式（不启动新的线程）运行任务，最好通过std::launch::async来明确说明
        auto f2 = std::async(std::launch::async, [&result]() {
            {
                std::lock_guard<std::mutex> lck(coutMtx);
                std::cout << "lambda Asyc task runs in thread: " << std::this_thread::get_id() << std::endl;
            }
            for (int i = 0; i <= MAX; ++i) {
                result += sqrt(i);
            }
        });
        f2.wait();
        tw.fi();
        std::cout << "Async task with lambda finish, result: " << result << std::endl;
    }

    class Worker {
    public:
        Worker(int min, int max) : mMin(min), mMax(max) {};

        double work() {
            mResult = 0;
            for (int i = 0; i <= mMax; ++i) {
                mResult += sqrt(i);
            }
            return mResult;
        }

        double getResult() { return mResult; }

    private:
        int mMin;
        int mMax;
        double mResult;
    };

    void test12_3() {
        Timer tw("Async Task in class");
        Worker w(0, MAX);
        // 通过async执行任务：这里指定了具体的任务函数以及相应的对象。
        // 请注意这里是&w，因此传递的是对象的指针。如果不写&将传入w对象的临时复制
        auto f3 = std::async(std::launch::async, &Worker::work, &w);
        f3.wait();
        tw.fi();
        std::cout << "Task in class finish, result: " << w.getResult() << std::endl;
    }

    void test12() {
        test12_1();
        std::cout << std::endl;
        test12_2();
        std::cout << std::endl;
        test12_3();
    }
}