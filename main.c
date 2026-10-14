#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // 1. expect: ./focker run <cmd> [args...]
    //    if argc < 3, print usage and return 1
    if (argc < 3) {
        printf("expect: ./focker run <cmd> [args...] \n");
        exit(1);
    }
    
    // dissasocciate process created by parent
    if (unshare(CLONE_NEWPID) == -1) {
        perror("unshare");
        exit(1);
    }

    // create child
    pid_t pid = fork();

    // child
    if (pid == 0) {
        if (unshare(CLONE_NEWUTS) == -1) {
            perror("unshare uts");
            exit(1);
        }
        if(sethostname("focker", 6) == -1) {
            perror("sethostname");
            exit(1);
        }
        execvp(argv[2], &argv[2]);
        exit(1);
    } else {
        // reap the child
        int status;
        waitpid(pid, &status, 0);
    }
}