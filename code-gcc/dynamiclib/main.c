/*
*
*
*
*
*/


#include <stdio.h>


int main ()
{
	int iRet = 0;

	printf("test app start... !!!\r\n");

	staticlib_welcome();
#ifndef ARM_BUILD
	sharedlib_welcome();
#endif
	libsdumpcupinfo();
	libsdumpmeminfo();

	return iRet;
}



