/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#ifndef __XR17V35X_H__
#define __XR17V35X_H__


#define     XR_CFG_RHR			0x00  	/* LCR[7]=0 */
#define     XR_CFG_THR  		0x00	/* LCR[7]=0 */
#define     XR_CFG_DLL  		0x00	/* LCR[7]=1 */
#define     XR_CFG_DLM  		0x01	/* LCR[7]=1 */
#define     XR_CFG_DLD  		0x02	/* LCR[7]=1 */
#define     XR_CFG_IER  		0x01	/* LCR[7]=0 */
#define     XR_CFG_ISR  		0x02	/* LCR[7]=0 read only */
#define     XR_CFG_FCR  		0x02	/* LCR[7]=0 write only */
#define     XR_CFG_LCR  		0x03	/*  */
#define     XR_CFG_MCR  		0x04	/*  */
#define     XR_CFG_LSR  		0x05	/*  */
#define     XR_CFG_MSR  		0x06	/* EFR bit4=1 */
#define     XR_CFG_SPR  		0x07	/*  */

#define     XR_CFG_FCTR			0x08
#define     XR_CFG_EFR			0x09
#define     XR_CFG_TXCNT		0x0A
#define     XR_CFG_TXTRG		0x0A
#define     XR_CFG_RXCNT		0x0B
#define     XR_CFG_RXTRG		0x0B
#define     XR_CFG_XOFF1		0x0C
#define     XR_CFG_XOFF2		0x0D
#define     XR_CFG_XON1			0x0E
#define     XR_CFG_XON2			0x0F

#define     XR_CFG_INT0			0x80
#define     XR_CFG_INT1			0x81
#define     XR_CFG_INT2			0x82
#define     XR_CFG_INT3			0x83
#define     XR_CFG_TIMERCNTL	0x84
#define     XR_CFG_REGA			0x85
#define     XR_CFG_TIMERLSB		0x86
#define     XR_CFG_TIMERMSB		0x87
#define     XR_CFG_8XMODE		0x88
#define     XR_CFG_4XMODE		0x89
#define     XR_CFG_RESET		0x8A
#define     XR_CFG_SLEEP		0x8B
#define     XR_CFG_DREV			0x8C
#define     XR_CFG_DVID			0x8D
#define     XR_CFG_REGB			0x8E
#define     XR_CFG_MPIOINT0_7	0x8F
#define     XR_CFG_MPIOLVL0_7	0x90
#define     XR_CFG_MPIO3T0_7	0x91
#define     XR_CFG_MPIOINV0_7	0x92
#define     XR_CFG_MPIOSEL0_7	0x93
#define     XR_CFG_MPIOOD0_7	0x94
#define     XR_CFG_MPIOINT8_15	0x95
#define     XR_CFG_MPIOLVL8_15	0x96
#define     XR_CFG_MPIO3T8_15	0x97
#define     XR_CFG_MPIOINV8_15	0x98
#define     XR_CFG_MPIOSEL8_15	0x99
#define     XR_CFG_MPIOOD8_15	0x9A

#ifdef __ARM_ARCH__
///#define   	FIOQSIZE          	0x5460   /* sould be already there in ioctls.h */
///#define   	FIOQSIZE          	0x545E   /* for ARM arch */
#define   	FIOQSIZE_ARM		FIOQSIZE
///#define   	FIOQSIZE_ARM		0x5460
///#define   	FIOQSIZE_ARM		0x545E
#define     EXAR_READ_REG		(FIOQSIZE_ARM + 1)
#define     EXAR_WRITE_REG     	(FIOQSIZE_ARM + 2)
#else
//#define   FIOQSIZE          0x5460   /* sould be already there in ioctls.h */
//#define   FIOQSIZE          0x545E   /* for ARM arch */
#define     EXAR_READ_REG		(FIOQSIZE + 1)
#define     EXAR_WRITE_REG     	(FIOQSIZE + 2)
#endif

///#define     MODEMDEVICE    	"/dev/ttyXR0"
///#define     MODEMDEVICE    	"/dev/ttyS0"

struct xrioctl_rw_reg {
    unsigned char reg;
    unsigned char regvalue;
};



#endif /* __XR17V35X_H__ */
