import subprocess

def test(cxx, std):
    print(cxx + ' ' + std + ' -fno-access-control test.cpp')
    try:
        subprocess.check_output([cxx, '-fno-access-control', std, 'test.cpp']).decode('ascii').strip()
        return True
    except:
        return False

for i in [7, 8, 9, 10, 11]:
    for j in ['11', '14', '17', '2a', '2b']:
        test('g++-' + str(i), '-std=c++' + j)

for i in [7, 8, 9, 10, 11, 12]:
    for j in ['11', '14', '17', '2a', '2b']:
        test('clang++-' + str(i), '-std=c++' + j)
