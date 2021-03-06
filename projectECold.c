#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv){
  if(argc == 1){
	printf("Wrong number of args, expected 2 or more, given %d\n", argc - 1);
	exit(1);
    //return 0;
  }
  else{
    //first checks for parallel flag or combination of parallel & sequential exec
    int parallel = 0;
    int parcheck = 0;
    if(strcmp(argv[1], "-p") == 0){
      parallel = 1;
    }
    else {
		for(int i = 2; i < argc; i++){ 
			if(strcmp(argv[i], "&&") == 0){
			parcheck = 1;
			}
		}
	}

    //Break if there are no processes to be run
    if(argc == parallel + 1){
      return 0;
    }

    if(parallel){

      //index is the value where parameters are first looked for.
      int index = 2;
      int processes = 0;

      //this loop fetches arguments and runs processes based on them.
      while(index < argc){

        //this fetches the number of arguments for the next command
        int i;
        for(i = index; i < argc; i++){
            if(strcmp(argv[i], "&") == 0){
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
        processes++;
      }

      /* If the processes run in parallel, this waits for all of the processes
       * to terminate before terminating the main program.
       */
      for(int i = 0; i < processes; i++){
        int status = wait(NULL);
        if(status == -1){
          printf("ERROR: there are no child processes left to wait on.\n");
          exit(1);
        }
      }
    }
    else if(parcheck){

      //index is the value where parameters are first looked for.
      int index = 1;
      int processes = 0;
	  int seqnum = 0;
	  int parnum = 0;
      //this loop fetches arguments and runs processes based on them.
      while(index < argc){
		//EC portion: checks for and updates for sequential and parallel execution
		int sequential = 0;
        //this fetches the number of arguments for the next command
        //loop also updates for how many sequential arguments occur
        int i;
        for(i = index; i < argc; i++){
            if(strcmp(argv[i], "&") == 0 || strcmp(argv[i], "&&") == 0){
			  if (strcmp(argv[i], "&&") == 0) {
				  sequential = 1;
				  seqnum += 1;
			  }
			  else parnum += 1;
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
        else if (sequential){
	      for(int i = 0; i < parnum; i++){
	        int status = wait(NULL);
	        if(status == -1){
	          printf("ERROR: there are no child processes left to wait on.\n");
	          exit(1);
	        }
          }
          //seqnum = parnum; //Doesn't help avoid ERROR in line 147
	    }
        processes++;
      }
      /* If the processes run in parallel, this waits for all of the processes
       * to terminate before terminating the main program.
       */
      for(int i = seqnum; i < processes; i++){
        int status = wait(NULL);
        if(status == -1){
         // printf("ERROR: there are no child processes left to wait on.\n");
         // Bug in EC portion: Depending on placement of "&" and "&&", wait() may be called one time too many
          exit(1);
        }
      }
    }
    else{
      //index is the value where parameters are first looked for.
      int index = 1;

      //this loop fetches arguments and runs processes based on them.
      while(index < argc){

        //this fetches the number of arguments for the next command
        int i;
        for(i = index; i < argc; i++){
            if(strcmp(argv[i], "&") == 0){
              break;
            }
        }
        //an array of string is then made to accomodate the correct number of arguments.
        int length = i - index;
        char* params[length + 1];
        params[length] = NULL;
        //arguments are copied into the array from argv one by one.
        for(int j = 0; j < length; j++){
          params[j] = argv[j+index];
        }
        //the index pointer is then updated to the location of the next legal argument.
        index = i + 1;

        pid_t status = fork();
        if(status == 0){
          execvp(params[0], params);
          printf("ERROR: had trouble executing command '%s'\n", params[0]);
          exit(1);
        }
        else if(status < 0){
          printf("ERROR: could not fork the main process");
          exit(1);
        }
        else{
          int i = 0;
          while(params[i] != NULL){
            i++;
          }
          int waitstatus = wait(NULL);
          if(waitstatus == -1){
            printf("ERROR: there are no child processes left to wait on");
            exit(1);
          }
        }
      }
    }
  }
  return 0;
}
