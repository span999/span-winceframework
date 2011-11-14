/*




*/
#ifndef _LIBSIO_I2CCHIP_MAX8698C_H_
#define _LIBSIO_I2CCHIP_MAX8698C_H_

#include "libsIO-i2cchip.h"


static const struct tI2CCHIPREG chipreglist_MAX8698C[];
/*   */
static const struct tI2CCHIPDATA chipdata_MAX8698C = {
	"Maxim",
	"MAX8698C",
	"N/A",
	"PMIC driver",
	0x66,
	0x0,
	1,
	(struct tI2CCHIPREG**)&chipreglist_MAX8698C,
};

static const struct tI2CCHIPREG chipreglist_MAX8698C[] = {
	{	0x00,	1,	"ONOFF1" },
	{	0x01,	1,	"ONOFF2" },
	{	0x02,	1,	"ADISCHG_EN1" },
	{	0x03,	1,	"ADISCHG_EN2" },
	{	0x04,	1,	"Buck1(VCC_ARM)" },
	{	0x05,	1,	"Buck1(VCC_ARM)" },
	{	0x06,	1,	"Buck2(VCC_INTERNAL)" },
	{	0x07,	1,	"Buck3(VCC_MEM)" },
	{	0x08,	1,	"LDO2/LDO3" },
	{	0x09,	1,	"LDO4" },
	{	0x0a,	1,	"LDO5" },
	{	0x0b,	1,	"LDO6" },
	{	0x0c,	1,	"LDO7" },
	{	0x0d,	1,	"LDO8" },
	{	0x0e,	1,	"LDO9" },
	{	0x0f,	1,	"LBCNFG" },
	{	0xff,	0,	"end of list" },
};



#endif	/* _LIBSIO_I2CCHIP_MAX8698C_H_ */
