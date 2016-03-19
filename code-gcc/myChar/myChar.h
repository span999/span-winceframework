/*
 * 
 * 
 */

#ifndef __MYCHAR_H__
#define __MYCHAR_H__

#include <linux/ioctl.h>

#if 1
struct mychar_arg_t {
   int vaild;
   int status;
   char name[16];
   unsigned int data;
};

	#if 0
	#define 	MYCHAR_GET_VALUE 	_IOR('mychar', 901, struct mychar_arg_t *)
	#define 	MYCHAR_CLR_VALUE 	_IO('mychar', 902)
	#define 	MYCHAR_SET_VALUE 	_IOW('mychar', 903, struct mychar_arg_t *)
	#endif

#else
typedef struct {
   int vaild;
   int status;
   char name[16];
   unsigned int data;
} mychar_arg_t;

	#if 0
	#define 	MYCHAR_GET_VALUE 	_IOR('mychar', 901, mychar_arg_t *)
	#define 	MYCHAR_CLR_VALUE 	_IO('mychar', 902)
	#define 	MYCHAR_SET_VALUE 	_IOW('mychar', 903, mychar_arg_t *)
	#endif
#endif


#ifndef MYCHAR_GET_VALUE
	#define 	MYCHAR_GET_VALUE 	901
#endif
#ifndef MYCHAR_CLR_VALUE
	#define 	MYCHAR_CLR_VALUE 	902
#endif
#ifndef MYCHAR_SET_VALUE
	#define 	MYCHAR_SET_VALUE 	903
#endif



#endif /* __MYCHAR_H__ */


