#!/usr/bin/python
# coding=utf-8

import os
import sys
import time
import re
import json

def lcov_to_json():
    data = {
                'Lines': { 'Total': 0, 'Hit': 0 },
                'Functions': { 'Total': 0, 'Hit': 0 },
                'Branches': { 'Total': 0, 'Hit': 0 }
            }
    with open('coverage.lcov', 'r') as f:
        for line in f:
            fnf = re.match('FNF:(.*)', line)
            if fnf:
                data['Functions']['Total'] = data['Functions']['Total'] + int(fnf.group(1))
            fnh = re.match('FNH:(.*)', line)
            if fnh:
                data['Functions']['Hit'] = data['Functions']['Hit'] + int(fnh.group(1))
            lf = re.match('LF:(.*)', line)
            if lf:
                data['Lines']['Total'] = data['Lines']['Total'] + int(lf.group(1))
            lh = re.match('LH:(.*)', line)
            if lh:
                data['Lines']['Hit'] = data['Lines']['Hit'] + int(lh.group(1))
            brf = re.match('BRF:(.*)', line)
            if brf:
                data['Branches']['Total'] = data['Branches']['Total'] + int(brf.group(1))
            brh = re.match('BRH:(.*)', line)
            if brh:
                data['Branches']['Hit'] = data['Branches']['Hit'] + int(brh.group(1))
    
    data['Functions']['Coverage'] = float(data['Functions']['Hit']) / float(data['Functions']['Total'])
    data['Lines']['Coverage'] = float(data['Lines']['Hit']) / float(data['Lines']['Total'])
    data['Branches']['Coverage'] = float(data['Branches']['Hit']) / float(data['Branches']['Total'])
    with open('coverage.json', 'w') as f:
        json.dump(data, f)


os.system('lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.lcov')
if 1 < len(sys.argv): # PATTERN include filter
    os.system('lcov --rc lcov_branch_coverage=1 --extract coverage.lcov "' + sys.argv[1] + '" --output-file coverage.lcov')
os.system('genhtml --rc lcov_branch_coverage=1 coverage.lcov -o coverage')

lcov_to_json()
