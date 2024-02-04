#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 256

typedef struct HuffmanLeaf
{
    char letter;
    int frequency;
    bool is_internal_node;
    struct huffman_leaf *left;
    struct huffman_leaf *right;
} huffman_leaf;

int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);
int parent(int i);
int left_child(int i);
int right_child(int i);
void min_heapify(huffman_leaf a[], int i, int n);
void print_heap(huffman_leaf a[], int n);

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
        hashArray[j] = (huffman_leaf){(char)j, 0, false, NULL, NULL};
    }
    int ch = EOF;
    int items_in_heap = 0;
    while (ch)
    {
        ch = getc(fd);
        if (ch == EOF)
        {
            ch = 0;
        }
        if (hashArray[ch].frequency == 0)
        {
            items_in_heap++;
        }
        hashArray[ch].frequency++;
    }
    printf("%d\n", items_in_heap);
    huffman_leaf minHeap[items_in_heap];
    int heap_boundary = 0;
    int number_of_huffman_leaf = sizeof(minHeap) / sizeof(huffman_leaf);
    int i = 0;
    while (heap_boundary < items_in_heap)
    {
        if (hashArray[i].frequency != 0)
        {
            huffman_leaf item = hashArray[i];
            minHeap[heap_boundary] = item;
            heap_boundary++;
        }
        i++;
    }
    for (int i = items_in_heap - 1; i >= 0; i--)
    {
        min_heapify(minHeap, i, items_in_heap - 1);
    }
    for (int i = 0; i < items_in_heap; i++)
    {
        huffman_leaf parent = minHeap[i];
        int left_idx = left_child(i);
        int right_idx = right_child(i);
        if (left_idx < number_of_huffman_leaf && minHeap[left_idx].frequency < parent.frequency)
        {
            printf("%d", left_idx);
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Left: %d", parent.frequency, minHeap[left_idx].frequency);
            return 1;
        }
        if (right_idx < number_of_huffman_leaf && minHeap[right_idx].frequency < parent.frequency)
        {
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Right: %d", parent.frequency, minHeap[right_idx].frequency);
            return 1;
        }
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

int parent(int i)
{
    return (i - 1) / 2;
}

int left_child(int i)
{
    return 2 * i + 1;
}

int right_child(int i)
{
    return 2 * i + 2;
}
void swap(huffman_leaf *x, huffman_leaf *y)
{
    huffman_leaf tmp = *x;
    *x = *y;
    *y = tmp;
}

void min_heapify(huffman_leaf a[], int i, int n)
{
    // get the current node
    int min = i;
    int left_idx = left_child(i);
    int right_idx = right_child(i);
    if (left_idx <= n && a[min].frequency > a[left_idx].frequency)
    {
        min = left_idx;
    }
    if (right_idx <= n && a[min].frequency > a[right_idx].frequency)
    {
        min = right_idx;
    }

    // perform swap if min has changed
    if (min != i)
    {
        swap(&a[min], &a[i]);
        min_heapify(a, min, n);
    }
}

void print_heap(huffman_leaf a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d, ", a[i].frequency);
    }
    printf("\n");
}