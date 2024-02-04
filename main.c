#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);

int main(int argc, char *argv[])
{
    FILE *fd;
    size_t index = 0;
    size_t size = 0;
    char *buffer = (char *)malloc(sizeof(char) * CHUNK_SIZE), *tmp = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    char *file_name = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    file_name = read_argv_params(argc, argv);
    fd = fopen(file_name, "r");
    if (fd == NULL)
    {
        printf("Error opening file");
        return 0;
    }
    int ch = EOF;
    while (ch)
    {
        ch = getc(fd);
        if (ch == EOF)
        {
            ch = 0;
        }
        if (size <= index)
        {
            size += CHUNK_SIZE;
            tmp = realloc(buffer, size);
            if (!tmp)
            {
                free(buffer);
                buffer = NULL;
                break;
            }
            buffer = tmp;
        }
        buffer[index++] = ch;
    }
    free(tmp);
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