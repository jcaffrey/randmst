CC=gcc -std=c99
CFLAGS=-I.

randmst: randmst.o 
	$(CC) -o randmst randmst.o -I.
