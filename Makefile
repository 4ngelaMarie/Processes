CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: project.c 
	$(CC) $(CFLAGS) -o project.o project.c
	$(CC) $(CFLAGS) -o projectEC.o projectEC.c

debug: project.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o project.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o projectEC.c

clean: 
	rm project.o
	rm projectEC.o
