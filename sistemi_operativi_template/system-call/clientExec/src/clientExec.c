#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "my_library.h"
#include "errExit.h"

int shmidInt;
int *ptr_count;
int shmid;
struct mynode *ptr_vet;
int semid;

const int semkey=1;
const int shmkey=2;
const int shmkeyint=3;

int main (int argc, char *argv[]) {
    printf("Hi, I'm ClientExec program!\n");

    // check command line input arguments
    if (argc <= 2) {
        printf("Usage: %s username, key, service arguments\n", argv[0]);
        return 0;
    }

    char username[20]="";
    strcpy(username,argv[1]);
    long int key=atol(argv[2]);


//GETTING SHARED MEMORY

    shmid=shmget(shmkey,300*sizeof(struct mynode), IPC_CREAT | S_IRUSR | S_IWUSR );
  	if(shmid==-1)
  			errExit("\nshmget error for shmid");

  	ptr_vet=(struct mynode *)shmat(shmid,NULL,0);

    shmidInt=shmget(shmkeyint,sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR );
  	if(shmidInt==-1)
  		errExit("\nshmget error for shmidInt");
    ptr_count=(int *)shmat(shmidInt,NULL,0);

//GETTING SEMAPHORES SET

    semid=semget(semkey, 1, IPC_CREAT | S_IWUSR | S_IRUSR);
    if (semid == -1)
        errExit("semget failed");

//CHECKING IF THERE'S A CORRISPONDING ENTRY

    int find=0;

    semOp(semid,0 ,-1);


    for(int i=0;i<*ptr_count;i++){
      if((strcmp(ptr_vet[i].id,username)==0) && (ptr_vet[i].key==key)){
          find=1;
          strcpy(ptr_vet[i].id,ptr_vet[*ptr_count-1].id);
          ptr_vet[i].key=ptr_vet[*ptr_count-1].key;
          ptr_vet[i].time=ptr_vet[*ptr_count-1].time;
          (*ptr_count)--;
          break;
      }
    }

    semOp(semid,0,1);

    int service=0;

    if(find==0)
      printf("\nMatching not found\n\n");
    else{
      printf("\nMatching found\n\n");

      service=key%10;

      switch (service) {
        case 1:
          if(execv("stampa", argv)==-1)
            errExit("execl failed");
          break;

        case 2:
          if(execv("salva", argv)==-1)
            errExit("execl failed");
          break;

        case 3:
          if(execv("invia", argv)==-1)
            errExit("execl failed");
          break;

        default:
          printf("\nSome error occured");
          break;

      }

    }

    return 0;
}
