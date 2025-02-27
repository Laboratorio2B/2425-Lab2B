

## Esercizi



### Sort parallelo (27/2/25)

Il programma `threadSort.c` utilizza `qsort` per ordinare un array di interi. Modificarlo in maniera da velocizzare l'ordinamento utilizzando *un* thread ausiliario. L'ordinamento deve essere fatto secondo il seguente schema:

1. l'array da ordinare viene logicamente diviso in due: la prima metà viene ordinata dal thread ausiliario, la seconda metà dal theread principale. Gli ordinamenti delle due metàpossono essere fatte con `qsort`

2. Le due metà ordinate devono essere combinate nel risultato finale utilizzando la funzione `merge()` che trovate nel sorgente. 


Completata questa prima parte, scrivere una nuova versione che utilizza *tre* thread ausiliari (quindi quattro in totale). Una coppia di thread deve effettare il sorting della prima metà utilizzando la strategia vista sopra, una seconda coppia ordina la seconda metà e alla fine il thread principale effettua il merge finale.

E' possibile risolvere l'esercizio utilizzando solo le funzione `pthread_create` e `pthread_join`, ma è comunque ammesso l'uso di mutex e semafori. 



