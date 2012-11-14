/*
 * Circular buffer example, keeps one slot open
 * ref : http://en.wikipedia.org/wiki/Circular_buffer
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "spRingBuf.h"
/*#include "toolhelps.h"*/


/*
#define	__DEBUG__
*/

#if 0
/* Opaque buffer element type.  This would be defined by the application. */
typedef struct { int value; } ElemType;

 
/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
#if 0
    ElemType   *elems;  /* vector of elements                   */
#else
	unsigned char *elems;  /* vector of elements                   */
#endif
} CircularBuffer;
#endif

static int g_elemsize = -1;



 
void cbInit(CircularBuffer *cb, int size, int elemsize )
{
	if( elemsize > 0 )
	{
		g_elemsize = elemsize;
		cb->size  = size + 1; /* include empty elem */
		cb->start = 0;
		cb->end   = 0;
#if 0
		cb->elems = (ElemType *)calloc(cb->size, sizeof(ElemType));
#else
		cb->elems = (unsigned char *)calloc(cb->size, g_elemsize);
#endif
	}
}


void cbFree(CircularBuffer *cb)
{
	if( cb )
		free(cb->elems); /* OK if null */
}


int cbIsFull(CircularBuffer *cb)
{
	int iRet = -1;

	if( cb )
	{
		iRet = ((cb->end + 1) % cb->size == cb->start);
	}
	
    return iRet;
}


int cbIsEmpty(CircularBuffer *cb)
{
	int iRet = -1;
	
	if( cb )
	{
		iRet = (cb->end == cb->start);
	}
	
    return iRet;
}

 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
#if 0
void cbWrite(CircularBuffer *cb, ElemType *elem)
#else
void cbWrite(CircularBuffer *cb, unsigned char *elem, int InSize )
#endif
{
	if( InSize <= g_elemsize )
	{
		#if 0
		cb->elems[cb->end] = *elem;
		#else
		memcpy( (cb->elems + (g_elemsize * cb->end)), elem, g_elemsize );
		#endif
		cb->end = (cb->end + 1) % cb->size;
		if (cb->end == cb->start)
			cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
	}
}

 
/* Read oldest element. App must ensure !cbIsEmpty() first. */
#if 0
void cbRead(CircularBuffer *cb, ElemType *elem)
#else
void cbRead(CircularBuffer *cb, unsigned char *elem, int OutSize )
#endif
{
	if( OutSize >= g_elemsize )
	{
		#if 0
		*elem = cb->elems[cb->start];
		#else
		memcpy( elem, (cb->elems + (g_elemsize * cb->start)), g_elemsize );
		#endif
		cb->start = (cb->start + 1) % cb->size;
	}
}


/* Copy oldest element. App must ensure !cbIsEmpty() first. */
void cbCopy(CircularBuffer *cb, unsigned char *elem, int OutSize )
{
	if( OutSize >= g_elemsize )
	{
		memcpy( elem, (cb->elems + (g_elemsize * cb->start)), g_elemsize );
		/*
		cb->start = (cb->start + 1) % cb->size;
		*/
	}
}



#if 0
int main(int argc, char **argv) {
    CircularBuffer cb;
    ElemType elem = {0};
 
    int testBufferSize = 10; /* arbitrary size */
    cbInit(&cb, testBufferSize);
 
    /* Fill buffer with test elements 3 times */
    for (elem.value = 0; elem.value < 3 * testBufferSize; ++ elem.value)
        cbWrite(&cb, &elem);
 
    /* Remove and print all elements */
    while (!cbIsEmpty(&cb)) {
        cbRead(&cb, &elem);
        printf("%d\n", elem.value);
    }
 
    cbFree(&cb);
    return 0;
}
#endif
