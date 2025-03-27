/**
 * @file         memory_pool.c
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory_pool.h"

// =============================================================================
// Private Functions.
// =============================================================================

static uint8_t *memory_pool_addr_from_index(const memory_pool_t *pool, uint16_t idx);
static uint16_t memory_pool_index_from_addr(const memory_pool_t *pool, const uint8_t *ptr);
static bool in_range(const memory_pool_t *pool, const uint8_t *ptr);

// =============================================================================
// Public Functions.
// =============================================================================

/**
 * Initialize memory pool
 *
 * @param[in] pool  Memory pool instance
 */
void memory_pool_init(memory_pool_t *pool)
{
    assert(NULL != pool);

    memset(pool, 0u, sizeof(memory_pool_t));
}

/**
 * Create memory pool
 *
 * @param[in] pool  Memory pool instance
 */
void memory_pool_create(memory_pool_t *pool, uint32_t size_of_each_block)
{
    assert(NULL != pool);
    assert(size_of_each_block > 0u);

    pool->m_num_of_blocks = (uint16_t)(MEMORY_POOL_SIZE / size_of_each_block);
    pool->m_size_of_each_block = size_of_each_block;
    pool->m_num_free_blocks = pool->m_num_of_blocks;
    memset(pool->m_mem_pool, 0u, MEMORY_POOL_SIZE);
    pool->m_next = pool->m_mem_pool;
}

/**
 * Allocate a buffer from the pool
 *
 * NULL is returned if no buffer is available.
 *
 * @param[in] pool  Memory pool instance
 * @return         Pointer to the allocated buffer
 */
void *memory_pool_allocate(memory_pool_t *pool)
{
    assert(NULL != pool);

    void *ret = NULL;
    if (pool->m_num_initialized < pool->m_num_of_blocks)
    {
        uint8_t *p = (void *)memory_pool_addr_from_index(pool, pool->m_num_initialized);
        *p = pool->m_num_initialized + 1;
        pool->m_num_initialized++;
    }

    if (pool->m_num_free_blocks > 0)
    {
        ret = (void *)pool->m_next;
        --pool->m_num_free_blocks;
        if (pool->m_num_free_blocks != 0)
        {
            pool->m_next = memory_pool_addr_from_index(pool, *((uint8_t *)pool->m_next));
        }
        else
        {
            pool->m_next = NULL;
        }
    }

    return ret;
}

/**
 * Free a buffer back to the pool
 *
 * @param[in] pool    Memory pool instance
 * @param[in] ptr     Pointer to the buffer to free
 * @return            error code (0:=error, 0:=success)
 */
int memory_pool_free(memory_pool_t *pool, void *ptr)
{
    assert(NULL != pool);

    if (ptr == NULL)
    {
        return -1;
    }

    if (!in_range(pool, ptr))
    {
        return -2;
    }

    if (pool->m_next != NULL)
    {
        (*(uint8_t *)ptr) = memory_pool_index_from_addr(pool, pool->m_next);
        pool->m_next = (void *)ptr;
    }
    else
    {
        *((uint8_t *)ptr) = pool->m_num_of_blocks;
        pool->m_next = (void *)ptr;
    }

    if (pool->m_num_free_blocks >= pool->m_num_of_blocks)
    {
        return -3;
    }

    ++pool->m_num_free_blocks;
    return 0;
}

// =============================================================================
// Private Functions.
// =============================================================================

/**
 * Get address from index
 *
 * @param[in] pool  Memory pool instance
 * @param[in] idx   Index of the buffer
 * @return          Pointer to the buffer
 */
static uint8_t *memory_pool_addr_from_index(const memory_pool_t *pool, uint16_t idx)
{
    assert(NULL != pool);

    return (uint8_t *)(pool->m_mem_pool + (idx * pool->m_size_of_each_block));
}

/**
 * Get index from address
 *
 * @param[in] pool  Memory pool instance
 * @param[in] ptr   Pointer to the buffer
 * @return          Index of the buffer
 */
static uint16_t memory_pool_index_from_addr(const memory_pool_t *pool, const uint8_t *ptr)
{
    assert(NULL != pool);
    assert(NULL != ptr);

    return (uint16_t)(ptr - pool->m_mem_pool) / pool->m_size_of_each_block;
}

/**
 * Proof if ptr is in pool
 *
 * @param[in] pool  Memory pool instance
 * @param[in] ptr   Pointer to the buffer
 * @return          Is in buffer
 */
static bool in_range(const memory_pool_t *pool, const uint8_t *ptr)
{
    assert(NULL != pool);
    assert(NULL != ptr);

    if ((&pool->m_mem_pool[0] <= ptr) && (ptr <= &pool->m_mem_pool[MEMORY_POOL_SIZE]))
    {
        return true;
    }
    return false;
}