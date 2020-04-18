#include <iostream>
//#include <stdlib.h>
#include "app.h"

std::string learning_cpp::Greeter::greeting() {
    return std::string("Hello, World!");
}

namespace foo {
    void bar();
}

void foo::bar(){ std::cout << "foo:bar" << std::endl; }

using namespace foo;

int main () {
    learning_cpp::Greeter greeter;
    std::cout << greeter.greeting() << std::endl;
    bar();
    return 0;
}
