# il funzionamento del makefile lo vedremo prossimamente

# definizione del compilatore e dei flag di compilazione
# che vengono usati dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -O -g
LDLIBS=-lm


parole: parole.o strint.o
	$(CC) parole.o strint.o $(LDLIBS) -o parole
	
parole.o: parole.c strint.h
	$(CC) parole.c $(CFLAGS) -c -o parole.o
	
strint.o: strint.c strint.h
	$(CC) strint.c $(CFLAGS) -c -o strint.o

	
	
	
