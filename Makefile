CC=gcc
CFLAGS=-Wall

main:	*.c 
	$(CC) -o csbnpsr *.c $(CFLAGS)
