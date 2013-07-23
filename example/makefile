

# Include h2unit.h into product source file in order to enable memory leak detection
# Include h2unit.h into unit test source file to involve macro declare such as H2CASE/H2EQ_/H2STUB and so on
H2FLAGS +=  -I.. -include h2unit.h

# h2unit.cpp contains main() which execute test cases
all :  
	g++ -Wall -g $(H2FLAGS) -c -o h2unit.o ../h2unit.cpp	
	g++ -Wall -g $(H2FLAGS) -c -o demo_basic.o demo_basic.cpp
	g++ -Wall -g $(H2FLAGS) -c -o test_basic.o test_basic.cpp
	gcc -Wall -g $(H2FLAGS) -c -o demo_dynamic_stub.o demo_dynamic_stub.c 
	g++ -Wall -g $(H2FLAGS) -c -o demo_dynamic_stub_cpp.o demo_dynamic_stub.cpp  
	g++ -Wall -g $(H2FLAGS) -c -o test_dynamic_stub.o test_dynamic_stub.cpp
	g++ -Wall -g $(H2FLAGS) -c -o demo_memory.o demo_memory.cpp
	g++ -Wall -g $(H2FLAGS) -c -o test_memory.o test_memory.cpp
	g++ -Wall -g $(H2FLAGS) -c -o demo_exception.o demo_exception.cpp
	g++ -Wall -g $(H2FLAGS) -c -o test_exception.o test_exception.cpp
	g++ -Wall -o a.out h2unit.o demo_basic.o test_basic.o demo_dynamic_stub.o demo_dynamic_stub_cpp.o test_dynamic_stub.o demo_exception.o test_exception.o demo_memory.o test_memory.o 
	./a.out -v
	
# -v : full output
# -r : random execute sequence
# -b : black-white color output	
# -i : include-filter, testcases which unit name or case name match will be executed.
# -x : exclude-filter, testcases which unit name or case name match will be ignored.

	
	
clean:
	rm -rf a.out *.o *.log *.xml *.html Debug *.ncb *.sln *.suo
