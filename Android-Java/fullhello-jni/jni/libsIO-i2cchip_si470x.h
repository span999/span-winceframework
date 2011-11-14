/*




*/
#ifndef _LIBSIO_I2CCHIP_SI470X_H_
#define _LIBSIO_I2CCHIP_SI470X_H_

#include "libsIO-i2cchip.h"


static const struct tI2CCHIPREG chipreglist_si470x[];
/*   */
static const struct tI2CCHIPDATA chipdata_si470x = {
	"SILICON LABS",
	"si470x",
	"N/A",
	"FM RDS/TMC Radio Receiver",
	0x10,
	0x0,
	1,
	(struct tI2CCHIPREG**)&chipreglist_si470x,
};

static const struct tI2CCHIPREG chipreglist_si470x[] = {
	{	0x00,	1,	"DEVICEID" },
	{	0x01,	1,	"CHIPID" },
	{	0x02,	1,	"POWERCFG" },
	{	0x03,	1,	"CHANNEL" },
	{	0x04,	1,	"SYSCONFIG1" },
	{	0x05,	1,	"SYSCONFIG2" },
	{	0x06,	1,	"SYSCONFIG3" },
	{	0x07,	1,	"TEST1" },
	{	0x08,	1,	"TEST2" },
	{	0x09,	1,	"BOOTCONFIG" },
	{	0x0a,	1,	"STATUSRSSI" },
	{	0x0b,	1,	"READCHAN" },
	{	0x0c,	1,	"RDSA" },
	{	0x0d,	1,	"RDSB" },
	{	0x0e,	1,	"RDSC" },
	{	0x0f,	1,	"RDSD" },
	{	0xff,	0,	"end of list" },
};




#endif	/* _LIBSIO_I2CCHIP_SI470X_H_ */
