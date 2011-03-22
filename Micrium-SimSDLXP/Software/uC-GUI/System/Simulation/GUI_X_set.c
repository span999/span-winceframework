/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : SIM_X_set.c
Purpose     : Windows Simulator externals
              The init routine in this file can be used to set up the
              simulator
---------------------------END-OF-HEADER------------------------------
*/


#if defined(USE_SDL_LCD)
#include "GUI_X_uCOS.c"
#else
/// you will need add lib include path to to the the "Common Properties" -> "Linker" -> "Input" -> "Aditional Dependencies". 
/// ..\..\..\..\..\uC-GUI\System\Simulation\GUISim.lib
/// ..\..\..\..\..\uC-GUI\System\Simulation\Simulation.res
#include "SIM_X.c"
#endif