/*
Isaiah Larsen
CS3060-601 Fall 2018
#2
I promise that this source code file has, in its entirety,
been written by myself and by no other person or persons.
If at any time an exact copy of this source code is found
to be used by another person in this term, I understand
that both myself and the student that submitted the
copy will recieve a zero on this assignment.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FD_READ 0
#define FD_WRITE 1
#define SIZE 50

int main(int argc, char *argv[]){
	FILE *fptr = stdin;
	int nums[SIZE];
	pid_t p;
	int fp[2];
	printf("Start\n");
	pipe(fp);
	p = fork();

	//check for process error
	if(p<0){
		perror("Creating process");
		return -1;
	}
	if(p == 0){//Child Process
		close(fp[FD_WRITE]);
		dup2(fp[FD_READ], 0);
		close(fp[FD_READ]);
		//call the bc program
		execlp("bc", "bc", NULL);
		perror("Cannot exec");
		return -1;
	}
	else{
		close(fp[FD_READ]);
		printf("Parent\n");
		//check arguments
		if(argc > 1){//read file
			fptr = fopen(argv[1], "r");

			//Check for null file ptr
			if(fptr == NULL){
				perror("Problem opening file"); 
				return -1;
			}else{
				while(fscanf(fptr, "%d%d%d",&nums[0],&nums[1],&nums[2]) == 3){
					printf("The result of (%d * %d) / %d is ", nums[0],nums[1],nums[2]);
					dprintf(fp[FD_WRITE], "scale=4; (%d*%d)/%d\n",nums[0],nums[1],nums[2]);
					printf("\n");
				}
			}

		}else{//read from stdin
			//read three integers at a time
			while(scanf("%d%d%d", &nums[0],&nums[1],&nums[2]) == 3){
				printf("The result of (%d * %d) / %d is ", nums[0],nums[1],nums[2]);
				dprintf(fp[FD_WRITE], "scale=4; (%d*%d)/%d\n",nums[0],nums[1], nums[2]);

			}
		}

		close(fp[FD_WRITE]);
		int ret;
		wait(&ret);
		printf("Child returned");
	}
	return 0;




}
