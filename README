# 42_Heilbronn - pipex Project

*This project has been created as part of the 42 curriculum by ykonka*.

## Description

* The goal of this project is to implement the `pipex` program which replicates the behaviour of the following shell command.
	- < `infile` `cmd1` `cmd2` > `outfile`

* Implementation done in `C` program

* For this, we were authorized to use the following functions:

	- `open`, `close`, `read`, `write`,
	- `malloc`, `free`, `perror`,
	- `strerror`, `access`, `dup`, `dup2`,
	- `execve`, `exit`, `fork`, `pipe`,
	- `unlink`, `wait`, `waitpid`


## Instructions

### Compilation

To get started with this project, follow these steps:
```bash
$ make
```
```
$ make bonus
```

### Execution

```bash
$ ./pipex infile cmd1 cmd2 outfile
```

### Example

Here’s an example of how to use the pipex function in Bash Shell:

```bash
# pipex 2 commands
$ ./pipex infile cat "wc -l" outfile
#  pipex multiple commands
$ ./pipex infile cat cat cat outfile
#  pipex heredoc
$ ./pipex here_doc EOF cat cat outfile
```

## Resources

- `man bash`: To get to know what shell program is?, how redirection, arguments parsing done in bash

- `man pipe`, `man wait`, `man fork`, `man execve` `man dup2`, `man access`, `man exit`: To understand what each of these functions do and how they are helpful in the pipex implmentation.
- ai is used for testcases and refractorization
