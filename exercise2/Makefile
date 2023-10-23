all: sum_multi-thread.o sum_serial.o
	gcc -o sum_multi-thread sum_multi-thread.o -lpthread
	gcc -o sum_serial sum_serial.o

sum_multi-thread.o: sum_multi-thread.c
	gcc -c sum_multi-thread.c -lpthread
	
sum_serial.o: sum_serial.c
	gcc -c sum_serial.c
	
clean:
	rm -f *.o sum_serial
	rm -f *.o sum_multi-thread