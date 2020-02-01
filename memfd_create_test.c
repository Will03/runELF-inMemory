#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main()
{
    int fd;
    pid_t child;
    char buf[BUFSIZ] = "";
    char testBuf[256] = "";
    ssize_t br;

    fd = syscall(SYS_memfd_create, "foofile", 0);
    if (fd == -1)
    {
        perror("memfd_create");
        exit(EXIT_FAILURE);
    }

    child = fork();
    if (child == 0)
    {
        dup2(fd, 1);
        close(fd);
        execlp("/bin/date", "/bin/date", NULL);
        perror("execlp date");
        exit(EXIT_FAILURE);
    }
    else if (child == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    waitpid(child, NULL, 0);

    lseek(fd, 0, SEEK_SET);
    br = read(fd, buf, BUFSIZ);
    if (br == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[br] = 0;

    printf("child said: '%s'\n", buf);

    exit(EXIT_SUCCESS);
}