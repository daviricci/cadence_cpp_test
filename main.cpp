#include <iostream>
#include <fstream>
#include "core.h"
#include <string>
#include <stdio.h>

int main() {
    core::Manipulator manipulator("file.txt");
    manipulator.evaluate();
    bool a = true;
    return 0;
}
