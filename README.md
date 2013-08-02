# Introduce
H2UNIT is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit)

It should only involve two source files: **h2unit.h** and **h2unit.cpp**.

"H2" in name is the chemical symbol of hydrogen. "H2UNIT" means a light-weight unit test framework as light as hydrogen. 

The special feature is **Dynamic STUB**.

# Example
Example is the BEST manual document.

All the code of following features can be found in the example folder, and it is executable, the result is visable.


# Features
### 1. Simple Test Case
`H2UNIT_CASE` macro is used to define a independent test case.
```
H2UNIT_CASE("case name")
{
    ......
}
```
### 2. Test Suite and Test Case
`H2UNIT` is used to define a Test Suite. `H2CASE` is used to define a Test Case belong to specified Test Suite. 
setup() is executed before every test case in this test suite. teardown() is executed after every test case whatever success or fail. setup/teardown can be omitted to use default which do nothing.
```
H2UNIT(hello_unit)
{
    void setup() {   
    }
    void teardown() {   
    }
}
H2CASE(hello_unit, "case name 1")
{
}
H2CASE(hello_unit, "case name 2")
{
}
```
### 3. Checkpoint and Compare
*    H2EQ_TRUE(expr) : check the result of `expr` is true.
*    H2EQ_MATH(expected, actually) : check the actually value is same as expected value, value type can be (unsigned)char/(unsigned)short/(unsigned)int/(unsigned)long/(unsigned) long long/float/double.
*    H2EQ_STRCMP(expected, actually) : check the actually string is same as expected string with strcmp.
*    H2EQ_WILDCARD(express, acutally) : check the actually string is wildcard match expected express.
*    H2EQ_REGEX(express, acutally) : check the actually string is regular express match expected express.
*    H2NE_MATH/H2NE_STRCMP/H2NE_WILDCARD/H2NE_REGEX : check the actually result is not same as expected result.
*    H2EQ_RANGE(from, to, actually) : check the actually value is between from and to.
*    H2EQ_INSET(inset, count, actually) : check the actually value is in set.
*    H2NE_RANGE/H2NE_INSET : check the actually value is out of range or set.

### 4. Automatic Register and main()
All the test cases (wrote by H2UNIT/H2CASE and H2UNIT_CASE) are registered to a global list automatically. 

It means engineer should not write extra code to add test cases to a list or array like normal unit test framework: cppunit cmockery and so on.

**h2unit.cpp** contains `main()` function, and `main()` will execute test cases.

### 5. Dynamic STUB
Unit test cases should be easy to maintain. Dependency breaking is the key way. 

Typically, function pointer replacement is a common method to break dependency.
```
// product source code 
int foobar_impl(int a, int b)
{
   ......
   return 0;
}
int (*foobar) (int, int) = foobar_impl;
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
// unit test code
int foobar_stub(int a, int b)
{
   ......
   return 1;
}
void this_is_a_test_case()
{
   foobar = foobar_stub;
   do_something();
   foobar = foobar_impl;
}
```
The most disadvantage of function pointer replacement is changing of product source code.

Objective of Dynamic STUB is same as function pointer replacement, but it is unnecessary to change product source code. It is easier to use.

```
// product source code 
int foobar(int a, int b)
{
   ......
   return 0;
}
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
// unit test code
int foobar_stub(int a, int b)
{
   ......
   return 1;
}
H2UNIT_CASE("demo dynamic stub")
{
   H2STUB(foobar, foobar_stub);
   do_something();
}
```

static function also can be Dynamic STUBed.

```
// product source code 
static int foobar(int a, int b)
{
   ......
   return 0;
}
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
// unit test code
int foobar_stub(int a, int b)
{
   ......
   return 1;
}
H2UNIT_CASE("demo dynamic stub")
{
   H2STUB("foobar", foobar_stub);
   do_something();
}
```

C++ class method can also be Dynamic STUBed, please read the example to find the demo code.
  
### 6. Memory Leak Detection 
In order to detect memory leak, It should include h2unit.h in every source code files.

gcc can use `-include` option to force include h2unit.h:
```
   gcc -include h2unit.h ...
```
Every test case (wrote by H2UNIT/H2CASE and H2UNIT_CASE) should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

`H2LEAK_BLOCK` can be used to detect memory leak in a code block. C2 is leaked in the following:
```
H2UNIT_CASE("memory leak in block")
{
   void * C1, * C2;
   H2LEAK_BLOCK() {
      C1 = malloc(1);
      free(C1);
   }
   H2LEAK_BLOCK() {
      C2 = malloc(2);
   }
   free(C2);
}
```

### 7. Memory overflow/underflow
Writing to the previous and tail of allocated memory, memory overflow/underflow will be detected. 

### 8. Memory Faulty Injection
`H2_FAULTY_INJECT_MEMORY` can used to control the remain memory resource, in other word, it can make malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```
H2CASE(memory_leak, "test memory faulty injection")
{
   H2_FAULTY_INJECT_MEMORY(10);
   H2EQ_TRUE(NULL == malloc(11));
}
```
### 9. Expect C++ exception
`H2CATCH_THROW(expressions, exceptype)` can be used to expect there is a exception will be thrown out while execute `expressions`.

### 10. Color high-lighted console output



