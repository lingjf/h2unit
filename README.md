# Introduce
H2UNIT is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit)

"H2" in name is the chemical symbol of hydrogen. "H2UNIT" means a light-weight unit test framework as light as hydrogen. 

The special feature is **Dynamic STUB/MOCK** and **JSON**.

# Example
Example is the BEST manual document.

All the code of following features can be found in the [example folder](https://github.com/lingjf/h2unit/tree/master/example), [test h2unit by h2unit](https://github.com/lingjf/h2unit/tree/master/test), and it is executable, the result is visable.


# Features
### 1. Header-only Single-file **h2unit.h** 
Only need to include ONE source file: **h2unit.h** 

**h2unit.h** contains `main()` function, and `main()` will execute test cases.
h2unit user no need to write main() anymore.

### 2. Test Suite and Test Case
`H2UNIT` is used to define a Test Suite. `H2CASE` is used to define a Test Case belong to specified Test Suite. 
setup() is executed before every test case in this test suite. teardown() is executed after every test case whatever success or fail. setup/teardown can be omitted to use default.
```C++
H2UNIT(suite_name)
{
    void setup() {   
    }
    void teardown() {   
    }
}
H2CASE(suite_name, case_name 1)
{
}
H2CASE(suite_name, case_name 2)
{
}
```
`H2UNIT_CASE` macro is used to define a standalone test case.
```C++
H2UNIT_CASE(case_name)
{
    ......
}
```
All the test cases are registered to a global list automatically. 

It means engineer should not write extra code to add test cases to a list or array like normal unit test framework: cppunit cmockery and so on.

### 3. Compare
*    `H2EQ`(expr) : check the result of `expr` is true.
*    `H2EQ`(expect, actual) : check the actual value matches with expect value.

### 4. Matcher
*    `_` / `Any()` : matches any value .
*    `Null`() : matches if value is null .
*    `Eq`(expect) : matches if value equals expect .
*    `Ge`(expect) : matches if value >= expect .
*    `Gt`(expect) : matches if value > expect .
*    `Le`(expect) : matches if value <= expect .
*    `Lt`(expect) : matches if value < expect .
*    `Me`(expect) : matches if memcmp(expect, value) .
*    `Pe` / `PointeeEq`(expect) : matches if point to value equals expect .
*    `Re`(expect) : matches if value Regex equals expect .
*    `We`(expect) : matches if value Wildcard equals expect .
*    `HasSubstr`(expect) : matches if value has substr expect .
*    `StartsWith`(expect) : matches if value starts with expect .
*    `EndsWith`(expect) : matches if value ends with expect .
*    `CaseLess`(expect) : make inner matcher case-insensitive .
*    `Je` / `JsonEq`(expect) : matches if value JSON equals expect .
*    `Not`(expect) : matches if not matches inner matcher .
*    `AllOf`(expect...) : matches if value matches all of inner matchers .
*    `AnyOf`(expect...) : matches if value matches any one of inner matchers .
*    `NoneOf`(expect...) : matches if value not matches all of inner matchers .
*    `ListOf`(expect...) : matches if array value matches inner matchers .

Matcher can be used in H2EQ(expect, actual)
```C++
H2EQ(Ge(expect), actual)
```
### 5. Dynamic STUB
Unit test cases should be easy to maintain. Dependency breaking is the key way. 

Typically, function pointer replacement is a common method to break dependency.
```C++
/* product code */ 
int foobar_impl(int a, char * b)
{
   ......
}
int (*foobar) (int, int) = foobar_impl;
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
```

```C++
/* unit test code */
int foobar_fake(int a, char * b)
{
   ......
   return 1;
}
void this_is_a_test_case()
{
   foobar = foobar_fake;
   do_something();
   foobar = foobar_impl;
}
```
The most disadvantage of function pointer replacement is changing of product source code.

Objective of Dynamic STUB is same as function pointer replacement, but it is unnecessary to change product source code. It is easier to use.

```C++
/* product code */ 
int foobar(int a, char * b)
{
   ......
}
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
```

```C++
/* unit test code */
int foobar_fake(int a, char * b)
{
   ......
   return 1;
}
H2UNIT_CASE("demo dynamic stub with fake function")
{
   H2STUB(foobar, foobar_fake);
   do_something();
}
```

C++ lambda can substitute fake function

```C++
H2UNIT_CASE("demo dynamic stub with lambda")
{
   H2STUB(int, foobar, (int a, char * b)) {
      H2EQ(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something();
}
```

STUB C++ Class member function (normal method and virtual method)

```C++
H2UNIT_CASE("demo dynamic stub with class member function")
{
   H2STUB(int, Foo, bar, (int a, char * b)) {
      H2EQ(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ Class member function (static method)

```C++
H2UNIT_CASE("demo dynamic stub with class member function")
{
   H2STUB(int, Foo::bar, (int a, char * b)) {
      H2EQ(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ Class virtual member function with special constructor

```C++
H2UNIT_CASE("demo dynamic stub with class member function")
{
   H2STUB(int, Foo, bar, (int a, char * b), Foo(...)) {
      H2EQ(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ abstract Class member function

```C++
H2UNIT_CASE("demo dynamic stub with class member function")
{
   class Coo : Foo {}

   H2STUB(int, Foo, bar, (int a, char * b), Coo(...)) {
      H2EQ(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```


The principle of Dynamic STUB is :

Replacing the first several binary code of original function with "JMP" instruction, 
the result is once calling into original function, it will jump to fake function immediately,
parameters of original function in stack will be treated as parameters of fake function,
fake function return to the caller of original function directly.

`H2STUB()` is used to replace original with fake. All replaced binary code will be restored in default teardown phase.

### 6. Dynamic MOCK
Compare to Dynamic STUB, Dynamic Mock provide a easy way to stub function, check input arguments and inject return value.
```C++
/* product code */ 
int foobar(int a, char * b)
{
   ......
}
void do_something()
{
   ......
   z = foobar(x, y);
   ......
}
```

```C++
/* unit test code */
H2UNIT_CASE("demo dynamic mock")
{
   H2MOCK(foobar, int(int, char *)).once(1, "A").returns(11);

   do_something();
}
```
Expect foobar called with a equals 1, b equals "A" 1 time in this case, and make it returns 11.

*    `H2MOCK`(function_name, (arguments type), return type) : Mock normal function
*    `H2MOCK`(class name, method name, (arguments type), return type) : Mock member function
*    `once`() : Expect called 1 time
*    `twice`() : Expect called 2 times
*    `times`(n) : Expect called n times
*    `any`() : Expect called any times(include 0 times)
*    `atleast`(n) : Expect called atleast n times(>=n)
*    `atmost`(n) : Expect called atmost n times(<=n)
*    `between`(n,m) : Expect called >=n and <=m times
*    `with`(matcher...) : Expect arguments matches matchers
*    `th0`(matcher) : Expect 1st argument matches matcher
*    ...
*    `th9`(matcher) : Expect 10th argument matches matcher
*    `returns`(value) : Inject return value
*    `=`(lambda) : check arguments, return value, and other actions

```C++
H2MOCK(Foo, bar, int(int, char *)).once(1, _).returns(11)
                                  .twice(Gt(2), CaseLess("abc")).returns(22)
                                  .times(5).with(Not(3)).returns(33)
                                  .atleast(2).th0(4).th1("xyz")
                                  .any() = [](int a, char * b) {
                                     H2EQ(5, a);
                                     sprintf(b, "return value by argument");
                                     return 44;
                                  };
```
First expect Foo::bar called 1 time, and 1st argument equals 1, any of 2nd, and inject returns value 11;

then expect Foo::bar called 2 times, and 1st argument geat than 2, 2nd case-insensitive equals "abc", and inject returns value 22;

then expect Foo::bar called 5 times, and 1st argument not equals 3, ignore check 2nd (any of 2nd), and inject returns value 33;

then expect Foo::bar called atleast 2 times, and 1st argument equals 4, 2nd equals "xyz";

then expect Foo::bar called any times, and 1st argument equals 5, modify 2nd, and inject returns value 44.


### 7. Memory Leak Detection 
In order to detect memory leak, h2unit hook malloc,free,calloc,realloc,strdup,posix_memalign and other memory allocate functions.

Every test case (wrote by H2UNIT/H2CASE and H2UNIT_CASE) should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

`H2BLOCK` can be used to detect memory leak in a code block. p2 is leaked in the following:
```C++
H2UNIT_CASE(memory leak in block)
{
   void * p1, * p2;
   H2BLOCK() {
      p1 = malloc(1);
      free(p1);
   }
   H2BLOCK() {
      p2 = malloc(2);
   }
   free(p2);
}
```

### 8. Memory Faulty Injection
`H2BLOCK` can used to control the remain memory resource, in other word, it can make malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
H2CASE(memory_leak, "test memory faulty injection")
{
   H2BLOCK(10) {
      H2EQ_TRUE(NULL == malloc(11));
   }
}
```
`H2BLOCK` can used to initialize memory of allocated memory. In following case, *p is filled with ABC.
```C++
H2CASE(memory_leak, "test memory faulty injection")
{
   H2BLOCK(10, "ABC") {
      char *p = (char *)malloc(9);
   }
}
```

### 9. Memory overflow
Writing out of allocated memory area[start, start+size], memory overflow/underflow will be detected. 

### 10. Help Utilities
*    `H2DNS`(ip1, ip2, ...): Set DNS resolve results (getaddrinfo)

### 11. Human friendly console output

*    Failure is high-lighted
*    Expect and Actual are high-lighted
*    Difference is high-lighted
*    String difference is aligned
*    Memory difference is aligned
*    JSON difference is aligned
*    Memory leak backtrace is printed

### 12. Execute options

*    `-v` *verbose* output including successfull cases
*    `-r` execute cases in *random* sequence
*    `-c` enable/disable *colorfull* output, default is enable
*    `-m` enable/disable *memory* check(leak, overflow, double free), default is enable
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, HasSubString used. Default is `*` (include all)
*    `-x` {pattern} *exclude* filter, default is ` ` (exclude nothing)


