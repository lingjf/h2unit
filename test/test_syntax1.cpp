#include "../source/h2_unit.h"
/* clang-format off */

CASE() {}

SUITE() {}

CASE(syntax) {}

SUITE(syntax) {}

SUITE(syntax) {
    Setup() {};
}

SUITE(syntax) {
    Teardown() {};
}

SUITE(syntax)
{
    Setup() {};
    Teardown() {

    };
    Case(1) {};
    Case(2) {};

    Todo(1) {};
    Todo(2) {};
}

GlobalSetup() 
{
    
};

GlobalTeardown() {
};

TODO(syntax) {}

CASE(syntax) {
//    const char* e = "";
//    char t[1024] = {'\0'};
//    OK(e, t);
}
