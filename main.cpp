#include <iostream>
#include <fstream>
#include "core.h"


int main() {
    std::ofstream ofs("test.txt");
    core::Bomb b(58888, 178780, 5);
    ofs << b;
    ofs.close();
    core::Bomb b2;
    std::ifstream ifs("test.txt");
    ifs >> b2;
    std::cout<<b2;
    bool a = true;
    return 0;
}
