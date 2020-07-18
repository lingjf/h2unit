
ifeq ($(shell uname), Darwin) 
	LINKER_FLAGS=-Wl,-segprot,__TEXT,rwx,rwx
else
    LINKER_FLAGS=
endif

GCOV_FLAGS=-fprofile-arcs -ftest-coverage

all : twofiles

onefile : clean singlify
	cp ../h2unit.h .
	gcc -Wall -g $(GCOV_FLAGS) -c -o product_c.o product_c.c
	g++ -Wall -g $(GCOV_FLAGS) -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g -std=c++11 -c -o test_basic.o test_basic.cpp
	g++ -Wall -g -std=c++11 -c -o test_compare.o test_compare.cpp
	g++ -Wall -g -std=c++11 -c -o test_json.o test_json.cpp
	g++ -Wall -g -std=c++11 -c -o test_logic.o test_logic.cpp
	g++ -Wall -g -std=c++11 -c -o test_memory.o test_memory.cpp
	g++ -Wall -g -std=c++11 -c -o test_mock.o test_mock.cpp
	g++ -Wall -g -std=c++11 -c -o test_stub.o test_stub.cpp
	g++ -Wall $(LINKER_FLAGS) -o a.out \
		product_c.o product_cpp.o \
		test_basic.o \
		test_compare.o \
		test_logic.o \
		test_json.o \
		test_stub.o \
		test_mock.o \
		test_memory.o

twofiles : clean singlify
	cp ../build/h2unit.hpp h2unit.h
	cp ../build/h2unit.cpp h2unit.cpp
	gcc -Wall -g $(GCOV_FLAGS) -c -o product_c.o product_c.c
	g++ -Wall -g $(GCOV_FLAGS) -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g -std=c++11 -c -o h2unit.o h2unit.cpp
	g++ -Wall -g -std=c++11 -c -o test_basic.o test_basic.cpp
	g++ -Wall -g -std=c++11 -c -o test_compare.o test_compare.cpp
	g++ -Wall -g -std=c++11 -c -o test_json.o test_json.cpp
	g++ -Wall -g -std=c++11 -c -o test_logic.o test_logic.cpp
	g++ -Wall -g -std=c++11 -c -o test_memory.o test_memory.cpp
	g++ -Wall -g -std=c++11 -c -o test_mock.o test_mock.cpp
	g++ -Wall -g -std=c++11 -c -o test_stub.o test_stub.cpp
	g++ -Wall $(LINKER_FLAGS) $(GCOV_FLAGS) -o a.out \
		product_c.o product_cpp.o h2unit.o \
		test_basic.o \
		test_compare.o \
		test_logic.o \
		test_json.o \
		test_stub.o \
		test_mock.o \
		test_memory.o

coverage : twofiles
	./a.out
	python ../coverage.py

singlify:
	cd ../build && python singlify.py
	
clean:
	# git clean -fdx
