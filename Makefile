run: main 
	./main
	
main: main.o calculsecu.o
	gcc -Wall -o main main.o calculsecu.o -g

main.o: calculsecu.h  main.c
	gcc -Wall -c main.c
	
clean: 
	rm -f main.o
	rm -f main
	rm -f calculsecu.o
	rm -f calculsecu
	clear
