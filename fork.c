#include "shell.h"

/**
 * fork_exec - Forks the process to execute a command.
 * @command: The command to execute.
 * @full_path: Full path to the command.
 *
 * Returns: Nothing.
 */
void fork_exec(char **command, char *full_path)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (execve(full_path, command, NULL) == -1)
        {
            perror("Execve failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}