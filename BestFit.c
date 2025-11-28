/*
NANDISH JHA NAJ474 11282001
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "BestFitMonitor.h"

// Simulation parameters
#define NUM_THREADS 4
#define MAX_SLEEP_TIME 2
#define MAX_ALLOCATION 128
#define FREE_PROBABILITY 30  // 30% chance to free a block
#define NUMBER_OF_ITERATIONS 5

// Simple memory allocation simulation using monitors
int main() {
    int i, j, allocation_size, sleep_time;
    void *allocated_blocks[100];  // Store allocated addresses
    int num_allocated = 0;
    int free_decision;
    
    printf("=== Best Fit Memory Allocation Simulator with Monitor ===\n");
    printf("Configuration:\n");
    printf("- Max sleep time: %d seconds\n", MAX_SLEEP_TIME);
    printf("- Max allocation: %d bytes\n", MAX_ALLOCATION);
    printf("- Free probability: %d%%\n", FREE_PROBABILITY);
    printf("- Iterations: %d\n", NUMBER_OF_ITERATIONS);
    printf("- Total memory: %d bytes\n", TOTAL_MEMORY_SIZE);
    printf("=============================================\n\n");
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize memory system with monitor
    InitializeMemorySystem();
    
    // Simulation loop
    for (i = 0; i < NUMBER_OF_ITERATIONS; i++) {
        printf("\n--- Iteration %d ---\n", i + 1);
        
        // Generate random allocation size (16 to MAX_ALLOCATION bytes, aligned to 16)
        allocation_size = ((rand() % (MAX_ALLOCATION / 16)) + 1) * 16;
        
        // Allocate memory using monitor procedure
        printf("Attempting to allocate %d bytes\n", allocation_size);
        void *allocated_address = BF_Allocate(allocation_size);
        
        if (allocated_address != NULL) {
            printf("Successfully allocated %d bytes at address %p\n", 
                   allocation_size, allocated_address);
            
            // Store allocated address
            if (num_allocated < 100) {
                allocated_blocks[num_allocated++] = allocated_address;
            }
        } else {
            printf("Failed to allocate %d bytes\n", allocation_size);
        }
        
        // Random sleep
        sleep_time = (rand() % MAX_SLEEP_TIME) + 1;
        printf("Sleeping for %d seconds\n", sleep_time);
        sleep(sleep_time);
        
        // Randomly decide whether to free a block
        if (num_allocated > 0) {
            free_decision = rand() % 100;
            if (free_decision < FREE_PROBABILITY) {
                // Free a random block
                int block_index = rand() % num_allocated;
                void *addr_to_free = allocated_blocks[block_index];
                
                printf("Freeing memory at address %p\n", addr_to_free);
                BF_Free(addr_to_free);
                
                // Remove from allocated blocks array
                for (j = block_index; j < num_allocated - 1; j++) {
                    allocated_blocks[j] = allocated_blocks[j + 1];
                }
                num_allocated--;
            }
        }
    }
    
    // Free any remaining allocated blocks
    printf("\n--- Cleanup ---\n");
    printf("Cleaning up %d remaining allocated blocks\n", num_allocated);
    for (i = 0; i < num_allocated; i++) {
        printf("Final cleanup: freeing address %p\n", allocated_blocks[i]);
        BF_Free(allocated_blocks[i]);
    }
    
    printf("\n=== Simulation Complete ===\n");
    printf("Memory allocation simulation with monitor completed successfully.\n");
    
    return 0;
}
