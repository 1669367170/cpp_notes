#include "factory/SimpleFactory.h"
#include "factory/FactoryMethod.h"

namespace DesignPatternNS {
    void TestFactory() {
        std::cout << "Simple Factory" << std::endl;
        SimpleTankFactory *simpleTankFactory = new SimpleTankFactory();
        Tank *tank56 = simpleTankFactory->createTank(TankType::TANK_TYPE_56);
        tank56->type();
        Tank *tank96 = simpleTankFactory->createTank(TankType::TANK_TYPE_96);
        tank96->type();

        delete tank56;
        tank56 = nullptr;
        delete tank96;
        tank96 = nullptr;
        delete simpleTankFactory;
        simpleTankFactory = nullptr;

        std::cout << "Factory Method" << std::endl;
        Tank56Method *tank56Method = new Tank56Method();
        Tank *tank56_1 = tank56Method->createTank();
        tank56_1->type();

        Tank96Method *tank96Method = new Tank96Method();
        Tank *tank96_1 = tank96Method->createTank();
        tank96_1->type();

        delete tank56_1;
        delete tank96_1;
        delete tank56Method;
        delete tank96Method;
        tank56_1 = nullptr;
        tank96_1 = nullptr;
        tank56Method = nullptr;
        tank96Method = nullptr;
    }

    void Test() {
        TestFactory();
    }
}