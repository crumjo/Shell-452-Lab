/*
 * A shell programmed to use the system calls fork(),
 * execvp(), and wait(). Typing 'quit' exits the
 * program.
 *
 * @authors Dylan Shoup & Joshua Crum
 * @version 1/25/18
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/resource.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char* argv[]) {
    pid_t pid;
    //char input[32];
    char *input = (char*) malloc (32 * sizeof(char));
    char* word[16];
    char* buf;
    int i, j, status;
    struct rusage use;
    long totalsw, sw, totalt, time;
    static char *line_read = (char *)NULL;
    totalsw =0;
    totalt = 0;
    
    /* Bind tab to autocomplete. */
    rl_bind_key('\t', rl_complete);
    
    while(1) {
        
        /* Make sure it is not alread allocated. */
        if (line_read) {
            free (line_read);
            line_read = (char *)NULL;
        }
        
        i = 0;
        input = readline("");
        
        /* Run until user types 'quit'. */
        if(strcmp(input, "quit") == 0) {
            free(input);
            return 0;
        }

        /* Tokenize user input. */
        buf = strtok(input, " \n");
        for(j = 0; j < 16; j++){
            word[j] = NULL;
        }

        while(buf != NULL) {
            word[i] = buf;
            buf = strtok(NULL, " \n");
            i++;
        }
        word[i + 1] = NULL;

        /* Check for fork() error. */
        if((pid = fork()) < 0){
            perror("Fork Failure");
            free(input);
            exit(1);
        }

        /* Child branch. */
        else if(pid == 0){
            if(execvp(word[0], word) < 0){
                perror("Exec Failed");
                free(input);
                exit(1);
            }
            exit(0);
        }
        else{ //FIX ME not sure if this is where it goes
            waitpid(-1, &status, 0 );
            
            /* Get the child usage. */
            getrusage(RUSAGE_CHILDREN, &use);

            /* Calculate the time. */
            time = use.ru_utime.tv_usec - totalt;
            totalt = use.ru_utime.tv_usec;
            printf("Time taken (microseconds): %ld\n", time);

            sw = use.ru_nivcsw - totalsw;
            totalsw = use.ru_nivcsw;
            printf("Switches: %ld\n", sw); 
        }
        free(input);
    }
    return 0;
}
