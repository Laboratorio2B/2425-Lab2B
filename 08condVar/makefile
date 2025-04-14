# definizione del compilatore e dei flag di compilazione
# che vengono usate dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -g -O -pthread
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
EXECS=heap.out rw.out somma_primi_files.out stack.out
PC_EXECS=pctest.out pctestCV.out pctestC11.out atomic_pctestC11.out 


# primo target: gli eseguibili sono precondizioni del target
# quindi verranno tutti creati
all: $(EXECS) 

# dove versioni di prod/con con semafori oppure condvar
pctests: $(PC_EXECS)


# regola per la creazione degli eseguibili utilizzando xerrori.o
%.out: %.o xerrori.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# regola per la creazione di file oggetto che dipendono da xerrori.h
%.o: %.c xerrori.h
	$(CC) $(CFLAGS) -c $<

# compila pctest.c con opzione -O3 per ottimizzare la velocità 
pctest.o: pctest.c xerrori.h
	$(CC) $(CFLAGS) -O3 -c $< -o $@

# come sopra ma definisce la costante USACV quindi vengono
# usate le CV invece dei semafori nel paradigma prod-cons
pctestCV.o: pctest.c xerrori.h
	$(CC) $(CFLAGS) -O3 -c $<  -o $@ -DUSACV

# versione atomic dei precedenti
pctestC11.out: pctestC11.c semC11.h
	$(CC) $(CFLAGS) -O3  $< -o $@
atomic_pctestC11.out: atomic_pctestC11.c semC11.h
	$(CC) $(CFLAGS) -O3  $<  -o $@ 



# esempio di target che non corrisponde a una compilazione
# ma esegue la cancellazione dei file oggetto e degli eseguibili
clean: 
	rm -f *.o $(EXECS) $(PC_EXECS)


