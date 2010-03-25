#ifndef _SYS_MEM_H
#define _SYS_MEM_H

#include "types.h"

struct mem_block
{
    struct mem_block    *next;      /* pointer to next block */
    u_int                   size;           /* size of block (excluding this header) */
};

typedef struct
{
    struct mem_block *mh_free;
    struct mem_block *mheap_base;

    u_int     mh_startfree;         /* heap size (in bytes) at init time */
    u_int     mh_totfree;               /* current free heap size */
    u_int     mh_minfree;           /* minimum free heap size seen */
    u_int     mh_failed;                /* number of times alloc failed */
} mempool_t;


void mempool_create(mempool_t *mem, char * base, u_int size);
void *mempool_alloc(mempool_t *mem, unsigned  size);
void    mempool_free(mempool_t *mem, void* buf);

typedef struct
{
    void    *base;
    int     n_block;
    int     blocksize;
    char        used[4];
} blockmem_t;

void    memblock_create(blockmem_t *block, void *base, int n_block, int blocksize);
void    *memblock_alloc(blockmem_t *block);
int         memblock_free(blockmem_t *block, void *ptr);
void    memblock_reset(blockmem_t *block);

#endif
