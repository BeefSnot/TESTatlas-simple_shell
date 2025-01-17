#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

/* Function prototypes */
int ss_cd(char **cmds);
void ss_exit(char **args);
void ss_help(char **args);
void find_env(char **cmds);
int wrap_ss_exit(char **args);
int wrap_ss_help(char **args);

/**
 * wrap_ss_exit - Wrapper function for ss_exit to match function pointer type.
 * @args: Array of arguments.
 */
int wrap_ss_exit(char **args)
{
    ss_exit(args);
    return 0;
}

/**
 * wrap_ss_help - Wrapper function for ss_help to match function pointer type.
 * @args: Array of arguments.
 */
int wrap_ss_help(char **args)
{
    ss_help(args);
    return 0;
}

/**
 * ss_cd - Changes the current working directory.
 * @cmds: Array of arguments, cmds[0] is "cd", cmds[1] is the directory path.
 */
int ss_cd(char **cmds)
{
    if (cmds[1] == NULL)
    {
        fprintf(stderr, "ss: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(cmds[1]) != 0)
        {
            perror("ss");
        }
    }
    return 1;
}

/**
 * ss_exit - Exits the shell with an optional status code.
 * @args: Array of arguments, args[0] is "exit", args[1] is the exit status.
 */
void ss_exit(char **args)
{
    int status = 0;
    if (args[1] != NULL)
    {
        status = atoi(args[1]);
    }
    exit(status);
}

/**
 * ss_help - Prints help information about the shell.
 * @args: Array of arguments, args[0] is "help".
 */
void ss_help(char **args)
{
    (void)args;
    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    printf("  cd\n");
    printf("  exit\n");
    printf("  help\n");
}

/**
 * find_path - Finds and executes the command passed in the arguments.
 * @cmds: Array of commands.
 */
void find_path(char **cmds)
{
    int cmd_index = 0;
    char *current_command = cmds[cmd_index];

    int (*builtin_functions[])(char **) = {ss_cd, wrap_ss_exit, wrap_ss_help};

    size_t num_builtins = sizeof(builtin_functions) / sizeof(builtin_functions[0]);
    size_t i;

    while (current_command != NULL)
    {
        for (i = 0; i < num_builtins; i++)
        {
            if (builtin_functions[i](cmds) == 0)
            {
                return;
            }
        }
        cmd_index++;
        current_command = cmds[cmd_index];
    }
}

/**
 * find_env - Executes the command passed in the arguments.
 * @cmds: Array of command strings to be executed.
 */
void find_env(char **cmds)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(cmds[0], cmds);
        perror("ss");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("ss");
    }
}
