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
    struct HuffmanLeaf *left;
    struct HuffmanLeaf *right;
} huffman_leaf;

int CHUNK_SIZE = 16 * 1024;
char *read_argv_params(int, char *[]);
int parent(int i);
int left_child(int i);
int right_child(int i);
void min_heapify(huffman_leaf a[], int i, int n);
void print_heap(huffman_leaf a[], int n);
void check_min_heap_property(huffman_leaf a[], int n);
void build_huffman_tree(huffman_leaf a[], int n);
huffman_leaf dequeue(huffman_leaf a[], int *n);
void insert(huffman_leaf a[], huffman_leaf node, int *n);
void walk_tree(huffman_leaf tree);

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
    check_min_heap_property(minHeap, number_of_huffman_leaf);
    build_huffman_tree(minHeap, number_of_huffman_leaf);
    huffman_leaf HTree = minHeap[0];
    walk_tree(HTree);
    return 0;
}

void walk_tree(huffman_leaf tree)
{
    if (tree.left == NULL && tree.right == NULL)
    {
        return;
    }
    if (tree.left != NULL)
    {
        return walk_tree(*tree.left);
    }
    if (tree.right != NULL)
    {
        return walk_tree(*tree.right);
    }
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

void check_min_heap_property(huffman_leaf a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        huffman_leaf parent = a[i];
        int left_idx = left_child(i);
        int right_idx = right_child(i);
        if (left_idx < n && a[left_idx].frequency < parent.frequency)
        {
            printf("%d", left_idx);
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Left: %d", parent.frequency, a[left_idx].frequency);
            break;
        }
        if (right_idx < n && a[right_idx].frequency < parent.frequency)
        {
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Right: %d", parent.frequency, a[right_idx].frequency);
            break;
        }
    }
}

void build_huffman_tree(huffman_leaf a[], int n)
{
    int min_heap_boundary = n - 1;
    while (min_heap_boundary > 0)
    {
        huffman_leaf left = dequeue(a, &min_heap_boundary);
        huffman_leaf right = dequeue(a, &min_heap_boundary);
        huffman_leaf new_node = (huffman_leaf){'\0', left.frequency + right.frequency, true, &left, &right};
        insert(a, new_node, &min_heap_boundary);
    }
}

huffman_leaf dequeue(huffman_leaf a[], int *n)
{
    huffman_leaf node = a[0];
    a[0] = a[*n];
    *n = *n - 1;
    min_heapify(a, 0, *n);
    return node;
}

void insert(huffman_leaf a[], huffman_leaf node, int *n)
{
    // this might not work as expected
    *n = *n + 1;
    a[*n] = node;
    int i = *n - 1;
    while (i != 0 && a[parent(i)].frequency > a[i].frequency)
    {
        swap(&a[parent(i)], &a[i]);
        i = parent(i);
    }
}