# definizione del compilatore e dei flag di compilazione
# che vengono usate dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -g -O3 -pthread
LDLIBS=-lm -lrt -pthread


# su https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
# sono elencate le regole implicite e le variabili 
# usate dalle regole implicite 

# Variabili automatiche: https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# nei comandi associati ad ogni regola:
#  $@ viene sostituito con il nome del target
#  $< viene sostituito con il primo prerequisito
#  $^ viene sostituito con tutti i prerequisiti

# elenco degli eseguibili da creare
EXECS=stack.out mustack.out spinlock.out atspinlock.out muspinlock.out


# primo target: gli eseguibili sono precondizioni del target
# quindi verranno tutti creati
all: $(EXECS) 

stack.out: stack.c
	$(CC) $(CFLAGS)  $< -o $@ 
mustack.out: stack.c
	$(CC) $(CFLAGS)  $< -o $@ -DUSE_MUTEX

spinlock.out: spinlock.c
	$(CC) $(CFLAGS)  $< -o $@
muspinlock.out: spinlock.c
	$(CC) $(CFLAGS)  $< -o $@ -DUSE_MUTEX
atspinlock.out: spinlock.c
	$(CC) $(CFLAGS)  $< -o $@ -DUSE_ATOMIC_SUM



# esempio di target che non corrisponde a una compilazione
# ma esegue la cancellazione dei file oggetto e degli eseguibili
clean: 
	rm -f *.o $(EXECS)


