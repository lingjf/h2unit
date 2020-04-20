
COMPILER_FLAGS=-std=c++11

ifeq ($(shell uname), Darwin) 
	LINKER_FLAGS=-Wl,-segprot,__TEXT,rwx,rwx
else
    LINKER_FLAGS=
endif

all : twofiles

onefile : clean singlify
	gcc -Wall -g -c -o product_c.o product_c.c
	g++ -Wall -g -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_basic.o test_basic.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_compare.o test_compare.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_json.o test_json.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_logic.o test_logic.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_memory.o test_memory.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_mock.o test_mock.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../h2unit.h -c -o test_stub.o test_stub.cpp
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
	gcc -Wall -g -c -o product_c.o product_c.c
	g++ -Wall -g -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -c -o h2unit.o ../build/h2unit.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_basic.o test_basic.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_compare.o test_compare.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_json.o test_json.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_logic.o test_logic.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_memory.o test_memory.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_mock.o test_mock.cpp
	g++ -Wall -g $(COMPILER_FLAGS) -include ../build/h2unit.hpp -c -o test_stub.o test_stub.cpp
	g++ -Wall $(LINKER_FLAGS) -o a.out \
		product_c.o product_cpp.o h2unit.o \
		test_basic.o \
		test_compare.o \
		test_logic.o \
		test_json.o \
		test_stub.o \
		test_mock.o \
		test_memory.o


singlify:
	cd ../build && python singlify.py
	
clean:
	rm -rf a.out *.o *.xml .last_order
