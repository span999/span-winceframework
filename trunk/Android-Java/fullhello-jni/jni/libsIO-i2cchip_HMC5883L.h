/*




*/
#ifndef _LIBSIO_I2CCHIP_HMC5883L_H_
#define _LIBSIO_I2CCHIP_HMC5883L_H_

#include "libsIO-i2cchip.h"


static const struct tI2CCHIPREG chipreglist_HMC5883L[];
/*   */
static const struct tI2CCHIPDATA chipdata_HMC5883L = {
	"Honeywell",
	"hmc5883l",
	"N/A",
	"Compass",
	0x1e,
	0x0,
	1,
	(struct tI2CCHIPREG**)&chipreglist_HMC5883L,
};

static const struct tI2CCHIPREG chipreglist_HMC5883L[] = {
	{	0x00,	1,	"Configuration Register A" },
	{	0x01,	1,	"Configuration Register B" },
	{	0x02,	1,	"Mode Register" },
	{	0x03,	1,	"Data Output X MSB Register" },
	{	0x04,	1,	"Data Output X LSB Register" },
	{	0x05,	1,	"Data Output Y MSB Register" },
	{	0x06,	1,	"Data Output Y LSB Register" },
	{	0x07,	1,	"Data Output Z MSB Register" },
	{	0x08,	1,	"Data Output Z LSB Register" },
	{	0x09,	1,	"Status Register" },
	{	0x0a,	1,	"Identification Register A" },
	{	0x0b,	1,	"Identification Register B" },
	{	0x0c,	1,	"Identification Register C" },
	{	0xff,	0,	"end of list" },
};



#endif	/* _LIBSIO_I2CCHIP_HMC5883L_H_ */
