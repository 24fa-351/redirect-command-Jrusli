#include "redirect.h"

void split_command(char *cmd, char **args) {
    int ix = 0;
    char *token = strtok(cmd, " ");
    while (token != NULL) {
        args[ix++] = token;
        token = strtok(NULL, " ");
    }
    args[ix] = NULL;  // Null-terminate the array for execvp
}

int redirect_execute(const char *input_file, const char *cmd, const char *output_file) {
    int input_fd = -1, output_fd = -1;

    char *cmd_copy = strdup(cmd);
    char *cmd_args[256];
    split_command(cmd_copy, cmd_args);

    if (strcmp(input_file, "-") != 0) {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd < 0) {
            free(cmd_copy);
            return 1;
        }
    }

    if (strcmp(output_file, "-") != 0) {
        output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            if (input_fd != -1) close(input_fd);
            free(cmd_copy);
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        free(cmd_copy);
        return 1;
    } else if (pid == 0) {  
        if (input_fd != -1) {
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                exit(1);
            }
            close(input_fd);
        }

        if (output_fd != -1) {
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                exit(1);
            }
            close(output_fd);
        }

        execvp(cmd_args[0], cmd_args);
        exit(1);  // If execvp fails, child exits with failure status
    }

    int status;
    waitpid(pid, &status, 0);

    if (input_fd != -1) close(input_fd);
    if (output_fd != -1) close(output_fd);
    free(cmd_copy);

    return WEXITSTATUS(status);
}
