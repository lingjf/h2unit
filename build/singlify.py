#!/usr/bin/python
#coding=utf-8


import sys


h2unit = open('../h2unit.h', 'w')
with open('../source/h2_unit.h', 'r') as h2_unit:
    for line in h2_unit:
        if line.startswith('#include "h2_'):
            with open('../source/'+line[10:line.rindex('"')], 'r') as h2_f:
                for line in h2_f:
                    h2unit.write(line)
        else:
            h2unit.write(line)

h2unit.write("#ifndef ___H2UNIT_H_3_0__BACK_COMPATIBLE__\n")
h2unit.write("#define ___H2UNIT_H_3_0__BACK_COMPATIBLE__\n")

h2unit.write("#define H2EQ_TRUE H2EQ \n")
h2unit.write("#define H2EQ_MATH H2EQ \n")
h2unit.write("#define H2EQ_STRCMP H2EQ \n")
h2unit.write("#define H2EQ_STRCMP_NOCASE(e,a) H2EQ(CaseLess(e),a) \n")
h2unit.write("#define H2EQ_JSON(e,a) H2EQ(Je(e),a) \n")
h2unit.write("#define H2EQ_WILDCARD(e,a) H2EQ(We(e),a) \n")
h2unit.write("#define H2EQ_REGEX(e,a) H2EQ(Re(e),a) \n")
h2unit.write("#define H2EQ_MEMCMP(e,a,l) H2EQ(Me(e,l),a) \n")

h2unit.write("#define H2LEAK_BLOCK H2BLOCK \n")

h2unit.write("#endif\n")

h2unit.close()

