# Per maggiori informazioni sul comando make:
#     http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# Sulle variabili automatiche vedere:
#     https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
 
# definizione del compilatore e dei flag di compilazione
# che vengono usati dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -O -g
LDLIBS=-lm

EXECS=parole frequenze legginomi

all: $(EXECS)

parole: parole.o strint.o
	$(CC) parole.o strint.o $(LDLIBS) -o parole
	
parole.o: parole.c strint.h
	$(CC) parole.c $(CFLAGS) -c -o parole.o
	
strint.o: strint.c strint.h
	$(CC) strint.c $(CFLAGS) -c -o strint.o

frequenze: frequenze.o strint.o
	$(CC) $^ $(LDLIBS) -o $@
	
frequenze.o: frequenze.c strint.h
	$(CC) $< $(CFLAGS) -c -o $@
	
clean:
		rm -f $(EXECS) *.o
		
		
