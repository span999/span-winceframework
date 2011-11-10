/*




*/
#ifndef _LIBSIO_I2CCHIP_TPS65950_H_
#define _LIBSIO_I2CCHIP_TPS65950_H_

#include "libsIO-i2cchip.h"


static const struct tI2CCHIPREG chipreglist_TPS65950_48USB[] = {
	{	0x00,	2,	"COMMON_00" },
	{	0x02,	2,	"COMMON_02" },
	{	0x04,	2,	"COMMON_04" },
	{	0xff,	0,	"end of list" },
};

static const struct tI2CCHIPREG chipreglist_TPS65950_49AUDIO[] = {
	{	0x00,	2,	"COMMON_00" },
	{	0x02,	2,	"COMMON_02" },
	{	0x04,	2,	"COMMON_04" },
	{	0xff,	0,	"end of list" },
};

static const struct tI2CCHIPREG chipreglist_TPS65950_4a[] = {
	{	0x00,	2,	"COMMON_00" },
	{	0x02,	2,	"COMMON_02" },
	{	0x04,	2,	"COMMON_04" },
	{	0xff,	0,	"end of list" },
};

static const struct tI2CCHIPREG chipreglist_TPS65950_4b[] = {
	{	0x00,	2,	"COMMON_00" },
	{	0x02,	2,	"COMMON_02" },
	{	0x04,	2,	"COMMON_04" },
	{	0xff,	0,	"end of list" },
};

static const struct tI2CCHIPREG chipreglist_TPS65950_12SMARTREFLEX[] = {
	{	0x00,	2,	"COMMON_00" },
	{	0x02,	2,	"COMMON_02" },
	{	0x04,	2,	"COMMON_04" },
	{	0xff,	0,	"end of list" },
};




#endif	/* _LIBSIO_I2CCHIP_TPS65950_H_ */
