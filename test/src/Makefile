CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -I..


all:bms2A.o bms2B.o
	gcc -o bms2A bms2A.o -L. -lecc -lm
	gcc -o bms2B bms2B.o -L. -lecc -lm

clean:
	rm -f *.o bms2A bms2B

