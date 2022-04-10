#include "Coccurrency.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace ConcurrencyNS {
    void print_time() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        std::cout << "now is " << ss.str() << std::endl;
    }

    void sleep_thread() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[thread-" << std::this_thread::get_id() << "] is waking up" << std::endl;
    }

    void loop_thread() {
        for (int i = 0; i < 10; ++i) {
            std::cout << "[thread-" << std::this_thread::get_id() << "] print: " << i << std::endl;
        }
    }

    void test4() {
        print_time();
        std::thread t1(sleep_thread);
        std::thread t2(loop_thread);
        t1.join();
        t2.join();
//        t2.detach();
        print_time();
    }
}