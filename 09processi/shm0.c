#include "xerrori.h"

// esempio semplice creazione di un array in memoria condivisa
// l'array viene semplicemente scritto, lo ritrovo in /dev/shm
// a meno che non prenoti la cancellazione con sem_unlink

#define QUI __LINE__,__FILE__


int main(int argc,char *argv[])
{
  if(argc!=3) {
    fprintf(stderr,"Uso\n\t%s nome_shared_mem dim_array\n", argv[0]);
    exit(1);
  }
  // conversione input per ottenere dimensione array
  int n= atoi(argv[2]);
  if(n<1) termina("limite non valido");

  // ---- creazione array memoria condivisa
  int shm_size = n*sizeof(int); // numero byte nella memoria condivisa
  int fd = xshm_open(argv[1],O_RDWR | O_CREAT, 0660,QUI);
  xftruncate(fd, shm_size, QUI);
  int *a = simple_mmap(shm_size,fd, QUI);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  // scommentare per prenotare la cancellazione dell'oggetto nella shared memory 
  xshm_unlink(argv[1],QUI); // distrugge shm quando finito
  
  // riempio array
  for(int i=0; i<n; i++) {
    a[i] = i;
  }
  
  // unmap memoria condivisa e termina
  xmunmap(a,shm_size,QUI);

  // se ho commentato xshm_unlink() l'oggetto /dev/sham/nome
  // rimane nel filesystem (e occupa il relativo spazio fino al prossimo boot)
  return 0;
}

