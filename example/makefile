

all :  
	python ../build/singlify.py
	gcc -Wall -g -c -o product_c.o product_c.c
	g++ -Wall -g -std=c++11 -c -o product_cpp.o product_cpp.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_basic.o test_basic.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_compare.o test_compare.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_logic.o test_logic.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_json.o test_json.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_stub.o test_stub.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_mock.o test_mock.cpp
	g++ -Wall -g -I.. -std=c++11 -c -o test_memory.o test_memory.cpp
	g++ -Wall -o a.out product_c.o product_cpp.o test_basic.o test_compare.o test_logic.o test_json.o test_stub.o test_mock.o test_memory.o
	./a.out
	
# -v : full output
# -r : random execute sequence
# -c : black-white color output	
# -i : include-filter, testcases which unit name or case name match will be executed.
# -x : exclude-filter, testcases which unit name or case name match will be ignored.

	
clean:
	rm -rf a.out *.o *.xml
