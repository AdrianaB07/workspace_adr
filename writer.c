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

// WRITER 

int main(){
  caddr_t shmptr;
  unsigned int mode;
  int shmdes, index;
  sem_t sem_des;
  int SHM_SIZE;

  mode = S_IRWXU|S_IRWXG;

  if ( (shmdes = shm_open(shm_fn,O_CREAT|O_RDWR|O_TRUNC, mode)) == -1 ) {
     perror("shm_open failure");
     exit(0);
   }
  SHM_SIZE = sysconf(_SC_PAGE_SIZE);

  if(ftruncate(shmdes, SHM_SIZE) == -1){
    perror("ftruncate failure");
    exit(0);
  }

  if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED,
                shmdes,0)) == (caddr_t) -1){
    perror("mmap failure");
    exit(0);
  }
sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);

 if(sem_des == (void)-1){
   perror("sem_open failure");
   exit(0);
 }
 
    for(index = 0; index <= 1000; index++){

       printf("write %d into the shared memory shmptr[%d]\n", index, index);
       shmptr[index]=index;
       }
  sem_post(sem_des);
  munmap(shmptr, SHM_SIZE);
  /* ----------------------------*/
  close(shmdes); 
  sem_close(sem_des); 
  shm_unlink(shm_fn); 
}
