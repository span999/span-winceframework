/*
 *
 *
 */
#ifndef __SPSEMSHM_H__
#define __SPSEMSHM_H__





int getSemaphoresID( void );

int getNamedSemaphoresID( char *Name );

int getNamedSemaphoresIDs( char *Name );

int SemaphoresIDinit( int semid );

int SemaphoresIDinitwait( int semid );

int SemaphoresIDdestroy( int semid );

int SemaphoresIDset( int semid, int op );

int getNamedSharedMemoryID( char *Name, int iSize );

int getNamedSharedMemoryIDs( char *Name, int iSize );

int SharedMemoryIDinit( int shmid, char **This );

int SharedMemoryIDdeinit( char *This );

int SharedMemoryIDdestroy( int shmid );




#endif /* #ifndef __SPSEMSHM_H__ */
