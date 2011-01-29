#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H


#define GetSystemClock()    (80000000ul)
#define GetPeripheralClock()    (GetSystemClock() / (1 << OSCCONbits.PBDIV))
#define GetInstructionClock()   (GetSystemClock())


#define DISPLAY_CONTROLLER GFX_LCD_SIM_DRIVER
//#define DISPLAY_PANEL PH480272T_005_I06Q
#define COLOR_DEPTH 16

#ifdef _GRAPHICS_H

#define DISP_ORIENTATION		00
#define DISP_HOR_RESOLUTION		480
#define DISP_VER_RESOLUTION		272
#define DISP_DATA_WIDTH			24
#define DISP_HOR_PULSE_WIDTH    41
#define DISP_HOR_BACK_PORCH     2
#define DISP_HOR_FRONT_PORCH    2
#define DISP_VER_PULSE_WIDTH    10
#define DISP_VER_BACK_PORCH     2
#define DISP_VER_FRONT_PORCH    2
#endif

#endif // __HARDWARE_PROFILE_H
