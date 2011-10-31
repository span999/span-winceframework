/*




*/
#ifndef _LIBSIO_I2CCHIP_H_
#define _LIBSIO_I2CCHIP_H_


struct tI2CCHIPREG {
	int 	regNum;		///register number.
	int 	regWidth;	///register wodth in byte. (8bit=1,16bit=2)
	char* 	regName;	///register name
};


struct tI2CCHIPDATA {
	char* chipName;
};


#endif	/* _LIBSIO_I2CCHIP_H_ */
