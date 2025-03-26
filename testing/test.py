
import unittest
import subprocess
import re


def normalize_milliseconds(s0):
    s1 = re.sub(r'[\d\.]+ milliseconds*', '- [durations]', s0)
    s2 = re.sub(r'[\d\.]+ seconds*', '- [durations]', s1)
    s3 = re.sub(r'[\d\.]+ minutes*', '- [durations]', s2)
    s4 = re.sub(r'[\d\.]+ hours*', '- [durations]', s3)
    return s4


def h2read(path):
    with open(path, 'r') as f:
        return f.read()


def h2test(executable):
    actually = executable.replace('.out', '.act')
    with open(actually, 'w') as f:
        subprocess.call(executable, shell=True,
                        stdout=f, stderr=subprocess.STDOUT)
    return h2read(actually)


def do_test(executable, expectation):
    a1 = h2test(executable)
    a2 = normalize_milliseconds(a1)
    e1 = h2read(expectation)
    e2 = normalize_milliseconds(e1)
    # print(a1)
    # print(e1)
    return a2, e2


class TestConsoleOutput(unittest.TestCase):

    def test_1pass(self):
        a, e = do_test('./test_1pass.out', './test_1pass.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_1fail(self):
        a, e = do_test('./test_1fail.out', './test_1fail.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_1pass_2fail(self):
        a, e = do_test('./test_1pass_2fail.out', './test_1pass_2fail.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_equation(self):
        a, e = do_test('./test_equation.out', './test_equation.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_inequation(self):
        a, e = do_test('./test_inequation.out', './test_inequation.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_floating_point(self):
        a, e = do_test('./test_floating_point.out', './test_floating_point.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)

    def test_todo(self):
        a, e = do_test('./test_todo.out', './test_todo.exp')
        self.assertEqual(len(a), len(e))
        self.assertEqual(a, e)


if __name__ == '__main__':
    unittest.main()
