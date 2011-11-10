/*




*/
#ifndef __LIBSIO_COMMON_H__
#define __LIBSIO_COMMON_H__


extern int libsdumpcupinfo( void );
extern int libsdumpmeminfo( void );
extern int libsi2csearch( void );
extern int libsi2cdump( void );
extern int libsi2clist( void );
extern int libsi2cget( int iBus, int iAddr, int iIdx );


#endif /* __LIBSIO_COMMON_H__ */
