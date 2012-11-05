/*
 *
 *
 */
#ifndef __SPRINGBUF_H__
#define __SPRINGBUF_H__


#if 0
/* Opaque buffer element type.  This would be defined by the application. */
typedef struct { int value; } ElemType;
#endif
 
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



void cbInit(CircularBuffer *cb, int size, int elemsize );
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, unsigned char *elem, int InSize );
void cbRead(CircularBuffer *cb, unsigned char *elem, int OutSize );



#endif /* #ifndef __SPRINGBUF_H__ */
