#include "Coccurrency.h"

namespace ConcurrencyNS {

    int g_i = 0;
    std::mutex g_i_mutex;

    void safe_increment() {
        std::lock_guard<std::mutex> lock(g_i_mutex); // 方法结束的时候，局部变量std::lock_guard<std::mutex> lock会被销毁
        ++g_i;

        std::cout << "thread-" << std::this_thread::get_id() << ": " << g_i << std::endl;
    }

    void test10() {
        std::cout << "main: " << g_i << std::endl;

        std::thread t1(safe_increment);
        std::thread t2(safe_increment);

        t1.join();
        t2.join();
        std::cout << "main: " << g_i << std::endl;
    }

}