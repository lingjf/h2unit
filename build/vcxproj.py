import os
import sys
import re

dir = "."

if len(sys.argv) > 1:
    dir = sys.argv[1]

files = os.listdir(dir)
files.sort()
h_files = [file for file in files if file.endswith(".h") or file.endswith(".hpp") or file.endswith(".hxx")]
c_files = [file for file in files if file.endswith(".c") or file.endswith(".cpp") or file.endswith(".cxx") or file.endswith(".cc")]
vcxproj_files = [file for file in files if file.endswith(".vcxproj")]

for vcxproj_file in vcxproj_files:
    with open(os.path.join(dir, vcxproj_file)) as vcxproj:
        h_includes = []
        c_includes = []
        for line in vcxproj:
            t = re.match(r'\s*<ClCompile Include="(.*)" />', line)
            if t:
                c_includes.append(t.group(1))
            t = re.match(r'\s*<ClInclude Include="(.*)" />', line)
            if t:
                h_includes.append(t.group(1))
        l_dir = c_files + h_files
        l_xml = c_includes + h_includes
        for a in l_xml:
            f = False
            for b in l_dir:
                if a == b:
                    f = True
            if not f:
                print(a + " is not exist in repository")
        for b in l_dir:
            f = False
            for a in l_xml:
                if a == b:
                    f = True
            if not f:
                print(b + " is not add to " + vcxproj_file)
        for i in range(len(l_dir)):
            if l_dir[i] != l_xml[i]:
                print(l_dir[i] + "    " + l_xml[i])
                break
