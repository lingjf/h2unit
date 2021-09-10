
all : coverage

build :
	gcc -Wall -g -fprofile-arcs -ftest-coverage -c -o product_c.o product_c.c
	g++ -Wall -g -fprofile-arcs -ftest-coverage -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g -c -std=c++11 -o test_basic.o test_basic.cpp
	g++ -Wall -g -c -std=c++11 -o test_compare.o test_compare.cpp
	g++ -Wall -g -c -std=c++11 -o test_json.o test_json.cpp
	g++ -Wall -g -c -std=c++11 -o test_logic.o test_logic.cpp
	g++ -Wall -g -c -std=c++11 -o test_memory.o test_memory.cpp
	g++ -Wall -g -c -std=c++11 -o test_mock.o test_mock.cpp
	g++ -Wall -g -c -std=c++11 -o test_stub.o test_stub.cpp
	g++ -rdynamic -Wall -g -fprofile-arcs -ftest-coverage -o a.out product_c.o product_cpp.o test_basic.o test_compare.o test_logic.o test_json.o test_stub.o test_mock.o test_memory.o
	@if [ $(shell uname) = "Darwin" ]; then printf '\x07' | dd of=a.out bs=1 seek=160 count=1 conv=notrunc; fi

run : build
	./a.out

coverage : run
	gcovr --root . --filter 'product_*' --json-summary -o coverage.json
	gcovr -r . -e 'test_*' --html --html-details -o coverage.html

lcov : run
	lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.lcov
	lcov --rc lcov_branch_coverage=1 --extract coverage.lcov '*/product_*' --output-file coverage.lcov
	genhtml --rc lcov_branch_coverage=1 coverage.lcov -o coverage

clean :
	git clean -fdx
