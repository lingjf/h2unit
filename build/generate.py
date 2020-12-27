#!/usr/bin/python
# coding=utf-8

print('//////// >>>>> generated by build/generate.py')

def H2PP_EQ(count):
    for i in range(count):
        line = '#define _H2PP_EQ__{0}__{0} H2PP_PROBE()'.format(i)
        print(line)

H2PP_EQ(32)

def do_repeat(n, prefix, index = True, postfix = ''):
    s = ''
    for i in range(n):
        s = s + prefix
        if index:
            s = s + str(i)
        s = s + postfix
    return s

def do_join(count, prefix, split):
    s = ''
    for i in range(count):
        if i > 0:
            s = s + split
        s = s + prefix + str(i)
    return s

def __H2PP_TH(index):
    return '#define _H2PP_TH{0}('.format(index) + do_join(index + 1, '_', ', ') + ', ...) _{}'.format(index)

def H2PP_TH(count):
    for i in range(count):
        print(__H2PP_TH(i))
    for i in range(count):
        h2pp_th = '#define H2PP_TH{0}(...) _H2PP_TH{0}(__VA_ARGS__, '.format(i) + do_repeat(i, ', ', False, '') + ')'
        print(h2pp_th)

print('')
H2PP_TH(32)

def H2PP_INC(count):
    for i in range(count):
        line = '#define H2PP_INC_{} {}'.format(i, i + 1)
        print(line)

def H2PP_DEC(count):
    for i in range(count):
        line = '#define H2PP_DEC_{} {}'.format(i + 1, i)
        print(line)

print('')
H2PP_INC(32)
H2PP_DEC(32)

def _H2PP_RS(prefix, count):
    lines = '#define _H2PP_{}_1(...) __VA_ARGS__\n'.format(prefix)
    for i in range(1, count):
        line = '#define _H2PP_{}_{}(...) _H2PP_{}_{}(_H2PP_{}_{}(__VA_ARGS__))\n'.format(prefix, 2 ** i, prefix, 2 ** (i - 1), prefix, 2 ** (i - 1))
        lines = line + lines
    return lines

print('')
print(_H2PP_RS('EVAL', 11))
print(_H2PP_RS('RESCAN1', 8))
print(_H2PP_RS('RESCAN2', 8))
print(_H2PP_RS('RESCAN3', 8))
print(_H2PP_RS('RESCAN4', 8))

def __H2PP_0x(N):
    ns = '__H2PP_{}TH'.format(N)
    _n = ''
    for i in range(N):
        _n = _n + '_' + str(i) + ', '
    n1 = ''
    for i in range(N - 1):
        n1 = n1 + ', 1' 
    nn = ''
    for i in range(N, 0, -1):
        nn = nn + ', ' + str(i)
    print('#define ' + ns + '(' + _n + '_{}, ...) _{}'.format(N, N))
    print('#ifdef _MSC_VER')
    print('#   define H2PP_HAS_COMMA(...) H2PP_RESCAN(H2PP_MAKE_CALL(' + ns + ', (__VA_ARGS__' + n1 +', 0)))')
    print('#   define H2PP_ARG_COUNT(...) H2PP_RESCAN(H2PP_MAKE_CALL(' + ns + ', (__VA_ARGS__' + nn +')))')
    print('#else')
    print('#   define H2PP_HAS_COMMA(...) ' + ns + '(__VA_ARGS__' + n1 +', 0)')
    print('#   define H2PP_ARG_COUNT(...) ' + ns + '(__VA_ARGS__' + nn +')')
    print('#endif')

__H2PP_0x(64)

print('')
print('//////// <<<<< generated by build/generate.py')
print('')
print('')
print('')
print('//////// >>>>> generated by build/generate.py')

def inspect_string(n):
    s = ''
    if n == 0:
        s = '""'
    for i in range(n):
        if i > 0:
            s = s + ' " " '
        s = s + '#Inspect{0}'.format(i)
    return s

def __H2MOCK_0(count):
    for i in range(count):
        k = '#define __H2MOCK_0_{0}(Function, ReturnType, Arguments'.format(i + 3) + do_repeat(i, ', Inspect') + ') '
        v = 'h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, ' + inspect_string(i) + ', __FILE__, __LINE__)' + do_repeat(i, '.Inspect') + ' = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)'
        print(k + v)

def __H2MOCK_1(count):
    for i in range(count):
        k = '#define __H2MOCK_1_{0}(Class, Method, ReturnType, Arguments'.format(i + 4) + do_repeat(i, ', Inspect') + ') '
        v = 'H2PP_CAT(__H2MOCK_1_{0}_, H2PP_IS_EMPTY Arguments)'.format(i + 4) + '(Class, Method, ReturnType, Arguments' + do_repeat(i, ', Inspect') + ')'
        print(k + v)

def __H2MOCK_1_(count):
    for i in range(count):
        k = '(Class, Method, ReturnType, Arguments' + do_repeat(i, ', Inspect') + ') '
        v = 'h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, ' + inspect_string(i) + ', __FILE__, __LINE__)' + do_repeat(i, '.Inspect')
        print('#define __H2MOCK_1_{0}_1'.format(i + 4) + k + v + ' = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)')
        print('#define __H2MOCK_1_{0}_0'.format(i + 4) + k + v + ' = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)')

__H2MOCK_0(32)
__H2MOCK_1_(32)
__H2MOCK_1(32)

print('')

def __H3MOCK_0(count):
    for i in range(count):
        k = '#define __H3MOCK_0_{0}(Function, ReturnType, Arguments'.format(i + 3) + do_repeat(i, ', Inspect') + ') '
        v = 'h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, ' + inspect_string(i) + ', __FILE__, __LINE__)' + do_repeat(i, '.Inspect')
        print(k + v)

def __H3MOCK_1(count):
    for i in range(count):
        k = '#define __H3MOCK_1_{0}(Class, Method, ReturnType, Arguments'.format(i + 4) + do_repeat(i, ', Inspect') + ') '
        v = 'h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, ' + inspect_string(i) + ', __FILE__, __LINE__)' + do_repeat(i, '.Inspect')
        print(k + v)

__H3MOCK_0(32)
__H3MOCK_1(32)

print('//////// <<<<< generated by build/generate.py')