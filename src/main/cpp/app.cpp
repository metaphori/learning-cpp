#include <iostream>
#include <stdlib.h>
#include "app.h"

std::string learning_cpp::Greeter::greeting() {
    return std::string("Hello, World!");
}

int main () {
    learning_cpp::Greeter greeter;
    std::cout << greeter.greeting() << std::endl;
    return 0;
}
