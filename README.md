# Filesystem-Minishell
Minimal filesystem, similar to Linux and defining commands such as "touch", "mkdir", "tree", "pwd" and so on. First-year of my bachelor degree studies, data structures course's first assignment. 

# Additional files
A README file, written in Romanian, which compresses implementation details, a Makefile file that includes three rules build, clean, and run. The build rule creates the executabe, run starts the application, and clean, which deletes the executable, and one header file, "file.h", that includes the structures and the functions headers that I used in my main program.

# How to use the Makefile file/ start the minishell

Given the current Makefile file, you will run the following commands:

```sh
$ make build
$ make run
$ make clean
```
Use the "make clean" rule only after you delete the filesystem you created and exit from the app.
