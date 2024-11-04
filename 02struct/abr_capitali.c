#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // rischiesto per usare errno
#include <math.h>

// Scopo del programma:
// imparare a costruire, visualizzare e distruggere le liste in C


// prototipi delle funzioni che appaiono dopo il main()
void termina(const char *messaggio);


// definzione struct che rappresenta
// una città con nome, e coordinate 
// + campi left/right per costruire l'albero 
typedef struct capit {
  char *nome;
  double lat;
  double lon;
  struct capit *left, *right;
} capitale;


void capitale_stampa(const capitale *a, FILE *f) {
  fprintf(f,"%-16s (%f,%f)\n",a->nome,a->lat,a->lon);
}

capitale *capitale_crea(char *s, double lat, double lon)
{
  capitale *a  = malloc(sizeof(*a));
  a->lat = lat;
  a->lon = lon;
  a->nome = strdup(s); // creo una copia di s e l'assegno al nome
  a->left = a->right = NULL;
  return a;
}

void capitale_distruggi(capitale *a)
{
  free(a->nome);
  free(a);
}

// stampa tutti gli elementi dell'albero
// che ha come radice root
void abr_capitale_stampa(const capitale *root, FILE *f)
{
  if(root!=NULL) {
    abr_capitale_stampa(root->left,f);
    capitale_stampa(root,f);
    abr_capitale_stampa(root->right,f);
  }
}

// distrugge tutti gli elementi della lista lis
void abr_capitale_distruggi(capitale *root)
{
  if(root!=NULL) {
    abr_capitale_distruggi(root->left);
    abr_capitale_distruggi(root->right);
    capitale_distruggi(root);
  }
}


// legge un oggetto capitale dal file f
// restituisce il puntatore all'oggetto letto
// oppure NULL se non riesce a leggere dal file
capitale *capitale_leggi(FILE *f)
{
  assert(f!=NULL);
  char *s;
  double lat, lon;
  int e = fscanf(f,"%ms %lf %lf",&s,&lat,&lon);
  if(e!=3) {
    if(e==EOF) return NULL; // finito il file non ci sono altri dati
    else termina("Formato input file non valido");
  }
  capitale *c = capitale_crea(s,lat,lon);
  free(s);
  return c;
}

// inserisci il nuovo nodo "c" dentro l'albero 
// con root "radice", non inserisce se c
// è già presente, restituisce la root
// del nouvo albero contenente anche "c"
capitale *inserisci_abr(capitale *radice, capitale *c)
{
  c->left=c->right=NULL;
  // caso base albero vuoto
  if(radice==NULL) 
    return c;
  int ris = strcmp(c->nome,radice->nome);
  if(ris==0) {// i nomi sono uguali
    fprintf(stderr, "Nodo duplicato: ");
    capitale_stampa(c,stderr);
    capitale_distruggi(c);
  }
  else if(ris<0) // c->nome < radice->nome
    radice->left = inserisci_abr(radice->left,c);
  else // c->nome > radice->nome
    radice->right = inserisci_abr(radice->right,c);
  return radice;
}    
    

// crea un abr con gli oggetti capitale letti da 
// *f inserendoli usando l'ordinamento dei nomi 
capitale *crea_abr(FILE *f)
{
  capitale *radice=NULL;
  while(true) {
    capitale *b = capitale_leggi(f);
    if(b==NULL) break;
    // inserisco b in testa alla lista
    radice = inserisci_abr(radice,b);
  }  
  return radice;
}



int main(int argc, char *argv[])
{
  if(argc!=2) {
    printf("Uso: %s nomefile\n",argv[0]);
    exit(1);
  } 
  FILE *f = fopen(argv[1],"r");
  if(f==NULL) termina("Errore apertura file");

  // costruzione albero leggendo capitali dal file
  capitale *radice=crea_abr(f);
  puts("--- inizio lista ---");
  // stampa lista capitali appena creata
  abr_capitale_stampa(radice,stdout);  
  puts("--- fine lista ---");
  abr_capitale_distruggi(radice);

  if(fclose(f)==EOF)
    termina("Errore chiusura");
  
  return 0;
}



// stampa su stderr il  messaggio che gli passo
// se errno!=0 stampa anche il messaggio d'errore associato 
// a errno. dopo queste stampe termina il programma
void termina(const char *messaggio)
{
  if(errno==0) 
     fprintf(stderr,"%s\n",messaggio);
  else 
    perror(messaggio);
  exit(1);
}

