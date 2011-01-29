********************************************************************************
*                                     SETUP                                    *
********************************************************************************

Install Microchip Application Libraries  v2010-04-28  Installer.exe with all
default settings, especially the location (C:\Microchip Solutions)

Copy and paste Project folder "Graphics LCD Simulator Demo” into
C:\Micorchip Solutions

Copy GFX_LCD_Sim_Driver.c to C:\Microchip Solutions\Microchip\Graphics\Drivers
Copy GFX_LCD_Sim_Driver.h to C:\Microchip Solutions\Microchip\Include\Graphics

Modify the following Microchip Graphic library files.
Please note that these modifications, if carried out correctly, will not change
the way the graphics library works. The changes simply add a reference to the
new driver

C:\Microchip Solutions\Microchip\Graphics\DisplayDriver.c
add to the list of #elif's
#elif (DISPLAY_CONTROLLER == GFX_LCD_SIM_DRIVER)
	#include "Drivers\GFX_LCD_Sim_Driver.c"
	
C:\Microchip Solutions\Microchip\Include\Graphics\DisplayDriver.h
add to the list of #elif's
#elif (DISPLAY_CONTROLLER == GFX_LCD_SIM_DRIVER)
	#include "Graphics\GFX_LCD_Sim_Driver.h"

C:\Microchip Solutions\Microchip\Include\Graphics\Graphics.h
Under
/////////////GRAPHICS CONTROLLERS CODES FOR DRIVER LAYER ////////////
add to the list of controllers
#define GFX_LCD_SIM_DRIVER		200 //or any other number that isn't already there

Create an empty text file "out.txt” in "Graphics LCD Simulator Demo" project
Folder

Fire up MPLAB, load "Gfx LCD Simulator" project

Ensure "Build Configuration" is set to "Debug"

Ensure Debugger is set to MPLAB SIM

Select Debugger->Stimulus->New Workbook |
                                        +--> Important to do in this order!
Compile project                         |

On the Stimulus Workbook Click advanced button Attach GPR_Inject_Trace.scl Click
OK

Run GFXDisplaySimulator.exe In this program select File->Open and open out.txt
that you created earlier on

Run MPLAB Simulator

For Mouse messages sent to MPLAB click and drag the mouse slowly to see it work
properly

* GFXDisplaySimulator
**********************
This program is incomplete, in that it still has issues so run it at your own risk


Currently there's a bug in MPLAB 8.53 and 8.56 sim for PIC32 that gives incorrect results
for math operations that require division and/or multiplication.  So far I
have not tested all the objects in the gfx library.
The slider won’t work correctly due to this bug as it uses the above math to calculate
the sliders thumb position.
There are 2 work arounds for this if you would like to see the attached demo
project work as intended

1. The bug isn't present in MPLAB 8.40 and 8.30 It may not be present in other
versions. I haven’t tested them all and the problem may not be present in PIC24 or 33
simulator. If you want to simulate for the PIC32 Download MPLAB 8.40 or 8.30 install
and use them instead

2.  Modify Slider.c by placing _nop () between the math statements (Thanks
Yuantuh) in function SldSetPos()
Inlcuded in the download is a slider.c file with _nop()'s inserted, I have tested this
with MPLAB 8.56

Although this Project is set up for the pic32mx360f512l, there's no reason it
shouldn't work for other supported PICs You will have to change the name of the
chip in the scl file and of course write up your own project



********************************************************************************
*                                  HOW IT WORKS                                *
********************************************************************************

* Drawing the screen
********************
The GFX lib generally calls PutPixel to draw stuff on the LCD.
PutPixel receives 2 parameters, X and Y screen coordinates. The current drawing
colour is stored in _color variable.

We want these values sent to GFXDisplaySimulator so we pack x, y and _color into
a QWORD variable DotDat in PutPixel.

The register trace functionality of MPLAB STIMULUS can be triggered by the current
Program Counter (PC) so in PutPixel we call a dummy func TrigRegTrace, whos address
is used to trigger the trace facility using DotDat as the variable we are tracing.

DotDat is written to out.txt followed by a cr/lf combination.

GFXDisplaySimulator monitors out.txt for a change in the End Of File and reads in
any new data. Often the complete line isn't present so GFXDisplaySimulator has to
treat each line as a packet and wait for cr/lf to indicated that complete data packet
for a pixel is present before drawing it on the screen.

* Sending Mouse Messages
************************
Normally in PICs main there is a loop that calls GetTouchMsg. This is replaced with
GetEmulatorTouchMsg which is located in GFX_LCD_SimDriver.c

This is where it gets a bit complicated, due either to my lack of knowledge in
undocumented SCL, MPSIMs lack of functionality or both.

GFXDisplaySimulator creates 3 files for input into MPLAB SIM.
in1.txt, in2.txt and index.txt

MPSIM reads a file using SCL function triggerin_gpr(). Unfortunately it only reads
the file once, storing the values it reads into some internal buffer. Any changes
to that input file once triggerin_gpr() has been called are not seen by MPSIM.

What GFXDisplaySimulator does is write consecutive touch messages alternately to
in1.txt and in2.txt When ever a new touch message needs to be sent to MPSIM,
GFXDisplaySimulator reads index.txt which contains the file name of the txt file the
last message was written two (eg in1.txt), writes the touch message to the opposite 
txt file (in2.txt) then changes the file name in index.txt to the txt file where the
touch message was just written (in2.txt).

MPSIM can inject values from a file into a GPR based on a given trigger. In the SCL
file the Register injection process waits on a signal, in this case we have used RB0.
When it changes state, its value is checked to see if it is high.
Meanwhile in MPLAB when GetEmulatorTouchMsg gets called it sets RB0 to 1.
This causes the SCL file to load index.txt, read the file name in it into a string
variable 's' and call triggerin_gpr() with 's' as the filename parameter.
In effect what this does is force triggerin_gpr() to reload and use the latest touch
message from a different file each time new data is available.

Back in the PIC code, after RB1 has been set, the dummy fucntion that triggers triggerin_gpr
(TrigRegInjection()) is called. This makes MPSIM insert the data from the current in.txt
file into our QWORD varibale RegInjectVal. The values in RegInjectVal are placed into
pMsg which is finally sent to GOLMsg().

Feel free to post any questions and/or requests either on this thread or via PM

Alfonso Baz
