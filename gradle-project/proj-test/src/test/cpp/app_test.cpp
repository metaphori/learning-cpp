/*
 * This C++ source file was generated by the Gradle 'init' task.
 */

#include<iostream>
#include "lib.hpp"
#include <cassert>

int main() {
    std::cout << "Running test" << std::endl;
    assert(proj_lib::bar().compare("bar") == 0);
    return 0;
}
