#pragma once

#include <mutex>

// [单例模式]实现要点：
// 1. 私有化构造函数，以防止外界创建单例类的对象；
// 2. 使用类的私有静态指针变量指向类的唯一实例；
// 3. 使用一个公有的静态方法获取该实例；

// 线程安全的单例模式
template<typename T>
class Singleton {
public:
    static T &GetInstance() {
        static T instance;
        return instance;
    }

//    static T *GetInstancePtr() {
//        static T pInstance;
//        return &pInstance;
//    }

    // 构造函数
    Singleton() = delete;

    // 拷贝构造函数
    Singleton(const Singleton &) = delete;

    // 移动构造函数(无额外拷贝开销，也不会给新对象分配内存空间)
    Singleton(Singleton &&) = delete;

    // 赋值运算符
    Singleton &operator=(const Singleton &) = delete;

    // 移动赋值
//    Singleton &operator=(Singleton &&) = delete;

private:
    ~Singleton() = default;
};

// [非线程安全的懒汉单例模式] -> 线程安全，加锁
// 实现要点：第一次用到类实例的时候才会去实例化一个对象
// 在访问量较小，甚至可能不会去访问的情况下，采用懒汉实现，以时间换空间
std::mutex mt;

class LazySingleton {
public:
    static LazySingleton *GetInstance();

    ~LazySingleton() = default;

private:
    LazySingleton() = default; // 构造函数私有

    LazySingleton(const LazySingleton &) = delete; // 明确拒绝

    LazySingleton &operator=(const LazySingleton &) = delete;

    static LazySingleton *pInstance;
};

LazySingleton *LazySingleton::pInstance = nullptr;

LazySingleton *LazySingleton::GetInstance() {
    if (pInstance == nullptr) {
        mt.lock();
        if (pInstance == nullptr) {
            pInstance = new LazySingleton();
        }
        mt.unlock();
    }
    return pInstance;
}

// [饿汉-单例模式]
// 实现要点：单例类定义的时候就进行初始化
// 在访问量比较大，或者可能访问的线程比较多时，采用饿汉实现，空间换时间
// 线程安全，一定要在合适的地方去delete它
class EagerSingleton {
public:
    static EagerSingleton *GetInstance();

private:
    EagerSingleton() = default;

    EagerSingleton(const EagerSingleton &) = delete;

    EagerSingleton &operator=(const EagerSingleton &) = delete;

    static EagerSingleton *pInstance;
};

EagerSingleton *EagerSingleton::pInstance = new EagerSingleton();

EagerSingleton *EagerSingleton::GetInstance() {
    return pInstance;
}
