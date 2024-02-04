#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 256
int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);

int main(int argc, char *argv[])
{
    FILE *fd;
    u_int64_t hashArray[SIZE];
    char *file_name = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    file_name = read_argv_params(argc, argv);
    fd = fopen(file_name, "r");
    if (fd == NULL)
    {
        printf("Error opening file");
        return 0;
    }
    for (int i = 0; i < SIZE; i++)
    {
        hashArray[i] = 0;
    }
    int ch = EOF;
    while (ch)
    {
        ch = getc(fd);
        if (ch == EOF)
        {
            printf("Ending...");
            break;
        }
        hashArray[ch]++;
    }
    for (int j = 0; j < SIZE; j++)
    {
        printf("Character: %c, Frequency: %llu \n", j, hashArray[j]);
    }
    return 0;
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