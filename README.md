# Processes
- C program which replicates the fork/exec/wait functionality supported when executing multiple processes in the bash command prompt. Program accepts N executables/parameters (N > 0) and executes all of them either in parallel or sequentially based on whether a -p (for parallel) flag is set or by the use of "&&" and "&".
## Example Inputs: <br />
Parallel:<br />

- bash> ./myBashProj -p /bin/cat file1 file2 "&" ./btree.o 10 "&" /bin/echo "Hello!"<br />
Sequential:<br />

- bash> ./myBashProj /bin/cat file1 file2 "&" ./btree.o 10 "&" /bin/echo "Hello!"<br />

Sequential and Parallel:<br />
 Executables around "&&" are executed sequentially and executables separated by "&" run in parallel<br />
- bash> ./myBashProj /bin/cat file1 file2 "&&" ./btree.o 10 "&" /bin/echo "Hello!"

## Contributers:
#### Angela Almquist
#### Nathaniel Gorski
#### Emma Schildgen

