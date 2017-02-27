CC = gcc -std=c99 -g -ggdb3

randmst: randmst.o
	$(CC) -o randmst randmst.o -lm

clean:
	rm -f randmst *.0 &~
