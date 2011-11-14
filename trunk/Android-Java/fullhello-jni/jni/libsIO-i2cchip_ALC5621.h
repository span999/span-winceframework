/*




*/
#ifndef _LIBSIO_I2CCHIP_ALC5621_H_
#define _LIBSIO_I2CCHIP_ALC5621_H_

#include "libsIO-i2cchip.h"




/*   */
static const struct tI2CCHIPDATA chipdata_ALC5621 = {
	"Realtek",
	"ALC5621",
	"N/A",
	"Audio driver",
	0x1a,
	0x0,
	2,
};

static const struct tI2CCHIPREG chipreglist_ALC5621[] = {
	{	0x00,	2,	"Reset" },
	{	0x02,	2,	"Speaker Output Volume" },
	{	0x04,	2,	"Headphone Output Volume" },
	{	0x06,	2,	"MONO_OUT/AUXOUT Volume" },
	{	0x08,	2,	"AUXIN Volume" },
	{	0x0a,	2,	"LINE_IN Volume" },
	{	0x0c,	2,	"STEREO DAC Volume" },
	{	0x0e,	2,	"MIC Volume" },
	{	0x10,	2,	"MIC Routing Control" },
	{	0x12,	2,	"ADC Record Gain" },
	{	0x14,	2,	"ADC Record Mixer Control" },
	{	0x16,	2,	"Avol Soft Volume Control Time" },
	{	0x1c,	2,	"Output Mixer Control" },
	{	0x22,	2,	"Microphone Control" },
	{	0x34,	2,	"Digital Audio Interface Control" },
	{	0x36,	2,	"Stereo AD/DA Clock Control" },
	{	0x38,	2,	"Companding Control" },
	{	0x3a,	2,	"Power Management Addition 1" },
	{	0x3c,	2,	"Power Management Addition 2" },
	{	0x3e,	2,	"Power Management Addition 3" },
	{	0x40,	2,	"Additional Control Register" },
	{	0x42,	2,	"Global Clock Control Register" },
	{	0x44,	2,	"PLL Control Register" },
	{	0x4c,	2,	"GPIO Pin Configuration" },
	{	0x4e,	2,	"GPIO Pin Polarity" },
	{	0x50,	2,	"GPIO Pin Sticky" },
	{	0x52,	2,	"GPIO Pin Wake-Up" },
	{	0x54,	2,	"GPIO Pin Status" },
	{	0x56,	2,	"Pin Sharing" },
	{	0x58,	2,	"Over-Current Status" },
	{	0x5a,	2,	"Jack Detect Control Register" },
	{	0x5e,	2,	"MISC Control" },
	{	0x60,	2,	"Stereo and Spatial Effect Block Control" },
	{	0x62,	2,	"EQ Control" },
	{	0x66,	2,	"EQ Mode Change Enable" },
	{	0x68,	2,	"AVC Control" },
	{	0x6a,	2,	"Index Address" },
	{	0x6c,	2,	"Index Data" },
	{	0x7c,	2,	"VENDOR ID 1" },
	{	0x7e,	2,	"VENDOR ID 2" },
	{	0xff,	0,	"end of list" },
};




#endif	/* _LIBSIO_I2CCHIP_ALC5621_H_ */
