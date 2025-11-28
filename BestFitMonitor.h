/*
NANDISH JHA NAJ474 11282001
*/

#ifndef BESTFITMONITOR_H
#define BESTFITMONITOR_H

#include "Monitor.h"
#include <list.h>

// Memory configuration
#define TOTAL_MEMORY_SIZE 1024  // Total memory size in bytes
#define MAX_THREADS 10          // Maximum number of threads

// Condition variable constants
#define MEM_AVAIL 0             // Condition variable for memory availability

// Memory block structure for free memory list
typedef struct MemoryBlock {
    int start_address;
    int size;
} MemoryBlock;

// Monitor procedures
void *BF_Allocate(int size);
void BF_Free(void *address);

// Initialize the memory management system
void InitializeMemorySystem(void);

#endif
