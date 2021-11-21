import os
import re

with open("test.vcxproj") as vcxproj:
    lXml = []
    for line in vcxproj:
        b = re.match(r'\s*<ClCompile Include="(.*)" />', line)
        if b:
            lXml.append(b.group(1))
    lDir = [file for file in os.listdir(".") if file.endswith(".cpp")]
    lDir.sort()
    for a in lXml:
        f = False
        for b in lDir:
            if a == b:
                f = True
        if not f:
            print(a + " is not exist in repository")
    for b in lDir:
        f = False
        for a in lXml:
            if a == b:
                f = True
        if not f:
            print(b + " is not add to test.vcxproj")
    for i in range(len(lDir)):
        if lDir[i] != lXml[i]:
            print(lDir[i] + "    " + lXml[i])
            break
