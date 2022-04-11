#include "src/concurrency/Coccurrency.h"
#include "src/message/versionA/Test.h"
#include <iostream>

int main() {
    std::cout << "Project Run!" << std::endl;
//    ConcurrencyNS::AllTest();
    MessageBusVersionA::Test();
}