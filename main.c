#include <stdio.h>
#include "redirect.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return 1;
    }

    char *inp = argv[1];
    char *cmd = argv[2];
    char *out = argv[3];
    return redirect_execute(inp, cmd, out);
}
