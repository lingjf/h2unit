#!/usr/bin/python
# coding=utf-8

import os
import sys
import time
import re

build_dir = os.path.dirname(os.path.realpath(__file__))

def read_version():
    with open(os.path.join(build_dir, '../source/h2_unit.hpp'), 'r') as f:
        for line in f:
            version = re.match(r'#define H2UNIT_VERSION \s*(.*)', line)
            if version:
                return version.group(1)
    return '    '

version_date = '/* v{0} {1} */'.format(read_version(), time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()))
github_url = '/* https://github.com/lingjf/h2unit */'
copyright = '/* Apache Licence 2.0 */'

# MSVC support:
#   UTF-16 little endian with or without byte order mark (BOM)
#   UTF-16 big endian with or without BOM
#   UTF-8 with BOM
# GCC/clang support:
#   UTF-8 with or without BOM

# minimal common is UTF-8 with BOM
def convert_utf8_to_utf8bom(filename):
    f = open(filename, 'r')
    t = f.read()
    f.close()
    with open(filename, 'wb') as f:
        if sys.version_info.major == 2:
            f.write(t.decode('utf-8').encode('utf-8-sig'))
        elif sys.version_info.major == 3:
            f.write(t.encode('utf-8').decode('utf-8').encode('utf-8-sig'))
        else:
            raise Exception('invalid python version')

def copy_line(line, f):
    f.write(line)

def copy_file(inf, outf):
    for line in inf:
        copy_line(line, outf)

def merge_files(inf, outf, one):
    for line in inf:
        inc = re.match(r'#\s*include "(.*[/]*h2_.*\.[ch]{1}pp)"', line)
        if not inc:
            copy_line(line, outf)
        else:
            if inc.group(1) == 'h2_unit.hpp':
                if one:
                    with open(os.path.join(build_dir, '../source/' + inc.group(1)), 'r') as f:
                        merge_files(f, outf, one)
                else:
                    outf.write('#include "h2unit.hpp"' + '\n')
            else:
                with open(os.path.join(build_dir, '../source/' + inc.group(1)), 'r') as f:
                    outf.write('// source/' + inc.group(1) + '\n')
                    copy_file(f, outf)


h2unit_h = os.path.join(build_dir, '../h2unit.h')
h2unit_hpp = os.path.join(build_dir, 'h2unit.hpp')
h2unit_cpp = os.path.join(build_dir, 'h2unit.cpp')

f_h2unit_h = open(h2unit_h, 'w')
f_h2unit_h.write('\n')
f_h2unit_h.write(version_date + '\n')
f_h2unit_h.write(github_url + '\n')
f_h2unit_h.write(copyright + '\n\n')
f_h2unit_h.write('#ifndef __H2UNIT_H__' + '\n')
f_h2unit_h.write('#define __H2UNIT_H__' + '\n')
with open(os.path.join(build_dir, '../source/h2_unit.cpp'), 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_h, True)
f_h2unit_h.write('#endif' + '\n')
f_h2unit_h.close()
convert_utf8_to_utf8bom(h2unit_h)

f_h2unit_hpp = open(h2unit_hpp, 'w')
f_h2unit_hpp.write('\n')
f_h2unit_hpp.write(version_date + '\n')
f_h2unit_hpp.write(github_url + '\n')
f_h2unit_hpp.write(copyright + '\n\n')
f_h2unit_hpp.write('#ifndef __H2UNIT_HPP__' + '\n')
f_h2unit_hpp.write('#define __H2UNIT_HPP__' + '\n')
with open(os.path.join(build_dir, '../source/h2_unit.hpp'), 'r') as f_h2_unit_hpp:
    merge_files(f_h2_unit_hpp, f_h2unit_hpp, False)
f_h2unit_hpp.write('#endif' + '\n')
f_h2unit_hpp.close()
convert_utf8_to_utf8bom(h2unit_hpp)

f_h2unit_cpp = open(h2unit_cpp, 'w')
f_h2unit_cpp.write('\n')
f_h2unit_cpp.write(version_date + '\n')
f_h2unit_cpp.write(github_url + '\n')
f_h2unit_cpp.write(copyright + '\n')
with open(os.path.join(build_dir, '../source/h2_unit.cpp'), 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_cpp, False)
f_h2unit_cpp.close()
convert_utf8_to_utf8bom(h2unit_cpp)

print('Concatenated ' + version_date)
