# Frequently asked questions


## mprotect failure

macOS (Mojave 10.14.6) macOS (Catalina 10.15.2) and later macOS mprotect fails, because [max_prot](https://stackoverflow.com/questions/60654834/using-mprotect-to-make-text-segment-writable-on-macos).

```Shell
   printf '\x07' | dd of=<executable> bs=1 seek=160 count=1 conv=notrunc
```

## unprotected commas

C/C++ preprocessor can't parse MACRO arguments which contains comma in '<>' correctly. 

A solution is wrapping such argument with parentheses '()'.


## write-strings/C2440 warning
C++ string literals are const

In C, the type of a string literal is array of char, but in C++, it's array of const char.

```C++
char* s1 = "test"; // Warning
const char* s2 = "test"; // OK
```

## private member method accessibility
In order to STUB/MOCK class private member function successfully, `private` token is substituted with `public` using MACRO definition by default.

Include `h2unit.h/hpp` before other header files.

If test target is C language project, and `private` is used as normal token, define `TEST_C` in compiler options to prevent above substitution.

```Shell
   g++ -DTEST_C ...
```

## static function accessibility
static function is unaccessible outside of source file. In order to STUB/MOCK such function successfully, there are two solutions:

### solution 1: include source file in test file

```C++
   // test_module.cpp
   #include "h2unit.h"
   #include "module.c/cpp"
   CASE(a test)
   {
      ...
   }
```

### solution 2: STUB/MOCK by function string name

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


## STUB in dynamic library

```Shell
   LD_BIND_NOW=1 ./a.out 
   gcc --Wl,z,now
```

## Variadic parameter function can't MOCK

use STUB with separate fake function instead

## sqrt() in math.h can't be STUB/MOCK

compiler insert sqrtsd ASM instruction directly instead of function call

