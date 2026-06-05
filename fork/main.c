#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 Linux man on wait: https://man7.org/linux/man-pages/man2/wait.2.html
 Learning source: https://raw.githubusercontent.com/Areadrill/HaPOS/master/Operating%20Systems%20-%20Three%20Easy%20Pieces.pdf
*/

int main(int argc, char *argv[]) {
    printf("%d :: man, what are we doing?\n", (int) getpid());

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "the kernel rejected our sacrifice to fork:: %d\n", rc);
        exit(1);
    }

    // my question is why does the fork start executing from here, the og 
    // source code on `Interlude: Process API` has it tagged as an else-if statement
    if (rc == 0) {
        printf(" %d :: do you know else is a fork of their parent process:[]?\n", (int) getpid());
    } else {
        // according to man, it says wait Waits for all background job
        // But since the os performs virtualisation, this program gets ran it's own 'virtual space'
        // And then calls `fork` and that runs the background of this process. So by saying `wait(NULL)` 
        // All programs that are children of **this** program would wait. So by convention, if we had 10k 
        // processes all doing some heavy task, calling wait(NULL) will indeed wait for all of them to exit
        // and I assume the OS does to a certain degree as well? Thats why even when NIX shutsdown, it just 
        // shows a bunch of logs first. Also makes sense of Go's `context` and `sync.WaitGroup` packagees or Java and Rusts `thread.join() -> Handle` semantics

        int wc = wait(NULL);
        printf("%d :: it's me again , I'm responsible for %d's behaviour.\n", getpid(), rc, (int) getpid() );
    }


    return 0;
}

/*
  cprojects::fork | gcc main.c -o main && ./main
29526 :: man, what are we doing?
 29527 :: do you know else is a fork of their parent process:[]?
29526 :: it's me again , I'm responsible for 29527's behaviour.
  cprojects::fork | 
  cprojects::fork | 
 */