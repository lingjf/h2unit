
H2FLAGS +=  -I ..  -include h2unit.h


all : h2unit.o product_1.o product_2.o test_product_1.o test_product_2.o 
	g++ -Wall -o run.tests *.o
	./run.tests -vr

	
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
	rm -rf run.tests *.o *.log *.xml *.html Debug *.ncb *.sln *.suo
