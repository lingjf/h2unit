
ifeq ($(shell uname), Darwin) 
    LINKER_FLAGS=-Wl,-segprot,__TEXT,rwx,rwx
else
    LINKER_FLAGS=
endif

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
	g++ $(GCOV_FLAGS) $(LINKER_FLAGS) -Wall -o a.out product_c.o product_cpp.o test_basic.o test_compare.o test_logic.o test_json.o test_stub.o test_mock.o test_memory.o

cov :
	./a.out
	python ../h2cov.py

clean :
	git clean -fdx
