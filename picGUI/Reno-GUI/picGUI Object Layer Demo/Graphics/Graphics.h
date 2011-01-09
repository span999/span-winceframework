/*********************************************************************
 * Module for Microchip Graphics Library
 * The header file joins all header files used in the graphics library
 * and contains compile options and defaults.
 *********************************************************************
 * FileName:        Graphics.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30
 * Compiler:        C30 V3.00
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright  2007 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     11/12/07	Version 1.0 release
 ********************************************************************/

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

//////////////////// COMPILE OPTIONS AND DEFAULTS ////////////////////

////// COMPILE OPTIONS FOR APPLICATION //////
#include "GraphicsConfig.h"

//////// GRAPHICS_LIBRARY_VERSION ///////////
// MSB is version, LSB is subversion
#define GRAPHICS_LIBRARY_VERSION  0x0100

///////// GOL OBJECTS EMBOSS SIZE ///////////

// Emboss size set for 3-D effect
#define GOL_EMBOSS_SIZE                 3		

////////////////////////////// INCLUDES //////////////////////////////
#include <stdlib.h>
#include "GenericTypeDefs.h"

//#include "DisplayP.h"
#include "DisplayL.h"

//#include "S6d0129P.h"  // Portrait driver for S6D0129 controller
//#include "S6d0129L.h"  // Landscape driver for S6D0129 controller

//#include "R61505UP.h"  // Portrait driver for R61505U controller 
//#include "R61505UL.h"  // Landscape driver for R61505U controller 

//#include "LGDP4531L.h"  // Landscape driver for LGDP4531 controller 

//#include "HIT1270L.h"  // Landscape driver for Densitron HIT1270 controller

//#include "SSD1339.h"  // Driver for Solomon Systech. SSD1339 controller 

#include "Primitive.h"  // Graphic primitives

#include "ScanCodes.h"  // Scan codes for AT keyboard
#include "GOL.h"        // GOL layer 
#include "Button.h"
#include "Window.h"
#include "GroupBox.h"
#include "StaticText.h"
#include "Slider.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Picture.h"
#include "ProgressBar.h"
#include "EditBox.h"
#include "ListBox.h"
#include "RoundDial.h"
#include "Meter.h"
#ifdef USE_CUSTOM
// Included for custom control demo
#include "CustomControlDemo.h"
#endif

/*********************************************************************
* Macros: RGB565CONVERT(red, green, blue)
*
* Overview: Converts true color into 5:6:5 RGB format.
*
* PreCondition: none
*
* Input: Red, Green, Blue components.
*
* Output: 5 bits red, 6 bits green, 5 bits blue color.
*
* Side Effects: none
*
********************************************************************/
#define RGB565CONVERT(red, green, blue) 	(WORD)( (( red   >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) | ( blue  >> 3 ))

#endif
