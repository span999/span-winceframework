/*




*/
#ifndef _LIBSIO_I2CCHIP_H_
#define _LIBSIO_I2CCHIP_H_



/*
 * struct for registers
 */
struct tI2CCHIPREG {
	int 	regNum;			///register number.
	int 	regWidth;		///register width in byte. (8bit=1,16bit=2)
	char* 	regName;		///register name
	char* 	regDescript;	///register description
};



/*
 * struct for chip information
 */
struct tI2CCHIPDATA {
	char* 	vendorName;
	char* 	chipName;
	char* 	version;
	char*	description;
	int		chipID;
	int		chipID2;
	int 	regWidth;	///register width in byte. (8bit=1,16bit=2)
};







#endif	/* _LIBSIO_I2CCHIP_H_ */
