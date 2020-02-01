#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
    printf("-----victim-----\n");
    printf("My process ID : %d,\nParent process ID: %d\n",getpid(),getppid());
    printf("file name = %s\n\n",argv[0]);
    FILE *myself = fopen(argv[0],"r");
    if (myself == NULL){
        while(1){
            printf("can't find myself. run ELF in-memory successfully.\n");
            sleep(5);
        }
    }
    else{
        while(1){

            printf("just a normal ELF\n");
            sleep(5);
        }
    }
}