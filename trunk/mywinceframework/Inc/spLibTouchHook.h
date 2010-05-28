#ifndef __SPLIBTOUCHHOOK_H__
#define __SPLIBTOUCHHOOK_H__


/*
with this header, you can get information(structured) of touch screen activty.
It's easy, set the callback function after init function.
you will get the touch information(structured) in callback function you just set.
After all, be sure to run deinit function after done.
*/


typedef enum _TOUCH_EVENT_TYPE
{
	TOUCH_NONE_EVENT = 0,
	TOUCH_DOWN_EVENT,
	TOUCH_MOVE_EVENT,
	TOUCH_UP_EVENT,
} TOUCH_EVENT_TYPE;

typedef struct TOUCH_EVENT_STRUC{
	TOUCH_EVENT_TYPE	teType;
	INT					tiX;
	INT					tiY;
}TOUCH_EVENT_DATA, *PTOUCH_EVENT_DATA;



typedef BOOL (*PFN_TOUCH_HOOK_LIB_CALLBACK)(TOUCH_EVENT_DATA ThisTouchData );

DWORD spLibTouchHook_Init( DWORD dwParam );
DWORD spLibTouchHook_Deinit( DWORD dwParam );
BOOL spLibTouchHook_SetCallback( PFN_TOUCH_HOOK_LIB_CALLBACK pfnTouchHookLibCB );



#endif	///#ifndef __SPLIBTOUCHHOOK_H__