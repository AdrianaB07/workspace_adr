#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


char shm_fn[] = "my_shm";
char sem_fn[] = "my_sem"; 

// READER 
int main(){
  caddr_t shmptr;
  int shmdes, index;
  sem_t semdes;
  int SHM_SIZE;


  SHM_SIZE = sysconf(_SC_PAGE_SIZE);

  if ( (shmdes = shm_open(shm_fn, O_RDWR, 0)) == -1 ) {
     perror("shm_open failure");
     exit(0);
   }

  if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED,
               shmdes,0)) == (caddr_t) -1){
     perror("mmap FAILURE");
    exit(0);
  }

semdes = sem_open(sem_fn, 0, 0644, 0);

 if(semdes == (void) -1){
   perror("sem_open FAILURE ");
   exit(0);
 }

if(!sem_wait(semdes)){

 
   for(index = 0; index < 100; index++)
        printf("The shared memory shmptr[%d] = %d\n", index,shmptr[index]);

   sem_post(semdes);
  }

  munmap(shmptr, SHM_SIZE);


  close(shmdes);


  sem_close(semdes);
  sem_unlink(sem_fn);
}
