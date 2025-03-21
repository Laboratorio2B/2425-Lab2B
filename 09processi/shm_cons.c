// esempio produttore/consumatore tra processi
// utilizzando memoria condivisa e named semaphores
// processo consumatore
#include "xerrori.h"

#define QUI __LINE__,__FILE__
#define Buf_size 10 
#define Shm        "/my_shm"
#define Sem_items  "/my_items"
#define Sem_slots  "/my_slots"



int main(int argc,char *argv[])
{
  if(argc!=1) {
    fprintf(stderr,"Uso\n\t%s\n", argv[0]);
    exit(1);
  }

  // richiede che il produttore abbia già creato i semafori
  // e la memoria condivisa per il buffer
  // e che essi siano visibili in /dev/shm

  // ---- apertura buffer prod-cons in memoria condivisa
  int shm_size = Buf_size*sizeof(int);
  int fd = xshm_open(Shm,O_RDWR, 0,QUI);
  xftruncate(fd, shm_size, QUI);
  int *b = simple_mmap(shm_size,fd, QUI);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
   
  // ---- apertura semafori named: i semafori devono esistere
  // e per questo motivo non specifico mode e valore iniziale
  // e di conseguenza non uso xsem_open()
  sem_t *free_slots = sem_open(Sem_slots,0);
  sem_t *data_items = sem_open(Sem_items,0);
  if(free_slots==NULL || data_items==NULL) 
    xtermina("Non riesco ad aprire i semafori",QUI);
 
  // loop consumatore 
  int cindex = 0;
  while(true) {
    xsem_wait(data_items,QUI);
    int d = b[cindex%Buf_size];
    cindex++;
    xsem_post(free_slots,QUI);
    printf("Item %d read\n",d);
    if(d <0) break;
  }
  
  // unmap memoria condivisa e chiude i semafori
  xmunmap(b,shm_size,QUI);
  xsem_close(data_items,QUI);
  xsem_close(free_slots,QUI);
  return 0;
}
