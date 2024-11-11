#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // rischiesto per usare errno
#include <math.h>



// prototipi delle funzioni che appaiono dopo il main()
void termina(const char *messaggio);


// definizione struct che rappresenta
// una città con nome, e coordinate 
// + campi left/right per costruire l'albero 
typedef struct strint {
  char *s;
  int n;
  struct strint *left, *right;
} strint;


void strint_stampa(const strint *a, FILE *f) {
  fprintf(f,"%20s (%d)\n",a->s,a->n);
}

strint *strint_crea(char *w, int v)
{
  strint *a  = malloc(sizeof(*a));
  a->n = v;
  a->s = strdup(w); // creo una copia di s
  a->left = a->right = NULL;
  return a;
}

void strint_distruggi(strint *a)
{
  free(a->s);
  free(a);
}

// stampa tutti gli elementi dell'albero
// che ha come radice root
void abr_strint_stampa(const strint *root, FILE *f)
{
  if(root!=NULL) {
    abr_strint_stampa(root->left,f);
    strint_stampa(root,f);
    abr_strint_stampa(root->right,f);
  }
}

// stampa tutti gli elementi dell'albero
// che ha come radice root
// facendo una visita in preorder
// il parametro depth è la profondita di root
void abr_strint_stampa_preorder(const strint *root, FILE *f, int depth)
{
  // stampa depth spazi per mostrare la profondità del nodo
  for(int i=0;i<depth;i++) fprintf(f,". ");
  if(root!=NULL) {
    strint_stampa(root,f);
    abr_strint_stampa_preorder(root->left,f,depth+1);
    abr_strint_stampa_preorder(root->right,f,depth+1);
  }
  else fprintf(f,"X\n");
}



// distrugge tutti gli elementi dell'ABR con radice root
void abr_strint_distruggi(strint *root)
{
  if(root!=NULL) {
    abr_strint_distruggi(root->left);
    abr_strint_distruggi(root->right);
    strint_distruggi(root);
  }
}


// inserisci il nuovo nodo "c" dentro l'albero 
// con radice "root", non inserisce se c
// è già presente, restituisce la root
// del nuovo albero contenente anche "c"
strint *abr_inserisci(strint *root, strint *c)
{
  c->left=c->right=NULL;
  // caso base albero vuoto
  if(root==NULL) 
    return c;
  int ris = strcmp(c->s,root->s);
  if(ris==0) {// i nomi sono uguali
    fprintf(stderr, "Nodo duplicato: ");
    strint_stampa(c,stderr);
    strint_distruggi(c);
  }
  else if(ris<0) // c->s < root->s
    root->left = abr_inserisci(root->left,c);
  else // c->s > root->s
    root->right = abr_inserisci(root->right,c);
  return root;
}    
    

// cerca la stringa w dentro l'abr con radice root
// restituisce il puntatore al nodo se trovata
// altrimenti NULL
strint *abr_ricerca(strint *root, char *w)
{
  if(root==NULL) return NULL;
  int cfr = strcmp(w,root->s);
  if(cfr==0) return root;
  else if(cfr<0) // ricerco a sinistra
    return abr_ricerca(root->left,w);
  else // ricerca a destra
    return abr_ricerca(root->right,w);
}

// dato un abr di radice root restituisce
// la sua altezza = numero di livelli =
// profondità massima di una foglia
int abr_altezza(strint *root)
{
  if(root==NULL) return 0;
  int hl = abr_altezza(root->left);
  int hr = abr_altezza(root->right);
  return (hl>hr) ? hl+1 : hr+1;
}


int main(int argc, char *argv[])
{
  if(argc<2) {
    printf("Uso: %s [s1 s2 ...]\n",argv[0]);
    exit(1);
  } 

  // inserisce le stringhe passate sulla linea di comando
  // nell'abr con intero consecutivo 
  strint *root = NULL;
  for(int i=1;i<argc;i++) {
    strint *si = strint_crea(argv[i],i-1);
    root = abr_inserisci(root,si);
  }
  puts("--- inizio visita ---");
  abr_strint_stampa(root,stdout);
  puts("--- fine visita ---");

  abr_strint_distruggi(root);

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

