#ifndef REDIRECT_H
#define REDIRECT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


void split_command(char *cmd, char **argv);


int redirect_execute(const char *inp, const char *cmd, const char *out);

#endif
