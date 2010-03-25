/*
 *  Copyright (c) 2004, Jun Li, lj_sourceforge@users.sourceforge.net.
 *  All rights reserved. 
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 *  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "types.h"
#include "mem.h"
#include "../../../inc/picoos.h"
#include <string.h>

#define ALIGN_SIZE      4

#define MEMBLOCKSIZE    ((sizeof(struct mem_block) & (ALIGN_SIZE - 1)) ? \
                        ((sizeof(struct mem_block) + ALIGN_SIZE) & ~(ALIGN_SIZE - 1)) : \
                        (sizeof(struct mem_block)))



/*  OS_MEM_Create()
        Called at system init time to set up heap for use. MUST be called
        before any calls to calloc1(). Takes a single contiguous memory space
        and sets it up to be used by calloc1() anbd mem_free().
    PARAM1: 
        char *base - address for start of heap area in memory
        long size    - size of heap area at address.
    RETURNS: 
        void
*/

void mempool_create(mempool_t *mem, char * base, u_int size)
{
    /* make sure the heap is aligned */
    if ((u_int)base & (ALIGN_SIZE-1))
    {
        base = (char*)(((u_int)base + (ALIGN_SIZE-1)) & ~(ALIGN_SIZE-1));
        size -= (ALIGN_SIZE-1);
    }

    mem->mheap_base = (struct mem_block *)base;
    mem->mh_free = (struct mem_block *)base;

    /* trim heap to multiple of ALIGN_SIZE */
    size &= ~(ALIGN_SIZE-1);

    /* start with no free space (we will add to this) */
    mem->mh_totfree = 0;

    mem->mh_free->size = (unsigned)size - MEMBLOCKSIZE;
    mem->mh_totfree += mem->mh_free->size;
    mem->mh_free->next = NULL;

    /* set starting and minimum free space from the just-initialized total */
    mem->mh_startfree = mem->mh_minfree = mem->mh_totfree;
}



/*  OS_MEM_Alloc
        Similar to standard malloc()
    PARAM1
        unsigned size
    RETURNS
        pointer to memif OK, else NULL
*/

void *mempool_alloc(mempool_t *mem, unsigned size)
{
    struct mem_block *bp;
    struct mem_block *newb;
    struct mem_block *lastb;
    unsigned    int     lostsize;           /* size of data block plus struct */

    osCriticalDeclare();

    osEnterCritical();

    /* increase requested size enough to ensure future alignment */
    if ((long)size & (ALIGN_SIZE-1))
        size = (size + ALIGN_SIZE) & ~(ALIGN_SIZE-1);

    lostsize = size + MEMBLOCKSIZE;     /* size we will take from heap */

    bp = mem->mh_free;                      /* init vars for list search */
    lastb = NULL;
    while (bp)
    {
        if (bp->size >= size)               /* take first-fit free block */
        {
            /* Decide if the block is big enough to be worth dividing */
            if (bp->size > (size + (MEMBLOCKSIZE * 2)))
            {
                /* Divide block and return front part to caller. First make a new block after the portion we will return    */
                newb = (struct mem_block *)((char*)(bp) + lostsize);
                newb->size = bp->size - lostsize;
                newb->next = bp->next;

                /* modify bp to reflect smaller size we will return */
                bp->next = newb;
                bp->size = size;
            }
            else                    /* not worth fragmenting block, return whole thing */
            {
                /* adjust lostsize */
                lostsize = bp->size + MEMBLOCKSIZE;       
            }
            
            if (lastb)   /* unlink block from queue */
                lastb->next = bp->next;
            else
                mem->mh_free = bp->next;

            /* keep statistics */
            mem->mh_totfree -= lostsize;
            if (mem->mh_totfree < mem->mh_minfree)
                mem->mh_minfree = mem->mh_totfree;
            bp->next = mem->mheap_base;     /* tag next ptr with illegal value */

            osExitCritical();
            return  (void *)((char*)(bp) + MEMBLOCKSIZE);
        }
        lastb = bp;
        bp = bp->next;
    }
    
    osExitCritical();
    mem->mh_failed++;               /* count out of memory conditions */
    
    return NULL;                /* failure return - no memory */
}


