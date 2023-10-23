all: pi_multi-thread.o pi_serial.o
	gcc -o pi_multi-thread pi_multi-thread.o -lpthread
	gcc -o pi_serial pi_serial.o

pi_multi-thread.o: pi_multi-thread.c
	gcc -c pi_multi-thread.c -lpthread
	
pi_serial.o: pi_serial.c
	gcc -c pi_serial.c
	
clean:
	rm -f *.o pi_serial
	rm -f *.o pi_multi-thread