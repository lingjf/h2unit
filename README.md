<a id="top"></a>
[![Github Releases](https://img.shields.io/github/release/lingjf/h2unit.svg)](https://github.com/catchorg/h2unit/releases)
[![Selftest status](https://github.com/lingjf/h2unit/actions/workflows/ci.yml/badge.svg)](https://github.com/lingjf/h2unit/actions/workflows/ci.yml)


# Introduce
*H2UNIT* is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit) <br>
*H2* in name is the chemical symbol of hydrogen. <br>
*H2UNIT* means a light-weight unit test framework as light as hydrogen. <br>
The special features is :

### 1. Header-only Single-file 
Only need to include *1* *ONE* *一* *いち* source file: [**h2unit.h**](h2unit.h) 

**h2unit.h** contains [`main()`](source/h2_unit.cpp#L60) function, and `main()` will execute test cases.
user no need to write main() anymore.

### 2. Dynamic MOCK

### 3. JSON Compare

# Example
Example is a helpful manual document.

All the code of following features can be found in the [example folder](example/), [test h2unit by h2unit](test/), and it is executable, the result is visable.


# Features

### 1. Test Suite and Test Case
[`SUITE`](source/core/h2_core.hpp) is used to define a Test Suite. [`Case`](source/core/h2_core.hpp) is used to define a Test Case inside of Test Suite. <br>
Variables defined in Suite scope are shared by cases which can see them. <br>
[`Setup`](source/core/h2_core.hpp) is executed before every test case. <br>
[`Cleanup`](source/core/h2_core.hpp) is executed after every test case whatever success or fail. <br>
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
[`CASE`](source/core/h2_core.hpp) macro is used to define a standalone test case.
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
*    [`OK`](source/assert/h2_assert.hpp)(expr) : check the result of `expr` is true.
*    [`OK`](source/assert/h2_assert.hpp)(expect, actual) : check the actual value matches with expect value.
*    [`Ok`](source/assert/h2_assert.hpp)(expect < <= > >= == != actual) : compare the actual value with expect value, only support six operator(< <= > >= == !=).(lowercase k)

Add user specified error description by following << operator.
```C++
CASE(Case Name)
{
    OK(1, 2) << 1 << "!=" << 2;
}
```
### 3. Matcher
*    [`_`](source/h2_unit.hpp#L321) / [`Any`](source/h2_unit.hpp#L321) : matches any value 
*    [`IsNull`](source/h2_unit.hpp#L321) : matches if value is null 
*    [`NotNull`](source/h2_unit.hpp#L321) : matches if value is not null 
*    [`IsTrue`](source/h2_unit.hpp#L321) : matches if value is true 
*    [`IsFalse`](source/h2_unit.hpp#L321) : matches if value is false 
*    [`Eq`](source/h2_unit.hpp#L321)(expect [, epsilon]) : matches if value equals expect (one of [strcmp wildcard regex] equals for string compare), float value near equals expect, default epsilon is 0.00001 
*    [`Nq`](source/h2_unit.hpp#L321)(expect) : matches if value not equals expect 
*    [`Ge`](source/h2_unit.hpp#L321)(expect) : matches if value >= expect 
*    [`Gt`](source/h2_unit.hpp#L321)(expect) : matches if value > expect 
*    [`Le`](source/h2_unit.hpp#L321)(expect) : matches if value <= expect 
*    [`Lt`](source/h2_unit.hpp#L321)(expect) : matches if value < expect 
*    [`Me`](source/h2_unit.hpp#L321)(expect, [length]) : matches if memcmp(expect, value, length) == 0 
     *    [`M1e`](source/h2_unit.hpp#L321)(expect, [length]) : bit level memcmp
     *    [`M8e`](source/h2_unit.hpp#L321)(expect, [length]) : byte level memcmp
     *    [`M16e`](source/h2_unit.hpp#L321)(expect, [length]) : int16 level memcmp
     *    [`M32e`](source/h2_unit.hpp#L321)(expect, [length]) : int32 level memcmp
     *    [`M64e`](source/h2_unit.hpp#L321)(expect, [length]) : int64 level memcmp
*    [`Re`](source/h2_unit.hpp#L321)(expect) : matches if value Regex equals expect 
*    [`We`](source/h2_unit.hpp#L321)(expect) : matches if value Wildcard equals expect 
*    [`Je`](source/h2_unit.hpp#L321)(expect) : matches if value JSON equals expect 
*    [`Se`](source/h2_unit.hpp#L321)(expect) : matches if string strictly equals expect as strcmp() 
*    [`Substr`](source/h2_unit.hpp#L321)(expect) : matches if value has substr expect 
*    [`StartsWith`](source/h2_unit.hpp#L321)(expect) : matches if value starts with expect 
*    [`EndsWith`](source/h2_unit.hpp#L321)(expect) : matches if value ends with expect 
*    [`~`](source/h2_unit.hpp#L321) / [`CaseLess`](source/h2_unit.hpp#L321)(expect) : make inner matcher case-insensitive, right operator must be Matcher, `~"Hello World"` not works 
*    [`Pointee`](source/h2_unit.hpp#L321)(expect) : matches if point to value equals expect 
*    [`!`](source/h2_unit.hpp#L321) / [`Not`](source/h2_unit.hpp#L321)(expect) : matches if not matches inner matcher, right operator must be Matcher, !3 is considered as normal semantics 
*    [`&&`](source/h2_unit.hpp#L321)(expect) : Logical AND of two matchers, left and right operator shoud at least one Matcher 
*    [`||`](source/h2_unit.hpp#L321)(expect) : Logical OR of two matchers 
*    [`AllOf`](source/h2_unit.hpp#L321)(expect...) : matches if value matches all of inner matchers, act as AND logical operator
*    [`AnyOf`](source/h2_unit.hpp#L321)(expect...) : matches if value matches any one of inner matchers, act as OR logical operator
*    [`NoneOf`](source/h2_unit.hpp#L321)(expect...) : matches if value not matches all of inner matchers 
*    [`ListOf`](source/h2_unit.hpp#L321)(expect...) : matches if sequence container(array, vector, ...) item matches inner matchers 
*    [`CountOf`](source/h2_unit.hpp#L321)(expect...) : matches if container(array, vector, ...) item count matches inner matchers 
*    [`Have`/`Has`](source/h2_unit.hpp#L321)(expect ...) : matches if there are items in container(vector, set, map, ...) match every inner matchers
*    [`In`](source/h2_unit.hpp#L321)(expect...) : matches if acutal value matches any of inner matchers 

Matcher can be used in OK(expect, actual), for example:
```C++
OK(Ge(1.4142), sqrt(2))
```
It asserts sqrt(2) result 1.41421356237 is greater or equal than 1.4142

#### 3.1. Memcmp matcher
Expection is described by buffer and length explicitly, comparsion is same with libc memcmp.
```C++
CASE(memory compare)
{
   unsigned char e1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
   unsigned char *a1 = ...
   OK(Me(e1, 5), a1);
   
   const char *e2 = "\x8E\xC8\x8E\xC8\xF8";
   unsigned char *a2 = ...
   OK(Me(e2, 5), a2);
}
```
##### 3.1.1. Compare width

- `M1e` bit width
- `M8e` byte width
- `M16e` word width (2 bytes) 
- `M32e` dword width (4 bytes) 
- `M64e` qword width (8 bytes) 

`Me` deduce compare width by data type or string format.
|  data type             |  width   |  matcher  |
|  ----                  |  ----    |  ----     |
|  unsigned char *       |  1 bytes |  M8e      |
| [unsigned] short *     |  2 bytes |  M16e     |
| [unsigned] int *       |  4 bytes |  M32e     |
| [unsigned] long long * |  8 bytes |  M64e     |

```C++
const char* e3 = "1000 1110 1";
```
For [const] char * type, "01" formatted string is considered as M1e, others is M8e.

##### 3.1.2. String expection deduce

```C++
const char* e4 = "8EC88EC8F8";
const char* e5 = "8PC88EC8F8";
```
All characters in e4 are hexidecimal characters, so e4 has two possible:
- {'8', 'E', 'C', '8', '8', 'E', 'C', '8', 'F', '8'} maximal length 10 bytes
- {0x8E, 0xC8, 0x8E, 0xC8, 0xF8} maximal length 5 bytes

Any of possible matches, Me matches.

There is not hexidecimal character `P` in e5, so e5 has only one possible:
- {'8', 'P', 'C', '8', '8', 'E', 'C', '8', 'F', '8'} maximal length 10 bytes

```C++
const char* e6 = "1000 1110 1";
```
All characters in e6 are binary integers, so e6 has two possible:
- {'1', '0', '0', '0', ' ', '1', '1', '1', '0', ' ', '1'} maximal length 11 bytes
- {0x8E, 0x80} maximal length 9 bits

Why not provide hexidecimal possible i.e. {0x10, 0x00, 0x11, 0x10, 0x1} maximal length 5 bits ?

User write "01" string, typically want bit level comparasion.


If length is not specified explicitly, maximal length is used.

##### 3.1.3. Compare length

*    [`M1e`](source/h2_unit.hpp#L321)(expect, [length]) : number of bits.
*    [`M8e`](source/h2_unit.hpp#L321)(expect, [length]) : number of bytes.
*    [`M16e`](source/h2_unit.hpp#L321)(expect, [length]) : number of int16.
*    [`M32e`](source/h2_unit.hpp#L321)(expect, [length]) : number of int32.
*    [`M64e`](source/h2_unit.hpp#L321)(expect, [length]) : number of int64.
*    [`Me`](source/h2_unit.hpp#L321)(expect, [length]) : following deduced type.

#### 3.2. Array size

OK(expect, actual, [size])

the third argument in OK is used to specified C/C++ generic array size,
for STL containers it is not necessary.

```C++
CASE(memory compare)
{
   int a[] = {1, 2, 3};
   OK(Has(1, 3), a, 3);
}
```

#### 3.3. User defined Matcher
```C++
MATCHER(InRange, left, right, (a << " not in [" << left << ", " << right << "]"))
{
   return left <= a && a <= right;
}
```
Define a new Matcher named 'InRange', with 2 arguments left and right in constructor.
In matcher body, specific `a` is actual value variable name.
If matches fail, error message is result of 
```C++
std::cout << a << " not in [" << left << ", " << right << "]";
```

in following case, left is 1, right is 4, a is 5, error message is `"5 not in [1, 4]"`.

```C++
Case(test user defined matcher)
{
   OK(InRange(1, 4), 5);
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
[STUB](source/stub/h2_stub.hpp) is easier to use.

[UNSTUB](source/stub/h2_stub.hpp) reset STUB, recover original function.

#### 4.1. Normal function substitution

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

#### 4.2. STUB overload function

```C++
bool is_equal(double a, double b)
{
   return a == b;
}
bool is_equal(char * a, char * b)
{
   return strcmp(a, b) == 0;
}
```

```C++
/* unit test code */
bool is_equal_fake(char * a, char * b)
{
   return stricmp(a, b) == 0;
}
CASE(demo dynamic stub with fake function)
{
   STUB((bool(*)(char*, char*))is_equal, is_equal_fake);
   do_something_with_call_is_equal();
}

CASE(demo dynamic stub with fake function 2)
{
   STUB(is_equal, bool(char*, char*), is_equal_fake);
   do_something_with_call_is_equal();
}
```

#### 4.3. STUB class member function

##### 4.3.1. nonstatic or virtual member function
```C++
class Foo {
   int bar(int a, char * b) { ... }
}
```

```C++
/* unit test code */
int Foo_bar_fake(Foo* foo, int a, char * b)
{
   OK(1, a);
   sprintf(b, "return value by argument");
   return 1;
}
CASE(normal member function)
{
   STUB(Foo, bar, int(int a, char * b), Foo_bar_fake);
   do_something(Foo::bar);
}
```

##### 4.3.2. static member function
```C++
class Foo {
   static int bar(int a, char * b) { ... }
}
```

```C++
/* unit test code */
int Foo_bar_fake(int a, char * b)
{
   OK(1, a);
   sprintf(b, "return value by argument");
   return 1;
}
CASE(static class member function)
{
   STUB(Foo::bar, int(int a, char * b), Foo_bar_fake);
   do_something(Foo::bar);
}
```

##### 4.3.3. template function or template class

```C++
template <typename T1, typename T2>
int foobar(T1 a, T2 b) { ... }

template <typename U1, typename U2>
class Foo {
   template <typename T1, typename T2>
   int bar(T1 a, T2 b) { ... }
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
CASE(template function)
{
   STUB((foobar<int, char*>), int(int a, char * b), foobar_fake);
   ...do_something(foobar<int, char*>);
}
int Foo_bar_fake(Foo<int, char*>* foo, float a, std::string b)
{
   OK(1, a);
   OK("str", b);
   return 1;
}
CASE(template class member function)
{
   STUB((Foo<int, char*>), (bar<float, std::string>), int(float, std::string), Foo_bar_fake);
   ...do_something(Foo<int, char*>::bar<float, std::string>);
}
```

##### 4.3.4. Abstract or none default constructor class
In MSVC, it can't find vtable right now, should provide a class instance (object or pointer)

```C++
class Foo {
   int bar(int a, char * b) { ... }
}
```

```C++
/* unit test code */
int Foo_bar_fake(Foo* foo, int a, char * b)
{
   OK(1, a);
   sprintf(b, "return value by argument");
   return 1;
}
CASE(virtual member function in msvc)
{
   Foo foo;
   STUB(foo, Foo::bar, int(int a, char * b), Foo_bar_fake);
   do_something(Foo::bar);
}
```

#### 4.4. STUB inplace
With help of lambda, fake_function can following with STUB.

`This` is dedicated variable of class instance pointr like `this`.

```C++
/* product code */ 
int foobar(int a, char * b)
{
   ......
}
```

```C++
/* unit test code */
CASE(demo dynamic stub inplace)
{
   STUBS(foobar, int, (int a, char* b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 1;
   }

   do_something();
}
```

The principle of Dynamic STUB is :

Replacing the first several binary code of original function with "JMP" instruction, 
the result is once calling into original function, it will jump to fake function immediately,
parameters of original function in stack will be treated as parameters of fake function,
fake function return to the caller of original function directly.

`STUB()` and `STUBS()` is used to replace original with fake. All replaced binary code will be restored in default teardown phase.

STUB formula:
```Shell
   STUB([[Class Instance], Class Type,] Function Name, Return Type(Parameter List), Substitute Function Name);
   UNSTUB([[Class Instance], Class Type,] Function Name, Return Type(Parameter List));

   STUBS([[Class Instance], Class Type,] Function Name, Return Type, (Parameter List)) { ... }
   UNSTUBS([[Class Instance], Class Type,] Function Name, Return Type, (Parameter List));
```

### 5. Dynamic MOCK
Compare to Dynamic STUB, Dynamic MOCK provide a easy way to check call times, input arguments and inject return value.

```C++
/* unit test code */
CASE(demo dynamic mock function)
{
   MOCK(foobar, int(int a, char * b)).Once(1, "A").Return(11);
   do_something();
}

CASE(demo dynamic mock class method)
{
   MOCK(Foo, bar, int(int a, char * b)).Once(1, "A").Return(11);
   do_something();
}
```
Expect foobar called with *a* equals *1*, *b* equals *"A"* *1 time* in this case, and make it returns *11*.

#### 5.1. Chained Inspection

##### 5.1.1. Checkin of call

-    `Once`([matcher...]) : Expect called 1 time, and expect arguments match matchers, default match any
-    `Twice`([matcher...]) : Expect called 2 times
-    `Times`(n, [matcher...]) : Expect called n times
-    `Any`([matcher...]) : Expect called any times(include 0 times)
-    `Atleast`(n, [matcher...]) : Expect called atleast n times(>=n)
-    `Atmost`(n, [matcher...]) : Expect called atmost n times(<=n)
-    `Between`(n, m, [matcher...]) : Expect called >=n and <=m times

If checkin not specified, default is `Any`.

`greed`(boolean) : match call in greed mode or not, default is true

##### 5.1.2. Arguments check

-    `With`(matcher...) : Expect arguments match matchers
-    `Th0~15`(matcher) : Expect 1st~16th argument match matcher

##### 5.1.3. Return

-    `Return`(value) : Inject return value
-    without `Return`(...) : Delegate to origin function/method


#### 5.2. [`MOCK`](source/mock/h2_mock.hpp) inplace

With help of lambda, fake_function can following with MOCK.

`This` is dedicated variable of class instance pointr like `this`.

```C++
/* unit test code */
CASE(demo dynamic stub inplace)
{
   MOCKS(Foo, bar, int, (int a, char* b)) {
      OK(1, a);
      OK(2, This->m);
      sprintf(b, "return value by argument");
      return 1;
   }

   do_something();
}
```
`{ }` following MOCKS(...), when `Return` is provided substitute function is ignored.


#### 5.3. [`MOCK`](source/mock/h2_mock.hpp) primitive 

MOCK formula:
```Shell
   MOCK([[Class Instance], Class Type,] Function Name, Return Type(Parameter List)) [.Chained Inspection];
   UNMOCK([[Class Instance], Class Type,] Function Name, Return Type(Parameter List));

   MOCKS([[Class Instance], Class Type,] Function Name, Return Type, (Parameter List), Chained Inspection)
   {  
      ... substitute function body ...
      check... ;
      return... ;
   };
   UNMOCKS([[Class Instance], Class Type,] Function Name, Return Type, (Parameter List));
```

In Substitute function, Can check arguments, return value, and other actions. 

```C++
MOCK(Foo, bar, int(int a, char * b))
     .Once(1, _).Return(11)
     .Twice(Gt(2), CaseLess("abc")).Return(22)
     .Times(5).With(Not(3)).Return(33)
     .Atleast(2).Th0(4).Th2("xyz")
     .Any().Return(44);

MOCKS(Foo, bar, int, (int a, char * b), 
       Once(1, _).Return(11)
      .Twice(Gt(2), CaseLess("abc")).Return(22)
      .Times(5).With(Not(3)).Return(33)
      .Atleast(2).Th0(4).Th2("xyz")
      .Any()) {
        OK(5, a);
        sprintf(b, This->name);
        return 44;
      };
```
First expect Foo::bar called 1 time, and 1st argument equals 1, any of 2nd, and inject Return value 11; <br>
then expect Foo::bar called 2 times, and 1st argument geat than 2, 2nd case-insensitive equals "abc", and inject Return value 22; <br>
then expect Foo::bar called 5 times, and 1st argument not equals 3, ignore check 2nd (any of 2nd), and inject Return value 33; <br>
then expect Foo::bar called atleast 2 times, and 1st argument equals 4, 2nd equals "xyz", and inject Return value 44; <br>
then expect Foo::bar called any times, and 1st argument equals 5, modify 2nd, and inject Return value 44. <br>

#### 5.5. Others
##### 5.5.1. unprotected commas

C/C++ preprocessor can't parse MACRO arguments which contains comma in '<>' correctly. 

A solution is wrapping such argument with parentheses '()'.

##### 5.5.2. mprotect failure

macOS (Mojave 10.14.6) macOS (Catalina 10.15.2) and later macOS mprotect fails, because [max_prot](https://stackoverflow.com/questions/60654834/using-mprotect-to-make-text-segment-writable-on-macos).

```Shell
   printf '\x07' | dd of=<executable> bs=1 seek=160 count=1 conv=notrunc
```

##### 5.5.3. private member method accessibility
In order to STUB/MOCK class private member function successfully, `private` token is substituted with `public` using MACRO definition by default.

Include `h2unit.h/hpp` before other header files.

If test target is C language project, and `private` is used as normal token, define `TEST_C` in compiler options to prevent above substitution.

```Shell
   g++ -DTEST_C ...
```

##### 5.5.4. static function accessibility
static function is unaccessible outside of source file. In order to STUB/MOCK such function successfully, there are two solutions:

###### 5.5.4.1. solution 1: include source file in test file

```C++
   // test_module.cpp
   #include "h2unit.h"
   #include "module.c/cpp"
   CASE(a test)
   {
      ...
   }
```

###### 5.5.4.2. solution 2: STUB/MOCK by function string name

```C++
   static int a_static_function(char * str) {
      ...
   }
```
```C++
   CASE(a test)
   {
      STUB("a_static_function", fake_function);
      MOCK("a_static_function", int(char *)).Once().Return(11);
   }
```

If function is overload function, arguments type should specified, i.e. C++ demangled function name.

```C++
   CASE(a test)
   {
      STUB("a_static_function(char*)", fake_function);
      MOCK("a_static_function(char*)", int(char *)).Once().Return(11);
   }
```

  `nm --demangle ./a.out | grep a_static_function` can find out demangled function name.

### 6. Extended JSON Compare 

#### 6.1. javascript style JSON for convenience 
```Python
"{
   'name' : 'Zhang3',
   'score' : {
      'Math' : 100,
      'English' : 99
   }
}"
```
```Python
"{
   name : Zhang3,
   score : {
      Math : 100,
      English : 99
   }
}"
```
is same as standard JSON but without escape character or quotes
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
          name: Zhang3, 
          score: { ... }
      }"), student_tojson(student));

MOCK(foobar, int(int a, char * b)).Once(1, Je("{
                                                   name: Zhang3, 
                                                   score: { ... }
                                              }");
```

[`JE`](source/assert/h2_assert.hpp) is abbreviated for OK(Je(expect json), actual json)

#### 6.3. math expression is support in JSON, it is evaluated to number when parsing 
```C++
JE("{
         width: 100 + 10 + 1, 
         height: 111 * sqrt(4)
    }", rectangle_tojson(rectangle));
```
It expect rectangle width equal to 111, and height equal to 222

#### 6.4. Regex is support in JSON compare
```C++
JE("{
         name: /Zhang[0-9]+/, 
         score: { ... }
    }", student_tojson(student));
```

#### 6.5. JSON Compare selector
Inspired by [jq](https://github.com/stedolan/jq), JE/Je can compare JSON subset specified by selector.

```Python
scores = {
    Zhang3 : { Math : 100, English : 99 },
    Li4 : { Math : 97, English : 100 },
    Wang5 : { Math : 98, English : 98 }
}
```
```C++
JE("{ Math : 100, English : 99 }", scores, ".Zhang3");

MOCK(foobar, int(char *)).Once(Je("{ Math : 100, English : 99 }", ".Zhang3"));

JE("99", scores, ".Zhang3.English");

MOCK(foobar, int(char *)).Once(Je("99", ".Zhang3.English"));
```
- Object Identifier-Index: .foo, .foo.bar
  
   When given a JSON object as input, it produces the value at the key "foo", or null if there's none present.
- Array Index: [2], [2][3]

   When the index value is an integer, `[<value>]` can index arrays. Arrays are zero-based, so `[2]` returns the third element.
   Negative indices are allowed, with -1 referring to the last element, -2 referring to the next to last element, and so on.

#### 6.6. JSON Compare rule
##### 6.6.1. Value Compare
   Matches when Expect Value is Regex, otherwise Equals .
##### 6.6.2. Object compare
   Every key in Expect Object is Exist in Actual Object, and value equals.
##### 6.6.3. Array compare
   Size is same, and each element equals.


### 7. Memory Check
#### 7.1. Memory Leak Detection 
In order to detect memory leak, h2unit hook malloc,free,new,delete,calloc,realloc,strdup,posix_memalign and other memory allocate functions.

Every test case should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

[`BLOCK`](source/h2_unit.hpp) can be used to detect memory leak in a code block. p2 is reported as leak in the following:
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

Some libc functions allocate memory inside and cache them, these memory will be report as leak.

'asctime, asctime_r, ctime, ctime_r, localtime, localtime_r, gmtime, gmtime_r, mktime, strtod, strtold, sscanf, sprintf, vsnprintf' are ignored by default. 

Others can use `UNMEM` to avoid.

```C++
   UNMEM(strtoull)
```

#### 7.2. Memory Faulty Injection
[`BLOCK`](source/h2_unit.hpp) can used to control the remain memory resource, it can makes malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
CASE(test out of memory)
{
   BLOCK(limit=10) {
      OK(NULL == malloc(11));
   }
}
```
[`BLOCK`](source/h2_unit.hpp) can used to initialize allocated memory. In following case, p is filled with *C555*.
```C++
CASE(test memory initialize)
{
   BLOCK(limit=10, fill=0xC555) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](source/h2_unit.hpp) align allocate memory, In following case, p mod 4 equals 3.
```C++
CASE(test ptr align)
{
   BLOCK(align=3) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](source/h2_unit.hpp) Ignore memory leak detection, in BLOCK.
```C++
CASE(test ignore memory leak)
{
   BLOCK(noleak) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](source/h2_unit.hpp) Ignore memory check, in BLOCK.
```C++
CASE(test ignore memory leak)
{
   BLOCK(numem) {
      char *p = (char *)malloc(8);
   }
}
```

#### 7.3. Memory overflow
Writing out of allocated memory area[start, start+size], memory overflow/underflow will be detected. 

#### 7.4. Double free Detection

#### 7.5. Asymmetrical allocate and free
`delete` memory which allocated by `malloc`, `free` memory which allocated by `new` will be detected.

#### 7.6. Use after free Detection
Read/Write memory which already freed, will be detected.

### 8. Capture C++ Exception
*    Uncaught Exception 
     It will be detected and reported as failure.
     Unfortunately conflict with `Memory Check` in MacOS, disable `Memory Check` (`-m` option) to enable `Uncaught Exception`, otherwise uncaught exception will cause terminate/crash.

*    Thrown Exception 
     Any thrown exception can be detected and reported as failure if `-x` option is set.

### 9. Global Setup/Cleanup
*    [`GlobalSetup`](source/core/h2_core.hpp): Invoked before test case
*    [`GlobalCleanup`](source/core/h2_core.hpp): Invoked after all test case executed
*    [`GlobalSuiteSetup`](source/core/h2_core.hpp): Invoked before every suite
*    [`GlobalSuiteCleanup`](source/core/h2_core.hpp): Invoked after every suite executed
*    [`GlobalCaseSetup`](source/core/h2_core.hpp): Invoked before every case
*    [`GlobalCaseCleanup`](source/core/h2_core.hpp): Invoked after every case executed
```C++
GlobalSetup() {
   WSAStartup();
}

GlobalCleanup() {
   WSACleanup();
}
```
Global Setup/Cleanup can define multiple times, all of them will be invoked.

### 10. DNS Hijack
[`DNS`](source/h2_unit.hpp)("hostname", "ip1", "ip2", "alias1", "alias2", ...): Set DNS resolve results (getaddrinfo, gethostbyname)

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

### 11. Socket Hijack
*    [`SOCK`](source/h2_unit.hpp)(){}: Monitor TCP/UDP send/recv.
*    [`Ptx`](source/h2_unit.hpp)(from, to, payload, length): Check sent packet, parameters are all matcher.
*    [`Pij`](source/h2_unit.hpp)(packet, size, [from=ip:port [, to=ip:port]]): Inject UDP/TCP packet as received packet.
If not specified `to`, any of socket can receive the packet.

If not specified `from`, the packet is received from where last send to.
```C++
CASE(test net)
{
   SOCK() { // Hook Socket API
      sendto(fd, "1234567890", 10, 0, (struct sockaddr*)&remote, sizeof(remote));
      Ptx("*:9527", "1.2.3.4:8888", Me("1234567890", 10), 10); // assert outgoing packet
      Ptx("*:9527", "1.2.3.4:8888", Me("1234567890", 10)); // ignore payload size check
      Ptx("*:9527", "1.2.3.4:8888"); // ignore payload and payload size check

      Pij(buffer1, 100, from=4.3.2.1:8888, to=*:4444); // Inject as received packet from 4.3.2.1:8888 to local port 4444 socket
      Pij(buffer2, 100, from=4.3.2.1:8888); // Inject as received packet from 4.3.2.1:8888 to any local socket
      Pij(buffer3, 100); // Inject as received packet from last sendto peer to any local socket
      recvfrom(fd, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&remote, &length);
      OK(Me("9876543210", 10), buffer);
   }
}
```

### 12. Capture STDOUT/STDERR/syslog
[`COUT`](source/h2_unit.hpp)(Matcher, [STDOUT] [stderr] [syslog]){}: Capture STDOUT STDERR and syslog output (printf(), std::cout<<, ...)

```C++
CASE(test printf)
{
   COUT("abcde") {
      printf("a");
      std::cout << "b";
      std::cerr << "c";
      std::clog << "d";
      syslog(LOG_DEBUG, "e");
   }
}
```

### 13. Performance benchmark
[`PF`](source/h2_unit.hpp)(n){}: Fail if following block cost more then n milliseconds

```C++
CASE(test performance)
{
   PF(11) {
      do_something();
   }
}
```

### 14. Parameterized test
#### 14.1. Value-Parameterized test
*    CASES(values...): Automatically generate CASE with each value (access by x)
*    CASESS(values...): Automatically generate CASE with fullmesh value (access by x, y)
*    Cases(values...): Automatically generate Case with each value inside of SUITE (access by x)
*    Casess(values...): Automatically generate Case with fullmesh value inside of SUITE (access by x, y)


```C++
CASES(1,2,3) // Generate 3 CASE : x=1;x=2;x=3
{
   OK(x...);
}
SUITE(suite)
{
   Cases(1,2,3) // Generate 3 Case : x=1;x=2;x=3
   {
      OK(x...);
   }
}
CASESS(1,2,3)             // (1,2,3)x(1,2,3)
{                         // Generate 9 CASE : x,y=(1,1);(1,2);(1,3);
   OK(x...y);             //                       (2,1);(2,2);(2,3);
}                         //                       (3,1);(3,2);(3,3);
CASESS((1,2,3), (4,5,6))  // (1,2,3)x(4,5,6)
{                         // Generate 9 CASE : x,y=(1,4);(1,5);(1,6);
   OK(x...y);             //                       (2,4);(2,5);(2,6);
}                         //                       (3,4);(3,5);(3,6);
SUITE(suite)
{
   Casess(1,2,3)            // Generate 9 Case : (1,2,3)x(1,2,3)
   {
      OK(x...y);
   }
   Casess((1,2,3), (4,5,6)) // Generate 9 Case : (1,2,3)x(4,5,6)
   {
      OK(x...y);
   }
}

```

#### 14.2. Type-Parameterized test
*    CASES_T(types...): Automatically generate CASE with each type (access by x)
*    CASESS_T(types...): Automatically generate CASE with fullmesh type (access by x, y)
*    Cases_t(types...): NOT implemented, Automatically generate CASE with each type inside of SUITE (access by x)
*    Casess_t(types...): NOT implemented, Automatically generate CASE with fullmesh type inside of SUITE (access by x, y)

```C++
CASES_T(short,int,long) // Generate 3 CASE : x=short;x=int;x=long
{
   OK(x...);
}
CASESS_T(short,int,long)  // (short,int,long)x(short,int,long)
{                         // Generate 9 CASE : x,y=(short,short);(short,int);(short,long);
   OK(x...y);             //                       (int,short);(int,int);(int,long);
}                         //                       (long,short);(long,int);(long,long);
CASESS_T((short,int,long), (char,unsigned,double))  // (short,int,long)x(char,unsigned,double)
{                                                   // Generate 9 CASE : x,y=(short,char);(short,unsigned);(short,double);
   OK(x...y);                                       //                       (int,char);(int,unsigned);(int,double);
}                                                   //                       (long,char);(long,unsigned);(long,double);

```

*    H2Foreach(Macro, values...): Call Macro(value)
*    H2Fullmesh(Macro, values...): Call Macro(values x values)
*    H2Fullmesh(Macro, (x-values...), (y-values...)): Call Macro(x-values x y-values)


# Coverage

- Add `-fprofile-arcs -ftest-coverage` into compiler and linker options.
   for CMake: <br>
      TARGET_COMPILE_OPTIONS(a.out PRIVATE  --coverage) <br>
      TARGET_LINK_OPTIONS(a.out PRIVATE  --coverage)

- Prerequisites gcovr or lcov is installed.

```Shell
   make
   ./a.out
   gcovr -r . -e 'test_*' --html --html-details -o coverage.html
```

# Compile speed
In order to speed up compile progress, split header-only-single-file [h2unit.h](h2unit.h) into 
two files: [h2unit.cpp](build/h2unit.cpp) and [h2unit.hpp](build/h2unit.hpp). Refer to [`print`](print/) for sample usage.

twofiles speed up 2~3 times than onefile.

# Execute options

*    `-l` *list* out suites and cases
*    `-m` Disable *memory* check(leak, overflow, trample, double free, asymmetric free)
*    `-s` *shuffle* cases then test in random order if no previous fails
*    `-b` [n] *break* test once n (default 1) cases failed
*    `-n` Only test previous *failed* cases
*    `-p` Disable test percentage *progressing*
*    `-r` [n] repeat run n *rounds*
*    `-c` Disable *colorful* output, black-white output
*    `-x` Thrown *exception* is considered as failure
*    `-d/D` *debug* mode, -D for gdb attach but requires password
*    `-f` [n] *fold* json object or array, 0:unfold, 1:fold simple, 2:fold same, 3:fold missing peer
*    `-y` *copy-paste* JSON C/C++ source code
*    `-v` *verbose* output including successfull cases
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-e` {pattern} *exclude* filter, default is ` ` (exclude nothing)

# Support platform
*    Linux gcc 5.5+ (regex support, SFINAE support), clang 7+, x86, x86_64, arm64(aarch64)
*    macOS 10.14+ 10.15 11.05, xcode 10.3+
*    Windows Visual Studio 2019+ 16+ MSVC 14.20+ _MSC_VER 1913+
*    Windows Cygwin, MinGW 64, ucrt64, clang64
*    Windows WSL (Debian, Ubuntu, Kali, openSUSE)

# Limitations
*    Variadic parameter function can't MOCK, use STUB with separate fake function instead
*    MOCK function arguments up to 16 count
*    sqrt() in math.h can be STUB/MOCK, because compiler insert sqrtsd ASM instruction directly instead of function call

```Shell
   LD_BIND_NOW=1 ./a.out 
   gcc --Wl,z,now
```










