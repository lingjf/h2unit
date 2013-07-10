

# Include h2unit.h into product source file in order to enable memory leak detection
# Include h2unit.h into unit test source file to involve macro declare such as H2CASE/H2EQUAL/H2STUB and so on
H2FLAGS +=  -I ..  -include h2unit.h

# h2unit.cpp contains main() which execute test cases
all : h2unit.o demo_basic.o test_basic.o demo_dynamic_stub.o demo_dynamic_stub_cpp.o test_dynamic_stub.o demo_memory_operation.o test_memory_operation.o 
	g++ -Wall -o h2unit.out h2unit.o demo_basic.o test_basic.o demo_dynamic_stub.o demo_dynamic_stub_cpp.o test_dynamic_stub.o demo_memory_operation.o test_memory_operation.o 
	./h2unit.out -v
	
# -v : full output
# -r : random execute sequence
# -b : black-white color output	
# -i : include-filter, testcases which unit name or case name match will be executed.
# -x : exclude-filter, testcases which unit name or case name match will be ignored.

	
h2unit.o : ../h2unit.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o h2unit.o ../h2unit.cpp	
	
	
demo_basic.o : demo_basic.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o demo_basic.o demo_basic.cpp
test_basic.o : test_basic.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o test_basic.o test_basic.cpp
	
demo_dynamic_stub.o : demo_dynamic_stub.c demo_dynamic_stub.h
	gcc -Wall -g $(H2FLAGS) -c -o demo_dynamic_stub.o demo_dynamic_stub.c 
demo_dynamic_stub_cpp.o : demo_dynamic_stub.cpp demo_dynamic_stub.hpp
	g++ -Wall -g $(H2FLAGS) -c -o demo_dynamic_stub_cpp.o demo_dynamic_stub.cpp  
test_dynamic_stub.o : test_dynamic_stub.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o test_dynamic_stub.o test_dynamic_stub.cpp
	
demo_memory_operation.o : demo_memory_operation.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o demo_memory_operation.o demo_memory_operation.cpp
test_memory_operation.o : test_memory_operation.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o test_memory_operation.o test_memory_operation.cpp
	
clean:
	rm -rf h2unit.out *.o *.log *.xml *.html Debug *.ncb *.sln *.suo
