#include<stdio.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<unistd.h>
#include <sys/wait.h>
#include<linux/memfd.h>

int main(void){
    char victim[] = "./victim";
    char memFilePath[32] = "\0";
    FILE* vicfd;
    int memfd;
    pid_t child;
    size_t ret;
    printf("\n-----main-----\n");
    printf("My process ID : %d,\nParent process ID: %d\n",getpid(),getppid());  

    //use syscall to memfd_create for creating memory file
    memfd = syscall(SYS_memfd_create, "HowToRunELFinMem", MFD_CLOEXEC);
    printf("My memory fd: %d\n",memfd);

    //get victim binary length
    vicfd = fopen(victim,"r");
    fseek(vicfd, 0L, SEEK_END);
    int fileSZ = ftell(vicfd);
    rewind (vicfd);

    //read binary file to memory file
    char* vicBuf = (char *)malloc(sizeof(char)*fileSZ+1);
    ret = fread(vicBuf,sizeof(char),fileSZ,vicfd);
    if (ret!= fileSZ){
        perror("fread error");
        exit(EXIT_FAILURE);
    }
    write(memfd,vicBuf,fileSZ);

    //get memory file path
    sprintf(memFilePath,"/proc/self/fd/%d",memfd);

    //start execute memfile
    child = fork();
    if (child == 0)
    {
        printf("start execute victim\n");
        execve(memFilePath, NULL,NULL);
        perror("execl finish");
        exit(EXIT_SUCCESS);
    }
    else if (child == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    waitpid(child, NULL, 0);

    printf("finish exec\n");
    return 0;
}