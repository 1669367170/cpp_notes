// [简单工厂模式]
// 主要特点是需要在工厂类中做判断，从而创造相应的产品，当增加新产品时，需要修改工厂类。使用简单工厂模式，我们只需要知道具体的产品型号就可以创建一个产品。
// 缺点：工厂类集中了所有产品类的创建逻辑，如果产品量较大，会使得工厂类变的非常臃肿。

#pragma once

#include <string>
#include <iostream>

// 产品类型
enum class TankType {
    TANK_TYPE_56,
    TANK_TYPE_96,
    TANK_TYPE_NUM
};

// 抽象产品类
class Tank {
public:
    virtual const std::string &type() = 0;
};

// 具体的产品类
class Tank56 : public Tank {
public:
    Tank56() : Tank(), m_strType("TANK56") {}

    const std::string &type() override {
        std::cout << m_strType.data() << std::endl;
        return m_strType;
    }

private:
    std::string m_strType;
};

class Tank96 : public Tank {
public:
    Tank96() : Tank(), m_strType("TANK96") {}

    const std::string &type() override {
        std::cout << m_strType.data() << std::endl;
        return m_strType;
    }

private:
    std::string m_strType;
};

// 工厂类
class SimpleTankFactory {
public:
    // 根据产品信息创建具体的产品类实例，返回一个抽象产品类
    Tank *createTank(TankType type) {
        switch (type) {
            case TankType::TANK_TYPE_56:
                return new Tank56();
            case TankType::TANK_TYPE_96:
                return new Tank96();
            default:
                return nullptr;
        }
    }
};
