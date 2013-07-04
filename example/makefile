

# Include h2unit.h into product source file in order to enable memory leak detection
# Include h2unit.h into unit test source file to involve macro declare such as H2CASE/H2EQUAL/H2STUB and so on
H2FLAGS +=  -I ..  -include h2unit.h

# h2unit.cpp contains main() which execute test cases
all : h2unit.o product_1.o product_2.o test_product_1.o test_product_2.o 
	g++ -Wall -o h2unit.out h2unit.o product_1.o product_2.o test_product_1.o test_product_2.o 
	./h2unit.out -v
	
# -v : full output
# -r : random execute sequence
# -b : black-white color output	

	
h2unit.o : ../h2unit.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o h2unit.o ../h2unit.cpp	
	
product_1.o : product_1.c product_1.h
	gcc -Wall -g $(H2FLAGS) -c -o product_1.o product_1.c
product_2.o : product_2.cpp product_2.h
	g++ -Wall -g $(H2FLAGS) -c -o product_2.o product_2.cpp	

test_product_1.o : test_product_1.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o test_product_1.o test_product_1.cpp
test_product_2.o : test_product_2.cpp ../h2unit.h
	g++ -Wall -g $(H2FLAGS) -c -o test_product_2.o test_product_2.cpp
	
clean:
	rm -rf h2unit.out *.o *.log *.xml *.html Debug *.ncb *.sln *.suo
