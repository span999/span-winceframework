; Copyright (c) 2002-2009,  Microchip Technology Inc.
;
; Microchip licenses this software to you solely for use with Microchip
; products.  The software is owned by Microchip and its licensors, and
; is protected under applicable copyright laws.  All rights reserved.
;
; SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
; WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
; NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
; FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
; MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
; CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
; EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
; OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
; TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
; OR OTHER SIMILAR COSTS.
;
; To the fullest extent allowed by law, Microchip and its licensors
; liability shall not exceed the amount of fees, if any, that you
; have paid directly to Microchip to use this software.
;
; MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
; OF THESE TERMS.

;#define TBLWT_BUG               ; Enable this to work around timing bug found in some PIC18Fxx20s
;#define INVERT_UART             ; If you don't have an RS232 transceiver, you might want this option
#define USE_AUTOBAUD            ; when using UART, you probably want AUTOBAUD
;#define FIXED_SPBRG 0x2B        ; otherwise, you have to specify a fixed SPBRG
;#define FIXED_SPBRG .49         ; 57600bps at 45.1584MHz
;#define FIXED_SPBRG .12          ; 2.5Mbps at 40MHz
#define USE_MAX_INTOSC          ; Sets OSCCON<IRCF2:IRCF0> for maximum INTOSC frequency (8MHz)
#define USE_PLL                 ; Sets OSCTUNE.PLLEN bit at start up for frequency multiplication.
;#define PICDEM_LCD2             ; RB0 = 1 required to enable MAX3221 TX output on PICDEM LCD 2 demo board
#define USE_SOFTBOOTWP          ; enable software boot block write protection
;#define USE_SOFTCONFIGWP        ; enable software config words write protection

; Bootloader must start at the beginning of a FLASH Erase Block. If unspecified,
; bootloader will automatically be located at the end of program memory address space.
;#define BOOTLOADER_ADDRESS   0          ; bootloader at beginning, application start/ISR vectors require remapping
#define BOOTLOADER_ADDRESS   0          ; bootloader at beginning, application start/ISR vectors require remapping
;#define BOOTLOADER_ADDRESS   END_FLASH - (ERASE_FLASH_BLOCKSIZE * 20)  ; useful for running under debugger (debug executive wants to reside at the end of memory space too)
;#define BOOTLOADER_ADDRESS  (END_FLASH - ERASE_FLASH_BLOCKSIZE) ; use on J parts to locate inside flash config erase block

#ifdef BOOTLOADER_ADDRESS
  #if BOOTLOADER_ADDRESS == 0
    ; For Bootloader located at program memory address 0, the application firmware must 
    ; provide remapped reset and interrupt vectors outside of the Boot Block. The following 
    ; #defines tell the bootloader firmware where application entry points are to be expected:
    #define AppVector           0x400   ; application start up code should be located here.
    #define AppHighIntVector    0x408   ; application high priority interrupt should be located here
    #define AppLowIntVector     0x418   ; application low priority interrupt should be located here
  #endif
#endif

; Because we need to know the total size of the bootloader before the assembler has finished
; compiling the source code, you must estimate the final bootloader size and write it down 
; here ahead of time. This number is in bytes (twice the instruction word count).
#define BOOTLOADERSIZE  .641

#define MAJOR_VERSION   .1              ; Bootloader Firmware Version
#define MINOR_VERSION   .3
