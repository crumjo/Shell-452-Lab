#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]){
	pid_t pid;
	char input[32];
	char* word[16];
	char* buf;
	int i,j;
	while(strcmp(input,"quit")!=0){
		i=0;
		fgets(input,16,stdin);
		
		buf = strtok(input, " \n");
		for(j = 0; j < 16; j++){
			word[j] = NULL;
		}	
		while(buf!=NULL){
			word[i] = buf;
			buf = strtok(NULL, " \n");
			i++;
		}
		word[i+1] = NULL;
		
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
		else{
			wait(0);
		}
	}
	return 0;
}
