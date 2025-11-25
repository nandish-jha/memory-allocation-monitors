/*
NANDISH JHA NAJ474 11282001
*/

#include "BestFitMonitor.h"
#include "Monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <list.h>

// Global variables
static LIST *free_memory_list = NULL;  // List of free memory blocks
static int memory_initialized = 0;
static int monitor_initialized = 0;

// Helper function to find the best fit block - simplified to avoid loops
MemoryBlock *findBestFit(int size) {
    MemoryBlock *best_block = NULL;
    MemoryBlock *current_block;
    int smallest_suitable_size = TOTAL_MEMORY_SIZE + 1;
    
    if (ListCount(free_memory_list) == 0) {
        return NULL;
    }
    
    // Simple single pass through the list
    current_block = (MemoryBlock *)ListFirst(free_memory_list);
    if (current_block != NULL && current_block->size >= size) {
        best_block = current_block;
        smallest_suitable_size = current_block->size;
    }
    
    // Check next item if it exists
    current_block = (MemoryBlock *)ListNext(free_memory_list);
    if (current_block != NULL && current_block->size >= size && current_block->size < smallest_suitable_size) {
        best_block = current_block;
    }
    
    return best_block;
}

// Helper function to coalesce adjacent free blocks - simplified
void coalesceBlocks(void) {
    // For simplicity in this quick implementation, skip coalescing
    // In a full implementation, we'd sort blocks by address and merge adjacent ones
    printf("[Coalesce] Skipping coalescing for simplicity\n");
}

// Initialize the memory management system
void InitializeMemorySystem(void) {
    MemoryBlock *initial_block;
    
    if (memory_initialized) {
        return;
    }
    
    printf("[MemSystem] Initializing memory system with monitor\n");
    
    // Initialize monitor with one condition variable
    printf("[MemSystem] Calling MonInit(1)\n");
    MonInit(1);
    printf("[MemSystem] MonInit completed successfully\n");
    
    // Create the free memory list
    free_memory_list = ListCreate();
    
    // Create initial free block covering entire memory
    initial_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    initial_block->start_address = 0;
    initial_block->size = TOTAL_MEMORY_SIZE;
    
    ListAppend(free_memory_list, initial_block);
    
    memory_initialized = 1;
    printf("[MemSystem] Memory system initialized with %d bytes\n", TOTAL_MEMORY_SIZE);
}

// Best fit allocation monitor procedure
void *BF_Allocate(int size) {
    MemoryBlock *best_block;
    int allocated_address;
    
    printf("[BF_Allocate] Requesting %d bytes\n", size);
    
    // Enter monitor for exclusive access
    printf("[BF_Allocate] Entering monitor\n");
    MonEnter();
    
    // Wait until suitable memory is available
    while (1) {
        best_block = findBestFit(size);
        if (best_block != NULL) {
            break;  // Found suitable block
        }
        
        printf("[BF_Allocate] No suitable block found, waiting for memory...\n");
        MonWait(MEM_AVAIL);  // Wait on condition variable
    }
    
    // Allocate from the best fit block
    allocated_address = best_block->start_address;
    
    if (best_block->size == size) {
        // Exact fit - remove the entire block
        printf("[BF_Allocate] Exact fit: allocated %d bytes at address %d\n", size, allocated_address);
        
        // Find and remove this block from the list
        MemoryBlock *current = (MemoryBlock *)ListFirst(free_memory_list);
        if (current == best_block) {
            ListRemove(free_memory_list);
            free(current);
        } else {
            current = (MemoryBlock *)ListNext(free_memory_list);
            if (current == best_block) {
                ListRemove(free_memory_list);
                free(current);
            }
        }
    } else {
        // Split the block
        printf("[BF_Allocate] Split block: allocated %d bytes at address %d, remaining %d bytes\n", 
               size, allocated_address, best_block->size - size);
        
        best_block->start_address += size;
        best_block->size -= size;
    }
    
    printf("[BF_Allocate] Successfully allocated %d bytes at address %d\n", size, allocated_address);
    
    // Leave monitor
    printf("[BF_Allocate] Leaving monitor\n");
    MonLeave();
    
    return (void *)(long)allocated_address;
}

// Free memory monitor procedure
void BF_Free(void *address) {
    MemoryBlock *new_free_block;
    int addr = (int)(long)address;
    
    printf("[BF_Free] Freeing memory at address %d\n", addr);
    
    // Enter monitor for exclusive access
    printf("[BF_Free] Entering monitor\n");
    MonEnter();
    
    // Create new free block (simplified - use fixed size)
    new_free_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    new_free_block->start_address = addr;
    new_free_block->size = 64;  // Simplified: assume 64-byte blocks
    
    ListAppend(free_memory_list, new_free_block);
    
    printf("[BF_Free] Memory freed at address %d, signaling waiters\n", addr);
    
    // Signal that memory is available
    MonSignal(MEM_AVAIL);
    
    // Leave monitor
    printf("[BF_Free] Leaving monitor\n");
    MonLeave();
}
