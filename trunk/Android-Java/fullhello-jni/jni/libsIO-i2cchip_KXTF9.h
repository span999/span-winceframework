/*




*/
#ifndef _LIBSIO_I2CCHIP_KXTF9_H_
#define _LIBSIO_I2CCHIP_KXTF9_H_

#include "libsIO-i2cchip.h"



/*   */
static const struct tI2CCHIPDATA chipdata_KXTF9 = {
	"kionix",
	"KXTF9",
	"N/A",
	"3D-accelerometer",
	0x0f,
	0x0,
	1,
};

static const struct tI2CCHIPREG chipreglist_KXTF9[] = {
	{	0x00,	1,	"XOUT_HPF_L" },
	{	0x01,	1,	"XOUT_HPF_H" },
	{	0x02,	1,	"YOUT_HPF_L" },
	{	0x03,	1,	"YOUT_HPF_H" },
	{	0x04,	1,	"ZOUT_HPF_L" },
	{	0x05,	1,	"ZOUT_HPF_H" },
	{	0x06,	1,	"XOUT_L" },
	{	0x07,	1,	"XOUT_H" },
	{	0x08,	1,	"YOUT_L" },
	{	0x09,	1,	"YOUT_H" },
	{	0x0a,	1,	"ZOUT_L" },
	{	0x0b,	1,	"ZOUT_H" },
	{	0x0c,	1,	"DCST_RESP" },
	{	0x0d,	1,	"Not Used" },
	{	0x0e,	1,	"Not Used" },
	{	0x0f,	1,	"WHO_AM_I" },
	{	0x10,	1,	"TILT_POS_CUR" },
	{	0x11,	1,	"TILT_POS_PRE" },
	{	0x12,	1,	"Kionix Reserved" },
	{	0x13,	1,	"Kionix Reserved" },
	{	0x14,	1,	"Kionix Reserved" },
	{	0x15,	1,	"INT_SRC_REG1" },
	{	0x16,	1,	"INT_SRC_REG2" },
	{	0x17,	1,	"Not Used" },
	{	0x18,	1,	"STATUS_REG" },
	{	0x19,	1,	"Not Used" },
	{	0x1a,	1,	"INT_REL" },
	{	0x1b,	1,	"CTRL_REG1" },
	{	0x1c,	1,	"CTRL_REG2" },
	{	0x1d,	1,	"CTRL_REG3" },
	{	0x1e,	1,	"INT_CTRL_REG1" },
	{	0x1f,	1,	"INT_CTRL_REG2" },
	{	0x20,	1,	"INT_CTRL_REG3" },
	{	0x21,	1,	"DATA_CTRL_REG" },
	{	0x28,	1,	"TILT_TIMER" },
	{	0x29,	1,	"WUF_TIMER" },
	{	0x2a,	1,	"Not Used" },
	{	0x2b,	1,	"TDT_TIMER" },
	{	0x2c,	1,	"TDT_H_THRESH" },
	{	0x2d,	1,	"TDT_L_THRESH" },
	{	0x2e,	1,	"TDT_TAP_TIMER" },
	{	0x2f,	1,	"TDT_TOTAL_TIMER" },
	{	0x30,	1,	"TDT_LATENCY_TIMER" },
	{	0x31,	1,	"TDT_WINDOW_TIMER" },
	{	0x3a,	1,	"SELF_TEST" },
	{	0x5a,	1,	"WUF_THRESH" },
	{	0x5c,	1,	"TILT_ANGLE" },
	{	0x5f,	1,	"HYST_SET" },
	{	0xff,	0,	"end of list" },
};




#endif	/* _LIBSIO_I2CCHIP_KXTF9_H_ */
