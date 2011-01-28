



///#include <stdio.h>
///#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>

///include this for grlib
///#include "grlib/grlib_set.h"

/// include this for PIC graphic lib
///#include "PICgraphic_set.h"

///#include "SDLDislpayEmulator/SDLDisplay.h"

#ifndef _WINGDI_
	#define _WINGDI_
#endif	

///#include "GenericTypeDefs.h"
///#include "Graphics\Primitive.h"
#include "Graphics\Graphics.h"
///#include "Graphics\GOL.h"

void vApplicationIdleHook()
{
	Sleep(INFINITE);	// to reduce processor usage

}

WORD GOLDrawCallback()
{
	return 0;
}

WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	return 0;
}