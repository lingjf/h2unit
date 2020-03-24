#!/usr/bin/python
#coding=utf-8

import sys
import time

version_datetime = '/* v{0}  {1} */'.format('5.0', time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()))
project_github_url = '/* https://github.com/lingjf/h2unit */'
software_copyright = '/* Apache Licence 2.0 */'

def copy_line1(line, f): #compat line
    l0 = line.strip()
    if len(l0) and not l0.startswith('//') and not (l0.startswith('/*') and l0.endswith('*/')):
        l1 = line.rstrip('\r\n')
        if l1.endswith('\\'):
            f.write(l1[0:-2])
        else:
            f.write(l1 + '\n')

def copy_line2(line, f):
    f.write(line)

def merge_files(inf, outf):
    for line in inf:
        if line.startswith('#include "h2_'):
            with open('../source/'+line[10:line.rindex('"')], 'r') as f:
                merge_files(f, outf)
        else:
            copy_line2(line, outf)

h2unit_h = '../h2unit.h'
h2unit_hpp = '../h2unit.hpp'
h2unit_cpp = '../h2unit.cpp'
if len(sys.argv) > 1:
    h2unit_h = sys.argv[1] + '/h2unit.h'
    h2unit_hpp = sys.argv[1] + '/h2unit.hpp'
    h2unit_cpp = sys.argv[1] + '/h2unit.cpp'

f_h2unit_h = open(h2unit_h, 'w')
f_h2unit_h.write(version_datetime + '\n')
f_h2unit_h.write(project_github_url + '\n')
f_h2unit_h.write(software_copyright + '\n')
f_h2unit_h.write('#ifndef H2_1FILE' + '\n')
f_h2unit_h.write('#define H2_1FILE' + '\n')
with open('../source/h2_unit.hpp', 'r') as f_h2_unit_hpp:
    merge_files(f_h2_unit_hpp, f_h2unit_h)
with open('../source/h2_unit.cpp', 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_h)
f_h2unit_h.write('#endif' + '\n')
f_h2unit_h.close()


f_h2unit_hpp = open(h2unit_hpp, 'w')
f_h2unit_hpp.write(version_datetime + '\n')
f_h2unit_hpp.write(project_github_url + '\n')
f_h2unit_hpp.write(software_copyright + '\n')
f_h2unit_hpp.write('#ifndef H2_2FILES' + '\n')
f_h2unit_hpp.write('#define H2_2FILES' + '\n')
with open('../source/h2_unit.hpp', 'r') as f_h2_unit_hpp:
    merge_files(f_h2_unit_hpp, f_h2unit_hpp)
f_h2unit_hpp.write('#endif' + '\n')
f_h2unit_hpp.close()

f_h2unit_cpp = open(h2unit_cpp, 'w')
f_h2unit_cpp.write(version_datetime + '\n')
f_h2unit_cpp.write(project_github_url + '\n')
f_h2unit_cpp.write(software_copyright + '\n')
f_h2unit_cpp.write('#include "h2unit.hpp"' + '\n')
f_h2unit_cpp.write('#ifdef H2_2FILES' + '\n')
with open('../source/h2_unit.cpp', 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_cpp)
f_h2unit_cpp.write('#endif' + '\n')
f_h2unit_cpp.close()
