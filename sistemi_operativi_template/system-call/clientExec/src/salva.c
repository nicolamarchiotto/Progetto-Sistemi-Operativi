#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_library.h"
#include "errExit.h"

int main (int argc, char *argv[]) {

    char buf[50]="";

    if(argc<5){
      errExit("Wrong usage of Salva program! filename_destionation text_to_write");
    }

    printf("Hi, I'm Salva program!\n");

    int file=open(argv[3], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if(file==-1)
      printf("\nErrore open file per servizio Stampa");

    for(int i=4;i<argc;i++){
      strcat(buf, argv[i]);
      strcat(buf, " ");
    }

    ssize_t w=write(file, buf, strlen(buf));
    if(w==-1)
      printf("\nErrore write per il servizio Stampa");
    else
      printf("\nService Stampa completed\n");

    close(file);

    return 0;
}
