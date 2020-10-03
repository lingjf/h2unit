# Introduce
*H2UNIT* is a unit test framework for C/C++. [https://github.com/lingjf/h2unit](https://github.com/lingjf/h2unit) <br>
*H2* in name is the chemical symbol of hydrogen. <br>
*H2UNIT* means a light-weight unit test framework as light as hydrogen. <br>
The special features is :

### 1. Header-only Single-file 
Only need to include *1* *ONE* *一* *いち* source file: [**h2unit.h**](h2unit.h) 

**h2unit.h** contains [`main()`](source/h2_unit.cpp#L56) function, and `main()` will execute test cases.
user no need to write main() anymore.

### 2. Dynamic MOCK

### 3. JSON Compare

# Example
Example is the BEST manual document.

All the code of following features can be found in the [example folder](example/), [test h2unit by h2unit](test/), and it is executable, the result is visable.


# Features

### 1. Test Suite and Test Case
[`SUITE`](source/h2_unit.hpp#L114) is used to define a Test Suite. [`Case`](source/h2_unit.hpp#L114) is used to define a Test Case inside of Test Suite. <br>
Variables defined in Suite scope are shared by cases which can see them. <br>
[`Cleanup`](source/h2_unit.hpp#L114) is executed after every test case whatever success or fail. <br>
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
[`CASE`](source/h2_unit.hpp#L114) macro is used to define a standalone test case.
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
*    [`OK`](source/h2_unit.hpp#L114)(expr) : check the result of `expr` is true.
*    [`OK`](source/h2_unit.hpp#L114)(expect, actual) : check the actual value matches with expect value.

Add user specified error description by following << operator.
```C++
CASE(Case Name)
{
    OK(1, 2) << 1 << "!=" << 2;
}
```
### 3. Matcher
*    [`_`](source/h2_unit.hpp#L266) / [`Any`](source/h2_unit.hpp#L266) : matches any value 
*    [`IsNull`](source/h2_unit.hpp#L266) : matches if value is null 
*    [`NotNull`](source/h2_unit.hpp#L266) : matches if value is not null 
*    [`IsTrue`](source/h2_unit.hpp#L266) : matches if value is true 
*    [`IsFalse`](source/h2_unit.hpp#L266) : matches if value is false 
*    [`Eq`](source/h2_unit.hpp#L266)(expect [, epsilon]) : matches if value equals expect (one of [strcmp wildcard regex] equals for string compare), float value near equals expect, default epsilon is 0.00001 
*    [`Nq`](source/h2_unit.hpp#L266)(expect) : matches if value not equals expect 
*    [`Ge`](source/h2_unit.hpp#L266)(expect) : matches if value >= expect 
*    [`Gt`](source/h2_unit.hpp#L266)(expect) : matches if value > expect 
*    [`Le`](source/h2_unit.hpp#L266)(expect) : matches if value <= expect 
*    [`Lt`](source/h2_unit.hpp#L266)(expect) : matches if value < expect 
*    [`Me`](source/h2_unit.hpp#L266)(expect, [length]) : matches if memcmp(expect, value, length) == 0 
     *    [`M1e`](source/h2_unit.hpp#L266)(expect, [length]) : bit level memcmp
     *    [`M8e`](source/h2_unit.hpp#L266)(expect, [length]) : byte level memcmp
     *    [`M16e`](source/h2_unit.hpp#L266)(expect, [length]) : int16 level memcmp
     *    [`M32e`](source/h2_unit.hpp#L266)(expect, [length]) : int32 level memcmp
     *    [`M64e`](source/h2_unit.hpp#L266)(expect, [length]) : int64 level memcmp
*    [`Re`](source/h2_unit.hpp#L266)(expect) : matches if value Regex equals expect 
*    [`We`](source/h2_unit.hpp#L266)(expect) : matches if value Wildcard equals expect 
*    [`Je`](source/h2_unit.hpp#L266)(expect) : matches if value JSON equals expect 
*    [`Se`](source/h2_unit.hpp#L266)(expect) : matches if string strictly equals expect as strcmp() 
*    [`Substr`](source/h2_unit.hpp#L266)(expect) : matches if value has substr expect 
*    [`StartsWith`](source/h2_unit.hpp#L266)(expect) : matches if value starts with expect 
*    [`EndsWith`](source/h2_unit.hpp#L266)(expect) : matches if value ends with expect 
*    [`~`](source/h2_unit.hpp#L266) / [`CaseLess`](source/h2_unit.hpp#L266)(expect) : make inner matcher case-insensitive, right operator must be Matcher, `~"Hello World"` not works 
*    [`Pointee`](source/h2_unit.hpp#L266)(expect) : matches if point to value equals expect 
*    [`!`](source/h2_unit.hpp#L266) / [`Not`](source/h2_unit.hpp#L266)(expect) : matches if not matches inner matcher, right operator must be Matcher, !3 is considered as normal semantics 
*    [`&&`](source/h2_unit.hpp#L266)(expect) : Logical AND of two matchers, left and right operator shoud at least one Matcher 
*    [`||`](source/h2_unit.hpp#L266)(expect) : Logical OR of two matchers 
*    [`AllOf`](source/h2_unit.hpp#L266)(expect...) : matches if value matches all of inner matchers, act as AND logical operator
*    [`AnyOf`](source/h2_unit.hpp#L266)(expect...) : matches if value matches any one of inner matchers, act as OR logical operator
*    [`NoneOf`](source/h2_unit.hpp#L266)(expect...) : matches if value not matches all of inner matchers 
*    [`ListOf`](source/h2_unit.hpp#L266)(expect...) : matches if sequence container(array, vector, ...) item matches inner matchers 
*    [`CountOf`](source/h2_unit.hpp#L266)(expect...) : matches if container(array, vector, ...) item count matches inner matchers 
*    [`Have`/`Has`](source/h2_unit.hpp#L266)(expect ...) : matches if there are items in container(vector, set, map, ...) match every inner matchers
*    [`In`](source/h2_unit.hpp#L266)(expect...) : matches if acutal value matches any of inner matchers 

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

*    [`M1e`](source/h2_unit.hpp#L266)(expect, [length]) : number of bits.
*    [`M8e`](source/h2_unit.hpp#L266)(expect, [length]) : number of bytes.
*    [`M16e`](source/h2_unit.hpp#L266)(expect, [length]) : number of int16.
*    [`M32e`](source/h2_unit.hpp#L266)(expect, [length]) : number of int32.
*    [`M64e`](source/h2_unit.hpp#L266)(expect, [length]) : number of int64.
*    [`Me`](source/h2_unit.hpp#L266)(expect, [length]) : following deduced type.

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
[STUB](source/stub/h2_use.hpp) is easier to use.

[UNSTUB](source/stub/h2_use.hpp) reset STUB, recover original function.

#### 4.1. raw function substitution

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
bool is_equal_fake(char * a, char * b)
{
   return stricmp(a, b) == 0;
}
```

```C++
CASE(demo dynamic stub with fake function)
{
   STUB((bool(*)(char*, char*))is_equal, is_equal_fake);
   do_something_with_call_is_equal();
}
```

```C++
CASE(demo dynamic stub with fake function)
{
   STUB(is_equal, bool, (char*, char*), is_equal_fake);
   do_something_with_call_is_equal();
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
   STUB([Class Name,] Function Name, Return Type, (Parameter List), Substitute Function Name);

   UNSTUB([Class Name,] Function Name, Return Type, (Parameter List));
```

### 5. Dynamic MOCK
Compare to Dynamic STUB, Dynamic Mock provide a easy way to check call times, input arguments and inject return value.

```C++
/* unit test code */
CASE(demo dynamic mock function)
{
   MOCK(foobar, int, (int a, char * b), Once(1, "A")) { return 11; };

   do_something();
}

CASE(demo dynamic mock class method)
{
   MOCK(Foo, bar, int, (int a, char * b), Once(1, "A")) { return 11; };

   do_something();
}
```
Expect foobar called with *a* equals *1*, *b* equals *"A"* *1 time* in this case, and make it returns *11*.

#### 5.1. [`MOCK`](source/h2_unit.hpp#L114) primitive 

##### 5.1.1. Mock normal function
  
```C++
   MOCK(Function Name, Return Type, (Parameter List) [, Chained Inspection]) {
      check... ;
      return... ;
   };
```

including class static member method.

##### 5.1.2. Mock class member function
  
```C++
   MOCK([Class Name,] Method Name, Return Type, (Parameter List) [, Chained Inspection]) { 
      check... ;
      return... ;
   };
```

[UNMOCK](source/mock/h2_use.hpp) reset MOCK, recover original function.

MOCK formula:
```C++
   MOCK([Class Name,] Function Name, Return Type, (Parameter List) [, Chained Inspection]) { 
      ... substitute function body ...
      check... ;
      return... ;
   };

   UNMOCK([Class Name,] Function Name, Return Type, (Parameter List));
```

#### 5.2. Chained Inspection

##### 5.2.1. Checkin of call

-    `Once`([matcher...]) : Expect called 1 time, and expect arguments match matchers, default match any
-    `Twice`([matcher...]) : Expect called 2 times
-    `Times`(n, [matcher...]) : Expect called n times
-    `Any`([matcher...]) : Expect called any times(include 0 times)
-    `Atleast`(n, [matcher...]) : Expect called atleast n times(>=n)
-    `Atmost`(n, [matcher...]) : Expect called atmost n times(<=n)
-    `Between`(n, m, [matcher...]) : Expect called >=n and <=m times

If checkin not specified, default is `Any`.

`greed`(boolean) : match call in greed mode or not, default is true

##### 5.2.2. Arguments check

-    `With`(matcher...) : Expect arguments match matchers
-    `Th0~15`(matcher) : Expect 1st~16th argument match matcher

##### 5.2.3. Return

-    `Return`() : Delegate to origin function/method
-    `Return`(value) : Inject return value

#### 5.3. Substitute function

`{ }` following MOCK(...), when `Return` is provided substitute function is ignored.

`This` is dedicated variable of class instance pointr like `this`.

In Substitute function, Can check arguments, return value, and other actions. 

#### 5.4. MOCK scenarios

##### 5.4.1. normal function

```C++
CASE(simple function)
{
   MOCK(foobar, int, (int a, char * b), Once(1, "A")) { 
      return 11; 
   };
   do_something_with_call_foobar();
}
```

##### 5.4.2. overload function

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
CASE(mock overload function)
{
   MOCK(is_equal, bool, (char *, char *)) {
      return false;
   };
   do_something_with_call_is_equal();
}
```

##### 5.4.3. nonstatic or virtual member function

```C++
CASE(normal member function)
{
   MOCK(Foo, bar, int, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

##### 5.4.4. static member function

```C++
CASE(static class member function)
{
   MOCK(Foo::bar, int, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   do_something(Foo);
}
```

##### 5.4.5. template function or template member function

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
CASE(template function)
{
   MOCK((foobar<int, char*>), int, (int a, char * b)) {
      OK(1, a);
      sprintf(b, "return value by argument");
      return 2;
   };
   ...do_something(foobar);

   MOCK((Foo<int, char*>), (bar<float, std::string>), int, (float, std::string)) {
      OK(1, a);
      return 2;
   };
   ...do_something(Foo bar);
}
```


```C++
MOCK(Foo, bar, int, (int a, char * b), 
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

clang >= version 11, default make code section (__TEXT) max protect (maxprot) `rx`, mprotect() in STUB/MOCK fails.

To fix it, add `-Wl,-segprot,__TEXT,rwx,rwx` in link(ld) option to make maxprot `rwx`. 

##### 5.5.3. private member method accessibility
In order to STUB/MOCK class private member function successfully, `private` token is substituted with `public` using MACRO definition by default.

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
      MOCK("a_static_function", int, (char *), Once) { return 11; }
   }
```

If function is overload function, arguments type should specified, i.e. C++ demangled function name.

```C++
   CASE(a test)
   {
      STUB("a_static_function(char*)", fake_function);
      MOCK("a_static_function(char*)", int, (char *), Once) { return 11; }
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

[`JE`](source/h2_unit.hpp#L114) is abbreviated for OK(Je(expect json), actual json)

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

MOCK(foobar, int, (char *), Once(Je("{ Math : 100, English : 99 }", ".Zhang3"))) {};

JE("99", scores, ".Zhang3.English");

MOCK(foobar, int, (char *), Once(Je("99", ".Zhang3.English"))) {};
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

[`BLOCK`](source/h2_unit.hpp#L114) can be used to detect memory leak in a code block. p2 is reported as leak in the following:
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
[`BLOCK`](source/h2_unit.hpp#L114) can used to control the remain memory resource, it can makes malloc() fail with "out of memory" error. The following case will fail due to malloc() fail.
```C++
CASE(test out of memory)
{
   BLOCK(limit=10) {
      OK(NULL == malloc(11));
   }
}
```
[`BLOCK`](source/h2_unit.hpp#L114) can used to initialize allocated memory. In following case, p is filled with *C555*.
```C++
CASE(test memory initialize)
{
   BLOCK(limit=10, fill=0xC555) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](source/h2_unit.hpp#L114) align allocate memory, In following case, p mod 4 equals 3.
```C++
CASE(test ptr align)
{
   BLOCK(align=3) {
      char *p = (char *)malloc(8);
   }
}
```
[`BLOCK`](source/h2_unit.hpp#L114) Ignore memory leak detection, in BLOCK.
```C++
CASE(test ignore memory leak)
{
   BLOCK(noleak) {
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

### 8. Global Setup/Teardown
*    [`GlobalSetup`](source/h2_unit.hpp#L114): Invoked before test case
*    [`GlobalTeardown`](source/h2_unit.hpp#L114): Invoked after all test case executed
*    [`GlobalSuiteSetup`](source/h2_unit.hpp#L114): Invoked before every suite
*    [`GlobalSuiteTeardown`](source/h2_unit.hpp#L114): Invoked after every suite executed
*    [`GlobalCaseSetup`](source/h2_unit.hpp#L114): Invoked before every case
*    [`GlobalCaseTeardown`](source/h2_unit.hpp#L114): Invoked after every case executed
```C++
GlobalSetup() {
   WSAStartup();
}

GlobalTeardown() {
   WSACleanup();
}
```
Global Setup/Teardown can define multiple times, all of them will be invoked.

### 9. DNS Hijack
[`DNS`](source/h2_unit.hpp#L114)("hostname", "ip1", "ip2", "alias1", "alias2", ...): Set DNS resolve results (getaddrinfo, gethostbyname)

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
*    [`SOCK`](source/h2_unit.hpp#L114)(): Monitor TCP/UDP send/recv, and return sent packets.
*    [`SOCK`](source/h2_unit.hpp#L114)(packet, size, from=ip:port, to=ip:port]): Inject UDP/TCP packet as received packet.
If not specified `to`, any of socket can receive the packet.

If not specified `from`, the packet is received from where last send to.
```C++
CASE(test net)
{
   SOCK(); // Start Hook Socket API

   sendto(sock, ...);
   OK(..., SOCK()); // Fetch outgoing packet

   SOCK(buffer1, 100, from=4.3.2.1:8888, to=*:4444); // Inject as received packet from 4.3.2.1:8888 to local port 4444 socket
   SOCK(buffer2, 100, from=4.3.2.1:8888); // Inject as received packet from 4.3.2.1:8888 to any local socket
   SOCK(buffer3, 100); // Inject as received packet from last sendto peer to any local socket
   recvfrom(sock, ...);
}
```

### 11. Capture STDOUT/STDERR/syslog
[`COUT`](source/h2_unit.hpp#L114)(): Capture STDOUT STDERR and syslog output (printf(), std::cout<<, ...)
*    `COUT`(): Toggle(Start/Stop) Capture STDOUT and STDERR
*    `COUT`(stdout stderr syslog): Start Capture STDOUT STDERR and syslog
*    `COUT`(STDOUT): Start Capture STDOUT only
*    `COUT`(STDerr): Start Capture STDERR only
*    `COUT`(stop): Stop Capture, and return buffer captured

```C++
CASE(test printf)
{
   COUT(); // Start Capture
   printf("...");
   std::cout << ...;
   OK("...", COUT()); // Stop Capture and return captured string
}
```

### 12. Parameterized test
#### 12.1. Value-Parameterized test
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

#### 12.2. Type-Parameterized test
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
*    ForForEach(Macro, values...): Call Macro(value)
*    ForFullmesh(Macro, values...): Call Macro(values x values)
*    ForFullmesh(Macro, (x-values...), (y-values...)): Call Macro(x-values x y-values)


# Coverage

- Add `-fprofile-arcs -ftest-coverage` into compiler and linker options.
- Prerequisites lcov is installed.
- Tool [`coverage.py`](coverage.py) generate coverage report.

```Shell
   make
   ./a.out
   python coverage.py "*product*" #Extract files matching PATTERN from FILE
```

## coverage.json
```JSON
   {
      "Lines": { "Total": 33, "Hit": 33, "Coverage": 1.0 },
      "Functions": { "Total": 8, "Hit": 8, "Coverage": 1.0 },
      "Branches": { "Total": 8, "Hit": 5, "Coverage": 0.625 }
   }
```
- Line coverage: Lines.Coverage
- Function coverage: Functions.Coverage
- Branch coverage: Branches.Coverage

## html report
coverage/index.html 

# Compile speed
In order to speed up compile progress, split header-only-single-file [h2unit.h](h2unit.h) into 
two files: [h2unit.cpp](build/h2unit.cpp) and [h2unit.hpp](build/h2unit.hpp). Refer to `example/make twofiles` for sample usage.

twofiles speed up 2~3 times than onefile.

# Execute options

*    `-v` *verbose* output including successfull cases
*    `-q` Compact output without failure detail
*    `-l` *list* out suites and cases
*    `-s` *shuffle* cases and execute in random order
*    `-b` [n] *breaking* test once failure occurred
*    `-o` *only* execute last failed cases
*    `-p` show/Hide execute *progressing* (default show)
*    `-r` [n] repeat run n *rounds* when no failure
*    `-c` enable/disable *colorful* output, default is enable
*    `-m` enable/disable *memory* check(leak, overflow, trample, double free, asymmetric free), default is enable
*    `-d/D` *debug* mode, -D for gdb attach but requires password
*    `-f` *fold* simple json object or array 
*    `-y` *copy-paste* JSON C/C++ source code  
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-e` {pattern} *exclude* filter, default is ` ` (exclude nothing)

# Limitations
*    C++11 is required
*    GCC 5.5+ (regex support, SFINAE support)
*    Variadic parameter function can't MOCK, use STUB with separate fake function instead
*    MOCK function arguments up to 15 count
*    sqrt() in math.h can be STUB/MOCK, because compiler insert sqrtsd ASM instruction directly instead of function call
