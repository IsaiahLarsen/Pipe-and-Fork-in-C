#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FD_READ 0
#define FD_WRITE 1

int main(){
	pid_t p;
	int fp[2];
	//char *opts[4] = {"ls", "-l", "-a", NULL};
	printf("Start\n");
	pipe(fp);
	p = fork();
	if(p<0){
		perror("Creating process");
		return -1;	
	}
	if(p == 0){//Child Process
		close(fp[FD_WRITE]);
		dup2(fp[FD_READ], 0);
		close(fp[FD_READ]);
		execlp("sort", "sort", "-n", NULL);
		perror("Cannot exec");
		return -1;	
	}
	else{
		close(fp[FD_READ]);
		printf("Parent\n");
		write(fp[FD_WRITE], "24 56", 5);
		close(fp[FD_WRITE]);
		int ret;
		wait(&ret);
		printf("Child returned");
	}
	return 0;




}
