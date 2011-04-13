
/*
	header of uC-GUI 
*/

#ifndef _RENOGUI_H_
#define	_RENOGUI_H_


#include "LCD_ConfDefaults.h"  /* valid LCD configuration */
#include "GUI.h"
#if GUI_WINSUPPORT
//  #include "PROGBAR.h"
  #include "LISTBOX.h"
  #include "FRAMEWIN.h"
  #include "BUTTON.h"
  #include "TEXT.h"
#endif


#ifndef NULL
	#define NULL (0)
#endif



#endif	/* _RENOGUI_H_ */