/**
 * @file         memory_pool.h
 *
 * @version      1.0.0
 * @date         24.03.2025
 *
 * @brief        Memory pool for async I/0 buffers.
 * @note	     "Fast Efficient Fixed-Size Memory Pool No Loops and No Overhead"
 * by Ben Kenwright, School of Coputer Science https://arxiv.org/pdf/2210.16471
 *
 * @author       Christoph Jans
 */

#ifndef memory_pool_H
#define memory_pool_H

// =============================================================================
// External Modules.
// =============================================================================

#include <stdint.h>

// =============================================================================
// Macros
// =============================================================================

#define MEMORY_POOL_SIZE (1024u)

// =============================================================================
// Definitions.
// =============================================================================

typedef struct
{
    uint16_t m_num_of_blocks;             // Num of blocks
    uint16_t m_num_free_blocks;           // Num of remaining blocks
    uint16_t m_num_initialized;           // Num of initialized blocks
    uint8_t *m_next;                      // Pointer to next free block
    uint8_t m_mem_pool[MEMORY_POOL_SIZE]; // Pool memory
    uint32_t m_size_of_each_block;        // Size of each block
} memory_pool_t;

// =============================================================================
// Public Prototypes.
// =============================================================================

void memory_pool_init(memory_pool_t *pool);
void memory_pool_create(memory_pool_t *pool, uint32_t size_of_each_block);
void *memory_pool_allocate(memory_pool_t *pool);
int memory_pool_free(memory_pool_t *pool, void *buffer);

#endif // memory_pool_H

/*** end of file ***/