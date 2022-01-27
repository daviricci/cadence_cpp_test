#include <iostream>
#include <fstream>
#include "core.h"
#include <string>
#include <stdio.h>

int main() {
    core::Bomb b1(2, 2, 3);
    core::Bomb b2(5, 6, 2);
    core::Bomb b3(9, 9, 1);
    b1.it_is_connected(&b2);
    std::cout<<core::calculate_distance_between_bomb_and_vertical(&b2,5);
    core::Manipulator manipulator("file.txt");
    bool a = true;
    return 0;
}
