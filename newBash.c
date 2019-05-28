#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char** next(int* index, int argc, char** argv, int parallel, int parcheck);

int main(int argc, char** argv){
  if(argc == 1){
    return 0;
  }
  else{
    //first checks if the parallel flag is checked.
    int parallel = 0;
    int parcheck = 0;
    if(strcmp(argv[1], "-p") == 0){
      parallel = 1; 
    }
    //EC: there may exist parallel execution
    for(int i = 2; i < argc; i++){ 
      if(strcmp(argv[i], "&&") == 0){
		  parcheck = 1;
	  }
	}
    //startval is the value where parameters are first looked for.
    int startVal = 1 + parallel;
    //index points to the index of the first unused element of the argument array.
    int* index = &startVal;

    //this counts the number of processes which are called.
    int processes = 0;

    //this loop fetches arguments and runs processes based on them.
    while(*index < argc){
      char** params = next(index, argc, argv, parallel, parcheck); //edit: updates parallel
      if(fork() == 0){
        execvp(params[0], params);
      }
      else{
        free(params);
        processes++;
        /* If processes are running in series, waits for one to
        *  to finish before starting the next.
        */
        if(parallel == 0){
          wait(NULL);
        }
      }
    }
    /* If the processes run in parallel, this waits for all of the processes
     * to terminate before terminating the main program.
     */
    for(int i = 0; i < processes; i++){
      wait(NULL);
    }
  }
  return 0;
}

/* This returns an array containing all of the arguments for a command,
 * as stored in argv, starting at position *index.
 */
char** next(int* index, int argc, char** argv, int parallel, int parcheck){
  //this fetches the number of arguments for the next command
  int i;
  for(i = *index; i < argc; i++){
      if(strcmp(argv[i], "&") == 0){
		if (parcheck == 1)
		{
			parallel = 1; //EC: update parallel
		}
        break;
      }
      else if(strcmp(argv[i], "&&") == 0){ //EC: run sequentially
		parallel = 0;
		break;
	  }
  }
  //an array of string is then made to accomodate the correct number of arguments.
  int length = i - *index;
  char** params = (char**) malloc(8 * (length + 1));
  params[length] = NULL;
  //arguments are copied into the array from argv one by one.
  for(int j = 0; j < length; j++){
    params[j] = (char*)malloc(strlen(argv[j + (*index)]) * sizeof(char));
    strcpy(params[j], argv[j + (*index)]);
  }
  //the index pointer is then updated to the location of the next legal argument.
  *index = i + 1;

  return params;
}
