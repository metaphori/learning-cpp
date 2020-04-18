#include <iostream>
//#include <stdlib.h>
#include "app.h"
#include "lib.hpp"

std::string learning_cpp::Greeter::greeting() {
    return std::string("Hello, World!");
}

namespace foo {
    void bar();
}

void foo::bar(){ std::cout << "local foo:bar" << std::endl; }

int main () {
    learning_cpp::Greeter greeter;
    std::cout << greeter.greeting() << std::endl;
    foo::bar(); // NB: foo:bar() would call bar() since "foo:" would be a label
    bar(); // imported from "lib.hpp"
    proj_lib::foo();
    return 0;
}
