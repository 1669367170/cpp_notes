#include "Coccurrency.h"

namespace ConcurrencyNS {
    void init() {
        std::cout << "thread-" << std::this_thread::get_id() << " Initialing..." << std::endl;
    }

    void workerFuncA(std::once_flag *flag) {
        // 即便在多线程环境下，也能保证只调用某个函数一次(std::once_flag配合使用)
        std::call_once(*flag, init);
    }

    void test5() {
        std::once_flag flag;
        std::thread t1(workerFuncA, &flag);
        std::thread t2(workerFuncA, &flag);
        std::thread t3(workerFuncA, &flag);

        t1.join();
        t2.join();
        t3.join();
    }
}

