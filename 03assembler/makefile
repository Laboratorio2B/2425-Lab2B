# compilatore e flag per macchine arm7
CC=gcc
CFLAGS=-std=c11 -Wall -g 

# compilatore e flag per cross-compilazione
# CC=arm-linux-gnueabihf-gcc
# CFLAGS=-std=c11 -g -Wall -static 


# stampa elenco dei primi <=N
# l'eseguibile primi dipende da primi.c e primo.s
primi: primi.c primo.s
	$(CC) $(CFLAGS) primi.c primo.s -o primi
	
# confronta le stringhe passate sulla linea di comando con strcmp e armcmp
# $^ = insieme dei prerequisiti
# $@ = il target della regola 
strcmp: strcmp.c strcmp.s
	$(CC) $(CFLAGS) $^ -o $@
	

# costruisce array di numeri primi in assembler in array_primi.s
# primo.s contiene la funzione con il test di primalità
# array_primi.c contiene il main e la stampa
# usiamo ancora le variabili automatiche $^ e $@
array_primi: array_primi.c array_primi.s primo.s
	$(CC) $(CFLAGS) $^ -o $@



