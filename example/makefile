
GCOV_FLAGS=-fprofile-arcs -ftest-coverage

all :
	gcc $(GCOV_FLAGS) -Wall -g -c -o product_c.o product_c.c
	g++ $(GCOV_FLAGS) -Wall -g -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g -std=c++11 -c -o test_basic.o test_basic.cpp
	g++ -Wall -g -std=c++11 -c -o test_compare.o test_compare.cpp
	g++ -Wall -g -std=c++11 -c -o test_json.o test_json.cpp
	g++ -Wall -g -std=c++11 -c -o test_logic.o test_logic.cpp
	g++ -Wall -g -std=c++11 -c -o test_memory.o test_memory.cpp
	g++ -Wall -g -std=c++11 -c -o test_mock.o test_mock.cpp
	g++ -Wall -g -std=c++11 -c -o test_stub.o test_stub.cpp
	g++ $(GCOV_FLAGS) -rdynamic -Wall -o a.out product_c.o product_cpp.o test_basic.o test_compare.o test_logic.o test_json.o test_stub.o test_mock.o test_memory.o
	@if [ $(shell uname) = "Darwin" ]; then printf '\x07' | dd of=a.out bs=1 seek=160 count=1 conv=notrunc; fi
cov :
	./a.out
	python ../h2cov.py

clean :
	git clean -fdx
