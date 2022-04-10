#include "Coccurrency.h"

namespace ConcurrencyNS {
    void hello() {
        std::cout << "Hello World from new thread." << std::endl;
    }

    void test1() {
        std::thread t(hello);
        t.join();
    }

    void test2() {
        std::thread t([]() { std::cout << "Hello World from lambda thread." << std::endl; });
        t.join();
    }

    void hello_with_params(std::string name) {
        std::cout << "Welcome to thread" << name << std::endl;
    }

    void test3() {
        // 这里传参是拷贝的方式，若传递指针或者引用，需要考虑参数对象的生命周期. 因为线程的运行长度很可能会超过参数的生命周期
        std::thread t(hello_with_params, "with params");
        t.join();
    }
}