/*  OS_MEM_Free
        Find block which contains buf and insert it in free 
        list. Maintains list in order, low to high memory. 
    PARAM1
        char * buf - buffer to add to free list.
    RETURNS
        void
*/
void    mempool_free(mempool_t *mem, void* buf)
{
    struct mem_block    *freep;
    struct mem_block    *tmp;
    struct mem_block    *last;
    osCriticalDeclare();
    int                 merged = 0;             /* indicates freep not merged into free list */

    /* find pointer to prepended mem_block struct */
    freep = (struct mem_block*)((unsigned int)buf - MEMBLOCKSIZE);
    osEnterCritical();
    if (freep->next != mem->mheap_base)      /* sanity check next ptr for tag */
    {
        osExitCritical();
        return;
    }

    mem->mh_totfree += ((unsigned long)freep->size + MEMBLOCKSIZE);

    last = NULL;
    for (tmp = mem->mh_free; tmp; tmp = tmp->next)
    {
        if (freep < tmp)  /* found slot to insert freep */
        {
            /* see if we can merge with next block */
            if (((char*)freep + freep->size + MEMBLOCKSIZE) == (char*)tmp)
            {
                freep->next = tmp->next;
                freep->size += (tmp->size + MEMBLOCKSIZE);
                if (last)
                    last->next = freep;
                else
                    mem->mh_free = freep;
                merged++;
            }

            /* ...and see if we can merge with previous block */
            if (last && (((char*)last + last->size + MEMBLOCKSIZE) == (char*)freep))
            {
                last->size += (freep->size + MEMBLOCKSIZE);
                if (merged) /* if already merged, preserve next ptr */
                {
                    last->next = freep->next;
                }
                merged++;
            }

            /* if didn't merge with either adjacent block, insert into list */
            if (!merged)   
            {
                if (last)
                {
                    freep->next = last->next;
                    last->next = freep;
                }
                else     /* no last, put at head of list */
                {
                    freep->next = mem->mh_free;
                    mem->mh_free = freep;
                }
                mem->mh_totfree -= MEMBLOCKSIZE;   /* we didn't get a header back */
            }

            osExitCritical();
            return;
        }
        
        last = tmp;       /* set "last" pointer for next loop */
    }
    
    /* got to end of list without finding slot for freep; add to end */
    if (last)
    {
        /* See if we can merge it with last block */
        if (((char*)last + last->size + MEMBLOCKSIZE) == (char*)freep)
        {
            last->size += (freep->size + MEMBLOCKSIZE);
        }
        else
        {
            freep->next = last->next;
            last->next = freep;
        }
    }
    else     /* there was no free list */
    {
        mem->mh_free = freep;
        freep->next = NULL;
    }

    osExitCritical();
}

void memblock_create(blockmem_t *block, void *base, int n_block, int blocksize)
{
    block->base = base;
    block->n_block = n_block;
    block->blocksize = blocksize;
    memset(block->used, 0, block->n_block+1);
}

void memblock_reset(blockmem_t *block)
{
    memset(block->used, 0, block->n_block+1);
}

void *memblock_alloc(blockmem_t *block)
{
    int index;
    osCriticalDeclare();
    
    if (!block)
        return NULL;
    osEnterCritical();
    index = strlen(block->used);
    if (index >= block->n_block)
    {
        osExitCritical();
        return NULL;
    }
    block->used[index] = '1';
    osExitCritical();
    return (void *)((u_int)block->base+index*block->blocksize);
}

int memblock_free(blockmem_t *block, void *ptr)
{
    int index;
    if ((u_int)ptr < (u_int)block->base)
        return 0;
    index = ((u_int)ptr - (u_int)block->base)/block->blocksize;
    if (index >= block->n_block)
        return 0;
    block->used[index] = 0;
    return 1;
}
