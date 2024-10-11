# sbash
#### Video Demo:  <URL HERE>
#### Description:
sbash is a simplified implementation of a Unix-like shell, developed as part of the final project for the CS50x course. This project aims to replicate basic shell functionalities, such as command execution, redirection handling, and environment variable management. The code is written in C, with a focus on a modular structure to enhance understanding, maintainability, and ease of debugging. This shell was developed with educational purposes in mind, but it aims to deliver a functional tool, capable of handling essential shell operations.

### Implemented Features:

- **Built-in commands**: `cd`, `echo`, `unset`, `export`, providing basic shell capabilities.
- **Execution of external commands**: Supports running external programs and applications, similar to a standard shell.
- **Input and output redirection**: This feature allows the shell to redirect input and output streams, supporting operations like `>` for file output and `<` for file input.
- **Pipes**: sbash supports piping (`|`) between commands, enabling command chaining where the output of one command becomes the input of the next.
- **Signal handling**: Proper handling of signals such as `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`, ensuring the shell can manage interruptions and EOFs (end of file) appropriately.
- **Environment variable inheritance**: sbash is capable of managing and modifying environment variables, which are passed on to child processes.
- **Hereditary process forking**: The shell handles process creation through forking, essential for running multiple tasks concurrently.

### Files and their Functions:

- **builtins.c** / **builtins_utils.c**: Implements built-in commands like `cd`, `echo`, `export`, ensuring fundamental shell functionality.
- **chdir.c**: Manages the `cd` command, allowing users to change directories.
- **check.c** / **check_utils.c**: Implements error-checking mechanisms, ensuring the smooth execution of commands and providing user feedback in case of incorrect inputs or system failures.
- **echo.c**: Handles the `echo` built-in, printing text or variables to the terminal.
- **exec.c**: Manages the execution of external commands through process forking and exec functions. This is one of the core components of sbash.
- **expand.c** / **expand_utils.c**: Handles environment variable expansion, allowing variables to be recognized and expanded during command execution.
- **export.c**: Provides the `export` functionality, enabling the user to modify or set environment variables.
- **file_utils.c**: Contains utilities related to file manipulation, assisting in redirections and piping.
- **heredoc.c**: Implements the `heredoc` functionality, which allows multiline input until a specified delimiter is reached.
- **init_struct.c**: Initializes the core data structures used throughout the shell, facilitating organization and clarity in managing resources.
- **list_utils.c**: Manages list operations, which are essential for managing arguments, tokens, and other dynamic data.
- **main.c**: Contains the main function that controls the primary loop of the shell, continuously waiting for user input.
- **memory.c**: Implements memory management, including allocation and deallocation, to prevent memory leaks and ensure efficient resource usage.
- **sbash.c** / **sbash.h**: Defines the structure of the shell, containing important functions and prototypes.
- **signal.c**: Implements signal handling, such as detecting `Ctrl+C` (to interrupt processes) and `Ctrl+D` (to send EOF).
- **unset.c**: Implements the `unset` command, which removes environment variables from the shell's context.
- **utils.c**: General utility functions that support other components of the shell.

### Design Decisions:

During the development of sbash, several design decisions were made to prioritize modularity and code readability. Each functionality is divided into its own file or group of files, ensuring that each component is easy to maintain and extend. This structure also helps in isolating potential bugs, as each module can be tested independently.

One of the main challenges faced during the project was implementing the pipe functionality (`|`). This feature requires careful management of processes and file descriptors, as the output of one command must be passed seamlessly as the input of the next. Achieving this required a solid understanding of process forking and inter-process communication (IPC).

Another design consideration was the implementation of signal handling. Signals like `SIGINT` (sent by `Ctrl+C`) must be caught and properly handled to avoid terminating the shell unexpectedly. This feature ensures that the user has control over interrupting long-running processes without crashing the entire shell.

### Challenges Faced:

Developing a shell involves understanding many low-level system concepts, such as process control, memory management, and system calls. One of the most significant challenges was ensuring that all processes are managed correctly, especially when dealing with external commands and piping. This required an in-depth study of how the Unix process model works, particularly how shells traditionally fork processes and manage file descriptors.

Additionally, handling redirection and piping simultaneously introduced complexity, as we needed to carefully manage file descriptor duplication and closure to ensure that input and output streams behave as expected.

### How to Compile and Run:

1. Ensure that all required C dependencies and the GCC compiler are installed on your system.
2. To compile the project, use the provided Makefile:
```bash
make
```
3. After successful compilation, an executable named sbash will be generated. You can run the shell with:
```bash
./sbash
```
4. Once inside the shell, type commands and see them executed just like in a standard Unix terminal. You can test various commands, including both built-in commands and external programs.

By following this modular approach, sbash aims to be a simplified yet educational tool for learning about the inner workings of Unix-like shells.
