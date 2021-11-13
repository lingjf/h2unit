# Reference

## File Structure
### H2UNIT_IMPORT_MAIN

On windows platform, define `H2UNIT_IMPORT_MAIN` in one of test source files to import main(), in others should not define it to avoid multiple definition.

On Linux or macOS unix platform, `H2UNIT_IMPORT_MAIN` is not necessary with help of weak reference.

```C++
// test1.cpp
#define H2UNIT_IMPORT_MAIN
#include "h2unit.h"
```

```C++
// test2.cpp
#include "h2unit.h"
```

```Shell
g++ -std=c++11 test1.cpp test2.cpp -o a.out
./a.out
```

Include h2unit.h/hpp before other include files.

### h2unit.h/hpp/cpp

`h2unit.h` is all in one file.

```C++
// test1.cpp
#include "h2unit.h"
```

```Shell
g++ -std=c++11 test1.cpp -o a.out
./a.out
```

In order to speed up compilation, split h2unit.h into two files: `h2unit.hpp` and `h2unit.cpp`.

```C++
// test2.cpp
#include "h2unit.hpp"
```

```Shell
g++ -std=c++11 h2unit.cpp test2.cpp -o a.out
./a.out
```

## Test Structure

### H2SUITE alias SUITE

`SUITE` define a group of test cases. Suite name is not requred quoted by double quotation marks `"`.

```C++
#include "h2unit.h"
SUITE(suite name)
{

}
```

### H2Case alias Case

`Case` define a test case inside `SUITE`. Case name is not requred quoted by double quotation marks `"`.

```C++
#include "h2unit.h"
SUITE(suite name)
{
    Case(case name)
    {

    }
}
```

### H2Setup alias Setup

`Setup()` is executed before every test case.

Variables defined in Suite scope are shared by cases which can see them.

```C++
#include "h2unit.h"
SUITE(suite name)
{
    shared_variables = 0;
    shared_code as setup;

    Setup()
    {

    }
    
    Case(case name)
    {

    }
}
```

### H2Cleanup alias Cleanup

`Cleanup()` is executed after every test case whatever success or fail.

Each case is executed separately and begin from first of suite scope, shared variables are initialized, then shared code is executed until case section, after test code, `Cleanup()` is invoked to release resources.

```C++
#include "h2unit.h"
SUITE(suite name)
{
    shared_variables = 0;
    shared_code as setup;
    
    Cleanup()
    {

    }

    Case(case name)
    {

    }
}
```

### H2CASE alias CASE

`CASE` define a standalone test case outside `SUITE`.

```C++
#include "h2unit.h"
CASE(case name)
{

}
```

### H2Todo alias Todo

`Todo` define a none-executable test case inside `SUITE`.

```C++
#include "h2unit.h"
SUITE(suite name)
{
    Todo(case name)
    {

    }
}
```

### H2TODO alias TODO

`TODO` define a standalone none-executable test case outside `SUITE`.

```C++
#include "h2unit.h"
TODO(case name)
{

}
```

