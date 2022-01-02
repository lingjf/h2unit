
import os
import sys
import re
import subprocess
from datetime import datetime

if sys.version_info > (3, 2):
    from datetime import timezone, timedelta


h2unit_version = '5.17'


build_dir = os.path.dirname(os.path.realpath(__file__))


def get_date_time():
    if sys.version_info > (3, 2):
        return datetime.today().astimezone(timezone(timedelta(hours=+8))).strftime('%Y-%m-%d')
    else:
        return datetime.today().strftime('%Y-%m-%d')


def get_git_commit_hash():
    return subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('ascii').strip()


def get_git_branch_tag():
    c = 'git symbolic-ref -q --short HEAD || git describe --tags --exact-match --all'
    t = subprocess.check_output(c, shell=True).decode('ascii').strip()
    if not t.startswith('tags/'):
        return 'branches/' + t
    return t


def get_revision():
    return get_git_branch_tag() + '/' + get_git_commit_hash()


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
f_h2unit_h.write('/* https://github.com/lingjf/h2unit */\n')
f_h2unit_h.write('/* Apache Licence 2.0 */\n\n')
f_h2unit_h.write('#ifndef __H2UNIT_H__' + '\n')
f_h2unit_h.write('#define __H2UNIT_H__' + '\n')
f_h2unit_h.write('#define H2UNIT_VERSION ' + h2unit_version + '\n')
f_h2unit_h.write('#define H2UNIT_DATE ' + get_date_time() + '\n')
f_h2unit_h.write('#define H2UNIT_REVISION ' + get_revision() + '\n')
with open(os.path.join(build_dir, '../source/h2_unit.cpp'), 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_h, True)
f_h2unit_h.write('#endif' + '\n')
f_h2unit_h.close()

f_h2unit_hpp = open(h2unit_hpp, 'w')
f_h2unit_hpp.write('\n')
f_h2unit_hpp.write('/* https://github.com/lingjf/h2unit */\n')
f_h2unit_hpp.write('/* Apache Licence 2.0 */\n\n')
f_h2unit_hpp.write('#ifndef __H2UNIT_HPP__' + '\n')
f_h2unit_hpp.write('#define __H2UNIT_HPP__' + '\n')
f_h2unit_hpp.write('#define H2UNIT_VERSION ' + h2unit_version + '\n')
f_h2unit_hpp.write('#define H2UNIT_DATE ' + get_date_time() + '\n')
f_h2unit_hpp.write('#define H2UNIT_REVISION ' + get_revision() + '\n')
with open(os.path.join(build_dir, '../source/h2_unit.hpp'), 'r') as f_h2_unit_hpp:
    merge_files(f_h2_unit_hpp, f_h2unit_hpp, False)
f_h2unit_hpp.write('#endif' + '\n')
f_h2unit_hpp.close()

f_h2unit_cpp = open(h2unit_cpp, 'w')
f_h2unit_cpp.write('\n')
f_h2unit_cpp.write('/* https://github.com/lingjf/h2unit */\n')
f_h2unit_cpp.write('/* Apache Licence 2.0 */\n\n')
with open(os.path.join(build_dir, '../source/h2_unit.cpp'), 'r') as f_h2_unit_cpp:
    merge_files(f_h2_unit_cpp, f_h2unit_cpp, False)
f_h2unit_cpp.close()


print('Concatenated v{0} {1} {2} {3} '.format(h2unit_version, datetime.today().strftime('%Y-%m-%d %H:%M:%S'), get_git_branch_tag(), get_git_commit_hash()))
