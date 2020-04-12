
CASE() {}

SUITE() {}

CASE(syntax) {}

SUITE(syntax) {}

SUITE(syntax) {
    Cleanup() {}
}

#define D101 101

SUITE(syntax D101)
{
    Cleanup() {

    }
    Case(D101) {}
    Case(2) {
        int e = 1, a = 1;
        OK(e, a) << e << " not equals " << a;
    }

    Todo(1) {}
    Todo(2) {}
}

GlobalSetup() 
{
    
}

GlobalTeardown() {
}

GlobalSuiteSetup() {

}

GlobalSuiteTeardown() {

}

GlobalCaseSetup() {

}

GlobalCaseTeardown() {

}

TODO(syntax) {}
