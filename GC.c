#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct MemBlock {
    void *ptr;
    size_t size;
    bool in_use;
    char name[50];
    struct MemBlock *next;
} MemBlock;

MemBlock *head = NULL;

const size_t TOTAL_RAM = 48ULL * 1024 * 1024 * 1024;  // Assume 48 GB of RAM

void* gc_malloc(size_t size, const char *name);
void* gc_calloc(size_t num, size_t size, const char *name);
void* gc_realloc(void *ptr, size_t size, const char *name);
void gc_free(void *ptr);
void gc_collect();

int main() {
    int *arr = (int *)gc_malloc(10 * sizeof(int), "arr");
    if (!arr) return 1;

    int *arr2 = (int *)gc_malloc(10 * sizeof(int), "arr2");
    if (!arr2) return 1;

    int *arr3 = (int *)gc_malloc(10 * sizeof(int), "arr3");
    if (!arr3) return 1;

    int *arr4 = (int *)gc_malloc(10 * sizeof(int), "arr5");
    if (!arr4) return 1;

    int *arr5 = (int *)gc_malloc(10 * sizeof(int), "arr5");
    if (!arr5) return 1;

    int *arr6 = (int *)gc_malloc(10 * sizeof(int), "arr6");
    if (!arr6) return 1;

    int *arr7 = (int *)gc_malloc(10 * sizeof(int), "arr7");
    if (!arr7) return 1;

    int *arr8 = (int *)gc_malloc(10 * sizeof(int), "arr8");
    if (!arr8) return 1;

    int *arr9 = (int *)gc_malloc(10 * sizeof(int), "arr9");
    if (!arr9) return 1;

    int *arr10 = (int *)gc_malloc(10 * sizeof(int), "arr10");
    if (!arr10) return 1;

    int *arr11 = (int *)gc_malloc(10 * sizeof(int), "arr11");
    if (!arr11) return 1;

    int *arr12 = (int *)gc_malloc(10 * sizeof(int), "arr12");
    if (!arr12) return 1;

    gc_free(arr);
    gc_free(arr2);
    gc_free(arr3);
    gc_free(arr4);
    gc_free(arr5);
    gc_free(arr6);
    gc_free(arr7);
    gc_free(arr8);
    gc_free(arr9);
    gc_free(arr10);
    gc_free(arr11);
    gc_free(arr12);

    // Run garbage collection and print details
    gc_collect();

    return 0;
}

void* gc_malloc(size_t size, const char *name) {
    void *ptr = malloc(size);
    if (!ptr) return NULL;

    MemBlock *new_block = (MemBlock *)malloc(sizeof(MemBlock));
    new_block->ptr = ptr;
    new_block->size = size;
    new_block->in_use = true;
    strncpy(new_block->name, name, sizeof(new_block->name) - 1);
    new_block->name[sizeof(new_block->name) - 1] = '\0'; // Ensure null-termination
    new_block->next = head;
    head = new_block;

    return ptr;
}

void* gc_calloc(size_t num, size_t size, const char *name) {
    void *ptr = calloc(num, size);
    if (!ptr) return NULL;

    MemBlock *new_block = (MemBlock *)malloc(sizeof(MemBlock));
    new_block->ptr = ptr;
    new_block->size = num * size;
    new_block->in_use = true;
    strncpy(new_block->name, name, sizeof(new_block->name) - 1);
    new_block->name[sizeof(new_block->name) - 1] = '\0';
    new_block->next = head;
    head = new_block;

    return ptr;
}

void* gc_realloc(void *ptr, size_t size, const char *name) {
    if (!ptr) return gc_malloc(size, name);

    MemBlock *block = head;
    while (block) {
        if (block->ptr == ptr) {
            break;
        }
        block = block->next;
    }

    if (!block) {
        return gc_malloc(size, name);
    }

    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) return NULL;

    block->ptr = new_ptr;
    block->size = size;
    block->in_use = true;
    strncpy(block->name, name, sizeof(block->name) - 1);
    block->name[sizeof(block->name) - 1] = '\0';

    return new_ptr;
}

void gc_free(void *ptr) {
    if (!ptr) return;

    MemBlock *block = head;
    while (block) {
        if (block->ptr == ptr) {
            block->in_use = false; // Mark as not in use
            return;
        }
        block = block->next;
    }
}

void gc_collect() {
    MemBlock *block = head;
    MemBlock *prev = NULL;

    printf("\nFreed Memory Blocks:\n");
    printf("-------------------------------------------------------------------\n");
    printf("| Variable Name |     Address      | Size (bytes)| %% of Total RAM | \n");
    printf("-------------------------------------------------------------------\n");

    while (block) {
        if (!block->in_use) {
            size_t block_size = block->size;
            double percent_ram = (double)block_size / TOTAL_RAM * 100.0;

            printf("| %-13s | %p | %11zu | %13.8f%% |\n", block->name, block->ptr, block_size, percent_ram);

            // Remove the block from the list
            if (prev) {
                prev->next = block->next;
            } else {
                head = block->next;
            }

            free(block->ptr);  // Free the allocated memory
            MemBlock *to_free = block;
            block = prev ? prev->next : head;
            free(to_free);     // Free the metadata structure
        } else {
            prev = block;
            block = block->next;
        }
    }

    printf("-------------------------------------------------------------------\n");
}
