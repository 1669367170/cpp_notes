#include "Coccurrency.h"
#include <set>

namespace ConcurrencyNS {
    // 银行账户类
    class Account {
    public:
        Account(std::string name, double money) : mName(name), mMoney(money) {};

        void changeMoney(double amount) { mMoney += amount; }

        std::string getName() { return mName; }

        double getMoney() { return mMoney; }

        std::mutex *getLock() { return &mMoneyLock; }

    private:
        std::string mName;
        double mMoney;
        std::mutex mMoneyLock;
    };

    // 银行类
    class Bank {
    public:
        void addAccount(Account *account) {
            mAccounts.insert(account);
        }

#define NO_DEAD_LOCK

        bool transferMoney(Account *accountA, Account *accountB, double amount) {
            // 修改每个账户前，需要获取相应锁
            // TODO 可能会发生死锁，两个线程同时获取其中一个账号的锁，然后又想获取另一个账号的锁 -> 如t1拥有lock(A)在等lock(B)；t2拥有lock(B)在等lock(A)
#ifndef NO_DEAD_LOCK
            std::lock_guard<std::mutex> guardA(*accountA->getLock());
            std::lock_guard<std::mutex> guardB(*accountB->getLock());
#else
            // 简单的避免死锁原则：对所有锁进行排序，必须按顺序来获取锁，不乱序
            std::lock(*accountA->getLock(), *accountB->getLock()); // lock获取两把锁
            // adopt_lock表示：现在是已经获取到互斥体了的状态了，不用再次加锁，避免二次加锁
            std::lock_guard<std::mutex> lockA(*accountA->getLock(), std::adopt_lock);
            std::lock_guard<std::mutex> lockB(*accountB->getLock(), std::adopt_lock);
#endif
            // TODO 改进版本
            if (amount > accountA->getMoney()) {
                return false;
            }
            accountA->changeMoney(-amount);
            accountB->changeMoney(amount);
            return true;
        }

        double totalMoney() const {
            double sum = 0;
            for (auto a : mAccounts) {
                sum += a->getMoney();
            }
            return sum;
        }

    private:
        std::set<Account *> mAccounts;
    };

    std::mutex sCountLock; // 避免输出混乱加锁使用

    [[noreturn]] void randomTransfer(Bank *bank, Account *accountA, Account *accountB) {
        while (true) {
            std::cout << "thread-[" << std::this_thread::get_id() << "] is running." << std::endl;
            double randomMoney = ((double) rand() / RAND_MAX) * 100;
//            srand(time(0)); // 更新种子，真随机数
//            double randomMoney = -100 + (rand()) % 100;
            if (bank->transferMoney(accountA, accountB, randomMoney)) {
                std::lock_guard<std::mutex> lockGuard(sCountLock);
                std::cout << "Transfer " << randomMoney << " from " << accountA->getName() << " to "
                          << accountB->getName() << ", Bank totalMoney: " << bank->totalMoney() << std::endl;
            } else {
                std::lock_guard<std::mutex> lockGuard(sCountLock);
                std::cout << "Transfer failed, " << accountA->getName() << " has only $" << accountA->getMoney()
                          << ", but " << randomMoney << " required" << std::endl;
            }
        }
    }

    void test9() {
        Account a("A", 100);
        Account b("B", 100);
        Bank aBank;
        aBank.addAccount(&a);
        aBank.addAccount(&b);

        std::thread t1(randomTransfer, &aBank, &a, &b);
        std::thread t2(randomTransfer, &aBank, &a, &b);
        t1.join();
        t2.join();
    }
}