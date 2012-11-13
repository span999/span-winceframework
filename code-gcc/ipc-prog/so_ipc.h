/*


*/



#ifndef __SO_IPC_H__
#define __SO_IPC_H__


/* Semaphores */
int getSemaphoresID( void );
int getNamedSemaphoresID( char *Name );
int SemaphoresIDinit( int semid );
int SemaphoresIDdestroy( int semid );
int SemaphoresIDset( int semid, int op );


#define		semLOCK(x)		SemaphoresIDset(x,1)
#define		semUNLOCK(x)	SemaphoresIDset(x,0)


/* Shared memory */
int getNamedSharedMemoryID( char *Name, int iSize );
int SharedMemoryIDinit( int shmid, char **This );
int SharedMemoryIDdeinit( char *This );
int SharedMemoryIDdestroy( int shmid );






#endif /* __SO_IPC_H__ */

