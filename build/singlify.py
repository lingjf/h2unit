#!/usr/bin/python
#coding=utf-8


import sys
import time

def copy_line1(line, f): #compat h2unit.h
    l0 = line.strip()
    if len(l0) and not l0.startswith('//') and not (l0.startswith('/*') and l0.endswith('*/')):
        l1 = line.rstrip('\r\n')
        if l1.endswith('\\'):
            f.write(l1[0:-2])
        else:
            f.write(l1 + '\n')

def copy_line2(line, f):
    f.write(line)

h2unit_h = '../h2unit.h'
if len(sys.argv) > 1:
    h2unit_h = sys.argv[1]

version_datetime = '/* v{0}  {1} */'.format('4.2', time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()))
project_github_url = '/* https://github.com/lingjf/h2unit */'
software_copyright = '/* Apache Licence 2.0 */'

f_h2unit = open(h2unit_h, 'w')
f_h2unit.write(version_datetime + '\n')
f_h2unit.write(project_github_url + '\n')
f_h2unit.write(software_copyright + '\n')

with open('../source/h2_unit.h', 'r') as f_h2_unit:
    for line in f_h2_unit:
        if line.startswith('#include "h2_'):
            with open('../source/'+line[10:line.rindex('"')], 'r') as f_h2_cpp:
                for line in f_h2_cpp:
                    if not line.startswith('#include "h2_'):
                        copy_line2(line, f_h2unit)
        else:
            copy_line2(line, f_h2unit)

f_h2unit.close()

