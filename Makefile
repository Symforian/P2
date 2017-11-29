osadnicy: 
	g++ -Wall -Werror -pthread -o osadnicy.o osadnicy.cpp


run:
	./osadnicy.o 10 11 3 7
clean:
	rm *.o
