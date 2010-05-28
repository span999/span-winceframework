#ifndef __SPDLLTOUCHHOOKEXT_H__
#define __SPDLLTOUCHHOOKEXT_H__

#include <Tchddi.h>

typedef enum _TOUCHHOOKEXT_ACT_TYPE
{
	TYPE_ALL = 0,
	TYPE_DOWN,
	TYPE_MOVE,
	TYPE_UP,
} TOUCHHOOKEXT_ACT_TYPE;


typedef void (*PFN_TOUCHHOOKEXT_DOWN)( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y );
typedef void (*PFN_TOUCHHOOKEXT_MOVE)( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y );
typedef void (*PFN_TOUCHHOOKEXT_UP)( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y );
typedef void (*PFN_TOUCHHOOKEXT_POINT)( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y );



#endif	///#ifndef __SPDLLTOUCHHOOKEXT_H__