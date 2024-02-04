#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 256
int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);

typedef struct HuffmanLeaf
{
    char letter;
    int frequency;
} huffman_leaf;

int main(int argc, char *argv[])
{
    FILE *fd;
    huffman_leaf hashArray[SIZE];
    char *file_name = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    file_name = read_argv_params(argc, argv);
    fd = fopen(file_name, "r");
    if (fd == NULL)
    {
        printf("Error opening file");
        return 0;
    }
    for (int j = 0; j < SIZE; j++)
    {
        hashArray[j] = (huffman_leaf){(char)j, 0};
    }
    int ch = EOF;
    while (ch)
    {
        ch = getc(fd);
        if (ch == EOF)
        {
            ch = 0;
        }
        hashArray[ch].frequency++;
    }
    for (int j = 0; j < SIZE; j++)
    {
        printf("Character: %c, Frequency: %d \n", hashArray[j].letter, hashArray[j].frequency);
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