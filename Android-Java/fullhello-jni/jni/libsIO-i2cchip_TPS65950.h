/*




*/
#ifndef _LIBSIO_I2CCHIP_TPS65950_H_
#define _LIBSIO_I2CCHIP_TPS65950_H_

#include "libsIO-i2cchip.h"


static const struct tI2CCHIPREG chipreglist_TPS65950_48USB[];
/*   */
static const struct tI2CCHIPDATA chipdata_TPS65950_48USB = {
	"Ti",
	"TPS65950",
	"N/A",
	"USB OTG driver",
	0x48,
	0x0,
	2,
	(struct tI2CCHIPREG**)&chipreglist_TPS65950_48USB,
};

static const struct tI2CCHIPREG chipreglist_TPS65950_48USB[] = {
	{	0x00,	2,	"COMMON_00", "description" },
	{	0x02,	2,	"COMMON_02", "description" },
	{	0x04,	2,	"COMMON_04", "description" },
	{	0xff,	0,	"end of list", "description" },
};


static const struct tI2CCHIPREG chipreglist_TPS65950_49AUDIO[];
/*   */
static const struct tI2CCHIPDATA chipdata_TPS65950_49AUDIO = {
	"Ti",
	"TPS65950",
	"N/A",
	"Audio driver",
	0x49,
	0x0,
	2,
	(struct tI2CCHIPREG**)&chipreglist_TPS65950_49AUDIO,
};

static const struct tI2CCHIPREG chipreglist_TPS65950_49AUDIO[] = {
	{	0x00,	2,	"COMMON_00", "description" },
	{	0x02,	2,	"COMMON_02", "description" },
	{	0x04,	2,	"COMMON_04", "description" },
	{	0xff,	0,	"end of list", "description" },
};


static const struct tI2CCHIPREG chipreglist_TPS65950_4aPM[];
/*   */
static const struct tI2CCHIPDATA chipdata_TPS65950_4aPM = {
	"Ti",
	"TPS65950",
	"N/A",
	"PM driver 1",
	0x4a,
	0x0,
	2,
	(struct tI2CCHIPREG**)&chipreglist_TPS65950_4aPM,
};

static const struct tI2CCHIPREG chipreglist_TPS65950_4aPM[] = {
	{	0x00,	2,	"COMMON_00", "description" },
	{	0x02,	2,	"COMMON_02", "description" },
	{	0x04,	2,	"COMMON_04", "description" },
	{	0xff,	0,	"end of list", "description" },
};


static const struct tI2CCHIPREG chipreglist_TPS65950_4bPM[];
/*   */
static const struct tI2CCHIPDATA chipdata_TPS65950_4bPM = {
	"Ti",
	"TPS65950",
	"N/A",
	"PM driver 2",
	0x4b,
	0x0,
	2,
	(struct tI2CCHIPREG**)&chipreglist_TPS65950_4bPM,
};

static const struct tI2CCHIPREG chipreglist_TPS65950_4bPM[] = {
	{	0x00,	2,	"COMMON_00", "description" },
	{	0x02,	2,	"COMMON_02", "description" },
	{	0x04,	2,	"COMMON_04", "description" },
	{	0xff,	0,	"end of list", "description" },
};


static const struct tI2CCHIPREG chipreglist_TPS65950_12SMARTREFLEX[];
/*   */
static const struct tI2CCHIPDATA chipdata_TPS65950_12SMARTREFLEX = {
	"Ti",
	"TPS65950",
	"N/A",
	"MARTRE FLEX",
	0x12,
	0x0,
	2,
	(struct tI2CCHIPREG**)&chipreglist_TPS65950_12SMARTREFLEX,
};

static const struct tI2CCHIPREG chipreglist_TPS65950_12SMARTREFLEX[] = {
	{	0x00,	2,	"COMMON_00", "description" },
	{	0x02,	2,	"COMMON_02", "description" },
	{	0x04,	2,	"COMMON_04", "description" },
	{	0xff,	0,	"end of list", "description" },
};




#endif	/* _LIBSIO_I2CCHIP_TPS65950_H_ */
