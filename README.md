# Introduce
*H2UNIT* is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit) <br>
*H2* in name is the chemical symbol of hydrogen. <br>
*H2UNIT* means a light-weight unit test framework as light as hydrogen. <br>
The special features is :

### 1. Header-only Single-file 
Only need to include *1* *ONE* *一* *いち* source file: [**h2unit.h**](h2unit.h) 

**h2unit.h** contains [`main()`](source/h2_unit.h#L283) function, and `main()` will execute test cases.
user no need to write main() anymore.

### 2. Dynamic STUB/MOCK

### 3. JSON Compare

# Example
Example is the BEST manual document.

All the code of following features can be found in the [example folder](example/), [test h2unit by h2unit](test/), and it is executable, the result is visable.


# Features

### 1. Test Suite and Test Case
[`SUITE`](source/h2_unit.h#L96) is used to define a Test Suite. [`Case`](source/h2_unit.h#L96) is used to define a Test Case inside of Test Suite. <br>
Variables defined in Suite scope are shared by cases which can see them. <br>
[`Cleanup`](source/h2_unit.h#L96) is executed after every test case whatever success or fail. <br>
Each case is executed separately and begin from first of suite scope, 
   shared variables are initialized, then shared code is executed until case section,
   after test code, Cleanup() is invoked to release resources. <br>
```C++
SUITE(Suite Name)
{
   shared_variables = 0;
   shared_code as setup;

   Cleanup() {
   }

   Case(Case Name 1)
   {
   }
   Case(Case Name 2)
   {
   }
   Todo(Case Name 3)
   {
      /* don't run */
   }

   shared_code as cleanup;
}
```
[`CASE`](source/h2_unit.h#L96) macro is used to define a standalone test case.
```C++
CASE(Case Name)
{
    ......
}
```
```C++
TODO(Case Name)
{
    ...... /* don't run */
}
```
All the test cases are registered to a global list automatically (C++ global object's constructor is invoked automatically before main). 

It means user should not write extra code to register test cases.

### 2. Compare
*    [`OK`](source/h2_unit.h#L96)(expr) : check the result of `expr` is true.
*    [`OK`](source/h2_unit.h#L96)(expect, actual) : check the actual value matches with expect value.

Add user specified error description by following << operator.
```C++
CASE(Case Name)
{
    OK(1, 2) << 1 << "!=" << 2;
}
```
### 3. Matcher
*    [`_`](source/h2_unit.h#L96) / [`Any`](source/h2_unit.h#L96) : matches any value .
*    [`IsNull`](source/h2_unit.h#L96) : matches if value is null .
*    [`NotNull`](source/h2_unit.h#L96) : matches if value is not null .
*    [`IsTrue`](source/h2_unit.h#L96) : matches if value is true .
*    [`IsFalse`](source/h2_unit.h#L96) : matches if value is false .
*    [`Eq`](source/h2_unit.h#L96)(expect) : matches if value equals expect (one of [strcmp wildcard regex] equals for string compare).
*    [`Nq`](source/h2_unit.h#L96)(expect) : matches if value not equals expect .
*    [`Ge`](source/h2_unit.h#L96)(expect) : matches if value >= expect .
*    [`Gt`](source/h2_unit.h#L96)(expect) : matches if value > expect .
*    [`Le`](source/h2_unit.h#L96)(expect) : matches if value <= expect .
*    [`Lt`](source/h2_unit.h#L96)(expect) : matches if value < expect .
*    [`Me`](source/h2_unit.h#L96)(expect) : matches if memcmp(expect, value) .
*    [`Re`](source/h2_unit.h#L96)(expect) : matches if value Regex equals expect .
*    [`We`](source/h2_unit.h#L96)(expect) : matches if value Wildcard equals expect .
*    [`Je`](source/h2_unit.h#L96)(expect) : matches if value JSON equals expect .
*    [`Se`](source/h2_unit.h#L96)(expect) : matches if string strictly equals expect as strcmp() .
*    [`Contains`](source/h2_unit.h#L96)(expect) : matches if value has substr expect .
*    [`StartsWith`](source/h2_unit.h#L96)(expect) : matches if value starts with expect .
*    [`EndsWith`](source/h2_unit.h#L96)(expect) : matches if value ends with expect .
*    [`~`](source/h2_unit.h#L96) / [`CaseLess`](source/h2_unit.h#L96)(expect) : make inner matcher case-insensitive, right operator must be Matcher, `~"Hello World"` not works .
*    [`Pointee`](source/h2_unit.h#L96)(expect) : matches if point to value equals expect .
*    [`!`](source/h2_unit.h#L96) / [`Not`](source/h2_unit.h#L96)(expect) : matches if not matches inner matcher, right operator must be Matcher, !3 is considered as normal semantics .
*    [`&&`](source/h2_unit.h#L96)(expect) : Logical AND of two matchers, left and right operator shoud at least one Matcher .
*    [`||`](source/h2_unit.h#L96)(expect) : Logical OR of two matchers .
*    [`AllOf`](source/h2_unit.h#L96)(expect...) : matches if value matches all of inner matchers, act as AND logical operator.
*    [`AnyOf`](source/h2_unit.h#L96)(expect...) : matches if value matches any one of inner matchers, act as OR logical operator.
*    [`NoneOf`](source/h2_unit.h#L96)(expect...) : matches if value not matches all of inner matchers .
*    [`ListOf`](source/h2_unit.h#L96)(expect...) : matches if array value matches inner matchers .

Matcher can be used in OK(expect, actual), for example:
```C++
OK(Ge(1.4142), sqrt(2))
```
It asserts sqrt(2) result 1.41421356237 is greater or equal than 1.4142

#### 3.1. User defined Matcher
```C++
MATCHER(Between, left, right, (a << " not in [" << left << ", " << right << "]"))
{
   return left <= a && a <= right; 
}
```
Define a new Matcher named 'Between', with 2 arguments left and right in constructor.
In matcher body, specific `a` is actual value variable name.
If matches fail, error message is result of 
```C++
std::cout << a << " not in [" << left << ", " << right << "]";
```

in following case, left is 1, right is 4, a is 5, error message is `"5 not in [1, 4]"`.

```C++
Case(test user defined matcher)
{
   OK(Between(1, 4), 5);
}
```

### 4. Dynamic STUB
Unit test cases should be easy to maintain. Dependency breaking is the key way. 

Typically, function pointer replacement is a common solution to break dependency.
```C++
/* product code */ 
int foobar_impl(int a, char * b)
{
   ......
}
int (*foobar) (int a, char * b) = foobar_impl;
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
The most disadvantage of function pointer replacement is changing of product source code. <br>
Objective of Dynamic STUB is same as function pointer replacement, but it is unnecessary to change product source code. <br> 
[STUB](source/h2_unit.h#L96) is easier to use.

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
   OK(1, a);
   sprintf(b, "return value by argument");
   return 1;
}
CASE(demo dynamic stub with fake function)
{
   STUB(foobar, foobar_fake);
   do_something();
}
```

STUB template function or overload function

```C++
double sin_fake(double x)
{
   return 3.14;
}
CASE(demo dynamic stub with fake function)
{
   STUB((double(*)(double))sin, sin_fake);
   do_something_with_call_sin();
}
```

With help of C++ lambda, separate fake function can sit together with [STUB](source/h2_unit.h#L96), it makes test code more tidy and fitness.

```C++
CASE(demo dynamic stub with lambda)
{
   STUB(int, foobar, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something();
}
```

STUB C++ Class nonstatic or virtual member function

```C++
CASE(demo dynamic stub with class member function)
{
   STUB(int, Foo, bar, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ Class static member function

```C++
CASE(demo dynamic stub with class member function)
{
   STUB(int, Foo::bar, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ Class virtual member function with quite special constructor which H2UNIT can't guess. <br>
If Class has default constructor, or simple constructor which H2UNIT can guess, Instantiate can be omitted.

```C++
CASE(demo dynamic stub with class member function)
{
   STUB(int, Foo, bar, (int a, char * b), Foo(...)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

STUB C++ abstract Class member function

```C++
CASE(demo dynamic stub with class member function)
{
   class Coo : Foo {...}

   STUB(int, Foo, bar, (int a, char * b), Coo(...)) {
      OK(1, a);
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

`STUB()` is used to replace original with fake. All replaced binary code will be restored in default teardown phase.

STUB formula:
```C++
   STUB(Return Type[, Class Name], Function Name, (Parameter List)[, Instantiate object]) {
      Check...
      Return...
   }
```

clang >= version 11, default make code section (__TEXT) max protect (maxprot) `rx`, mprotect() in STUB fails.
To fix it, add `-Wl,-segprot,__TEXT,rwx,rwx` in link(ld) option to make maxprot `rwx`. 

### 5. Dynamic MOCK
Compare to Dynamic STUB, Dynamic Mock provide a easy way to check call times, input arguments and inject return value.

```C++
/* unit test code */
CASE(demo dynamic mock)
{
   MOCK(foobar, int(int a, char * b)).once(1, "A").returns(11);

   do_something();
}
```
Expect foobar called with *a* equals *1*, *b* equals *"A"* *1 time* in this case, and make it returns *11*.

-   [`MOCK`](source/h2_unit.h#L96)
    -    `MOCK`(Function Name, Return Type(Parameter List)) 
          Mock normal function (class static method is considered as normal function)
    -    `MOCK`(Class Name, Method Name, Return Type(Parameter List)) 
          Mock class member function
-   [Expectation of call times](source/h2_mock.hpp#L157)
    -    `once`() : Expect called 1 time
    -    `twice`() : Expect called 2 times
    -    `times`(n) : Expect called n times
    -    `any`() : Expect called any times(include 0 times)
    -    `atleast`(n) : Expect called atleast n times(>=n)
    -    `atmost`(n) : Expect called atmost n times(<=n)
    -    `between`(n,m) : Expect called >=n and <=m times
-   [Arguments check](source/h2_mock.hpp#L206)
    -    `with`(matcher...) : Expect arguments matches matchers
    -    `th1~9`(matcher) : Expect 1st~9th argument matches matcher
-   [Actions](source/h2_mock.hpp#L223), only the lastest action works, previous is overwrite.
    -    `returns`(value) : Inject return value
    -    `does`(lambda) : Check arguments, inject return value, and other actions
    -    `=`(lambda) : Same as *does* without *()*

```C++
MOCK(Foo, bar, int(int, char *)).once(1, _).returns(11)
                                .twice(Gt(2), CaseLess("abc")).returns(22)
                                .times(5).with(Not(3)).returns(33)
                                .atleast(2).th1(4).th2("xyz")
                                .any() = [](int a, char * b) {
                                   OK(5, a);
                                   sprintf(b, "return value by argument");
                                   return 44;
                                };
```
First expect Foo::bar called 1 time, and 1st argument equals 1, any of 2nd, and inject returns value 11; <br>
then expect Foo::bar called 2 times, and 1st argument geat than 2, 2nd case-insensitive equals "abc", and inject returns value 22; <br>
then expect Foo::bar called 5 times, and 1st argument not equals 3, ignore check 2nd (any of 2nd), and inject returns value 33; <br>
then expect Foo::bar called atleast 2 times, and 1st argument equals 4, 2nd equals "xyz"; <br>
then expect Foo::bar called any times, and 1st argument equals 5, modify 2nd, and inject returns value 34. <br>

MOCK formula:
```C++
   MOCK([Class Name, ]Function Name, Return Type(Parameter List)[, Instantiate object])
   .times(Number).with(Matcher List).th1~9(Matcher).returns(Return Value).does(Lambda)=Lambda;
```
### 6. Extended JSON Compare 

#### 6.1. `'` is same as `"` , it's helpful to write JSON in C/C++ source code 
```Python
"{
   'name' : 'Zhang3',
   'score' : {
      'Math' : 100,
      'English' : 99
   }
}"
```
is same as standard JSON but without escape character
```C++
"{
   \"name\" : \"Zhang3\",
   \"score\" : {
      \"Math\" : 100,
      \"English\" : 99
   }
}"
```

#### 6.2. Matcher *Je* is used to compare JSON
```C++
OK(Je("{
          'name': 'Zhang3', 
          'score': { ... }
      }"), student_tojson(student));

MOCK(foobar, int(int a, char * b)).once(1, Je("{
                                                   'name': 'Zhang3', 
                                                   'score': { ... }
                                              }");
```

[`JE`](source/h2_unit.h#L96) is abbreviated for OK(Je(expect json), actual json)

#### 6.3. math expression is support in JSON, it is evaluated to number when parsing 
```C++
JE("{
         'width': 100 + 10 + 1, 
         'height': 111 * sqrt(4)
    }", rectangle_tojson(rectangle));
```
It expect rectangle width equal to 111, and height equal to 222

#### 6.4. Regex is support in JSON compare
```C++
JE("{
         'name': /Zhang[0-9]+/, 
         'score': { ... }
    }", student_tojson(student));
```

#### 6.5. JSON Compare principle
##### 6.5.1. Value Compare
   Matches when Expect Value is Regex, otherwise Equals .
##### 6.5.2. Object compare
   Every key in Expect Object is Exist in Actual Object, and value equals.
##### 6.5.3. Array compare
   Size is same, and each element equals.


### 7. Memory Check
#### 7.1. Memory Leak Detection 
In order to detect memory leak, h2unit hook malloc,free,new,delete,calloc,realloc,strdup,posix_memalign and other memory allocate functions.

Every test case should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

[`BLOCK`](source/h2_unit.h#L96) can be used to detect memory leak in a code block. p2 is reported as leak in the following:
```C++
CASE(memory leak in block)
{
   void * p1, * p2;
   BLOCK() {
      p1 = malloc(1);
      free(p1);
   }
   BLOCK() {
      p2 = malloc(2);
   }
   free(p2);
}
```

#### 7.2. Memory Faulty Injection
[`BLOCK`](source/h2_unit.h#L96) can used to control the remain memory resource, it can makes malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
CASE(test out of memory)
{
   BLOCK(10) {
      OK(NULL == malloc(11));
   }
}
```
[`BLOCK`](source/h2_unit.h#L96) can used to initialize allocated memory. In following case, p is filled with *ABCABCAB*.
```C++
CASE(test memory initialize)
{
   BLOCK(10, "ABC") {
      char *p = (char *)malloc(8);
   }
}
```

#### 7.3. Memory overflow
Writing out of allocated memory area[start, start+size], memory overflow/underflow will be detected. 

### 8. Global Setup/Teardown
*    [`GlobalSetup`](source/h2_unit.h#L96): Invoked before test case.
*    [`GlobalTeardown`](source/h2_unit.h#L96): Invoked after all test case executed.
*    [`GlobalSuiteSetup`](source/h2_unit.h#L96): Invoked before every suite.
*    [`GlobalSuiteTeardown`](source/h2_unit.h#L96): Invoked after every suite executed.
*    [`GlobalCaseSetup`](source/h2_unit.h#L96): Invoked before every case.
*    [`GlobalCaseTeardown`](source/h2_unit.h#L96): Invoked after every case executed.
```C++
GlobalSetup() {
   WSAStartup();
};

GlobalTeardown() {
   WSACleanup();
};
```
Global Setup/Teardown can define multiple times, all of them will be invoked.

### 9. DNS Hijack
[`DNS`](source/h2_unit.h#L96)("hostname", "ip1", "ip2", "alias1", "alias2", ...): Set DNS resolve results (getaddrinfo, gethostbyname)

DNS resolve is default controlled, and result is empty.
```C++
CASE(test dns)
{
   DNS("192.168.1.23"); // Resolve all domains to 192.168.1.23
   DNS("h2unit.com", "192.168.1.23", "1.2.3.4"); // Resolve h2unit.com to 192.168.1.23 1.2.3.4
   DNS("h2unit.com", "d1.h2unit.com", "d2.h2unit.com"); // Resolve h2unit.com to cname d1.h2unit.com d2.h2unit.com
   getaddrinfo(...);
}
```

### 10. Socket Hijack
*    [`SOCK`](source/h2_unit.h#L96)(): Monitor TCP/UDP send/recv, and return sent packets.
*    [`SOCK`](source/h2_unit.h#L96)(packet, size, [from, [to]]): Inject UDP/TCP packet as received packet.
If not specified `to`, any of socket can receive the packet.

If not specified `from`, the packet is received from where last send(to).
```C++
CASE(test net)
{
   H2SOCK(); // Start Hook Socket API

   sendto(sock, ...);
   OK(..., H2SOCK()); // Fetch outgoing packet

   H2SOCK(buffer1, 100, "4.3.2.1:8888", "*:4444"); // Inject as received packet from 4.3.2.1:8888 to local port 4444 socket
   H2SOCK(buffer2, 100, "4.3.2.1:8888"); // Inject as received packet from 4.3.2.1:8888 to any local socket
   H2SOCK(buffer3, 100); // Inject as received packet from last sendto peer to any local socket
   recvfrom(sock, ...);
}
```

### 11. Capture STDOUT/STDERR/syslog
[`COUT`](source/h2_unit.h#L96)(): Capture STDOUT STDERR and syslog output (printf(), std::cout<<, ...).
*    `COUT`(""): Start Capture STDOUT and STDERR.
*    `COUT`("stdout stderr syslog"): Start Capture STDOUT STDERR and syslog.
*    `COUT`("STDOUT"): Start Capture STDOUT only.
*    `COUT`("STDerr"): Start Capture STDERR only.
*    `COUT`(): Stop Capture, and return buffer captured.

```C++
CASE(test printf)
{
   COUT(""); // Start Capture
   printf("...");
   std::cout << ...;
   OK("...", COUT()); // Stop Capture and return captured string
}
```

### 12. Parameterized test
*    CASES(name, values...): Automatically generate CASE with each value (access by x)
*    Cases(name, values...): Automatically generate Case inside of SUITE
*    CASESS(name, values...): Automatically generate CASE with fullmesh value (access by x, y)
*    Casess(name, values...): Automatically generate Case inside of SUITE
```C++
CASES(test foreach, 1,2,3) // Generate 3 CASE : x=1;x=2;x=3
{
   OK(x...);
}
SUITE(suite)
{
   Cases(test foreach, 1,2,3) // Generate 3 Case : x=1;x=2;x=3
   {
      OK(x...);
   }
}
CASESS(test (1,2,3)x(1,2,3) fullmesh, 1,2,3) 
{                              // Generate 9 CASE : x,y=(1,1);(1,2);(1,3);
   OK(x...y);                  //                       (2,1);(2,2);(2,3);
}                              //                       (3,1);(3,2);(3,3);
CASESS(test (1,2,3)x(4,5,6) fullmesh, (1,2,3), (4,5,6)) 
{                              // Generate 9 CASE : x,y=(1,4);(1,5);(1,6);
   OK(x...y);                  //                       (2,4);(2,5);(2,6);
}                              //                       (3,4);(3,5);(3,6);
SUITE(suite)
{
   Casess(test, 1,2,3)            // Generate 9 CASE : (1,2,3)x(1,2,3)
   {
      OK(x...y);
   }
   Casess(test, (1,2,3), (4,5,6)) // Generate 9 CASE : (1,2,3)x(4,5,6)
   {
      OK(x...y);
   }
}
```

*    ForForEach(Macro, values...): Call Macro(value)
*    ForFullmesh(Macro, values...): Call Macro(values x values)
*    ForFullmesh(Macro, (x-values...), (y-values...)): Call Macro(x-values x y-values)

### 13. Human friendly console output

*    Failure is high-lighted
*    Expect and Actual are high-lighted
*    Difference is high-lighted
*    String difference is aligned
*    Memory difference is aligned
*    JSON difference is aligned
*    Memory leak backtrace is printed

# Execute options

*    `-v` *verbose* output including successfull cases
*    `-l` [sca] *list* out suites and cases
*    `-r` [sca] execute cases in *random* order
*    `-b` [n] *breaking* test once failure occurred
*    `-c` enable/disable *colorfull* output, default is enable
*    `-m` enable/disable *memory* check(leak, overflow, trample, double free), default is enable
*    `-d/D` *Debug* mode, -D for gdb attach but requires password
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-x` {pattern} *exclude* filter, default is ` ` (exclude nothing)

# Limitations
*    C++11 is required
*    GCC 5.5+ (regex support, SFINAE support)
*    Variadic parameter function can't MOCK, use STUB with separate fake function instead.
*    sqrt() in math.h can be STUB/MOCK, because compiler insert sqrtsd ASM instruction directly instead of function call.
