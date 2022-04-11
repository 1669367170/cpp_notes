// [C++并发编程-相关基础知识]

// [参考原文链接1] - C++ 并发编程（从C++11到C++17）
// 作者：paul.pub
// 链接：https://paul.pub/cpp-concurrency/#id-%E4%B8%BA%E4%BB%80%E4%B9%88%E8%A6%81%E5%B9%B6%E5%8F%91%E7%BC%96%E7%A8%8B
// [参考原文链接2] - 用C++实现数据总线的方法系列-基本概念
// 作者：JasonLiThirty
// 链接：https://www.jianshu.com/p/6e17f02f7cf8
// 来源：简书
// [参考书籍] - 深入应用C++11 代码优化与工程级应用



// *****************************************************************************
// 1. 并发：如果多个队列可以交替使用某台咖啡机，则这一行为就是并发的。(单个处理器)
// 2. 并行：如果存在多台咖啡机可以被多个队列交替使用，则就是并行。(多个处理器)
// [注] 队列中的每个人类比于计算机的任务，咖啡机类比于计算机处理器
//      进程：操作系统进行资源分配和独立调度的基本单元；应用程序的载体；(程序+数据集合+进程控制块)
//      线程：不占用资源，独立调度的最小单元；程序执行的最小单元；
//    一次调用：多个线程需要同时调用某个函数，std::call_once 可以保证多个线程对该函数只调用一次。需要一个std::once_flag作为std::call_once的入参
//          std::once_flag m_flag; std::call_once(m_flag, [this](){StopExecute(); });
// -----------------------------------------------------------------------------
// 3. 竞争条件：当多个进程或者线程同时访问共享数据时，只要有一个任务会修改数据，那么就可能会发生问题。此时结果依赖于这些任务执行的相对时间，这种场景称为竞争条件（race condition）。
// 4. 临界区：访问共享数据的代码片段称之为临界区（critical section）-- 要避免竞争条件，需要对临界区进行保护
// -----------------------------------------------------------------------------
// 5. 互斥体：mutex -- 保证每个线程对于共享数据的访问是"互斥的" (timed_mutex/recursive_mutex/recursive_timed_mutex/shared_timed_mutex/shared_mutex)
//          递归锁：在同一个线程中，同一把锁可以锁定多次；共享锁：可以同时被多个线程同时获取到，多用在读者写者模型；
//    原子变量：std::atomic<T> 不需要使用互斥量来保护这个变量
// 6. 锁：lock/try_lock/unlock -- 锁定互斥体 (lock锁定互斥体，如果不可用，则阻塞) -- 锁的粒度尽可能小，耗时的操作尽量移到锁的外面
// -----------------------------------------------------------------------------
// 7. 死锁：各方等待其他方释放资源，但是所有方都不释放
// 8. RAII机制：资源获取即初始化 (使用前申请的资源如分配的堆内存等的生命周期，与一个对象的生存周期相绑定) -> 消除内存泄露并保证异常安全
//              在类的构造函数中分配资源，在类的析构函数中释放资源
// 9. 通用互斥管理的API：lock_guard/unique_lock/shared_lock(C++14)/scoped_lock(C++17)；锁定策略：defer_lock/try_to_lock/adopt_lock
//    unique_lock：更加灵活的锁管理类模板，构造时是否加锁是可选的，在对象析构时如果持有锁会自动释放锁，所有权可以转移。对象生命期内允许手动加锁和释放锁。
//    提供了更好的上锁和解锁控制接口(lock，try_lock，try_lock_for，try_lock_until 和unlock)
// -----------------------------------------------------------------------------
// 10. 条件变量：提供了一个开源让多个线程间"同步"协作的功能(阻塞一个或多个线程，直到收到另一个线程发出的通知或者超时了才唤醒) -- 如生产者-消费者模型
//     -> 条件变量常用API：condition_variable(与std::unique_lock关联使用)、condition_variable(与任何锁类型关联，比较灵活但效率比condition_variable差)、notify_all_at_thread_exit、cv_status
//     -> 条件变量相关成员函数：notify_one(通知一个去获取锁)/notify_all(通知所有等待的去竞争锁)；
//     wait(阻塞当前线程直至条件变量被唤醒)/wait_for(阻塞直至被唤醒或超时)/wait_until(阻塞直至被唤醒或到达指定时间点)
//     -> 执行过程：1) 拥有条件变量的线程首先获取mutex；
//                2) 循环检查条件，不满足则释放互斥量，同时阻塞线程直到条件满足；如果满足则向下执行；
//                3) 另一个线程获取mutex，执行完成后调用条件变量的notify_one或notify_all唤醒其他等待线程；
//                  std::unique_lock<std::mutex> lck(m_mtRun);
//                  m_cvRun.wait(lck, [this]{ return m_runDown; });
// -----------------------------------------------------------------------------
// 11. 异步：异步使得耗时的操作不影响当前主线程的执行流
//      ① aync：异步运行一个函数，并返回保有其结果的std::future (直接创建异步的task，异步任务返回结果在future中)
//      ② future：等待被异步设置的值
//      ③ packaged_task：打包一个函数，存储其返回值以进行异步获取 (包装的异步操作，获取一个异步操作的返回值)
//      ④ promise：存储一个值以进行异步获取 (包装的一个值，获取线程中的某个值)
//      ⑤ shared_future：等待被异步设置的值（可能为其他 future 所引用）
//      层次级别：async > packaged_task/promise > future
// 12. packaged_task：在一些业务中，我们可能会有很多的任务需要调度。这时我们常常会设计出任务队列和线程池的结构。此时，就可以使用packaged_task来包装任务
// 13. promise：将promise与future配对使用。这样就可以将返回结果和任务结束两个事情分开。


#pragma once

#include <iostream>
#include <thread>
#include <mutex> // call_once

namespace ConcurrencyNS {
    void test1();

    // lambda thread
    void test2();

    // thread with params
    void test3();

    // join/detach/chrono::system_clock/localtime
    void test4();

    // once_flag、call_once
    void test5();

    // concurrent_task_simple
    void test6();

    // concurrent_task_mutex
    void test7();

    // dead_lock
    void test9();

    // lock_guard RAII
    void test10();

    // condition_variable/unique_lock/notify_all
    void test11();

    // async
    void test12();

    // packaged_task/std::vector<std::future<double>>/std::promise<double>
    void test13();

    static void AllTest() {
        std::cout << "================ test1 ================" << std::endl;
        test1();
        std::cout << "================ test2 ================" << std::endl;
        test2();
        std::cout << "================ test3 ================" << std::endl;
        test3();
        std::cout << "================ test4 ================" << std::endl;
        test4();
        std::cout << "================ test5 ================" << std::endl;
        test5();
        std::cout << "================ test6 ================" << std::endl;
        test6();
        std::cout << "================ test7 ================" << std::endl;
        test7();
        std::cout << "================ test9 ================" << std::endl;
//        test9();
        std::cout << "================ test10 ================" << std::endl;
        test10();
        std::cout << "================ test11 ================" << std::endl;
//        test11();
        std::cout << "================ test12 ================" << std::endl;
        test12();
        std::cout << "================ test13 ================" << std::endl;
        test13();
    }
}