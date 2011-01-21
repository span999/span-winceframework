//*****************************************************************************
//
// fontcm12i.c - Font definition for the 12 point Cm italic font.
//
// Copyright (c) 2008-2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 4423 of the Stellaris Graphics Library.
//
//*****************************************************************************

//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include "grlib/grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Style: cm
//     Size: 12 point
//     Bold: no
//     Italic: yes
//     Memory usage: 1596 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 12 point Cm italic font.
//
//*****************************************************************************
static const unsigned char g_pucCm12iData[1396] =
{
      5,   5,   0,   8,  16,  10,   4,  97,  49,  33,  49,  49,
     33, 177, 240,  10,   5, 161,  33,  17,  33,  49, 240, 240,
    192,  21,   9, 240, 161,  81,  17,  81,  17,  56,  49,  17,
     72,  33,  33,  81,  17,  81,  33,  81, 240,  32,  15,   9,
    240, 131,  81,  18,  65, 115, 113, 129,  83,  98,  19, 240,
    240,  32,  11,  34,  81,  49,  17,  49,  49,  36,  65,  33,
     17,  81,  20,  83,  20,  97,  17,  17,  81,  17,  33,  65,
     33,  33,  49,  49,  17,  49,  67, 240, 160,  25,  10, 211,
     18,  49,  33,  17,  50,  33,  17,  52,  18,  53,  17,  33,
     65,  17,  33,  97,  34,  65,  69, 240, 240,  64,   6,   2,
     65,  17, 240,  64,  15,   5, 129,  49,  49,  65,  65,  49,
     65,  65,  65,  65,  65,  81,  48,  15,   5,  33,  81,  65,
     65,  65,  65,  65,  49,  65,  65,  49,  49, 144,  10,   6,
    129,  17,  50,  50,  50,   0,   6,  64,  12,   8, 240, 240,
     81, 113,  71,  65,  97, 240, 240, 112,   6,   2, 240,  49,
     17,  80,   6,   4, 240, 211, 240,  96,   5,   2, 240,  49,
    112,  16,   8,  97,  97, 113,  97, 113,  97, 113,  97, 113,
     97, 113,  97, 113, 112,  19,   7, 240,  34,  50,  33,  33,
     49,  18,  49,  17,  50,  17,  49,  33,  34,  51, 240, 144,
     12,   5, 240,  34,  65,  49,  65,  65,  65,  36, 240,  16,
     15,   6, 227,  34,  17,  33,  18,  35,  65,  50,  83,  33,
     33, 240,  80,  17,   7, 240,  19,  50,  33,  33,  17,  17,
     36,  97,  97,  33,  34,  36, 240, 144,  15,   5, 240,  49,
     65,  49,  49,  17,  17,  33,  19,  66,  49,  65, 112,  16,
      7, 240,  17,  33,  51,  66,  65,  33,  97,  97,  33,  33,
     51, 240, 160,  18,   7, 240,  35,  49,  33,  36,  34,  33,
     33,  49,  33,  49,  33,  33,  66, 240, 160,  15,   7, 243,
     17,  33,  18,  33,  49,  81,  97,  81,  97,  97, 240, 160,
     18,   7, 240,  19,  49,  49,  33,  49,  51,  49,  18,  33,
     49,  33,  34,  50, 240, 160,  16,   7, 240,  35,  49,  33,
     33,  49,  33,  34,  52,  81,  81,  51, 240, 160,   6,   3,
    240,  17, 161, 176,   7,   3, 240,  17, 161,  33, 128,  11,
      4, 240,  49,  97,  49,  49,  49,  33,  49, 112,   9,   9,
      0,   5, 103, 167,   0,   5, 112,  14,   5, 240, 129,  50,
     50,  50,  34,  33,  33,  17,  33,  19, 112,  16,   6, 146,
     34,  17,  17,  49,  33,  17,  65,  65,  17,  50, 161, 240,
    112,  22,   9, 240,  84,  68,  18,  19,  17,  33,  18,  49,
     17,  18,  33,  17,  33,  21,  34, 132, 240, 240,  16,  19,
      8, 209,  98,  81,  17,  65,  33,  65,  33,  68,  49,  49,
     33,  65,  34,  35, 240, 160,  18,   9, 240,  85,  81,  49,
     49,  49,  68,  81,  49,  49,  65,  49,  50,  38, 240, 240,
     17,   9, 240,  99,  17,  49,  49,  49, 113, 129, 129, 130,
     33,  83, 240, 240,  32,  20,   9, 240,  85,  65,  65,  49,
     65,  49,  65,  33,  81,  33,  65,  49,  50,  37, 240, 240,
     16,  15,   9, 240,  86,  49, 129, 131,  81, 129,  65,  49,
     49,  54, 240, 240,  14,   8, 240,  53,  49, 113, 115,  65,
     33,  65, 113,  99, 240, 224,  18,   9, 240,  99,  17,  49,
     49,  49, 113, 129,  51,  33,  65,  50,  49,  69, 240, 240,
     21,  10, 240, 115,  19,  65,  49,  65,  49,  85,  81,  49,
     65,  65,  65,  49,  67,  19, 240, 240,  48,  12,   6, 227,
     65,  65,  81,  81,  65,  81,  67, 240,  96,  14,   7, 240,
     35,  81,  97,  81,  97,  97,  49,  17,  67, 240, 160,  21,
     10, 240, 115,  34,  49,  49,  81,  33,  97,  17, 115,  97,
     49,  81,  49,  67,  34, 240, 240,  48,  14,   7, 240,  19,
     65,  97,  97,  81,  97,  97,  49,  21, 240, 128,  29,  11,
    240, 146,  66,  65,  65,  66,  50,  49,  17,  33,  17,  49,
     17,  17,  17,  65,  17,  17,  17,  49,  49,  33,  50,  67,
    240, 240,  80,  24,  10, 240, 114,  35,  65,  49,  67,  17,
     65,  33,  17,  65,  33,  17,  65,  49,  65,  65,  66,  49,
    240, 240,  64,  19,   8, 240,  67,  50,  49,  33,  65,  17,
     81,  17,  81,  17,  65,  33,  49,  67, 240, 208,  16,   9,
    240,  85,  81,  49,  49,  65,  53,  65, 129, 113, 115, 240,
    240,  48,  20,   8, 240,  67,  65,  49,  33,  65,  17,  81,
     17,  81,  17,  35,  50,  18,  67, 114,  97, 176,  19,   8,
    240,  52,  65,  49,  49,  49,  52,  49,  49,  49,  49,  49,
     49,  35,  34, 240, 160,  16,   7, 240,  19,  49,  49,  33,
     49,  34, 114,  97,  33,  34,  51, 240, 144,  15,   8, 240,
     38,  17,  33,  33,  65, 113,  97, 113, 113,  84, 240, 208,
     21,   9, 240,  51,  35,  33,  49,  65,  49,  65,  49,  49,
     65,  49,  49,  65,  49,  83, 240, 240,  32,  19,   9, 240,
     51,  50,  33,  49,  65,  49,  65,  33,  97,  17,  98, 113,
    129, 240, 240,  48,  28,  12, 240, 147,  19,  34,  33,  49,
     49,  49,  49,  33,  65,  34,  17,  81,  17,  17,  17,  82,
     34,  98,  34,  97,  49, 240, 240, 192,  20,  10, 240, 114,
     35,  65,  33,  97,  17, 129, 130, 113,  33,  81,  49,  66,
     35, 240, 240,  48,  17,   9, 240,  51,  50,  33,  49,  81,
     17,  98, 113, 129, 129, 115, 240, 240,  32,  17,   8, 240,
     53,  49,  33,  49,  33,  97,  97,  97,  49,  33,  49,  53,
    240, 192,  15,   6, 161,  65,  81,  65,  81,  81,  81,  65,
     81,  81,  65,  97,  64,   8,   4, 195,  17,  17, 240, 240,
     48,  15,   5, 129,  65,  65,  49,  65,  65,  65,  49,  65,
     65,  65,  49,  64,   8,   4, 145,  33,  17, 240, 240, 112,
      5,   2,  65, 240,  96,   6,   2,  97,  17, 240,  32,  14,
      6, 240, 240,  20,  18,  17,  33,  33,  33,  33,  37, 240,
     64,  15,   5, 177,  65,  65,  67,  18,  17,  17,  33,  17,
     18,  19, 240,  32,  12,   5, 240, 179,  18,  17,  17,  65,
     33,  34, 240,  32,  16,   6, 240,  17,  81,  81,  35,  34,
     17,  33,  33,  33,  33,  37, 240,  64,  12,   6, 240, 240,
     20,  21,  17,  81,  49,  35, 240,  80,  13,   6, 242,  65,
     65,  82,  65,  81,  81,  65,  81,  66, 160,  15,   7, 240,
    240, 116,  34,  17,  49,  33,  49,  33,  52,  81,  52, 160,
     17,   7, 242,  81,  97, 100,  49,  33,  34,  33,  33,  33,
     17,  17,  34, 240, 128,   9,   3, 113, 114,  33,  18,  17,
     34, 160,  11,   5, 209, 210,  65,  65,  50,  49,  65,  34,
    128,  17,   7, 240,  17,  97,  97,  81,  34,  35,  65,  17,
     49,  33,  17,  66, 240, 128,  11,   4, 146,  33,  49,  49,
     49,  33,  49,  50, 224,  22,  10,   0,   6,  36,  19,  49,
     33,  33,  49,  33,  33,  33,  33,  33,  17,  17,  33,  34,
    240, 240,  32,  17,   7, 240, 240,  81,  19,  49,  33,  49,
     33,  33,  33,  17,  17,  34, 240, 128,  14,   6, 240, 240,
     34,  34,  33,  17,  49,  17,  33,  50, 240,  96,  16,   7,
    240, 240,  98,  18,  50,  17,  49,  33,  33,  34,  36,  49,
     82, 192,  15,   6, 240, 240,  20,  18,  17,  33,  33,  33,
     33,  35,  81,  67, 128,  11,   6, 240, 240,  20,  17,  17,
     65,  81, 240, 208,  12,   5, 240, 194,  33,  17,  35,  17,
     33,  19, 240,  32,  12,   4, 161,  33,  49,  35,  33,  33,
     49,  17,  18, 224,  15,   6, 240, 240,  17,  33,  18,  17,
     49,  17,  49,  17,  52, 240,  64,  15,   6, 240, 240,  17,
     33,  18,  33,  33,  33,  33,  17,  50, 240,  96,  20,   8,
    240, 240, 177,  33,  17,  18,  33,  17,  33,  17,  33,  33,
     17,  17,  50,  18, 240, 176,  13,   6, 240, 240,  20,  17,
     17,  81,  81,  17,  20, 240,  80,  16,   6, 240, 240,  17,
     33,  18,  18,  33,  17,  49,  17,  51,  66,  35, 144,  10,
      4, 240,  83,  33,  33,  49,  17,  33, 224,   8,   6, 240,
    240, 101, 240, 240, 112,   8,  12,   0,   9,  11,   0,   9,
     16,   9,   5, 177,  17,  17,  17,   0,   5, 112,   6,   5,
    164,   0,   6,  48,
};

//*****************************************************************************
//
// The font definition for the 12 point Cm italic font.
//
//*****************************************************************************
const tFont g_sFontCm12i =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    10,

    //
    // The height of the font.
    //
    13,

    //
    // The baseline of the font.
    //
    10,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   15,   25,   46,   61,   93,  118,
         124,  139,  154,  164,  176,  182,  188,  193,
         209,  228,  240,  255,  272,  287,  303,  321,
         336,  354,  370,  376,  383,  394,  403,  417,
         433,  455,  474,  492,  509,  529,  544,  558,
         576,  597,  609,  623,  644,  658,  687,  711,
         730,  746,  766,  785,  801,  816,  837,  856,
         884,  904,  921,  938,  953,  961,  976,  984,
         989,  995, 1009, 1024, 1036, 1052, 1064, 1077,
        1092, 1109, 1118, 1129, 1146, 1157, 1179, 1196,
        1210, 1226, 1241, 1252, 1264, 1276, 1291, 1306,
        1326, 1339, 1355, 1365, 1373, 1381, 1390,
    },

    //
    // A pointer to the actual font data
    //
    g_pucCm12iData
};
