#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}
	
	//create a 2x2 array to seperate paramaters: columns are seperated by "&"
	int amprsnd_count = 0;
	for (int i = 0; i < argc-1; i++)
	{
		if (strcmp(argv[i+1],"&") == 0)
		{
			amprsnd_count++; //for forking
		}
	}
	
	int col = argc -(2*amprsnd_count)-1; //max number of arguments between "&"s
	char *input[amprsnd_count+1][col]; //2D array to keep track of seperate executions
	int i = 0;
	int temp = 0;
	
	if (strcmp(argv[1],"-p") == 0) //execute in parallel
	{
		for( int k = 0; k < argc-1; k++)
		{
			if (strcmp(argv[k+2],"&")!= 0) //drop the "-p"
			{ input[i][k-temp] = argv[k+2]; }
			else
			{ 
				input[i][k-temp] = NULL; //ends in NULL for execv
				temp = k+1;
				i++;
			}
		}
		if (amprsnd_count > 0) //everything below this line needs work
		{
			pid_t child_pid = fork();
			if (child_pid == 0)
		    {
				execv(input[0][0], input[0]);
				exit(0);
		    }
		    else  //need amprsnd_count condtional forking
		    {
				execv(input[1][0], input[0]);
				exit(0);
			}
		}
		
	}
	else //execute sequentially
	{
		for( int k = 0; k < argc-1; k++)
		{
			if (strcmp(argv[k+2],"&")!= 0)
			{ input[i][k-temp] = argv[k+1]; }
			else
			{ 
				input[i][k-temp] = NULL; //ends in NULL for execv
				temp = k+1;
				i++;
			}
		}
		if (amprsnd_count > 0)
		{
			pid_t child_pid = fork();
			if (child_pid == 0)
		    {
				execv(input[0][0], input[0]);
				exit(0);
		    }
		    else
		    {
				pid_t wait(int *status);	//sequential part
				execv(input[1][0], input[0]);
				exit(0);
			}
		}
	}
	
return 0;
}
