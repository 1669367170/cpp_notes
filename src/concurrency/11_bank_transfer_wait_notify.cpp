#include "Coccurrency.h"
#include <condition_variable>
#include <set>

namespace ConcurrencyNS {
    std::mutex sCountLock; // 避免输出混乱加锁使用

    // 银行账户类
    class Account {
    public:
        Account(std::string name, double money) : mName(name), mMoney(money) {};

        void changeMoney(double amount) {
            // 这里使用的是unique_lock，这是为了与条件变量相配合。因为条件变量会解锁和重新锁定互斥体。
            std::unique_lock<std::mutex> lck(mMoneyLock);
            {
                std::lock_guard<std::mutex> coutLck(sCountLock);
                std::cout << "thread-[" << std::this_thread::get_id() << "] {" << mMoney << "," << amount << ","
                          << mMoney + amount << "}" << std::endl;
            }
            // TODO 会存在所有线程都阻塞的情况？
            mConditionVar.wait(lck, [this, amount]() { return mMoney + amount > 0; });
            {
                std::lock_guard<std::mutex> coutLck(sCountLock);
                std::cout << "thread-[" << std::this_thread::get_id() << "] goes down. {" << mMoney << "," << amount
                          << "," << mMoney + amount << "}" << std::endl;
            }
            mMoney += amount;
            mConditionVar.notify_all();
        }

        std::string getName() { return mName; }

        double getMoney() { return mMoney; }

    private:
        std::string mName;
        double mMoney;
        std::mutex mMoneyLock;
        std::condition_variable mConditionVar;
    };

    // 银行类
    class Bank {
    public:
        void addAccount(Account *account) {
            mAccounts.insert(account);
        }

        void transferMoney(Account *accountA, Account *accountB, double amount) {
            accountA->changeMoney(-amount);
            accountB->changeMoney(amount);
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


    [[noreturn]] void randomTransfer(Bank *bank, Account *accountA, Account *accountB) {
        while (true) {
            std::cout << "thread-[" << std::this_thread::get_id() << "] is running." << std::endl;
            double randomMoney = ((double) rand() / RAND_MAX) * 100;
            {
                // 加了条件变量处理后，不用再管转账失败的情况
                // 局部作用域
                std::lock_guard<std::mutex> lockGuard(sCountLock);
                std::cout << "Try to Transfer " << randomMoney
                          << " from " << accountA->getName() << "(" << accountA->getMoney()
                          << ") to " << accountB->getName() << "(" << accountB->getMoney()
                          << "), Bank totalMoney: " << bank->totalMoney() << std::endl;
            }
            bank->transferMoney(accountA, accountB, randomMoney);
        }
    }

    void test11() {
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