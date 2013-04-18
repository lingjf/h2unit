CFLAGS += -g -I . -include h2unit.h
CXXFLAGS += -g -I . -include h2unit.h

all : h2unit.o product_1.o product_2.o test_product_1.o test_product_2.o 
	g++ -Wall -o run.tests *.o
	./run.tests -vcr

	
h2unit.o : h2unit.cpp h2unit.h
	g++ -Wall $(CXXFLAGS) -c -o h2unit.o h2unit.cpp	
	
product_1.o : example/product_1.c example/product_1.h
	gcc -Wall $(CFLAGS) -c -o product_1.o example/product_1.c
product_2.o : example/product_2.cpp example/product_2.h
	g++ -Wall $(CXXFLAGS) -c -o product_2.o example/product_2.cpp	

test_product_1.o : example/test_product_1.cpp h2unit.h
	g++ -Wall $(CXXFLAGS) -c -o test_product_1.o example/test_product_1.cpp
test_product_2.o : example/test_product_2.cpp h2unit.h
	g++ -Wall $(CXXFLAGS) -c -o test_product_2.o example/test_product_2.cpp
	
clean:
	rm -rf run.tests *.o *.log *.xml *.html
