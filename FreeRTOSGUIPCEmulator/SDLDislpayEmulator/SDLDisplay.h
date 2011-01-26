
#ifndef __SDLDISPLAY_H__
#define __SDLDISPLAY_H__

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void  SDLInit(void);
extern void  SDLDestroy(void);
extern bool  SDLProcessEvent(void);
extern void SDLScreenCallbackSet(void (*pfnCallback)(unsigned long ulMessage, long lX, long lY));
extern const tDisplay SDLDisplay;


#endif // __SDLDISPLAY_H__
