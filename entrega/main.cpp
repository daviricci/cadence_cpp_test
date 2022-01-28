#include "core.h"
#include <string>

int main() {
    core::Manipulator manipulator("input.txt");
    manipulator.evaluate();
    system("pause");
    return 0;
}
