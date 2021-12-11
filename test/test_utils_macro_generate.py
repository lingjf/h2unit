#!/usr/bin/python
# coding=utf-8

def do_join(count, prefix, split):
    s = ''
    for i in range(count):
        if i > 0:
            s = s + split
        s = s + prefix + str(i)
    return s

def test_h2pp_th(count):
    f.write('SUITE(H2PP_TH [generated])\n')
    f.write('{\n')
    for i in range(count):
        f.write('   int n' + str(i) + ' = -1;' + '\n')
    for i in range(count):
        args = do_join(i, '', ', ')
        f.write('   Case({0} arguments)\n'.format(i))
        f.write('   {\n')
        for j in range(count):
            f.write('      n{0} = H2PP_TH{0}({1}) + 0;'.format(j, args) + '\n')
            if i <= j:
                f.write('      OK(0, n{0});'.format(j) + '\n')
            else:
                f.write('      OK({0}, n{0});'.format(j) + '\n')
        f.write('   }\n')
    f.write('}\n')

def test_h2pp_narg(count):
    f.write('CASE(H2PP_NARG [generated])\n')
    f.write('{\n')
    for i in range(count):
        args = do_join(i, '', ', ')
        f.write('   int n{0} = H2PP_NARG({1});\n'.format(i, args))
        f.write('   OK({0}, n{0});\n'.format(i))
    f.write('}\n')

def test_h2pp_variadic_call(count):
    f.write('CASE(H2PP_VARIADIC_CALL [generated])\n')
    f.write('{\n')
    for i in range(count):
        f.write('#define M{}({}) {}\n'.format(i, do_join(i, '_', ', '), do_join(i, '_', ' + ')))
    f.write('   int n0 = H2PP_VARIADIC_CALL(M) 0;\n'.format(i))
    f.write('   OK(0, n0);\n')
    for i in range(1, count):
        f.write('   int n{0} = H2PP_VARIADIC_CALL(M, {1});\n'.format(i, do_join(i, '', ', ')))
        f.write('   OK({0}, n{1});\n'.format(do_join(i, '', ' + '), i))
    for i in range(count):
        f.write('#undef M{0}\n'.format(i))
    f.write('}\n')

def test_h2pp_repeat(count):
    f.write('SUITE(H2PP_REPEAT [generated])\n')
    f.write('{\n')
    f.write('   int n[{0}];\n'.format(count))
    f.write('   for (int i = 0; i < {0}; ++i) n[i] = -1;\n'.format(count))

    for it in range(count):
        f.write('   Case(repeat {0} split by comma 1 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define RepeatCB(a, i) n[i] = i + a\n')
        f.write('H2PP_REPEAT((, ), RepeatCB, 1, {});\n'.format(it))
        f.write('       for (int i = 0; i < {0}; ++i) OK(i < {1} ? i + 1 : -1, n[i]);\n'.format(count, it))
        f.write('#undef RepeatCB\n')
        f.write('   }\n')

        f.write('   Case(repeat {0} split by space 2 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define ADD(a, b) a + b\n')
        f.write('#define RepeatCB(ab, i) n[i] = i + ADD ab;\n')
        f.write('H2PP_REPEAT( , RepeatCB, (1, 2), {});\n'.format(it))
        f.write('       for (int i = 0; i < {0}; ++i) OK(i < {1} ? i + 1 + 2 : -1, n[i]);\n'.format(count, it))
        f.write('#undef RepeatCB\n')
        f.write('   }\n')
    f.write('}\n')

def test_h2pp_foreach(count):
    f.write('SUITE(H2PP_FOREACH [generated])\n')
    f.write('{\n')
    f.write('   int n[{0}];\n'.format(count))
    f.write('   for (int i = 0; i < {0}; ++i) n[i] = -1;\n'.format(count))

    for it in range(count):
        vector = do_join(it, '', ', ')

        f.write('   Case(foreach {0} split by comma 0 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define ForEachCB(Dummy, i, x) n[i] = i + x\n')
        f.write('H2PP_FOREACH((, ), ForEachCB, (), {});\n'.format(vector))
        f.write('       for (int i = 0; i < {0}; ++i) OK(i < {1} ? i * 2 : -1, n[i]);\n'.format(count, it))
        f.write('#undef ForEachCB\n')
        f.write('   }\n')

        f.write('   Case(foreach {0} split by semicolon 1 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define ForEachCB(a, i, x) n[i] = i + x + a\n')
        f.write('H2PP_FOREACH((; ), ForEachCB, (1), {});\n'.format(vector))
        f.write('       for (int i = 0; i < {0}; ++i) OK(i < {1} ? i * 2 + 1 : -1, n[i]);\n'.format(count, it))
        f.write('#undef ForEachCB\n')
        f.write('   }\n')

    f.write('}\n')

def test_h2pp_fullmesh(count):
    f.write('SUITE(H2PP_FULLMESH [generated])\n')
    f.write('{\n')
    f.write('   int n[{0}][{0}];\n'.format(count))
    f.write('   for (int i = 0; i < {0}; ++i) for (int j = 0; j < {0}; ++j) n[i][j] = -1;\n'.format(count))
    for it in range(count):
        vector = do_join(it, '', ', ')

        f.write('   Case(fullmesh {0} split by comma 1 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define FullMeshCB(a, i, j, x, y) n[i][j] = x*10000 + y*100 + a\n')
        f.write('H2PP_FULLMESH((, ), FullMeshCB, (1), ({}));\n'.format(vector))
        f.write('   for (int i = 0; i < {0}; ++i) for (int j = 0; j < {0}; ++j)\n'.format(count))
        f.write('      OK( i < {0} && j < {0} ? i*10000 + j*100 + 1 : -1, n[i][j]);\n'.format(it))

        f.write('#undef FullMeshCB\n')
        f.write('   }\n')

        f.write('   Case(fullmesh {0} split by semicolon 1 args)\n'.format(it))
        f.write('   {\n')
        f.write('#define FullMeshCB(a, i, j, x, y) n[i][j] = x*10000 + y*100 + a\n')
        f.write('H2PP_FULLMESH((; ), FullMeshCB, (1), ({0}), ({0}));\n'.format(vector))
        f.write('   for (int i = 0; i < {0}; ++i) for (int j = 0; j < {0}; ++j)\n'.format(count))
        f.write('      OK( i < {0} && j < {0} ? i*10000 + j*100 + 1 : -1, n[i][j]);\n'.format(it))

        f.write('#undef FullMeshCB\n')
        f.write('   }\n')

    f.write('}\n')

with open('test_utils_macro_generated.cpp', 'w') as f:
    f.write('//////// generated by test/test_utils_macro_generate.py do not edit\n')
    f.write('#include "../source/h2_unit.cpp"\n')
    test_h2pp_th(32)
    test_h2pp_narg(32)
    test_h2pp_variadic_call(32)
    test_h2pp_repeat(32)
    test_h2pp_foreach(32)
    test_h2pp_fullmesh(32)
