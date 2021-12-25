# Reference
 
## Matcher

*    [`_`](../source/h2_unit.hpp#L321) / [`Any`](../source/h2_unit.hpp#L321)() : matches any value 
*    [`Eq`](../source/h2_unit.hpp#L321)(expect [, epsilon]) : matches if value equals expect (number, string, and object with operator ==), float value near equals expect
*    [`TypeEq`](../source/h2_unit.hpp#L321)<Type>() : matches if type of value is Type 
*    [`Nq`](../source/h2_unit.hpp#L321)(expect) : matches if value not equals expect 
*    [`Ge`](../source/h2_unit.hpp#L321)(expect) : matches if value >= expect 
*    [`Gt`](../source/h2_unit.hpp#L321)(expect) : matches if value > expect 
*    [`Le`](../source/h2_unit.hpp#L321)(expect) : matches if value <= expect 
*    [`Lt`](../source/h2_unit.hpp#L321)(expect) : matches if value < expect 
*    [`Me`](../source/h2_unit.hpp#L321)(buffer, [length], [width]) : matches if memory is same 
*    [`Range`](../source/h2_unit.hpp#L321)([start,] end [, step]) : matches if value in range like python range style  
*    [`Re`](../source/h2_unit.hpp#L321)(expect) : matches if value Regex equals expect 
*    [`We`](../source/h2_unit.hpp#L321)(expect) : matches if value Wildcard equals expect 
*    [`Je`](../source/h2_unit.hpp#L321)(expect) : matches if value JSON equals expect 
*    [`Se`](../source/h2_unit.hpp#L321)(expect) : matches if string strictly equals expect as strcmp() 
*    [`Substr`](../source/h2_unit.hpp#L321)(expect) : matches if value has substr expect 
*    [`StartsWith`](../source/h2_unit.hpp#L321)(expect) : matches if value starts with expect 
*    [`EndsWith`](../source/h2_unit.hpp#L321)(expect) : matches if value ends with expect 
*    [`~`](../source/h2_unit.hpp#L321) / [`CaseLess`](../source/h2_unit.hpp#L321)(expect) : make inner matcher case-insensitive, right operator must be Matcher, `~"Hello World"` not works 
*    [`*`](../source/h2_unit.hpp#L321) / [`SpaceLess`](../source/h2_unit.hpp#L321)(expect) : trim leading and trailing whitespace, squash several whitespaces into one space, right operator must be Matcher, `*"Hello World"` not works 
*    [`Member`](../source/h2_unit.hpp#L321)(expect, Class, member, [arg1, arg2, ...]) : matches if member in object match inner matcher
*    [`Pointee`](../source/h2_unit.hpp#L321)(expect) : matches if point to value equals expect 
*    [`CastOf`](../source/h2_unit.hpp#L321)<Type>(expect) : matches if value cast to Type and equals expect 
*    [`!`](../source/h2_unit.hpp#L321) / [`Not`](../source/h2_unit.hpp#L321)(expect) : matches if not matches inner matcher, right operator must be Matcher, !3 is considered as normal semantics 
*    [`&&`](../source/h2_unit.hpp#L321)(expect) : Logical AND of two matchers, left and right operator shoud at least one Matcher 
*    [`Conditional`](../source/h2_unit.hpp#L321)(condition, expect1, expect2) : matches if value matches expect1 if condition is true, otherwise matches if value matches expect2
*    [`||`](../source/h2_unit.hpp#L321)(expect) : Logical OR of two matchers 
*    [`AllOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value matches all of inner matchers, act as AND logical operator. Calculates the logical conjunction of multiple matchers. Evaluation is shortcut, so subsequent matchers are not called if an earlier matcher returns false.
*    [`AnyOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value matches any one of inner matchers, act as OR logical operator. Calculates the logical disjunction of multiple matchers. Evaluation is shortcut, so subsequent matchers are not called if an earlier matcher returns true.
*    [`NoneOf`](../source/h2_unit.hpp#L321)(expect...) : matches if value not matches all of inner matchers 
*    [`ListOf`](../source/h2_unit.hpp#L321)(expect...) : matches if sequence container(array, vector, ...) or native array items matches inner matchers 
*    [`Every`](../source/h2_unit.hpp#L321)(expect) : matches if container(array, vector, set, map, ...) or native array items all matches inner matcher 
*    [`Has`](../source/h2_unit.hpp#L321)(expect) : matches if there are items in container(vector, set, map, ...) or native array match every inner matcher
*    [`CountOf`](../source/h2_unit.hpp#L321)(expect) : matches if container(array, vector, ...) or native array item count matches inner matcher
*    [`AvgOf`](../source/h2_unit.hpp#L321)(expect) : matches if average of items in container(vector, set, ...) or native array match every inner matcher
*    [`MaxOf`](../source/h2_unit.hpp#L321)(expect) : statistics matches if maximum of items in container(vector, set, ...) or native array match every inner matcher
*    [`MinOf`](../source/h2_unit.hpp#L321)(expect) : statistics matches if minimum of items in container(vector, set, ...) or native array match every inner matcher
*    [`MedianOf`](../source/h2_unit.hpp#L321)(expect) : statistics matches if median of items in container(vector, set, ...) or native array match every inner matcher
*    [`MeanOf`](../source/h2_unit.hpp#L321)(expect) : statistics matches if mean of items in container(vector, set, ...) or native array match every inner matcher


Matcher can be used in OK(expect, actual), for example:
```C++
OK(Ge(1.4142), sqrt(2))
```
It asserts sqrt(2) result 1.41421356237 is greater or equal than 1.4142

### Floating-point Eq (`Eq`)
```C++
CASE(Floating-point comparison)
{
   OK(Eq(3.14, epsilon), a);
}
```
#### absalute margin
```C++
   OK(Eq(3.14, 0.001), a);
```
matches if a in [3.14-0.001, 3.14+0.001].

#### percentage scale
```C++
   OK(Eq(3.14, 0.1%), a);
```
matches if delta in 0.1% range of expect i.e. actual value in [3.14 x 99.9%, 3.14 x 100.1%]

if epsilon is missing, using absalute margin and default epsilon is 0.00001

### Has element in container (`Has`)
Check element existence in container using `Has`.

Following sample is check if 2 exist in vector.
```C++
CASE(has element in vector)
{
   std::vector<int> a = {1, 2, 3};
   OK(Has(2), a);
}
```

Use two parameters in `Has` to check element existence in map container.
```C++
CASE(has element in map)
{
   std::map<std::string, int> a = {{"a": 1}, {"b": 2}, {"c": 3}};
   OK(Has("b", 2), a);
}
```

Also can use `Pair` to combine key and value.
```C++
CASE(has element in map)
{
   std::map<std::string, int> a = {{"a": 1}, {"b": 2}, {"c": 3}};
   OK(Has(Pair("b", 2)), a);
}
```
If only one parameter in `Has` to check key existence in map container.

`HasKey` is other way to check key existence in map constructor.
```C++
CASE(has element in map)
{
   std::map<std::string, int> a = {{"a": 1}, {"b": 2}, {"c": 3}};
   OK(Has("b"), a);
   OK(HasKey("b"), a);
}
```

If want to check value existence in map container, set first parameter to Any, or using `HasValue`.
```C++
CASE(has element in map)
{
   std::map<std::string, int> a = {{"a": 1}, {"b": 2}, {"c": 3}};
   OK(Has(Any, 2), a);
   OK(HasValue(2), a);
}
```

Following is two way to sepecify actual array size, typically used in MOCK, also available in Container matcher (Has/Every/ListOf/MaxOf/MinOf/AvgOf/MeanOf/MedianOf).

```C++
static int a_function(int a[])
{
   ...
}

CASE(sepecify actual array size)
{
   int a1[] = {1, 2, 3};

   OK(MaxOf(3)/3, a1);
   OK(MinOf(1)/3, a1);
   OK(AvgOf(2)/3, a1);
   OK(MeanOf(2)/3, a1);
   OK(MedianOf(2)/3, a1);

   MOCK(a_function, int(int*)).Once(MaxOf(2)/3).Return(1);
   OK(1, a_function(a1));
}

CASE(sepecify actual array size)
{
   int a1[] = {1, 2, 3};

   OK(MaxOf<3>(3), a1);
   OK(MinOf<3>(1), a1);
   OK(AvgOf<3>(2), a1);
   OK(MeanOf<3>(2), a1);
   OK(MedianOf<3>(2), a1);

   MOCK(a_function, int(int*)).Once(MaxOf<3>(2)).Return(1);
   OK(1, a_function(a1));
}
```

Also can specify subset of container.

```C++
static int a_function(int a[])
{
   ...
}

CASE(sepecify subset of container)
{
   int a1[] = {1, 2, 3};

   OK(MaxOf(2) (0, 2), a1);
   OK(MinOf(1) (0, 2), a1);
   OK(AvgOf(1.5) (0, 2), a1);
   OK(MeanOf(1.5) (0, 2), a1);
   OK(MedianOf(1.5) (0, 2), a1);

   MOCK(a_function, int(int*)).Once(MaxOf(1.5) (0, 2)).Return(1);
   OK(1, a_function(a1));
}

```

### Statistics matcher

```C++
CASE(Statistics)
{
   int a1[] = {1, 2, 3};

   OK(MaxOf(3), a1);
   OK(MinOf(1), a1);
   OK(AvgOf(2), a1);
   OK(MeanOf(2), a1);
   OK(MedianOf(2), a1);
}
```

### Member matcher (`Member`)
Used to check member data or member function in object or pointer.

```C++
Class C {
public:
   int count;
   int get(int x, int y) { return ...; }
};
```

#### member data

```C++
CASE(Member data)
{
   C c;
   OK(Member(100, C, count), c);
}
```

`Member(Matcher, Class, member)`

#### member method
```C++
CASE(Member function)
{
   C c;
   OK(Member(100, C, get, 1, 2), c);
   OK(Member(100, C, get, (1, 2)), c);
}
```

`Member(Matcher, Class/BaseClass, method, (type1)arg1, (type2)arg2, ...)`
`Member(Matcher, Class/BaseClass, method, ((type1)arg1, (type2)arg2, ...))`

### Memory compare matcher (`Me`)
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

### User defined Matcher
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
