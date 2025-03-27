#include <stdio.h>
#include <string.h>

#include "memory_pool.h"

int main()
{
    // Example usage of the memory_pool_t struct
    memory_pool_t pool;
    memory_pool_init(&pool);

    // Create a pool with block size 32 bytes and 10 blocks
    memory_pool_create(&pool, 32u);

    // Allocate a block from the pool
    uint8_t *blocks[MEMORY_POOL_SIZE / 32];
    for (int i = 0; i < MEMORY_POOL_SIZE / 32; i++)
    {
        blocks[i] = (uint8_t *)memory_pool_allocate(&pool);
        memset(blocks[i], i + 1, 32u);
    }

    // Deallocate the block back to the pool
    memory_pool_free(&pool, blocks[0]);
    memory_pool_free(&pool, blocks[0]);
    memory_pool_free(&pool, blocks[1]);

    return 0;
}
