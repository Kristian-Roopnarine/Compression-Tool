#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 256

char zero = '0';
char one = '1';

typedef struct HuffmanLeaf
{
    // this is an int because char type does not have support for all ASCII values
    int letter;
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
void min_heapify(huffman_leaf *a[], int i, int n);
void print_heap(huffman_leaf *a[], int n);
void check_min_heap_property(huffman_leaf *a[], int n);
void build_huffman_tree(huffman_leaf *a[], int n);
huffman_leaf *dequeue(huffman_leaf *a[], int *n);
void insert(huffman_leaf *a[], huffman_leaf *node, int *n);
void walk_tree(huffman_leaf *tree, char *[], char[], int);
huffman_leaf *create_huffman_leaf(char j);
void generate_prefix_code_table(huffman_leaf *, char *[]);
huffman_leaf *initialize_array(size_t);
void walker(huffman_leaf *, char *bitStr, int bitSize);

int main(int argc, char *argv[])
{
    FILE *fd;
    char *file_name = (char *)malloc(sizeof(char) * CHUNK_SIZE);
    file_name = read_argv_params(argc, argv);
    fd = fopen(file_name, "r");
    huffman_leaf *ptr = initialize_array(SIZE);
    if (fd == NULL)
    {
        printf("Error opening file");
        return 0;
    }
    int ch = EOF;
    int items_in_heap = 0;
    while (ch)
    {
        ch = getc(fd);
        if (ch == EOF)
        {
            break;
        }
        if (ptr[ch].frequency == 0)
        {
            items_in_heap++;
        }
        ptr[ch].frequency++;
    }
    huffman_leaf *p;
    printf("Items to add to heap: %d\n", items_in_heap);
    for (p = ptr; p < &ptr[SIZE]; p++)
    {
        printf("Character: %c, Character Number: %d, Frequency: %d\n", p->letter, p->letter, p->frequency);
    }
    huffman_leaf *minHeap[items_in_heap];
    int heap_boundary = 0;
    for (p = ptr; p < &ptr[SIZE]; p++)
    {
        if (p->frequency == 0)
        {
            continue;
        }
        minHeap[heap_boundary] = p;
        heap_boundary++;
    }
    for (int i = heap_boundary - 1; i >= 0; i--)
    {
        min_heapify(minHeap, i, heap_boundary - 1);
    }
    print_heap(minHeap, heap_boundary);
    build_huffman_tree(minHeap, heap_boundary);
    huffman_leaf *huffman_tree = minHeap[0];
    char bitStr[] = "";
    walker(huffman_tree, bitStr, 0);
    free(ptr);
    return 0;
}

void walker(huffman_leaf *tree, char bitStr[], int bitSize)
{
    char bitCopy[bitSize + 1];
    strcpy(bitCopy, bitStr);
    if (tree->left == NULL && tree->right == NULL)
    {
        printf("Frequency: %d, Character: %c, Bits: %s\n", tree->frequency, tree->letter, bitCopy);
        return;
    }
    bitSize++;
    if (tree->left)
    {
        strncat(bitCopy, &zero, 1);
        walker(tree->left, bitCopy, bitSize);
    }
    if (tree->right)
    {
        strncat(bitCopy, &one, 1);
        walker(tree->right, bitCopy, bitSize);
    }
}

huffman_leaf *initialize_array(size_t array_size)
{
    huffman_leaf *ptr = (huffman_leaf *)malloc((sizeof(huffman_leaf) * array_size));
    for (int i = 0; i < array_size; i++)
    {
        ptr[i].frequency = 0;
        ptr[i].letter = i;
        ptr[i].is_internal_node = false;
        ptr[i].left = NULL;
        ptr[i].right = NULL;
    }
    return ptr;
}

void generate_prefix_code_table(huffman_leaf *a, char *prefix_code_table[])
{
    // walk tree
    char bitRep[] = "";
    walk_tree(a, prefix_code_table, bitRep, 0);
}

huffman_leaf *create_huffman_leaf(char j)
{
    huffman_leaf *ptr = (huffman_leaf *)malloc(sizeof(huffman_leaf));
    ptr->frequency = 0;
    ptr->is_internal_node = false;
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->letter = j;
    return ptr;
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

void min_heapify(huffman_leaf *a[], int i, int n)
{
    // get the current node
    int min = i;
    int left_idx = left_child(i);
    int right_idx = right_child(i);
    if (left_idx <= n && a[min]->frequency > a[left_idx]->frequency)
    {
        min = left_idx;
    }
    if (right_idx <= n && a[min]->frequency > a[right_idx]->frequency)
    {
        min = right_idx;
    }

    // perform swap if min has changed
    if (min != i)
    {
        swap(a[min], a[i]);
        min_heapify(a, min, n);
    }
}

void print_heap(huffman_leaf *a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Character : %c, Frequency : %d \n", a[i]->letter, a[i]->frequency);
    }
    printf("\n");
}

void check_min_heap_property(huffman_leaf *a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        huffman_leaf *parent = a[i];
        int left_idx = left_child(i);
        int right_idx = right_child(i);
        if (left_idx < n && a[left_idx]->frequency < parent->frequency)
        {
            printf("%d", left_idx);
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Left: %d", parent->frequency, a[left_idx]->frequency);
            break;
        }
        if (right_idx < n && a[right_idx]->frequency < parent->frequency)
        {
            printf("Does not satisfy min heap property.\n");
            printf("Parent : %d, Right: %d", parent->frequency, a[right_idx]->frequency);
            break;
        }
    }
}

void build_huffman_tree(huffman_leaf *a[], int n)
{
    int min_heap_boundary = n - 1;
    while (min_heap_boundary > 0)
    {
        huffman_leaf *left = dequeue(a, &min_heap_boundary);
        huffman_leaf *right = dequeue(a, &min_heap_boundary);
        huffman_leaf *new_node = (huffman_leaf *)malloc(sizeof(huffman_leaf));
        new_node->frequency = left->frequency + right->frequency;
        new_node->is_internal_node = true;
        new_node->letter = '\0';
        new_node->left = left;
        new_node->right = right;
        insert(a, new_node, &min_heap_boundary);
    }
}

huffman_leaf *dequeue(huffman_leaf *a[], int *n)
{
    huffman_leaf *node = a[0];
    a[0] = a[*n];
    *n = *n - 1;
    min_heapify(a, 0, *n);
    return node;
}

void insert(huffman_leaf *a[], huffman_leaf *node, int *n)
{
    // n will be the position of the last element
    *n = *n + 1;
    a[*n] = node;
    int i = *n - 1;
    while (i != 0 && a[parent(i)]->frequency > a[i]->frequency)
    {
        swap(a[parent(i)], a[i]);
        i = parent(i);
    }
}