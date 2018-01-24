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
//#include <time.h>
#include <sys/resource.h>


int main(int argc, char* argv[]) {
	pid_t pid;
	char input[32];
	char* word[16];
	char* buf;
	int i, j, status;
    	struct rusage use;
	long totalsw, sw, totalt, time;

	totalsw =0;
	totalt = 0;
	while(1) {
		i = 0;
		fgets(input, 16, stdin);
		
        /* Run until user types 'quit'. */
        if(strcmp(input, "quit\n") == 0) {
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
		
		if((pid = fork()) < 0){
			perror("Fork Failure");
			exit(1);
		}
		else if(pid == 0){
			if(execvp(word[0], word) < 0){
				perror("Exec Failed");
				exit(1);
			}
			exit(0);
		}
		else{ //FIX ME not sure if this is where it goes
            		waitpid(-1, &status, 0 );
			getrusage(RUSAGE_CHILDREN, &use);//Get child usage.

            		time = use.ru_utime.tv_usec - totalt;
			totalt = use.ru_utime.tv_usec;
            		printf("Time taken (microseconds): %ld\n", time);

			sw = use.ru_nivcsw - totalsw;
			totalsw = use.ru_nivcsw;
			printf("Switches: %ld\n", sw); 
		}
	}
	return 0;
}
