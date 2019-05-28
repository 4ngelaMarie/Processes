#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv){
  if(argc == 1){
	printf("Wrong number of args, expected 2 or more, given %d\n", argc - 1);
	exit(1);
  }
  else{
    //index is the value where parameters are first looked for.
    int index = 1;
    /* EC PORTION: Process counts the number of active processes which are
     * running in parallel. Parnum determines if a sequential process has 
     * been entered into command prompt; this occurs when parnum = processes. 
     */
    int processes = 0;
    int parnum = 1;
    //this loop fetches arguments and runs processes based on them.
    while(index < argc){
      /*this fetches the number of arguments for the next command
       * and also determines if the next process should be run in parallel with
       * those before it or not.
       */
      int i;
      for(i = index; i < argc; i++){
        if(strcmp(argv[i], "&") == 0 || strcmp(argv[i], "&&") == 0){
  			  if (strcmp(argv[i], "&") == 0) {
				parnum += 1;
  			  }
  			  break;
        }
      }
      //an array of string is then made to accomodate the correct number of arguments.
      int length = i - index;
      char* params[length + 1];
      params[length] = NULL;
      //arguments are copied into the array from argv one by one.
      for(int j = 0; j < length; j++){
        params[j] = argv[j + index];
      }
      //the index pointer is then updated to the location of the next legal argument.
      index = i + 1;

      pid_t response = fork();
      if(response == 0){
        execvp(params[0], params);
        printf("ERROR: had trouble executing command '%s'\n", params[0]);
        exit(1);
      }
      else if(response < 0){
        printf("ERROR: could not fork the main process");
        exit(1);
      }
      else{
            processes++;
      }
      if (processes == parnum){
        for(int i = 0; i < parnum; i++){
          int status = wait(NULL);
          if(status == -1){
            printf("ERROR: there are no child processes left to wait on.\n");
            exit(1);
          }
        }
        //reset values for next loop iteration
        parnum = 1;
        processes = 0;
      }
    }
    /* If any remainging processes are running in parallel, the following waits for all
     * processes to terminate before terminating the main program.
     */
    for(int i = 0; i < processes; i++){
      int status = wait(NULL);
      if(status == -1){
        printf("ERROR: there are no child processes left to wait on.\n");
        exit(1);
      }
    }
  }
  return 0;
}
