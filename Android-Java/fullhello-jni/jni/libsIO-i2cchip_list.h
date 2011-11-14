/*




*/
#ifndef _LIBSIO_I2CCHIP_LIST_H_
#define _LIBSIO_I2CCHIP_LIST_H_

#include "libsIO-i2cchip.h"

///chips 
#include "libsIO-i2cchip_COMMON.h"
#include "libsIO-i2cchip_HMC5883L.h"
#include "libsIO-i2cchip_KXTF9.h"
#include "libsIO-i2cchip_ALC5621.h"
#include "libsIO-i2cchip_MAX8698C.h"
#include "libsIO-i2cchip_TPS65950.h"
#include "libsIO-i2cchip_si470x.h"


static const struct tI2CCHIPDATA* chiplistP[] = {
	&chipdata_COMMON,
	&chipdata_TPS65950_48USB,
	&chipdata_TPS65950_49AUDIO,
	&chipdata_TPS65950_4aPM,
	&chipdata_TPS65950_4bPM,
	&chipdata_TPS65950_12SMARTREFLEX,
	&chipdata_MAX8698C,
	&chipdata_KXTF9,
	&chipdata_HMC5883L,
	&chipdata_ALC5621,
	&chipdata_si470x,
	0									/* null for end*/
};




#endif	/* _LIBSIO_I2CCHIP_LIST_H_ */
