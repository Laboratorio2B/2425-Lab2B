# definizione del compilatore e dei flag di compilazione
# che vengono usate dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -g -O -pthread
LDLIBS=-lm -pthread -lrt


# su https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
# sono elencate le regole implicite e le variabili 
# usate dalle regole implicite 

# Variabili automatiche: https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# nei comandi associati ad ogni regola:
#  $@ viene sostituito con il nome del target
#  $< viene sostituito con il primo prerequisito
#  $^ viene sostituito con tutti i prerequisiti

# elenco degli eseguibili da creare
EXECS=moria.out contaprimi.out scrittore.out lettore.out shm0.out shm_prod.out shm_cons.out

# primo target: gli eseguibili sono precondizioni
# quindi verranno tutti creati
all: $(EXECS) 

# regola per la creazione degli eseguibili utilizzando xerrori.o
%.out: %.o xerrori.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# pattern rule che esprime che tutti i .o dipendono da xerrori.h
%.o: %.c xerrori.h


# esempio di target che non corrisponde a una compilazione
# ma esegue la cancellazione dei file oggetto e degli eseguibili
clean: 
	rm *.o $(EXECS)
	
# crea file zip della lezione	
zip:
	zip processi.zip *.c *.h makefile

	
	
	
	

