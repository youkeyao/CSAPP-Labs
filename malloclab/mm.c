/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Team",
    /* First member's full name */
    "youkeyao",
    /* First member's email address */
    "youkeyao@sjtu.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4             /*word size*/
#define DSIZE 8             /*Double word size*/
#define CHUNKSIZE (1<<10)   /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (unsigned int)(val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void insert_list(void *bp);
static void delete_list(void *bp);
static int get_index(size_t size);
static void mylog();

static char *heap_listp;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(24*WSIZE)) == (void *)-1) return -1;
    heap_listp += (20*WSIZE);

    PUT(heap_listp, 0); /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 1)); /* Epilogue header */

    for (int i = 1; i <= 20; i ++) {
        PUT(heap_listp - i*WSIZE, 0);
    }

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;

    return 0;
}

void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0)); /* Free block header */
    PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    if (size == 0) return NULL;
    if (size <= DSIZE) {
        size = 2 * DSIZE;
    }
    else {
        size = ALIGN(size + DSIZE);
    }
    int index = get_index(size);
    int truesize;
    char *ptr = NULL;

    while (index < 20) {
        int flag = 0;
        char *p = heap_listp - index*WSIZE;
        while (GET(p)) {
            p = (char*)GET(p);
            if (GET_SIZE(HDRP(p)) >= size) {
                flag = 1;
                ptr = p;
                break;
            }
        }
        if (flag) {
            break;
        }
        index ++;
    }

    if (!ptr) {
        ptr = extend_heap(MAX(size, CHUNKSIZE) / WSIZE);
        if (!ptr) return NULL;
    }

    delete_list(ptr);
    truesize = GET_SIZE(HDRP(ptr));
    if (truesize - size < 2 * DSIZE) {
        PUT(HDRP(ptr), PACK(truesize, 1));
        PUT(FTRP(ptr), PACK(truesize, 1));
    }
    else {
        PUT(HDRP(ptr), PACK(size, 1));
        PUT(FTRP(ptr), PACK(size, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(truesize - size, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(truesize - size, 0));
        insert_list(NEXT_BLKP(ptr));
    }

    return ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr = NULL;
    int oldsize = GET_SIZE(HDRP(ptr));
    int newsize;

    if (ptr == NULL) {
        newptr = mm_malloc(size);
    }
    else if (size == 0) {
        mm_free(ptr);
    }
    else {
        if (size <= DSIZE) {
            newsize = 2 * DSIZE;
        }
        else {
            newsize = ALIGN(size + DSIZE);
        }

        newptr = ptr;
        if (oldsize < newsize) {
            size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
            size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
            size_t prev_size = GET_SIZE(FTRP(PREV_BLKP(ptr)));
            size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
            if (!next_alloc && next_size + oldsize >= newsize) {
                delete_list(NEXT_BLKP(ptr));

                oldsize += next_size;
                PUT(HDRP(ptr), PACK(oldsize, 1));
                PUT(FTRP(ptr), PACK(oldsize, 1));
            }
            else if (!prev_alloc && prev_size + oldsize >= newsize) {
                delete_list(PREV_BLKP(ptr));

                oldsize += prev_size;
                PUT(FTRP(ptr), PACK(oldsize, 1));
                PUT(HDRP(PREV_BLKP(ptr)), PACK(oldsize, 1));
                newptr = PREV_BLKP(ptr);

                memmove(newptr, oldptr, size);
            }
            else if (!prev_alloc && !next_alloc && prev_size + next_size + oldsize >= newsize) {
                delete_list(NEXT_BLKP(ptr));
                delete_list(PREV_BLKP(ptr));

                oldsize += next_size + prev_alloc;
                PUT(HDRP(PREV_BLKP(ptr)), PACK(oldsize, 1));
                PUT(FTRP(NEXT_BLKP(ptr)), PACK(oldsize, 1));
                newptr = PREV_BLKP(ptr);

                memmove(newptr, oldptr, size);
            }
            else {
                newptr = mm_malloc(size);
                oldsize = GET_SIZE(HDRP(newptr));
                if (newptr == NULL)
                    return NULL;
                memmove(newptr, oldptr, size);
                mm_free(oldptr);
            }
        }
        if (oldsize - newsize >= 2 * DSIZE) {
            PUT(HDRP(newptr), PACK(newsize, 1));
            PUT(FTRP(newptr), PACK(newsize, 1));
            PUT(HDRP(NEXT_BLKP(newptr)), PACK(oldsize - newsize, 0));
            PUT(FTRP(NEXT_BLKP(newptr)), PACK(oldsize - newsize, 0));
            insert_list(NEXT_BLKP(newptr));
        }
    }

    return newptr;
}

void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && !next_alloc) {
        delete_list(NEXT_BLKP(bp));

        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc) {
        delete_list(PREV_BLKP(bp));

        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else if (!prev_alloc && !next_alloc) {
        delete_list(NEXT_BLKP(bp));
        delete_list(PREV_BLKP(bp));

        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    insert_list(bp);

    return bp;
}

void insert_list(void *bp)
{
    void *ptr = heap_listp;
    size_t size = GET_SIZE(HDRP(bp));

    ptr -= get_index(size) * WSIZE;

    if (GET(ptr)) ptr = GET(ptr);

    while (GET(ptr) && GET_SIZE(HDRP(ptr)) < GET_SIZE(HDRP(bp))) ptr = GET(ptr);

    PUT(bp, GET(ptr)); //bp->next = ptr->next
    PUT(bp + WSIZE, ptr); //bp->prev = ptr
    if (GET(ptr)) // if (ptr->next)
        PUT(GET(ptr) + WSIZE, bp); //ptr->next->prev = bp;
    PUT(ptr, bp); //ptr->next = bp
}

void delete_list(void *bp)
{
    if (GET(bp)) // if (bp->next)
        PUT(GET(bp) + WSIZE, GET(bp + WSIZE)); //bp->next->prev = bp->prev
    PUT(GET(bp + WSIZE), GET(bp)); //bp->prev->next = bp->next
}

int get_index(size_t size)
{
    if (size <= (1 << 4)) {
        return 1;
    }
    else if (size <= (1 << 5)) {
        return 2;
    }
    else if (size <= (1 << 6)) {
        return 3;
    }
    else if (size <= (1 << 7)) {
        return 4;
    }
    else if (size <= (1 << 8)) {
        return 5;
    }
    else if (size <= (1 << 9)) {
        return 6;
    }
    else if (size <= (1 << 10)) {
        return 7;
    }
    else if (size <= (1 << 11)) {
        return 8;
    }
    else if (size <= (1 << 12)) {
        return 9;
    }
    else if (size <= (1 << 13)) {
        return 10;
    }
    else if (size <= (1 << 14)) {
        return 11;
    }
    else if (size <= (1 << 15)) {
        return 12;
    }
    else if (size <= (1 << 16)) {
        return 13;
    }
    else if (size <= (1 << 17)) {
        return 14;
    }
    else if (size <= (1 << 18)) {
        return 15;
    }
    else if (size <= (1 << 19)) {
        return 16;
    }
    else if (size <= (1 << 20)) {
        return 17;
    }
    else if (size <= (1 << 21)) {
        return 18;
    }
    else if (size <= (1 << 22)) {
        return 19;
    }
    else {
        return 20;
    }
}

void mylog()
{
    for (int i = 1; i <= 12; i ++) {
        char *ptr = heap_listp - i * WSIZE;
        int flag = 0;
        if (GET(ptr)) printf("%d, ", i);
        while (GET(ptr)) {
            flag = 1;
            ptr = (char*)GET(ptr);
            printf("(%x, %d) ", (unsigned int)ptr, GET_SIZE(HDRP(ptr)));
        }
        if (flag) printf("\n");
    }
}








