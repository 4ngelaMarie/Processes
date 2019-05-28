# Processes
- C program which replicates the fork/exec/wait functionality supported when executing multiple processes in the bash command prompt. Program accepts N executables/parameters (N > 0) and executes all of them either in parallel or sequentially based on whether a -p (for parallel) flag is set or by the use of "&&" and "&".
- Example Inputs:
# Parallel:

- bash> ./myBashProj -p /bin/cat file1 file2 "&" ./btree.o 10 "&" /bin/echo "Hello!"
# Sequential:

- bash> ./myBashProj /bin/cat file1 file2 "&" ./btree.o 10 "&" /bin/echo "Hello!"

# Sequential and Parallel
- executables around "&&" are executed sequentially and executables separated by "&" run in parallel
- bash> ./myBashProj /bin/cat file1 file2 "&&" ./btree.o 10 "&" /bin/echo "Hello!"
