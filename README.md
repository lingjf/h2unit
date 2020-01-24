# Introduce
*H2UNIT* is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit) <br>
*H2* in name is the chemical symbol of hydrogen. <br>
*H2UNIT* means a light-weight unit test framework as light as hydrogen. <br>
The special feature is **Dynamic STUB/MOCK** and **JSON**.

# Example
Example is the BEST manual document.

All the code of following features can be found in the [example folder](example/), [test h2unit by h2unit](test/), and it is executable, the result is visable.


# Features
### 1. Header-only Single-file 
Only need to include *1* *ONE* *一* *いち* source file: [**h2unit.h**](h2unit.h) 

**h2unit.h** contains [`main()`](source/h2_unit.h#L299) function, and `main()` will execute test cases.
h2unit user no need to write main() anymore.

### 2. Test Suite and Test Case
[`SUITE`](source/h2_unit.h#L145) is used to define a Test Suite. [`Case`](source/h2_unit.h#L181) is used to define a Test Case inside of Test Suite. <br>
Variables defined in Suite scope are shared by CASEs which can see them. <br>
[Setup()](source/h2_unit.h#L156) is executed before every test case in this test suite. <br>
[Teardown()](source/h2_unit.h#L159) is executed after every test case whatever success or fail. <br>
Setup/Teardown can be omitted. <br>
Each case is executed separately and begin from first of suite scope, 
   shared variables are initialized, shared code is executed until case section, then Setup() is invoked,
   after test code, Teardown() is invoked. <br>
```C++
SUITE(Suite Name)
{
   shared_variables = 0;
   shared_code;

   Setup() {
   };
   Teardown() {
   };

   Case(Case Name 1)
   {
   };
   Case(Case Name 2)
   {
   };
   Todo(Case Name 3)
   {
      /* don't run */
   };
}
```
[`CASE`](source/h2_unit.h#L202) macro is used to define a standalone test case.
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

### 3. Compare
*    [`OK`](source/h2_unit.h#L132)(expr) : check the result of `expr` is true.
*    [`OK`](source/h2_unit.h#L132)(expect, actual) : check the actual value matches with expect value.

### 4. Matcher
*    `_` / `Any()` : matches any value .
*    `Null`() : matches if value is null .
*    `Eq`(expect) : matches if value equals expect .
*    `Ge`(expect) : matches if value >= expect .
*    `Gt`(expect) : matches if value > expect .
*    `Le`(expect) : matches if value <= expect .
*    `Lt`(expect) : matches if value < expect .
*    `Me`(expect) : matches if memcmp(expect, value) .
*    `Pe`(expect) : matches if point to value equals expect .
*    `Re`(expect) : matches if value Regex equals expect .
*    `We`(expect) : matches if value Wildcard equals expect .
*    `Contains`(expect) : matches if value has substr expect .
*    `StartsWith`(expect) : matches if value starts with expect .
*    `EndsWith`(expect) : matches if value ends with expect .
*    `CaseLess`(expect) : make inner matcher case-insensitive .
*    `Je`(expect) : matches if value JSON equals expect .
*    `Not`(expect) : matches if not matches inner matcher .
*    `AllOf`(expect...) : matches if value matches all of inner matchers .
*    `AnyOf`(expect...) : matches if value matches any one of inner matchers .
*    `NoneOf`(expect...) : matches if value not matches all of inner matchers .
*    `ListOf`(expect...) : matches if array value matches inner matchers .

Matcher can be used in OK(expect, actual), for example:
```C++
OK(Ge(1.4142), sqrt(2))
```
It asserts sqrt(2) result 1.41421356237 is greater or equal than 1.4142

### 5. Dynamic STUB
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
void this_is_the_test_case()
{
   foobar = foobar_fake;
   do_something();
   foobar = foobar_impl;
}
```
The most disadvantage of function pointer replacement is changing of product source code. <br>
Objective of Dynamic STUB is same as function pointer replacement, but it is unnecessary to change product source code. <br> 
[STUB](source/h2_unit.h#L278) is easier to use.

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

With help of C++ lambda, separate fake function can sit together with [STUB](source/h2_unit.h#L278), it makes test code more tidy and fitness.

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

### 6. Dynamic MOCK
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

-   [`MOCK`](source/h2_unit.h#L220)
    -    `MOCK`(Function Name, Return Type(Parameter List)) 
          Mock normal function (class static method is considered as normal function)
    -    `MOCK`(Class Name, Method Name, Return Type(Parameter List)) 
          Mock class member function
-   Expectation of call times
    -    `once`() : Expect called 1 time
    -    `twice`() : Expect called 2 times
    -    `times`(n) : Expect called n times
    -    `any`() : Expect called any times(include 0 times)
    -    `atleast`(n) : Expect called atleast n times(>=n)
    -    `atmost`(n) : Expect called atmost n times(<=n)
    -    `between`(n,m) : Expect called >=n and <=m times
-   Arguments check 
    -    `with`(matcher...) : Expect arguments matches matchers
    -    `th1~9`(matcher) : Expect 1st~9th argument matches matcher
-   Actions, only the lastest action works, previous is overwrite.
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
then expect Foo::bar called any times, and 1st argument equals 5, modify 2nd, and inject returns value 44. <br>

MOCK formula:
```C++
   MOCK([Class Name, ]Function Name, Return Type(Parameter List)[, Instantiate object])
   .times(Number).with(Matcher List).th1~9(Matcher).returns(Return Value).does(Lambda)=Lambda;
```
### 7. Extended JSON Compare 

#### 7.1. `'` is same as `"` , it's helpful to write JSON in C/C++ source code 
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

#### 7.2. Matcher *Je* is used to compare JSON
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

[`JE`](source/h2_unit.h#L133) is abbreviated for OK(Je(expect json), actual json)

#### 7.3. math expression is support in JSON, it is evaluated to number when parsing 
```C++
JE("{
         'width': 100 + 10 + 1, 
         'height': 111 * sqrt(4)
    }", rectangle_tojson(rectangle));
```
It expect rectangle width equal to 111, and height equal to 222

#### 7.4. Regex is support in JSON compare
```C++
JE("{
         'name': /Zhang[0-9]+/, 
         'score': { ... }
    }", student_tojson(student));
```

#### 7.5. JSON Compare principle
##### 7.5.1. Value Compare
   Matches when Expect Value is Regex, otherwise Equals .
##### 7.5.2. Object compare
   Every key in Expect Object is Exist in Actual Object, and value equals.
##### 7.5.3. Array compare
   Size is same, and each element equals.


### 8. Memory Check
#### 8.1. Memory Leak Detection 
In order to detect memory leak, h2unit hook malloc,free,new,delete,calloc,realloc,strdup,posix_memalign and other memory allocate functions.

Every test case (wrote by SUITE/CASE and TEST) should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

[`BLOCK`](source/h2_unit.h#L290) can be used to detect memory leak in a code block. p2 is reported as leak in the following:
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

#### 8.2. Memory Faulty Injection
[`BLOCK`](source/h2_unit.h#L290) can used to control the remain memory resource, it can makes malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
CASE(test out of memory)
{
   BLOCK(10) {
      OK(NULL == malloc(11));
   }
}
```
[`BLOCK`](source/h2_unit.h#L290) can used to initialize allocated memory. In following case, p is filled with *ABCABCAB*.
```C++
CASE(test memory initialize)
{
   BLOCK(10, "ABC") {
      char *p = (char *)malloc(8);
   }
}
```

#### 8.3. Memory overflow
Writing out of allocated memory area[start, start+size], memory overflow/underflow will be detected. 

### 9. Help Utilities
*    [`H2DNS`](source/h2_extra.cpp#L103)("ip1", "ip2", ...): Set DNS resolve results (getaddrinfo)

### 10. Human friendly console output

*    Failure is high-lighted
*    Expect and Actual are high-lighted
*    Difference is high-lighted
*    String difference is aligned
*    Memory difference is aligned
*    JSON difference is aligned
*    Memory leak backtrace is printed

# Execute options

*    `-v` *verbose* output including successfull cases
*    `-l` *list* out all suites and cases
*    `-r` execute cases in *random* sequence
*    `-b` *breaking* test once failure occurred
*    `-c` enable/disable *colorfull* output, default is enable
*    `-m` enable/disable *memory* check(leak, overflow, double free), default is enable
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-x` {pattern} *exclude* filter, default is ` ` (exclude nothing)

# Limitations
*    C++11 is required
*    GCC 5.0+
*    Variadic parameter function can't MOCK, use STUB with separate fake function instead


