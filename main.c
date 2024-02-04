#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);

int main(int argc, char *argv[])
{
    FILE *fd;
    char *file_name = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    file_name = read_argv_params(argc, argv);
    printf(" %s", file_name);
}

char *read_argv_params(int argc, char *argv[])
{
    for (char **pArgv = argv + 1; *pArgv != argv[argc]; pArgv++)
    {
        for (char *i = *pArgv; *i != '\0'; i++)
        {
            return i;
        }
    }
    return 0;
}