#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/*
 execvp manual: https://linux.die.net/man/3/execvp
 Learning source: https://raw.githubusercontent.com/Areadrill/HaPOS/master/Operating%20Systems%20-%20Three%20Easy%20Pieces.pdf
*/

int main(int argc, char *argv[]) {
    printf("%d :: examining the exec command instead\n", (int) getpid());
    int rc  = fork();
    if (rc < 0){
        fprintf(stderr, "sacrifice to fork was rejected: %d\n",rc);
        exit(1);
    }
    
    if (rc == 0) {
        printf(" %d: childproc came and conquered\n", (int) getpid());
        char *args[5];
        args[0] = strdup("grep");
        args[1] = strdup("-n");
        args[2] = strdup("this line number");
        args[3] = strdup("execc.c");
        args[4] = NULL;

        execvp(args[0], args);
        // Now this never runs, and I'm assuming it's because the execvp docs mentioned that when the syscall 
        // is called, the process's image is replaced with the shell process
        // So we can say this entire block leading after is nuked
        printf("this guy is sent to the gulag");
    } else {
        // wc is here is blocking mechanism that can and be used a sync mechanism, but shouldn't be 
        // it's akin to sync.WaitGroup{}.Wait() and when the child returns, it's akin to 
        // sync.WaitGroup{}.Done() inside the forked-process. I wonder, does this process keep an internal
        // counter like go's waitgroup does? I mean the proc struct is fat asf so definitely does some tracking
        // unless `disown()` is called by the parent porcess according to what fish tells me sometimes
        int wc = wait(NULL);
        printf("%d :: hmm, how did that turn out?. Stats: rc %d, wc: %d\n", (int) getpid(), rc, wc);
    }
    return 0;
}