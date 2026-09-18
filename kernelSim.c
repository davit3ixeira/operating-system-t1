#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid1, pid2, pid3, pid4, pid5, pid6;
    int fd1[2], fd2[2], fd3[2], fd4[2], fd5[2], fd6[2];
    int buf_A1_A2, buf_A2_A1, buf_A3_A4, buf_A4_A3, buf_A5_A6, buf_A6_A5;

    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    pipe(fd5);
    pipe(fd6);

    pid1 = fork();
    if (pid1 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        printf("Child 1: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a1", "a1", NULL);
        exit(0);
    }
    close(fd1[1]);

    pid2 = fork();
    if (pid2 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        printf("Child 2: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a2", "a2", NULL);
        exit(0);
    }
    close(fd2[1]);

    pid3 = fork();
    if (pid3 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        printf("Child 3: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a3", "a3", NULL);
        exit(0);
    }
    close(fd3[1]);

    pid4 = fork();
    if (pid4 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        printf("Child 4: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a4", "a4", NULL);
        exit(0);
    }
    close(fd4[1]);

    pid5 = fork();
    if (pid5 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd6[0]);
        close(fd6[1]);
        printf("Child 5: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a5", "a5", NULL);
        exit(0);
    }
    close(fd5[1]);

    pid6 = fork();
    if (pid6 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd6[0]);
        printf("Child 6: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a6", "a6", NULL);
        exit(0);
    }
    close(fd6[1]);

    else{
        wait(pid1);
        wait(pid2);
        wait(pid3);
        wait(pid4);
        wait(pid5);
        wait(pid6);
        printf("Parent (KernelSim): PID = %d\n", getpid());
    }

    return 0;
}