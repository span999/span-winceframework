#ifndef __SPCELOGCRC_H__
#define __SPCELOGCRC_H__
/*===========================================================================

              C R C    S E R V I C E S    D E C L A R A T I O N S

DESCRIPTION
  The following declarations are for use with the CRC Services.

Copyright(c) 1991,1992,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright(c) 1998,1999      by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright(c) 2005           by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/* Definitions for Half-Duplex Async-HDLC mode.
 * NOTE: Under most circumstances, the communications layer doesn't need to 
 * know about HDLC.  Diag handles this.  However, in the case of routing mode,
 * The communications layer needs to be able to determine the end of a packet
 * in order to forward packets.
 */
#define DIAG_ASYNC_CONTROL_CHAR 0x7E /* Flag character value.  Corresponds 
                                        to the ~ character                  */
#define DIAG_ASYNC_ESC_CHAR     0x7D /* Escape sequence 1st character value */
#define DIAG_ASYNC_ESC_MASK     0x20 /* Escape sequence complement value    */
#define DIAG_MIN_PKT_SIZE       3    /* Command code and CRC                */
#define DIAG_ESC_EXPAND_SIZE    2    /* Size of an expanded escape code     */
#define DIAG_ASYNC_TAIL_SIZE    3    /* CRC plus tail character             */
#define DIAG_FOOTER_SIZE        5    /* 2 byte CRC plus the possibility of 
                                        expanding per AHDLC protocol plus  
                                        the  ASYNC_FLAG                     */
#define DIAG_ESC_EXPAND_SHIFT 1      /* The number of bits to shift for
                                        expanded size due to escaping.      */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Residual CRC value to compare against return value of crc_16_calc().
** Use crc_16_calc() to calculate a 16 bit CRC, and append it to the buffer.
** When crc_16_calc() is applied to the unchanged result, it returns CRC_16_OK.
*/
#define CRC_16_OK               0xE2F0

/* Mask for CRC-16 polynomial:
**
**      x^16 + x^12 + x^5 + 1
**
** This is more commonly referred to as CCITT-16.
** Note:  the x^16 tap is left off, it's implicit.
*/
#define CRC_16_POLYNOMIAL       0x1021

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
** well as allows detection of an entire data stream of zeroes.
*/
#define CRC_16_SEED             0xFFFF

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Seed value for CRC register.  The all zeroes seed is inverted prior to
** being used in the step-wise CRC CCITT-16.  This behaves as CRC_16_SEED.
*/
#define CRC_16_STEP_SEED        (~((UINT16) CRC_16_SEED))

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Mask for CRC-16 polynomial:
**
**      x^16 + x^12 + x^5 + 1
**
** This is more commonly referred to as CCITT-16.
** Note:  the x^16 tap is left off, it's implicit.
*/
#define CRC_16_L_POLYNOMIAL     0x8408

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
** well as allows detection of an entire data stream of zeroes.
*/
#define CRC_16_L_SEED           0xFFFF

/* Seed value for CRC register.  The all ones seed is inverted prior to
** being used in the step-wise CRC CCITT-16.  This behaves as CRC_16_L_SEED.
*/
#define CRC_16_L_STEP_SEED        ((word) ~((word) CRC_16_L_SEED))

/* Residual CRC value to compare against return value of a CRC_16_L_STEP().
** Use CRC_16_L_STEP() to calculate a 16 bit CRC, complement the result,
** and append it to the buffer.  When CRC_16_L_STEP() is applied to the
** unchanged entire buffer, and complemented, it returns CRC_16_L_OK.
** Crc_16_l_calc() of the same entire buffer returns CRC_16_L_OK.
*/
#define CRC_16_L_OK             0x0F47

/*===========================================================================

MACRO CRC_16_L_STEP

DESCRIPTION
  This function calculates one byte step of an LSB-first 16-bit CRC over
  a specified number of data bits.  It can be used to produce a CRC and
  to check a CRC.

PARAMETERS
  xx_crc  Current value of the CRC calculation, 16-bits
  xx_c    New byte to figure into the CRC, 8-bits

DEPENDENCIES
  None

RETURN VALUE
  The new CRC value, 16-bits.  If this macro is being used to check a
  CRC, and is run over a range of bytes, the return value will be equal
  to CRC_16_L_OK (defined in crc.h) if the CRC checks correctly.  When
  generating a CRC to be appended to such a range of bytes, the final
  result must be XOR'd with CRC_16_L_SEED before being appended.

SIDE EFFECTS
  xx_crc is evaluated twice within this macro.

===========================================================================*/

/* crc_16_l_table is a pointer in the Boot Block jump table which
** points to the actual table crc_16_l_table 
*/
extern const UINT16 crc_16_l_table[];       /* Extern for macro (global) */

#define CRC_16_L_STEP(xx_crc,xx_c) \
  (((xx_crc) >> 8) ^ crc_16_l_table[((xx_crc) ^ (xx_c)) & 0x00ff])

  
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION CRC_16_CALC

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern UINT16 crc_16_calc
(
  BYTE *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  DWORD len
    /* Number of data bits to calculate the CRC over */
);


/*===========================================================================

FUNCTION CRC_16_L_CALC

DESCRIPTION
  This function calculates an LSB-first 16-bit CRC over a specified number
  of data bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_L_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern UINT16 crc_16_l_calc
(
  BYTE *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the LS bit of the first byte.
    */

  DWORD len
    /* Number of data bits to calculate the CRC over */
);

/*===========================================================================

FUNCTION CRC_16_STEP

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.  The CRC value
  passed in is used to continue the CRC calculation from a previous call,
  this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern UINT16 crc_16_step
(
  UINT16 seed,
    /* Either the result of a previous crc_16_step() or CRC_16_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  BYTE *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  DWORD byte_len
    /* Number of data bytes to calculate the CRC over */
);


/*===========================================================================

FUNCTION CRC_16_L_STEP

DESCRIPTION
  This function calculates a LSB-first 16-bit CRC over a specified number 
  of data bytes.  It can be used to produce a CRC and to check a CRC.  
  The CRC value passed in is used to continue the CRC calculation from a
  previous call; this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern UINT16 crc_16_l_step
(
  UINT16 seed,

  const void *buf_ptr,
    /* Either the result of a previous crc_16_l_step() or CRC_16_L_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  DWORD byte_len
    /* Number of data bytes to calculate the CRC over */
);


extern UINT16 FindCRC( BYTE *pData, DWORD dwLen );

/* EJECT */
#endif ///#ifndef__SPCELOGCRC_H__