All the test cases are registered to a global list automatically (C++ global object's constructor is invoked automatically before main). 

It means user should not write extra code to register test cases.

### Parameterized test
#### Value-Parameterized test
*    `Cases`(name, (values...)): Automatically generate `Case` with each value inside `SUITE` (access by x)
*    `Casess`(name, (values...)): Automatically generate `Case` with fullmesh value inside `SUITE` (access by x, y)
*    `CASES`(name, (values...)): Automatically generate `CASE` with each value (access by x)
*    `CASESS`(name, (values...)): Automatically generate `CASE` with fullmesh value (access by x, y)
*    `Todos`(name, (values...)): define a none-executable test case inside `SUITE` (access by x)
*    `Todoss`(name, (values...)): define a none-executable test case inside `SUITE` (access by x, y)
*    `TODOS`(name, (values...)): define a standalone none-executable test case outside `SUITE` (access by x)
*    `TODOSS`(name, (values...)): define a standalone none-executable test case outside `SUITE` (access by x, y)


```C++
CASES(name, (1,2,3)) // Generate 3 CASE : x=1;x=2;x=3
{
   OK(x...);
}
SUITE(suite)
{
   Cases(name, (1,2,3)) // Generate 3 Case : x=1;x=2;x=3
   {
      OK(x...);
   }
}
CASESS(name, (1,2,3))            // (1,2,3)x(1,2,3)
{                                // Generate 9 CASE : x,y=(1,1);(1,2);(1,3);
   OK(x...y);                    //                       (2,1);(2,2);(2,3);
}                                //                       (3,1);(3,2);(3,3);
CASESS(name, (1,2,3), (4,5,6))   // (1,2,3)x(4,5,6)
{                                // Generate 9 CASE : x,y=(1,4);(1,5);(1,6);
   OK(x...y);                    //                       (2,4);(2,5);(2,6);
}                                //                       (3,4);(3,5);(3,6);
SUITE(suite)
{
   Casess(name, (1,2,3))            // Generate 9 Case : (1,2,3)x(1,2,3)
   {
      OK(x...y);
   }
   Casess(name, (1,2,3), (4,5,6))   // Generate 9 Case : (1,2,3)x(4,5,6)
   {
      OK(x...y);
   }
}

```

#### Type-Parameterized test
*    `CASES_T`(name, (types...)): Automatically generate CASE with each type (access by x)
*    `CASESS_T`(name, (types...)): Automatically generate CASE with fullmesh type (access by x, y)
*    `Cases_t`(name, (types...)): NOT implemented, Automatically generate CASE with each type inside SUITE (access by x)
*    `Casess_t`(name, (types...)): NOT implemented, Automatically generate CASE with fullmesh type inside SUITE (access by x, y)
*    `TODOS_T`(name, (values...)): define a standalone none-executable test case outside `SUITE` (access by x)
*    `TODOSS_T`(name, (values...)): define a standalone none-executable test case outside `SUITE` (access by x, y)

```C++
CASES_T(name, (short,int,long)) // Generate 3 CASE : x=short;x=int;x=long
{
   OK(x...);
}
CASESS_T(name, (short,int,long))    // (short,int,long)x(short,int,long)
{                                   // Generate 9 CASE : x,y=(short,short);(short,int);(short,long);
   OK(x...y);                       //                       (int,short);(int,int);(int,long);
}                                   //                       (long,short);(long,int);(long,long);
CASESS_T(name, (short,int,long), (u8,u16,u32))  // (short,int,long)x(u8,u16,u32)
{                                               // Generate 9 CASE : x,y=(short,u8);(short,u16);(short,u32);
   OK(x...y);                                   //                       (int,u8);(int,u16);(int,u32);
}                                               //                       (long,u8);(long,u16);(long,u32);

```

*    H2Foreach(Macro, (values...)): Call Macro(value)
*    H2Fullmesh(Macro, (values...)): Call Macro(values x values)
*    H2Fullmesh(Macro, (x-values...), (y-values...)): Call Macro(x-values x y-values)


## Test Compare

### H2CP alias CP

[`CP`](../source/assert/h2_assert.hpp)(expect < <= > >= == != actual) : compare the actual value with expect value, only support six operator(< <= > >= == !=).(lowercase k)

```C++
Case(case name)
{
    CP(1 == 1);
    CP(1 != 2);
    CP(e < a);
    CP(e <= a);
    CP(e > a);
    CP(e >= a);
}
```

### H2OK alias OK
*    [`OK`](../source/assert/h2_assert.hpp)(expr) : check the result of `expr` is true.
*    [`OK`](../source/assert/h2_assert.hpp)(expect, actual) : check the actual value matches with expect value.

```C++
Case(case name)
{
    OK(a);
    OK(e , a);
}
```

### H2JE alias JE
[`JE`](../source/assert/h2_assert.hpp)(expect, actual) : compare JSON.


#### javascript style JSON for convenience 
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

#### math expression is support in JSON, it is evaluated to number when parsing 
```C++
JE("{
         width: 100 + 10 + 1, 
         height: 111 * sqrt(4)
    }", rectangle_tojson(rectangle));
```
It expect rectangle width equal to 111, and height equal to 222

#### Regex is support in JSON compare
```C++
JE("{
         name: /Zhang[0-9]+/, 
         score: { ... }
    }", student_tojson(student));
```

#### JSON Compare selector
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

JE("99", scores, ".Zhang3.English");

```

- Object Identifier-Index: .foo, .foo.bar
  
   When given a JSON object as input, it produces the value at the key "foo", or null if there's none present.
- Array Index: [2], [2][3]

   When the index value is an integer, `[<value>]` can index arrays. Arrays are zero-based, so `[2]` returns the third element.
   Negative indices are allowed, with -1 referring to the last element, -2 referring to the next to last element, and so on.

#### JSON Compare rule
##### Value Compare
   Matches when Expect Value is Regex, otherwise Equals .
##### Object compare
   Every key in Expect Object is Exist in Actual Object, and value equals.
##### Array compare
   Size is same, and each element equals.


Add user specified error description by following << operator.
```C++
CASE(case name)
{
    OK(1, 2) << 1 << "!=" << 2;
}
```

## Matcher

*    [`_`](../source/h2_unit.hpp#L321) / [`Any`](../source/h2_unit.hpp#L321) : matches any value 
*    [`IsNull`](../source/h2_unit.hpp#L321) : matches if value is null 
*    [`NotNull`](../source/h2_unit.hpp#L321) : matches if value is not null 
*    [`IsTrue`](../source/h2_unit.hpp#L321) : matches if value is true 
*    [`IsFalse`](../source/h2_unit.hpp#L321) : matches if value is false 
*    [`Eq`](../source/h2_unit.hpp#L321)(expect [, epsilon]) : matches if value equals expect (one of [strcmp wildcard regex] equals for string compare), float value near equals expect, default epsilon is 0.00001. 0.01_p matches if delta in 1% range of expect
*    [`Nq`](../source/h2_unit.hpp#L321)(expect) : matches if value not equals expect 
*    [`Ge`](../source/h2_unit.hpp#L321)(expect) : matches if value >= expect 
*    [`Gt`](../source/h2_unit.hpp#L321)(expect) : matches if value > expect 
*    [`Le`](../source/h2_unit.hpp#L321)(expect) : matches if value <= expect 
*    [`Lt`](../source/h2_unit.hpp#L321)(expect) : matches if value < expect 
*    [`Me`](../source/h2_unit.hpp#L321)(buffer, [length], [width]) : matches if memory is same 
*    [`Re`](../source/h2_unit.hpp#L321)(expect) : matches if value Regex equals expect 
*    [`We`](../source/h2_unit.hpp#L321)(expect) : matches if value Wildcard equals expect 
*    [`Je`](../source/h2_unit.hpp#L321)(expect) : matches if value JSON equals expect 
*    [`Se`](../source/h2_unit.hpp#L321)(expect) : matches if string strictly equals expect as strcmp() 
*    [`Substr`](../source/h2_unit.hpp#L321)(expect) : matches if value has substr expect 
*    [`StartsWith`](../source/h2_unit.hpp#L321)(expect) : matches if value starts with expect 
*    [`EndsWith`](../source/h2_unit.hpp#L321)(expect) : matches if value ends with expect 
*    [`~`](../source/h2_unit.hpp#L321) / [`CaseLess`](../source/h2_unit.hpp#L321)(expect) : make inner matcher case-insensitive, right operator must be Matcher, `~"Hello World"` not works 
*    [`*`](../source/h2_unit.hpp#L321) / [`SpaceLess`](../source/h2_unit.hpp#L321)(expect) : trim leading and trailing whitespace, squash several whitespaces into one space, right operator must be Matcher, `*"Hello World"` not works 
*    [`Pointee`](../source/h2_unit.hpp#L321)(expect) : matches if point to value equals expect 
*    [`!`](../source/h2_unit.hpp#L321) / [`Not`](../source/h2_unit.hpp#L321)(expect) : matches if not matches inner matcher, right operator must be Matcher, !3 is considered as normal semantics 
*    [`&&`](../source/h2_unit.hpp#L321)(expect) : Logical AND of two matchers, left and right operator shoud at least one Matcher 
*    [`||`](../source/h2_unit.hpp#L321)(expect) : Logical OR of two matchers 
*    [`AllOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value matches all of inner matchers, act as AND logical operator
*    [`AnyOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value matches any one of inner matchers, act as OR logical operator
*    [`NoneOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value not matches all of inner matchers 
*    [`ListOf`](../source/h2_unit.hpp#L321)(expect...) : matches if sequence container(array, vector, ...) item matches inner matchers 
*    [`CountOf`](../source/h2_unit.hpp#L321)(expect...) : matches if container(array, vector, ...) item count matches inner matchers 
*    [`Has`](../source/h2_unit.hpp#L321)(expect...) : matches if there are items in container(vector, set, map, ...) match every inner matchers


Matcher can be used in OK(expect, actual), for example:
```C++
OK(Ge(1.4142), sqrt(2))
```
It asserts sqrt(2) result 1.41421356237 is greater or equal than 1.4142


### Memory compare matcher
Expection is described by buffer, length and width.

#### explicit width

- `1` bit width, length is in bit unit.
- `8` byte width, length is in byte unit.
- `16` uint16 width (2 bytes), length is in uint16 unit.
- `32` uint32 width (4 bytes), length is in uint32 unit.
- `64` uint64 width (8 bytes), length is in uint64 unit.


```C++
CASE(memory compare bytes)
{
   unsigned char e[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
   unsigned char *a = ...
   OK(Me(e, 5, 8), a);
}

CASE(memory compare ints)
{
   unsigned int e[] = {1, 2, 3, 4, 5};
   unsigned char *a = ...
   OK(Me(e, 5, 32), a);
}
```

#### deduce width

If width is not specified, deduce compare width by data type or string format.

|  data type                |  width   |
|  ----                     |  ----    |
| [unsigned] char */[]      |  1 bytes |
| [unsigned] short */[]     |  2 bytes |
| [unsigned] int */[]       |  4 bytes |
| [unsigned] long long */[] |  8 bytes |

```C++
CASE(memory compare)
{
   unsigned short e[] = {0x018E, 0x23C8, 0x458E, 670xC8, 0x89F8};
   unsigned char *a = ...
   OK(Me(e, 5), a); ==> memcmp(e, a, 5*8)
}
```

If data type is not char*, and length is not specified, length is set to array size while data type is native array otherwise failed.


If data type is char*, and length is also not specified, deduce compare width by string format.

If buffer only contain '0' '1' ' ', width is considered 1, buffer is parsed to binary, length is set to count of '0' and '1'.

```C++
CASE(memory compare)
{
   const char* e = "1000 1110 1";
   unsigned char *a = ...
   OK(Me(e, 5), a); ==> bitcmp("100011101", a, 5)
}
```

If data type is char*, and only contain hexidecimal characters, width is considered 8, buffer is parsed to bytes array, length is set to array size.

```C++
CASE(memory compare)
{
   const char* e = "8EC88EC8F8";
   unsigned char *a = ...
   OK(Me(e), a); ==> memcmp([0x8E, 0xC8, 0x8E, 0xC8, 0xF8], a, 5*8)

   const char* e = "8Ep88EC8F8";
   unsigned char *a = ...
   OK(Me(e), a); ==> memcmp("8Ep88EC8F8", a, 10)
}
```

### Array size

OK(expect, actual, [size])

the third argument in OK is used to specified array size which ptr point to,
for STL containers and native array it is not necessary.

```C++
CASE(memory compare)
{
   int *a;
   OK(Has(1, 3), a, 3);
}
```

```C++
CASE(memory compare)
{
   int a[] = {1, 2, 3};
   OK(Has(1, 3), a);
}
```

### User defined Matcher
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

## H2STUB alias STUB

### Normal function substitution

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

### STUB overload function

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

### STUB class member function

#### nonstatic or virtual member function
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

#### static member function
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

#### template function or template class

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

#### Abstract or none default constructor class
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

### STUB inplace
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

[UNSTUB](../source/stub/h2_stub.hpp) reset STUB, recover original function.

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


## H2MOCK alias MOCK

### Chained Inspection

#### Checkin of call

-    `Once`([matcher...]) : Expect called 1 time, and expect arguments match matchers, default match any
-    `Twice`([matcher...]) : Expect called 2 times
-    `Times`(n, [matcher...]) : Expect called n times
-    `Any`([matcher...]) : Expect called any times(include 0 times)
-    `Atleast`(n, [matcher...]) : Expect called atleast n times(>=n)
-    `Atmost`(n, [matcher...]) : Expect called atmost n times(<=n)
-    `Between`(n, m, [matcher...]) : Expect called >=n and <=m times

If checkin not specified, default is `Any`.

`greed`(boolean) : match call in greed mode or not, default is true

#### Arguments check

-    `With`(matcher...) : Expect arguments match matchers
-    `Th0~15`(matcher) : Expect 1st~16th argument match matcher

#### Return

-    `Return`(value) : Inject return value
-    without `Return`(...) : Delegate to origin function/method


### [`MOCK`](../source/mock/h2_mock.hpp) inplace

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


### [`MOCK`](../source/mock/h2_mock.hpp) primitive 

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

MOCK function arguments up to 16 count.

## Memory Check
### Memory Leak Detection 
In order to detect memory leak, h2unit hook malloc,free,new,delete,calloc,realloc,strdup,posix_memalign and other memory allocate functions.

Every test case should balance the memory usage. In other word,
After a test case finished, the memory usage should be same as before the test case execute, otherwise memory leak detected.

[`BLOCK`](../source/h2_unit.hpp) can be used to detect memory leak in a code block. p2 is reported as leak in the following:
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

### Memory Faulty Injection
[`BLOCK`](../source/h2_unit.hpp) can used to control the remain memory resource, it can makes malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
CASE(test out of memory)
{
   BLOCK(limit=10) {
      OK(NULL == malloc(11));
   }
}
```
[`BLOCK`](../source/h2_unit.hpp) can used to initialize allocated memory. In following case, p is filled with *C555*.
```C++
CASE(test memory initialize)
{
   BLOCK(limit=10, fill=0xC555) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](../source/h2_unit.hpp) align allocate memory, In following case, p mod 4 equals 3.
```C++
CASE(test ptr align)
{
   BLOCK(align=3) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](../source/h2_unit.hpp) Ignore memory leak detection, in BLOCK.
```C++
CASE(test ignore memory leak)
{
   BLOCK(noleak) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](../source/h2_unit.hpp) Ignore memory check, in BLOCK.
```C++
CASE(test ignore memory leak)
{
   BLOCK(numem) {
      char *p = (char *)malloc(8);
   }
}
```

### Memory overflow
Writing out of allocated memory area[start, start+size], memory overflow/underflow will be detected. 

### Double free Detection

### Asymmetrical allocate and free
`delete` memory which allocated by `malloc`, `free` memory which allocated by `new` will be detected.

### Use after free Detection
Read/Write memory which already freed, will be detected.


## Global Setup/Cleanup

*    [`GlobalSetup`](../source/core/h2_core.hpp): Invoked before test case
*    [`GlobalCleanup`](../source/core/h2_core.hpp): Invoked after all test case executed
*    [`GlobalSuiteSetup`](../source/core/h2_core.hpp): Invoked before every suite
*    [`GlobalSuiteCleanup`](../source/core/h2_core.hpp): Invoked after every suite executed
*    [`GlobalCaseSetup`](../source/core/h2_core.hpp): Invoked before every case
*    [`GlobalCaseCleanup`](../source/core/h2_core.hpp): Invoked after every case executed

```C++
GlobalSetup() {
   WSAStartup();
}

GlobalCleanup() {
   WSACleanup();
}
```
Global Setup/Cleanup can define multiple times, all of them will be invoked.


## DNS Hijack
[`DNS`](../source/h2_unit.hpp)("hostname", "ip1", "ip2", "alias1", "alias2", ...): Set DNS resolve results (getaddrinfo, gethostbyname)

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

## Socket Hijack
*    [`SOCK`](../source/h2_unit.hpp)(){}: Monitor TCP/UDP send/recv.
*    [`Ptx`](../source/h2_unit.hpp)(from, to, payload, length): Check sent packet, parameters are all matcher.
*    [`Pij`](../source/h2_unit.hpp)(packet, size, [from=ip:port [, to=ip:port]]): Inject UDP/TCP packet as received packet.
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

## Capture STDOUT/STDERR/syslog
[`COUT`](../source/h2_unit.hpp)(Matcher, [STDOUT] [stderr] [syslog]){}: Capture STDOUT STDERR and syslog output (printf(), std::cout<<, ...)

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

## Performance benchmark
[`PF`](../source/h2_unit.hpp)(n){}: Fail if following block cost more then n milliseconds

```C++
CASE(test performance)
{
   PF(11) {
      do_something();
   }
}
```


## Capture C++ Exception
*    Uncaught Exception 
     It will be detected and reported as failure.
     Unfortunately conflict with `Memory Check` in MacOS, disable `Memory Check` (`-m` option) to enable `Uncaught Exception`, otherwise uncaught exception will cause terminate/crash.

*    Thrown Exception 
     Any thrown exception can be detected and reported as failure if `-x` option is set.


## Coverage

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
