






#ifndef __SDLDEFINE_H__
#define __SDLDEFINE_H__


#include <stdio.h>
#include <stdlib.h>




#ifndef HAVE_BOOLEAN
typedef int boolean;
typedef int bool;
#endif
#ifndef FALSE			/* in case these macros already exist */
#define FALSE	0		/* values of boolean */
#endif
#ifndef TRUE
#define TRUE	1
#endif




#endif	/* __SDLDEFINE_H__ */