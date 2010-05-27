/* MiTAC Int'l Corp. 2003

=========================================================================
Description:
    This file contains definitio for CPU related GPIOs


=========================================================================
Release notes

Modifier            sign            date            Description
----------------------------------------------------------------------------------------------------
Span liu            span0326      03/26/2004      add code for refesh DVT2 Backlight control
Span liu            span0220      02/20/2004      add code for PWM0(GPIO16) control backlight in flag "PWM0_BACKLIGHT"
span liu            span0128      01/28/2004      Enable GPIO57,
miranda             Xy48          12/30/2003      Add Carkit support
Henry Sun           sun07	  12/29/2003	   add SD_WP define
Ryan Lee            Ryn01         09/19/2003      Modify for Hitachi camera.
Andy Lin            andy07        07/18/2003      add MCU_AWAKE & MCU_WAKE_UP define
Andy Lin            andy06        06/23/2003      add AC_OUT & CHARGE_FULL define
span liu            span0704      06/20/2003      support AC IN (Charging) detect
Andy Lin            andy05        05/16/2003      add CLAM_CLOSED define
HenryL              HL001         06/10/2003      add MA3 reset GPIO 2 define
Jawa Chen           jawa01        05/22/2003      add GPIO 80-89 define 
Andy Lin            andy04        05/16/2003      add KedLED PWM control -- keyLED off define
miranda             Xy14          05/13/2003      Add for Amp validation  
Andy Lin            andy03        05/13/2003      add KedLED PWM control define
Andy Lin            andy02        05/13/2003      add Hall Switch define for  EVT3
Jacky               Jacky01       04/29/2003      Add define for MA3
Pat Kuo             Pat02         04/25/2003      support END key
Paul Jiao           Paul01        04/21/2003      Add for USB
miranda             Xy03          04/15/2003      Add define for PTT
Jack Lin            jack01        04/11/2003      Add MCU GPIO define
Yi Yang             yy01          04/09/2003      add define for SDMMC
Pat Kuo             Pat01         04/08/2003      control 2 side-key in keypad driver (volume up/down key is moved to here)
Xy                  Xy01          04/08/2003      add for phone jack detection
Bingo               bin02         04/07/2003      add some camera gpio definations
Guoshun Zhao        zhao01        04/07/2003      add audio PMU
Span Liu            span01        04/07/2003      add Backlight control definition
Horace Lee          hy02          04/04/2003      add the set GPIO70 direction definition (for RIL)
Andy Lin            andy01        04/04/2003      add define for Hall Switch
Weiber Cheng        wei01         04/04/2003      Add for vibrator defination
Bingo               bin01         04/01/2003      Add camera gpio macro defines
Horace Lee          hy01          03/27/2003      add the set GPIO66 direction definition
Pat Kuo                           03/26/2003      initial release

*/


// General Purpose I/O Register Definitions
//

#ifndef __GPIO_H
#define __GPIO_H

typedef struct {
    unsigned int GPLR_x;    // 0x40E00000 Pin Level Registers
    unsigned int GPLR_y;    // 0x40E00004
    unsigned int GPLR_z;    // 0x40E00008

    unsigned int GPDR_x;    // 0x40E0000C Pin Direction Registers
    unsigned int GPDR_y;    // 0x40E00010
    unsigned int GPDR_z;    // 0x40E00014

    unsigned int GPSR_x;    // 0x40E00018 Pin Output Set Registers
    unsigned int GPSR_y;    // 0x40E0001C
    unsigned int GPSR_z;    // 0x40E00020

    unsigned int GPCR_x;    // 0x40E00024 Pin Output Clear Registers
    unsigned int GPCR_y;    // 0x40E00028
    unsigned int GPCR_z;    // 0x40E0002C

    unsigned int GRER_x;    // 0x40E00030 Rising Edge Detect Enable Registers
    unsigned int GRER_y;    // 0x40E00034
    unsigned int GRER_z;    // 0x40E00038

    unsigned int GFER_x;    // 0x40E0003C Falling Edge Detect Enable Registers
    unsigned int GFER_y;    // 0x40E00040
    unsigned int GFER_z;    // 0x40E00044

    unsigned int GEDR_x;    // 0x40E00048 Edge Detect Status Registers
    unsigned int GEDR_y;    // 0x40E0004C
    unsigned int GEDR_z;    // 0x40E00050

    unsigned int GAFR0_x;   // 0x40E00054 Alternate Function Registers
    unsigned int GAFR1_x;   // 0x40E00058
    unsigned int GAFR0_y;   // 0x40E0005C
    unsigned int GAFR1_y;   // 0x40E00060
    unsigned int GAFR0_z;   // 0x40E00064
    unsigned int GAFR1_z;   // 0x40E00068

} GPIO_REGS, *PGPIO_REGS;

// For use with GPIO registers GPLR_x, GPSR_x, GPCR_x, GPDR_x, GRER_x, GFER_x, GEDR_x
//
#define GPIO_0      0x00000001
#define GPIO_1      0x00000002
#define GPIO_2      0x00000004
#define GPIO_3      0x00000008
#define GPIO_4      0x00000010
#define GPIO_5      0x00000020
#define GPIO_6      0x00000040
#define GPIO_7      0x00000080
#define GPIO_8      0x00000100
#define GPIO_9      0x00000200
#define GPIO_10     0x00000400
#define GPIO_11     0x00000800
#define GPIO_12     0x00001000
#define GPIO_13     0x00002000
#define GPIO_14     0x00004000
#define GPIO_15     0x00008000
#define GPIO_16     0x00010000
#define GPIO_17     0x00020000
#define GPIO_18     0x00040000
#define GPIO_19     0x00080000
#define GPIO_20     0x00100000
#define GPIO_21     0x00200000
#define GPIO_22     0x00400000
#define GPIO_23     0x00800000
#define GPIO_24     0x01000000
#define GPIO_25     0x02000000
#define GPIO_26     0x04000000
#define GPIO_27     0x08000000
#define GPIO_28     0x10000000
#define GPIO_29     0x20000000
#define GPIO_30     0x40000000
#define GPIO_31     0x80000000

// For use with GPIO registers GPLR_y, GPSR_y, GPCR_y, GPDR_y, GRER_y, GFER_y, GEDR_y
//
#define GPIO_32     0x00000001
#define GPIO_33     0x00000002
#define GPIO_34     0x00000004
#define GPIO_35     0x00000008
#define GPIO_36     0x00000010
#define GPIO_37     0x00000020
#define GPIO_38     0x00000040
#define GPIO_39     0x00000080
#define GPIO_40     0x00000100
#define GPIO_41     0x00000200
#define GPIO_42     0x00000400
#define GPIO_43     0x00000800
#define GPIO_44     0x00001000
#define GPIO_45     0x00002000
#define GPIO_46     0x00004000
#define GPIO_47     0x00008000
#define GPIO_48     0x00010000
#define GPIO_49     0x00020000
#define GPIO_50     0x00040000
#define GPIO_51     0x00080000
#define GPIO_52     0x00100000
#define GPIO_53     0x00200000
#define GPIO_54     0x00400000
#define GPIO_55     0x00800000
#define GPIO_56     0x01000000
#define GPIO_57     0x02000000
#define GPIO_58     0x04000000
#define GPIO_59     0x08000000
#define GPIO_60     0x10000000
#define GPIO_61     0x20000000
#define GPIO_62     0x40000000
#define GPIO_63     0x80000000

// For use with GPIO registers GPLR_z, GPSR_z, GPCR_z, GPDR_z, GRER_z, GFER_z, GEDR_z
//
#define GPIO_64     0x00000001
#define GPIO_65     0x00000002
#define GPIO_66     0x00000004
#define GPIO_67     0x00000008
#define GPIO_68     0x00000010
#define GPIO_69     0x00000020
#define GPIO_70     0x00000040
#define GPIO_71     0x00000080
#define GPIO_72     0x00000100
#define GPIO_73     0x00000200
#define GPIO_74     0x00000400
#define GPIO_75     0x00000800
#define GPIO_76     0x00001000
#define GPIO_77     0x00002000
#define GPIO_78     0x00004000
#define GPIO_79     0x00008000
#define GPIO_80     0x00010000
//jawa01 start
#define GPIO_81     0x00020000
#define GPIO_82     0x00040000
#define GPIO_83     0x00080000
#define GPIO_84     0x00100000
#define GPIO_85     0x00200000
#define GPIO_86     0x00400000
#define GPIO_87     0x00800000
#define GPIO_88     0x01000000
#define GPIO_89     0x02000000
//jawa01 end 
// For use with GPIO register GAFR0_x
//
#define GPIO_0_AF1              0x00000001
#define GPIO_1_AF1_GP_RST       0x00000004
#define GPIO_2_AF1_ADC0         0x00000010
#define GPIO_3_AF1_ADC1         0x00000040
#define GPIO_4_AF1_ADC2         0x00000100
#define GPIO_5_AF1_ADC3         0x00000400
#define GPIO_6_AF1_MMCCLK       0x00001000
#define GPIO_7_AF1_48MHzClock   0x00004000
#define GPIO_8_AF1_MMCCS0       0x00010000
#define GPIO_9_AF1_MMCCS1       0x00040000
#define GPIO_10_AF1_RTCCLK      0x00100000
#define GPIO_11_AF1_3P6MHz      0x00400000
#define GPIO_12_AF1_32KHz       0x01000000
#define GPIO_13_AF1             0x04000000
#define GPIO_14_AF1_MBREQ       0x10000000
#define GPIO_15_AF1             0x40000000

// For use with GPIO register GAFR1_x
//
#define GPIO_16_AF1             0x00000001
#define GPIO_17_AF1             0x00000004
#define GPIO_18_AF1_RDY         0x00000010
#define GPIO_19_AF1_DREQ1       0x00000040
#define GPIO_20_AF1_DREQ0       0x00000100
#define GPIO_21_AF1             0x00000400
#define GPIO_22_AF1             0x00001000
#define GPIO_23_AF1             0x00004000
#define GPIO_24_AF1             0x00010000
#define GPIO_25_AF1             0x00040000
#define GPIO_26_AF1_RXD         0x00100000
#define GPIO_27_AF1_EXTCLK      0x00400000
#define GPIO_28_AF1_BITCLK      0x01000000
#define GPIO_29_AF1_SDATA_IN0   0x04000000
#define GPIO_30_AF1_SDATA_OUT   0x10000000
#define GPIO_31_AF1_SYNC        0x40000000

// For use with GPIO register GAFR0_y
//
#define GPIO_32_AF1_SDATA_IN1   0x00000001
#define GPIO_32_AF1_SYSCLK      0x00000001
#define GPIO_33_AF1             0x00000004
#define GPIO_34_AF1_FFRXD       0x00000010
#define GPIO_35_AF1_CTS         0x00000040
#define GPIO_36_AF1_DCD         0x00000100
#define GPIO_37_AF1_DSR         0x00000400
#define GPIO_38_AF1_RI          0x00001000
#define GPIO_39_AF1_ADC5        0x00004000
#define GPIO_39_AF1_MMCCS1      0x00004000
#define GPIO_40_AF1             0x00010000
#define GPIO_41_AF1             0x00040000
#define GPIO_42_AF1_BTRXD       0x00100000
#define GPIO_43_AF1_ADC6        0x00400000
#define GPIO_44_AF1_CTS         0x01000000
#define GPIO_45_AF1             0x04000000
#define GPIO_46_AF1_ICP_RXD     0x10000000
#define GPIO_47_AF1_TXD         0x40000000

// For use with GPIO register GAFR1_y
//
#define GPIO_48_AF1             0x00000001
#define GPIO_49_AF1             0x00000004
#define GPIO_50_AF1             0x00000010
#define GPIO_51_AF1             0x00000040
#define GPIO_52_AF1             0x00000100
#define GPIO_53_AF1_MMCCLK      0x00000400
#define GPIO_54_AF1_MMCCLK      0x00001000
#define GPIO_55_AF1             0x00004000
#define GPIO_56_AF1_nPWAIT      0x00010000
#define GPIO_57_AF1_nIOIS16     0x00040000
#define GPIO_58_AF1             0x00100000
#define GPIO_59_AF1             0x00400000
#define GPIO_60_AF1             0x01000000
#define GPIO_61_AF1             0x04000000
#define GPIO_62_AF1             0x10000000
#define GPIO_63_AF1             0x40000000

// For use with GPIO register GAFR0_z
//
#define GPIO_64_AF1             0x00000001
#define GPIO_65_AF1             0x00000004
#define GPIO_66_AF1_MBREQ       0x00000010
#define GPIO_67_AF1_MMCCS0      0x00000040
#define GPIO_68_AF1_MMCCS1      0x00000100
#define GPIO_69_AF1_MMCCLK      0x00000400
#define GPIO_70_AF1_RTCCLK      0x00001000
#define GPIO_71_AF1_3P6MHz      0x00004000
#define GPIO_72_AF1_32KHz       0x00010000
#define GPIO_73_AF1_ADC7        0x00040000
#define GPIO_73_AF1_MBGNT       0x00040000
#define GPIO_74_AF1             0x00100000
#define GPIO_75_AF1             0x00400000
#define GPIO_76_AF1             0x01000000
#define GPIO_77_AF1             0x04000000
#define GPIO_78_AF1             0x10000000
#define GPIO_79_AF1             0x40000000

// For use with GPIO register GAFR1_z
//
#define GPIO_80_AF1             0x00000001

// For use with GPIO register GAFR0_x
//
#define GPIO_0_AF2              0x00000002
#define GPIO_1_AF2              0x00000008
#define GPIO_2_AF2              0x00000020
#define GPIO_3_AF2              0x00000080
#define GPIO_4_AF2              0x00000200
#define GPIO_5_AF2              0x00000800
#define GPIO_6_AF2              0x00002000
#define GPIO_7_AF2              0x00008000
#define GPIO_8_AF2              0x00020000
#define GPIO_9_AF2              0x00080000
#define GPIO_10_AF2             0x00200000
#define GPIO_11_AF2             0x00800000
#define GPIO_12_AF2             0x02000000
#define GPIO_13_AF2_MBGNT       0x08000000
#define GPIO_14_AF2             0x20000000
#define GPIO_15_AF2_nCS_1       0x80000000

// For use with GPIO register GAFR1_x
//
#define GPIO_16_AF2_PWM0        0x00000002
#define GPIO_17_AF2_PWM1        0x00000008
#define GPIO_18_AF2             0x00000020
#define GPIO_19_AF2             0x00000080
#define GPIO_20_AF2             0x00000200
#define GPIO_21_AF2_DVAL0       0x00000800
#define GPIO_22_AF2_DVAL1       0x00002000
#define GPIO_23_AF2_SCLK        0x00008000
#define GPIO_24_AF2_SFRM        0x00020000
#define GPIO_25_AF2_TXD         0x00080000
#define GPIO_26_AF2             0x00200000
#define GPIO_27_AF2             0x00800000
#define GPIO_28_AF2_BITCLK      0x02000000
#define GPIO_29_AF2_SDATA_IN    0x08000000
#define GPIO_30_AF2_SDATA_OUT   0x20000000
#define GPIO_31_AF2_SYNC        0x80000000

// For use with GPIO register GAFR0_y
//
#define GPIO_32_AF2             0x00000002
#define GPIO_33_AF2_nCS5        0x00000008
#define GPIO_34_AF2_ADC4        0x00000020
#define GPIO_34_AF2_MMCCS0      0x00000020
#define GPIO_35_AF2             0x00000080
#define GPIO_36_AF2             0x00000200
#define GPIO_37_AF2             0x00000800
#define GPIO_38_AF2             0x00002000
#define GPIO_39_AF2_FFTXD       0x00008000
#define GPIO_40_AF2_DTR         0x00020000
#define GPIO_41_AF2_RTS         0x00080000
#define GPIO_42_AF2             0x00200000
#define GPIO_43_AF2_BTTXD       0x00800000
#define GPIO_44_AF2             0x02000000
#define GPIO_45_AF2_RTS         0x08000000
#define GPIO_46_AF2_RXD         0x20000000
#define GPIO_47_AF2_ICP_TXD     0x80000000

// For use with GPIO register GAFR1_y
//
#define GPIO_48_AF2_nPOE        0x00000002
#define GPIO_49_AF2_nPWE        0x00000008
#define GPIO_50_AF2_nPIOR       0x00000020
#define GPIO_51_AF2_nPIOW       0x00000080
#define GPIO_52_AF2_nPCE1       0x00000200
#define GPIO_53_AF2_nPCE2       0x00000800
#define GPIO_54_AF2_pSKTSEL     0x00002000
#define GPIO_55_AF2_nPREG       0x00008000
#define GPIO_56_AF2             0x00020000
#define GPIO_57_AF2             0x00080000
#define GPIO_58_AF2_LDD0        0x00200000
#define GPIO_59_AF2_LDD1        0x00800000
#define GPIO_60_AF2_LDD2        0x02000000
#define GPIO_61_AF2_LDD3        0x08000000
#define GPIO_62_AF2_LDD4        0x20000000
#define GPIO_63_AF2_LDD5        0x80000000

// For use with GPIO register GAFR0_z
//
#define GPIO_64_AF2_LDD6        0x00000002
#define GPIO_65_AF2_LDD7        0x00000008
#define GPIO_66_AF2_LDD8        0x00000020
#define GPIO_67_AF2_LDD9        0x00000080
#define GPIO_68_AF2_LDD10       0x00000200
#define GPIO_69_AF2_LDD11       0x00000800
#define GPIO_70_AF2_LDD12       0x00002000
#define GPIO_71_AF2_LDD13       0x00008000
#define GPIO_72_AF2_LDD14       0x00020000
#define GPIO_73_AF2_LDD15       0x00080000
#define GPIO_74_AF2_LCD_FCLK    0x00200000
#define GPIO_75_AF2_LCD_LCLK    0x00800000
#define GPIO_76_AF2_LCD_PCLK    0x02000000
#define GPIO_77_AF2_LCD_ACBIAS  0x08000000
#define GPIO_78_AF2_nCS2        0x20000000
#define GPIO_79_AF2_nCS3        0x80000000

// For use with GPIO register GAFR1_z
//
#define GPIO_80_AF2_nCS4        0x00000002

// For use with GPIO register GAFR0_x
//
#define GPIO_0_AF3  0x00000003
#define GPIO_1_AF3  0x0000000C
#define GPIO_2_AF3  0x00000030
#define GPIO_3_AF3  0x000000C0
#define GPIO_4_AF3  0x00000300
#define GPIO_5_AF3  0x00000C00
#define GPIO_6_AF3  0x00003000
#define GPIO_7_AF3  0x0000C000
#define GPIO_8_AF3  0x00030000
#define GPIO_9_AF3  0x000C0000
#define GPIO_10_AF3 0x00300000
#define GPIO_11_AF3 0x00C00000
#define GPIO_12_AF3 0x03000000
#define GPIO_13_AF3 0x0C000000
#define GPIO_14_AF3 0x30000000
#define GPIO_15_AF3 0xC0000000

// For use with GPIO register GAFR1_x
//
#define GPIO_16_AF3 0x00000003
#define GPIO_17_AF3 0x0000000C
#define GPIO_18_AF3 0x00000030
#define GPIO_19_AF3 0x000000C0
#define GPIO_20_AF3 0x00000300
#define GPIO_21_AF3 0x00000C00
#define GPIO_22_AF3 0x00003000
#define GPIO_23_AF3 0x0000C000
#define GPIO_24_AF3 0x00030000
#define GPIO_25_AF3 0x000C0000
#define GPIO_26_AF3 0x00300000
#define GPIO_27_AF3 0x00C00000
#define GPIO_28_AF3 0x03000000
#define GPIO_29_AF3 0x0C000000
#define GPIO_30_AF3 0x30000000
#define GPIO_31_AF3 0xC0000000

// For use with GPIO register GAFR0_y
//
#define GPIO_32_AF3 0x00000003
#define GPIO_33_AF3 0x0000000C
#define GPIO_34_AF3 0x00000030
#define GPIO_35_AF3 0x000000C0
#define GPIO_36_AF3 0x00000300
#define GPIO_37_AF3 0x00000C00
#define GPIO_38_AF3 0x00003000
#define GPIO_39_AF3 0x0000C000
#define GPIO_40_AF3 0x00030000
#define GPIO_41_AF3 0x000C0000
#define GPIO_42_AF3 0x00300000
#define GPIO_43_AF3 0x00C00000
#define GPIO_44_AF3 0x03000000
#define GPIO_45_AF3 0x0C000000
#define GPIO_46_AF3 0x30000000
#define GPIO_47_AF3 0xC0000000

// For use with GPIO register GAFR1_y
//
#define GPIO_48_AF3 0x00000003
#define GPIO_49_AF3 0x0000000C
#define GPIO_50_AF3 0x00000030
#define GPIO_51_AF3 0x000000C0
#define GPIO_52_AF3 0x00000300
#define GPIO_53_AF3 0x00000C00
#define GPIO_54_AF3 0x00003000
#define GPIO_55_AF3 0x0000C000
#define GPIO_56_AF3 0x00030000
#define GPIO_57_AF3 0x000C0000
#define GPIO_58_AF3 0x00300000
#define GPIO_59_AF3 0x00C00000
#define GPIO_60_AF3 0x03000000
#define GPIO_61_AF3 0x0C000000
#define GPIO_62_AF3 0x30000000
#define GPIO_63_AF3 0xC0000000

// For use with GPIO register GAFR0_z
//
#define GPIO_64_AF3 0x00000003
#define GPIO_65_AF3 0x0000000C
#define GPIO_66_AF3 0x00000030
#define GPIO_67_AF3 0x000000C0
#define GPIO_68_AF3 0x00000300
#define GPIO_69_AF3 0x00000C00
#define GPIO_70_AF3 0x00003000
#define GPIO_71_AF3 0x0000C000
#define GPIO_72_AF3 0x00030000
#define GPIO_73_AF3 0x000C0000
#define GPIO_74_AF3 0x00300000
#define GPIO_75_AF3 0x00C00000
#define GPIO_76_AF3 0x03000000
#define GPIO_77_AF3 0x0C000000
#define GPIO_78_AF3 0x30000000
#define GPIO_79_AF3 0xC0000000

// For use with GPIO register GAFR1_z
//
#define GPIO_80_AF3 0x00000003

// GEDR bits - write to clear
#define GEDR_GPIO0_EDGE_CLR(reg)        (reg |= GPIO_0)
#define GEDR_GPIO1_EDGE_CLR(reg)        (reg |= GPIO_1)
//----------------------------------------------------------------

//#define GPIO OUTPUT LEVEL FOR FALCON SYSTEM DEFINE MACRO
//GPxR_X
#define  GPIO0_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_0)
#define  GPIO0_SET_HI            (pGPIOReg->GPSR_x |= GPIO_0)
#define  GPIO1_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_1)
#define  GPIO1_SET_HI            (pGPIOReg->GPSR_x |= GPIO_1)
#define  GPIO2_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_2)
#define  GPIO2_SET_HI            (pGPIOReg->GPSR_x |= GPIO_2)
#define  GPIO3_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_3)
#define  GPIO3_SET_HI            (pGPIOReg->GPSR_x |= GPIO_3)
#define  GPIO4_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_4)
#define  GPIO4_SET_HI            (pGPIOReg->GPSR_x |= GPIO_4)
#define  GPIO5_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_5)
#define  GPIO5_SET_HI            (pGPIOReg->GPSR_x |= GPIO_5)
#define  GPIO6_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_6)
#define  GPIO6_SET_HI            (pGPIOReg->GPSR_x |= GPIO_6)
#define  GPIO7_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_7)
#define  GPIO7_SET_HI            (pGPIOReg->GPSR_x |= GPIO_7)
#define  GPIO8_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_8)
#define  GPIO8_SET_HI            (pGPIOReg->GPSR_x |= GPIO_8)
#define  GPIO9_SET_LOW           (pGPIOReg->GPCR_x |= GPIO_9)
#define  GPIO9_SET_HI            (pGPIOReg->GPSR_x |= GPIO_9)
#define  GPIO10_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_10)
#define  GPIO10_SET_HI           (pGPIOReg->GPSR_x |= GPIO_10)
#define  GPIO11_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_11)
#define  GPIO11_SET_HI           (pGPIOReg->GPSR_x |= GPIO_11)
#define  GPIO12_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_12)
#define  GPIO12_SET_HI           (pGPIOReg->GPSR_x |= GPIO_12)
#define  GPIO13_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_13)
#define  GPIO13_SET_HI           (pGPIOReg->GPSR_x |= GPIO_13)
#define  GPIO14_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_14)
#define  GPIO14_SET_HI           (pGPIOReg->GPSR_x |= GPIO_14)
#define  GPIO15_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_15)
#define  GPIO15_SET_HI           (pGPIOReg->GPSR_x |= GPIO_15)
#define  GPIO16_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_16)
#define  GPIO16_SET_HI           (pGPIOReg->GPSR_x |= GPIO_16)
#define  GPIO17_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_17)
#define  GPIO17_SET_HI           (pGPIOReg->GPSR_x |= GPIO_17)
#define  GPIO18_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_18)
#define  GPIO18_SET_HI           (pGPIOReg->GPSR_x |= GPIO_18)
#define  GPIO19_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_19)
#define  GPIO19_SET_HI           (pGPIOReg->GPSR_x |= GPIO_19)
#define  GPIO20_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_20)
#define  GPIO20_SET_HI           (pGPIOReg->GPSR_x |= GPIO_20)
#define  GPIO21_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_21)
#define  GPIO21_SET_HI           (pGPIOReg->GPSR_x |= GPIO_21)
#define  GPIO22_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_22)
#define  GPIO22_SET_HI           (pGPIOReg->GPSR_x |= GPIO_22)
#define  GPIO23_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_23)
#define  GPIO23_SET_HI           (pGPIOReg->GPSR_x |= GPIO_23)
#define  GPIO24_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_24)
#define  GPIO24_SET_HI           (pGPIOReg->GPSR_x |= GPIO_24)
#define  GPIO25_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_25)
#define  GPIO25_SET_HI           (pGPIOReg->GPSR_x |= GPIO_25)
#define  GPIO26_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_26)
#define  GPIO26_SET_HI           (pGPIOReg->GPSR_x |= GPIO_26)
#define  GPIO27_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_27)
#define  GPIO27_SET_HI           (pGPIOReg->GPSR_x |= GPIO_27)
#define  GPIO28_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_28)
#define  GPIO28_SET_HI           (pGPIOReg->GPSR_x |= GPIO_28)
#define  GPIO29_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_29)
#define  GPIO29_SET_HI           (pGPIOReg->GPSR_x |= GPIO_29)
#define  GPIO30_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_30)
#define  GPIO30_SET_HI           (pGPIOReg->GPSR_x |= GPIO_30)
#define  GPIO31_SET_LOW          (pGPIOReg->GPCR_x |= GPIO_31)
#define  GPIO31_SET_HI           (pGPIOReg->GPSR_x |= GPIO_31)
//GPxR_Y
#define  GPIO32_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_32)
#define  GPIO32_SET_HI           (pGPIOReg->GPSR_y |= GPIO_32)
#define  GPIO33_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_33)
#define  GPIO33_SET_HI           (pGPIOReg->GPSR_y |= GPIO_33)
#define  GPIO34_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_34)
#define  GPIO34_SET_HI           (pGPIOReg->GPSR_y |= GPIO_34)
#define  GPIO35_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_35)
#define  GPIO35_SET_HI           (pGPIOReg->GPSR_y |= GPIO_35)
#define  GPIO36_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_36)
#define  GPIO36_SET_HI           (pGPIOReg->GPSR_y |= GPIO_36)
#define  GPIO37_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_37)
#define  GPIO37_SET_HI           (pGPIOReg->GPSR_y |= GPIO_37)
#define  GPIO38_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_38)
#define  GPIO38_SET_HI           (pGPIOReg->GPSR_y |= GPIO_38)
#define  GPIO39_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_39)
#define  GPIO39_SET_HI           (pGPIOReg->GPSR_y |= GPIO_39)
#define  GPIO40_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_40)
#define  GPIO40_SET_HI           (pGPIOReg->GPSR_y |= GPIO_40)
#define  GPIO41_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_41)
#define  GPIO41_SET_HI           (pGPIOReg->GPSR_y |= GPIO_41)
#define  GPIO42_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_42)
#define  GPIO42_SET_HI           (pGPIOReg->GPSR_y |= GPIO_42)
#define  GPIO43_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_43)
#define  GPIO43_SET_HI           (pGPIOReg->GPSR_y |= GPIO_43)
#define  GPIO44_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_44)
#define  GPIO44_SET_HI           (pGPIOReg->GPSR_y |= GPIO_44)
#define  GPIO45_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_45)
#define  GPIO45_SET_HI           (pGPIOReg->GPSR_y |= GPIO_45)
#define  GPIO46_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_46)
#define  GPIO46_SET_HI           (pGPIOReg->GPSR_y |= GPIO_46)
#define  GPIO47_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_47)
#define  GPIO47_SET_HI           (pGPIOReg->GPSR_y |= GPIO_47)
#define  GPIO48_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_48)
#define  GPIO48_SET_HI           (pGPIOReg->GPSR_y |= GPIO_48)
#define  GPIO49_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_49)
#define  GPIO49_SET_HI           (pGPIOReg->GPSR_y |= GPIO_49)
#define  GPIO50_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_50)
#define  GPIO50_SET_HI           (pGPIOReg->GPSR_y |= GPIO_50)
#define  GPIO51_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_51)
#define  GPIO51_SET_HI           (pGPIOReg->GPSR_y |= GPIO_51)
#define  GPIO52_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_52)
#define  GPIO52_SET_HI           (pGPIOReg->GPSR_y |= GPIO_52)
#define  GPIO53_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_53)
#define  GPIO53_SET_HI           (pGPIOReg->GPSR_y |= GPIO_53)
#define  GPIO54_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_54)
#define  GPIO54_SET_HI           (pGPIOReg->GPSR_y |= GPIO_54)
#define  GPIO55_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_55)
#define  GPIO55_SET_HI           (pGPIOReg->GPSR_y |= GPIO_55)
#define  GPIO56_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_56)
#define  GPIO56_SET_HI           (pGPIOReg->GPSR_y |= GPIO_56)
#define  GPIO57_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_57)
#define  GPIO57_SET_HI           (pGPIOReg->GPSR_y |= GPIO_57)
#define  GPIO58_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_58)
#define  GPIO58_SET_HI           (pGPIOReg->GPSR_y |= GPIO_58)
#define  GPIO59_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_59)
#define  GPIO59_SET_HI           (pGPIOReg->GPSR_y |= GPIO_59)
#define  GPIO60_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_60)
#define  GPIO60_SET_HI           (pGPIOReg->GPSR_y |= GPIO_60)
#define  GPIO61_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_61)
#define  GPIO61_SET_HI           (pGPIOReg->GPSR_y |= GPIO_61)
#define  GPIO62_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_62)
#define  GPIO62_SET_HI           (pGPIOReg->GPSR_y |= GPIO_62)
#define  GPIO63_SET_LOW          (pGPIOReg->GPCR_y |= GPIO_63)
#define  GPIO63_SET_HI           (pGPIOReg->GPSR_y |= GPIO_63)
//GPxR_Z
#define  GPIO64_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_64)
#define  GPIO64_SET_HI           (pGPIOReg->GPSR_z |= GPIO_64)
#define  GPIO65_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_65)
#define  GPIO65_SET_HI           (pGPIOReg->GPSR_z |= GPIO_65)
#define  GPIO66_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_66)
#define  GPIO66_SET_HI           (pGPIOReg->GPSR_z |= GPIO_66)
#define  GPIO67_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_67)
#define  GPIO67_SET_HI           (pGPIOReg->GPSR_z |= GPIO_67)
#define  GPIO68_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_68)
#define  GPIO68_SET_HI           (pGPIOReg->GPSR_z |= GPIO_68)
#define  GPIO69_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_69)
#define  GPIO69_SET_HI           (pGPIOReg->GPSR_z |= GPIO_69)
#define  GPIO70_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_70)
#define  GPIO70_SET_HI           (pGPIOReg->GPSR_z |= GPIO_70)
#define  GPIO71_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_71)
#define  GPIO71_SET_HI           (pGPIOReg->GPSR_z |= GPIO_71)
#define  GPIO72_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_72)
#define  GPIO72_SET_HI           (pGPIOReg->GPSR_z |= GPIO_72)
#define  GPIO73_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_73)
#define  GPIO73_SET_HI           (pGPIOReg->GPSR_z |= GPIO_73)
#define  GPIO74_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_74)
#define  GPIO74_SET_HI           (pGPIOReg->GPSR_z |= GPIO_74)
#define  GPIO75_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_75)
#define  GPIO75_SET_HI           (pGPIOReg->GPSR_z |= GPIO_75)
#define  GPIO76_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_76)
#define  GPIO76_SET_HI           (pGPIOReg->GPSR_z |= GPIO_76)
#define  GPIO77_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_77)
#define  GPIO77_SET_HI           (pGPIOReg->GPSR_z |= GPIO_77)
#define  GPIO78_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_78)
#define  GPIO78_SET_HI           (pGPIOReg->GPSR_z |= GPIO_78)
#define  GPIO79_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_79)
#define  GPIO79_SET_HI           (pGPIOReg->GPSR_z |= GPIO_79)
#define  GPIO80_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_80)
#define  GPIO80_SET_HI           (pGPIOReg->GPSR_z |= GPIO_80)
//jawa01 start
#define  GPIO81_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_81)
#define  GPIO81_SET_HI           (pGPIOReg->GPSR_z |= GPIO_81)
#define  GPIO82_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_82)
#define  GPIO82_SET_HI           (pGPIOReg->GPSR_z |= GPIO_82)
#define  GPIO83_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_83)
#define  GPIO83_SET_HI           (pGPIOReg->GPSR_z |= GPIO_83)
#define  GPIO84_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_84)
#define  GPIO84_SET_HI           (pGPIOReg->GPSR_z |= GPIO_84)
#define  GPIO85_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_85)
#define  GPIO85_SET_HI           (pGPIOReg->GPSR_z |= GPIO_85)
#define  GPIO86_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_86)
#define  GPIO86_SET_HI           (pGPIOReg->GPSR_z |= GPIO_86)
#define  GPIO87_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_87)
#define  GPIO87_SET_HI           (pGPIOReg->GPSR_z |= GPIO_87)
#define  GPIO88_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_88)
#define  GPIO88_SET_HI           (pGPIOReg->GPSR_z |= GPIO_88)
#define  GPIO89_SET_LOW          (pGPIOReg->GPCR_z |= GPIO_89)
#define  GPIO89_SET_HI           (pGPIOReg->GPSR_z |= GPIO_89)
//jawa02 end 
//================================================================================
//#define GPIO INTPUT LEVEL FOR FALCON  SYSTEM DEFINE MACRO
//GPLR_X
#define  GPIO0_LEVEL            (pGPIOReg->GPLR_x & GPIO_0)
#define  GPIO1_LEVEL            (pGPIOReg->GPLR_x & GPIO_1)
#define  GPIO2_LEVEL            (pGPIOReg->GPLR_x & GPIO_2)
#define  GPIO3_LEVEL            (pGPIOReg->GPLR_x & GPIO_3)
#define  GPIO4_LEVEL            (pGPIOReg->GPLR_x & GPIO_4)
#define  GPIO5_LEVEL            (pGPIOReg->GPLR_x & GPIO_5)
#define  GPIO6_LEVEL            (pGPIOReg->GPLR_x & GPIO_6)
#define  GPIO7_LEVEL            (pGPIOReg->GPLR_x & GPIO_7)
#define  GPIO8_LEVEL            (pGPIOReg->GPLR_x & GPIO_8)
#define  GPIO9_LEVEL            (pGPIOReg->GPLR_x & GPIO_9)
#define  GPIO10_LEVEL           (pGPIOReg->GPLR_x & GPIO_10)
#define  GPIO11_LEVEL           (pGPIOReg->GPLR_x & GPIO_11)
#define  GPIO12_LEVEL           (pGPIOReg->GPLR_x & GPIO_12)
#define  GPIO13_LEVEL           (pGPIOReg->GPLR_x & GPIO_13)
#define  GPIO14_LEVEL           (pGPIOReg->GPLR_x & GPIO_14)
#define  GPIO15_LEVEL           (pGPIOReg->GPLR_x & GPIO_15)
#define  GPIO16_LEVEL           (pGPIOReg->GPLR_x & GPIO_16)
#define  GPIO17_LEVEL           (pGPIOReg->GPLR_x & GPIO_17)
#define  GPIO18_LEVEL           (pGPIOReg->GPLR_x & GPIO_18)
#define  GPIO19_LEVEL           (pGPIOReg->GPLR_x & GPIO_19)
#define  GPIO20_LEVEL           (pGPIOReg->GPLR_x & GPIO_20)
#define  GPIO21_LEVEL           (pGPIOReg->GPLR_x & GPIO_21)
#define  GPIO22_LEVEL           (pGPIOReg->GPLR_x & GPIO_22)
#define  GPIO23_LEVEL           (pGPIOReg->GPLR_x & GPIO_23)
#define  GPIO24_LEVEL           (pGPIOReg->GPLR_x & GPIO_24)
#define  GPIO25_LEVEL           (pGPIOReg->GPLR_x & GPIO_25)
#define  GPIO26_LEVEL           (pGPIOReg->GPLR_x & GPIO_26)
#define  GPIO27_LEVEL           (pGPIOReg->GPLR_x & GPIO_27)
#define  GPIO28_LEVEL           (pGPIOReg->GPLR_x & GPIO_28)
#define  GPIO29_LEVEL           (pGPIOReg->GPLR_x & GPIO_29)
#define  GPIO30_LEVEL           (pGPIOReg->GPLR_x & GPIO_30)
#define  GPIO31_LEVEL           (pGPIOReg->GPLR_x & GPIO_31)
//GPLR_Y
#define  GPIO32_LEVEL           (pGPIOReg->GPLR_y & GPIO_32)
#define  GPIO33_LEVEL           (pGPIOReg->GPLR_y & GPIO_33)
#define  GPIO34_LEVEL           (pGPIOReg->GPLR_y & GPIO_34)
#define  GPIO35_LEVEL           (pGPIOReg->GPLR_y & GPIO_35)
#define  GPIO36_LEVEL           (pGPIOReg->GPLR_y & GPIO_36)
#define  GPIO37_LEVEL           (pGPIOReg->GPLR_y & GPIO_37)
#define  GPIO38_LEVEL           (pGPIOReg->GPLR_y & GPIO_38)
#define  GPIO39_LEVEL           (pGPIOReg->GPLR_y & GPIO_39)
#define  GPIO40_LEVEL           (pGPIOReg->GPLR_y & GPIO_40)
#define  GPIO41_LEVEL           (pGPIOReg->GPLR_y & GPIO_41)
#define  GPIO42_LEVEL           (pGPIOReg->GPLR_y & GPIO_42)
#define  GPIO43_LEVEL           (pGPIOReg->GPLR_y & GPIO_43)
#define  GPIO44_LEVEL           (pGPIOReg->GPLR_y & GPIO_44)
#define  GPIO45_LEVEL           (pGPIOReg->GPLR_y & GPIO_45)
#define  GPIO46_LEVEL           (pGPIOReg->GPLR_y & GPIO_46)
#define  GPIO47_LEVEL           (pGPIOReg->GPLR_y & GPIO_47)
#define  GPIO48_LEVEL           (pGPIOReg->GPLR_y & GPIO_48)
#define  GPIO49_LEVEL           (pGPIOReg->GPLR_y & GPIO_49)
#define  GPIO50_LEVEL           (pGPIOReg->GPLR_y & GPIO_50)
#define  GPIO51_LEVEL           (pGPIOReg->GPLR_y & GPIO_51)
#define  GPIO52_LEVEL           (pGPIOReg->GPLR_y & GPIO_52)
#define  GPIO53_LEVEL           (pGPIOReg->GPLR_y & GPIO_53)
#define  GPIO54_LEVEL           (pGPIOReg->GPLR_y & GPIO_54)
#define  GPIO55_LEVEL           (pGPIOReg->GPLR_y & GPIO_55)
#define  GPIO56_LEVEL           (pGPIOReg->GPLR_y & GPIO_56)
#define  GPIO57_LEVEL           (pGPIOReg->GPLR_y & GPIO_57)
#define  GPIO58_LEVEL           (pGPIOReg->GPLR_y & GPIO_58)
#define  GPIO59_LEVEL           (pGPIOReg->GPLR_y & GPIO_59)
#define  GPIO60_LEVEL           (pGPIOReg->GPLR_y & GPIO_60)
#define  GPIO61_LEVEL           (pGPIOReg->GPLR_y & GPIO_61)
#define  GPIO62_LEVEL           (pGPIOReg->GPLR_y & GPIO_62)
#define  GPIO63_LEVEL           (pGPIOReg->GPLR_y & GPIO_63)
//GPLR_Z
#define  GPIO64_LEVEL           (pGPIOReg->GPLR_z & GPIO_64)
#define  GPIO65_LEVEL           (pGPIOReg->GPLR_z & GPIO_65)
#define  GPIO66_LEVEL           (pGPIOReg->GPLR_z & GPIO_66)
#define  GPIO67_LEVEL           (pGPIOReg->GPLR_z & GPIO_67)
#define  GPIO68_LEVEL           (pGPIOReg->GPLR_z & GPIO_68)
#define  GPIO69_LEVEL           (pGPIOReg->GPLR_z & GPIO_69)
#define  GPIO70_LEVEL           (pGPIOReg->GPLR_z & GPIO_70)
#define  GPIO71_LEVEL           (pGPIOReg->GPLR_z & GPIO_71)
#define  GPIO72_LEVEL           (pGPIOReg->GPLR_z & GPIO_72)
#define  GPIO73_LEVEL           (pGPIOReg->GPLR_z & GPIO_73)
#define  GPIO74_LEVEL           (pGPIOReg->GPLR_z & GPIO_74)
#define  GPIO75_LEVEL           (pGPIOReg->GPLR_z & GPIO_75)
#define  GPIO76_LEVEL           (pGPIOReg->GPLR_z & GPIO_76)
#define  GPIO77_LEVEL           (pGPIOReg->GPLR_z & GPIO_77)
#define  GPIO78_LEVEL           (pGPIOReg->GPLR_z & GPIO_78)
#define  GPIO79_LEVEL           (pGPIOReg->GPLR_z & GPIO_79)
#define  GPIO80_LEVEL           (pGPIOReg->GPLR_z & GPIO_80)

//jawa01 start
#define  GPIO81_LEVEL           (pGPIOReg->GPLR_z & GPIO_81)
#define  GPIO82_LEVEL           (pGPIOReg->GPLR_z & GPIO_82)
#define  GPIO83_LEVEL           (pGPIOReg->GPLR_z & GPIO_83)
#define  GPIO84_LEVEL           (pGPIOReg->GPLR_z & GPIO_84)
#define  GPIO85_LEVEL           (pGPIOReg->GPLR_z & GPIO_85)
#define  GPIO86_LEVEL           (pGPIOReg->GPLR_z & GPIO_86)
#define  GPIO87_LEVEL           (pGPIOReg->GPLR_z & GPIO_87)
#define  GPIO88_LEVEL           (pGPIOReg->GPLR_z & GPIO_88)
#define  GPIO89_LEVEL           (pGPIOReg->GPLR_z & GPIO_89)
//jawa01 end
//================================================================================
//define interrupt

#define  GPIO0_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_0)  
#define  GPIO0_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_0)  
#define  GPIO0_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_0)  
#define  GPIO0_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_0)  
#define  GPIO0_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_0)  
#define  GPIO0_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_0)  
#define  GPIO0_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_0)  
#define  GPIO0_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_0)  

#define  GPIO1_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_1)  
#define  GPIO1_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_1)  
#define  GPIO1_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_1)  
#define  GPIO1_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_1)  
#define  GPIO1_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_1)  
#define  GPIO1_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_1)  
#define  GPIO1_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_1)  
#define  GPIO1_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_1)  
#define  GPIO1_PIN_GPIO                   (pGPIOReg->GAFR0_x &= ~(GPIO_1_AF3))    //andy02

#define  GPIO2_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_2)  
#define  GPIO2_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_2)  
#define  GPIO2_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_2)  
#define  GPIO2_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_2)  
#define  GPIO2_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_2)  
#define  GPIO2_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_2)  
#define  GPIO2_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_2)  
#define  GPIO2_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_2)  

#define  GPIO3_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_3)  
#define  GPIO3_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_3)  
#define  GPIO3_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_3)  
#define  GPIO3_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_3)  
#define  GPIO3_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_3)  
#define  GPIO3_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_3)  
#define  GPIO3_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_3)  
#define  GPIO3_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_3)  

#define  GPIO4_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_4)  
#define  GPIO4_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_4)  
#define  GPIO4_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_4)  
#define  GPIO4_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_4)  
#define  GPIO4_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_4)  
#define  GPIO4_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_4)  
#define  GPIO4_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_4)  
#define  GPIO4_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_4)  
#define  GPIO4_DIRECTION_CLEAR            (pGPIOReg->GPDR_x &=~GPIO_4)  // pin 4 input //yy01
//yy01 start: for SDMMC_CS0 pin
#define  GPIO8_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_8)  
#define  GPIO8_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_8)  
#define  GPIO8_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_8)  
#define  GPIO8_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_8)  
#define  GPIO8_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_8)  
#define  GPIO8_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_8)  
#define  GPIO8_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_8)  
#define  GPIO8_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_8)  
#define  GPIO8_DIRECTION_SET              (pGPIOReg->GPDR_x |=GPIO_8)                   // output
#define  GPIO8_DIRECTION_CLEAR            (pGPIOReg->GPDR_x &=~GPIO_8)                  // input
#define  GPIO8_AF1                        (pGPIOReg->GAFR0_x |= GPIO_8_AF1_MMCCS0)      // input
#define  GPIO8_AF_CLEAR                   (pGPIOReg->GAFR0_x &= ~GPIO_8_AF1_MMCCS0)     // input
//yy01 end: for SDMMC_CS0 pin

/// span0704 start
#define  GPIO12_AcIn_SET_OUTPUT                 (pGPIOReg->GPDR_x |=  GPIO_12)  
#define  GPIO12_AcIn_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_12)  
#define  GPIO12_AcIn_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_12)  
#define  GPIO12_AcIn_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_12)  
#define  GPIO12_AcIn_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_12)  
#define  GPIO12_AcIn_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_12)  
#define  GPIO12_AcIn_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |=  GPIO_12)  
#define  GPIO12_AcIn_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x &   GPIO_12)  
#define  GPIO12_AcIn_DIRECTION_SET              (pGPIOReg->GPDR_x |=  GPIO_12)                  // output
#define  GPIO12_AcIn_DIRECTION_CLEAR            (pGPIOReg->GPDR_x &= ~GPIO_12)                  // input
#define  GPIO12_AcIn_LEVEL                      GPIO12_LEVEL
#define  GPIO12_AcIn_ALT_GPIO                   (pGPIOReg->GAFR0_x &= ~(GPIO_12_AF3))
/// span0704 end
#define  GPIO18_SET_INPUT                  (pGPIOReg->GPDR_x &= ~GPIO_18)               //RDY In
#define  GPIO18_AF1                        (pGPIOReg->GAFR1_x |= GPIO_18_AF1_RDY)      // RDY
#define  GPIO18_AF_CLEAR                   (pGPIOReg->GAFR1_x &= ~GPIO_18_AF1_RDY)     // RDY
/* wei01 start, add for vibrator */
#define  GPIO61_SET_OUTPUT                 (pGPIOReg->GPDR_y |=  GPIO_61)  
#define  GPIO61_SET_INPUT                  (pGPIOReg->GPDR_y &= ~GPIO_61)  
#define  GPIO_VIBRATOR_ON                  GPIO61_SET_LOW
#define  GPIO_VIBRATOR_OFF                 GPIO61_SET_HI
#define  GPIO_VIBRATOR_SET_OUTPUT          GPIO61_SET_OUTPUT
/* wei01 end */

//Xy48 start:
#define  GPIO13_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_13)  
#define  GPIO13_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_13)  
#define  GPIO13_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_13)  
#define  GPIO13_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_13) 
#define  GPIO13_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |= GPIO_13)  
#define  GPIO13_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x & GPIO_13)
//Xy48 end.


//Xy01 start: add for phone jack detection
#define  GPIO14_FALLING_EDGE_SET           (pGPIOReg->GFER_x |=  GPIO_14)  
#define  GPIO14_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_x &= ~GPIO_14)  
#define  GPIO14_RISING_EDGE_SET            (pGPIOReg->GRER_x |=  GPIO_14)  
#define  GPIO14_RISING_EDGE_CLEAR          (pGPIOReg->GRER_x &= ~GPIO_14) 
#define  GPIO14_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_x |= GPIO_14)  
#define  GPIO14_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_x & GPIO_14)
//Xy01 end

//Span0128 Start
#define  GPIO57_SET_INPUT                  (pGPIOReg->GPDR_y &= ~GPIO_57)  //jawadebug 
#define  GPIO57_SET_GPIO                   (pGPIOReg->GAFR1_y &= ~(GPIO_57_AF3))  //jawadebug 
#define  GPIO57_FALLING_EDGE_SET           (pGPIOReg->GFER_y |=  GPIO_57)  
#define  GPIO57_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_y &= ~GPIO_57)  
#define  GPIO57_RISING_EDGE_SET            (pGPIOReg->GRER_y |=  GPIO_57)  
#define  GPIO57_RISING_EDGE_CLEAR          (pGPIOReg->GRER_y &= ~GPIO_57) 
#define  GPIO57_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_y |= GPIO_57)  
#define  GPIO57_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_y & GPIO_57) 
//Span0128 End

//Xy03 start:
#define  GPIO63_FALLING_EDGE_SET           (pGPIOReg->GFER_y |=  GPIO_63)  
#define  GPIO63_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_y &= ~GPIO_63)  
#define  GPIO63_RISING_EDGE_SET            (pGPIOReg->GRER_y |=  GPIO_63)  
#define  GPIO63_RISING_EDGE_CLEAR          (pGPIOReg->GRER_y &= ~GPIO_63) 
#define  GPIO63_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_y |= GPIO_63)  
#define  GPIO63_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_y & GPIO_63) 
//Xy03 end
#define  GPIO65_PIN_OUT                    (pGPIOReg->GPDR_z |= GPIO_65)  //jawadebug 
#define  GPIO65_PIN_GPIO                   (pGPIOReg->GAFR0_z &= ~(GPIO_65_AF3))  //jawadebug 
//andy01 start: add for hall_sw starts (use GPIO67)
#define  GPIO67_FALLING_EDGE_SET           (pGPIOReg->GFER_z |=  GPIO_67)  
#define  GPIO67_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_z &= ~GPIO_67)  
#define  GPIO67_RISING_EDGE_SET            (pGPIOReg->GRER_z |=  GPIO_67)  
#define  GPIO67_RISING_EDGE_CLEAR          (pGPIOReg->GRER_z &= ~GPIO_67) 
#define  GPIO67_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_z |= GPIO_67)  
#define  GPIO67_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_z & GPIO_67)  
#define  GPIO67_PIN_GPIO                   (pGPIOReg->GAFR0_z &= ~(GPIO_67_AF3))  
//andy01 end: add for hall_sw ends (use GPIO67)

#define  GPIO73_SET_OUTPUT                 (pGPIOReg->GPDR_z |= GPIO_73)    // Ryn01

//sun07 start: for SD_WP pin
#define  GPIO69_SET_OUTPUT                 (pGPIOReg->GPDR_z |=  GPIO_69)  
#define  GPIO69_SET_INPUT                  (pGPIOReg->GPDR_z &= ~GPIO_69)  
//sun07 end: for SD_WP pin



/* ----------------------------------------------------------------------------
    Keypad
*/
// Pat01 start
#define KPAD_SCAN_X         (GPIO_23|GPIO_24|GPIO_25)
#define KPAD_SCAN_Z         (GPIO_76|GPIO_77)
#define KPAD_RETURN_X       (GPIO_16|GPIO_26|GPIO_27)
#define KPAD_RETURN_Y       (GPIO_32)
#define KPAD_RETURN_Z       (GPIO_68)

#define KPAD_MATRIX_INT_SET_OUTPUT                  (pGPIOReg->GPDR_x |=  GPIO_10)  
#define KPAD_MATRIX_INT_SET_INPUT                   (pGPIOReg->GPDR_x &= ~GPIO_10)  
#define KPAD_MATRIX_INT_FALLING_EDGE_SET            (pGPIOReg->GFER_x |=  GPIO_10)  
#define KPAD_MATRIX_INT_FALLING_EDGE_CLEAR          (pGPIOReg->GFER_x &= ~GPIO_10)  
#define KPAD_MATRIX_INT_RISING_EDGE_SET             (pGPIOReg->GRER_x |=  GPIO_10)  
#define KPAD_MATRIX_INT_RISING_EDGE_CLEAR           (pGPIOReg->GRER_x &= ~GPIO_10)  
#define KPAD_MATRIX_INT_EDGE_DETECT_STATUS_CLEAR    (pGPIOReg->GEDR_x |=  GPIO_10)  
#define KPAD_MATRIX_INT_EDGE_DETECT_STATUS          (pGPIOReg->GEDR_x &   GPIO_10)

#define KPAD_SKEY1_SET_OUTPUT                 (pGPIOReg->GPDR_z |=  GPIO_67)  
#define KPAD_SKEY1_SET_INPUT                  (pGPIOReg->GPDR_z &= ~GPIO_67)  
#define KPAD_SKEY1_FALLING_EDGE_SET           (pGPIOReg->GFER_z |=  GPIO_67)  
#define KPAD_SKEY1_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_z &= ~GPIO_67)  
#define KPAD_SKEY1_RISING_EDGE_SET            (pGPIOReg->GRER_z |=  GPIO_67)  
#define KPAD_SKEY1_RISING_EDGE_CLEAR          (pGPIOReg->GRER_z &= ~GPIO_67)  
#define KPAD_SKEY1_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_z |=  GPIO_67)  
#define KPAD_SKEY1_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_z &   GPIO_67)  
#define KPAD_SKEY1_STATUS                     GPIO67_LEVEL
#define KPAD_SKEY1_PRESTATUS_INIT_VAL         GPIO_67

#define KPAD_SKEY2_SET_OUTPUT                 (pGPIOReg->GPDR_z |=  GPIO_72)  
#define KPAD_SKEY2_SET_INPUT                  (pGPIOReg->GPDR_z &= ~GPIO_72)  
#define KPAD_SKEY2_FALLING_EDGE_SET           (pGPIOReg->GFER_z |=  GPIO_72)  
#define KPAD_SKEY2_FALLING_EDGE_CLEAR         (pGPIOReg->GFER_z &= ~GPIO_72)  
#define KPAD_SKEY2_RISING_EDGE_SET            (pGPIOReg->GRER_z |=  GPIO_72)  
#define KPAD_SKEY2_RISING_EDGE_CLEAR          (pGPIOReg->GRER_z &= ~GPIO_72)  
#define KPAD_SKEY2_EDGE_DETECT_STATUS_CLEAR   (pGPIOReg->GEDR_z |=  GPIO_72)  
#define KPAD_SKEY2_EDGE_DETECT_STATUS         (pGPIOReg->GEDR_z &   GPIO_72)  
#define KPAD_SKEY2_STATUS                     GPIO72_LEVEL
#define KPAD_SKEY2_PRESTATUS_INIT_VAL         GPIO_72
// Pat01 end

// Pat02
#define KPAD_ENDKEY_SET_OUTPUT                (pGPIOReg->GPDR_x |=  GPIO_5)
#define KPAD_ENDKEY_SET_INPUT                 (pGPIOReg->GPDR_x &= ~GPIO_5)
#define KPAD_ENDKEY_FALLING_EDGE_SET          (pGPIOReg->GFER_x |=  GPIO_5)
#define KPAD_ENDKEY_FALLING_EDGE_CLEAR        (pGPIOReg->GFER_x &= ~GPIO_5)
#define KPAD_ENDKEY_RISING_EDGE_SET           (pGPIOReg->GRER_x |=  GPIO_5)
#define KPAD_ENDKEY_RISING_EDGE_CLEAR         (pGPIOReg->GRER_x &= ~GPIO_5)
#define KPAD_ENDKEY_EDGE_DETECT_STATUS_CLEAR  (pGPIOReg->GEDR_x |=  GPIO_5)
#define KPAD_ENDKEY_EDGE_DETECT_STATUS        (pGPIOReg->GEDR_x &   GPIO_5)
#define KPAD_ENDKEY_STATUS                    GPIO5_LEVEL
#define KPAD_ENDKEY_PRESTATUS_INIT_VAL        GPIO_5

/* ----------------------------------------------------------------------------
    MicroP
*/
//jack01 start
// MicroP  definition (GPIO_0)
#define MCU_INT_STATUS                   GPIO0_EDGE_DETECT_STATUS        //Read edge-detect status
#define MCU_INT_EDGE_CLR                 GPIO0_EDGE_DETECT_STATUS_CLEAR  //Clear edge-detect status by write bit 1.
#define MCU_INT_RISING_EDGE_SET          GPIO0_RISING_EDGE_SET           //Enable Rising-edge trigger.
#define MCU_INT_RISING_EDGE_CLR          GPIO0_RISING_EDGE_CLEAR         //Disable Rising-edge trigger.
#define MCU_INT_FALLING_EDGE_SET         GPIO0_FALLING_EDGE_SET          //Enable Falling-edge trigger.
#define MCU_INT_FALLING_EDGE_CLR         GPIO0_FALLING_EDGE_CLEAR        //Disable Falling-edge trigger.
//jack01 end

/* ----------------------------------------------------------------------------
    PMU
*/
//andy02 start
#define GPIO_BL_OFF                     GPIO62_SET_LOW
#define GPIO_BL_ON                      GPIO62_SET_HI
#ifdef PWM0_BACKLIGHT
#define GPIO_BACKLIGHT_OFF              GPIO16_SET_LOW								///span0220
#define GPIO_BACKLIGHT_ON               GPIO16_SET_HI								///span0220
#define GPIO_BACKLIGHT_SET_ALT          (pGPIOReg->GAFR1_x &= ~GPIO_16_AF3)         ///span0220
#endif

#ifndef ERFESH_DVT1		///span0326
/// GPIO62 hi for 7mA backlight enable
/// GPIO71 hi for 14mA backlight enable
#define GPIO_BACKLIGHT_HP_ALT			(pGPIOReg->GAFR0_z &= ~GPIO_71_AF3)
#define GPIO_BACKLIGHT_HP_OUTPUT		(pGPIOReg->GPDR_z |= GPIO_71)
#define GPIO_BACKLIGHT_HP_OFF			GPIO71_SET_LOW
#define GPIO_BACKLIGHT_HP_ON			GPIO71_SET_HI

#define GPIO_BACKLIGHT_LP_ALT			(pGPIOReg->GAFR1_y &= ~GPIO_62_AF3)
#define GPIO_BACKLIGHT_LP_OUTPUT		(pGPIOReg->GPDR_y |= GPIO_62)
#define GPIO_BACKLIGHT_LP_OFF			GPIO62_SET_LOW
#define GPIO_BACKLIGHT_LP_ON			GPIO62_SET_HI
#endif

#define KEYPAD_LED_ON                   GPIO17_SET_LOW
#define KEYPAD_LED_OFF                  GPIO17_SET_HI
#define TFT_RST_LOW                     GPIO19_SET_LOW
#define TFT_RST_HI                      GPIO19_SET_HI
#define TFT_POWER_ON                    GPIO20_SET_HI
#define TFT_POWER_OFF                   GPIO20_SET_LOW
#define KEYLEDPWM1                      (pGPIOReg->GAFR1_x |= GPIO_17_AF2_PWM1)     //andy03
#ifdef PWM0_BACKLIGHT
#define KEYLEDPWM0                      (pGPIOReg->GAFR1_x |= GPIO_16_AF2_PWM0)     ///span0220
#endif
#define KEYLEDGPIO                      (pGPIOReg->GAFR1_x &= ~GPIO_17_AF3)         //andy04
#define CLAM_CLOSED                     CLAM_PIN_LEVEL      //andy05
#define AC_OUT                          GPIO12_LEVEL        //andy06                        
#define CHARGE_FULL                     GPIO21_LEVEL        //andy06                        
#define MCU_AWAKE                       GPIO85_LEVEL        //andy07
#define MCU_WAKE_UP                     GPIO85_SET_LOW      //andy07
//andy02 end

/* andy02 remove, start
//andy01 start: add for hall_sw (use GPIO67)
#define CLAM_INT_STATUS                 GPIO67_EDGE_DETECT_STATUS        // Read edge-detect status
#define CLAM_INT_EDGE_CLR               GPIO67_EDGE_DETECT_STATUS_CLEAR  // Clear edge-detect status by write bit 1.
#define CLAM_INT_RISING_EDGE_SET        GPIO67_RISING_EDGE_SET           // Enable Rising-edge trigger.
#define CLAM_INT_RISING_EDGE_CLR        GPIO67_RISING_EDGE_CLEAR         // Disable Rising-edge trigger.
#define CLAM_INT_FALLING_EDGE_SET       GPIO67_FALLING_EDGE_SET          // Enable Falling-edge trigger.
#define CLAM_INT_FALLING_EDGE_CLR       GPIO67_FALLING_EDGE_CLEAR        // Disable Falling-edge trigger.
//andy01 end: add for hall_sw ends (use GPIO67)
//span01 start: for backlight control ON/OFF
#define GPIO_BL_OFF                     (pGPIOReg->GPSR_x |= GPIO_11)
#define GPIO_BL_ON                      (pGPIOReg->GPCR_x |= GPIO_11) 
// span01 end
*/ // andy02 remove, end

/* ----------------------------------------------------------------------------
    SD/MMC
*/
//yy01 start: add for SDMMC
#define SD_WP_AFR_CLEAR                         GPIO69_AFR_CLEAR        
#define SD_WP_DR_OUTPUT                         GPIO69_DR_SET   
#define SD_WP_LEVEL                         GPIO69_LEVEL          //sun07
#define SD_WP_ENABLE                         GPIO69_SET_INPUT          //sun07

       
#define SD_DETECT_ENABLE                        GPIO4_DIRECTION_CLEAR   
#define SD_DETECT_FALLING_EDGE_SET              GPIO4_FALLING_EDGE_SET
#define SD_DETECT_FALLING_EDGE_CLEAR            GPIO4_FALLING_EDGE_CLEAR
#define SD_DETECT_RISING_EDGE_SET               GPIO4_RISING_EDGE_SET
#define SD_DETECT_RISING_EDGE_CLEAR             GPIO4_RISING_EDGE_CLEAR
#define SD_DETECT_EDGE_DETECT_STATUS_CLEAR      GPIO4_EDGE_DETECT_STATUS_CLEAR
#define SD_DETECT_INT_STATUS                    GPIO4_EDGE_DETECT_STATUS
#define SD_DETECT_LEVEL                         GPIO4_LEVEL
#define SD_CS_SPI                               GPIO8_AF1
#define SD_CS_DISABLE                           GPIO8_AF_CLEAR
#define SD_CS_DIRECTION_OUTPUT                  GPIO8_DIRECTION_SET
#define SD_CS_DIRECTION_INPUT                   GPIO8_DIRECTION_CLEAR
#define SD_CS_HIGH                              GPIO8_SET_HI
#define SD_CS_LOW                               GPIO8_SET_LOW
//yy01 end: add for SDMMC
/*----------------------------------------------------------------------------
M-System RST
-----------------------------------------------------------------------------*/
#define GPIO_M_SYSTEM_RST_ON                    GPIO65_SET_LOW
#define GPIO_M_SYSTEM_RST_OFF                   GPIO65_SET_HI
/* ----------------------------------------------------------------------------
    YAMAHA
*/
//jacky01 start
#define GPIO_AUD_PWR_ON                  GPIO75_SET_HI
#define GPIO_AUD_PWR_OFF                 GPIO75_SET_LOW
#define GPIO_ML_POWER_ON                 GPIO71_SET_LOW
#define GPIO_ML_POWER_OFF                GPIO71_SET_HI
// HL001 start
#ifdef BSP_DVT_1
#define GPIO_ML_RESET_ON                 GPIO48_SET_LOW
#define GPIO_ML_RESET_OFF                GPIO48_SET_HI
#else
#define GPIO_ML_RESET_ON                 GPIO2_SET_LOW
#define GPIO_ML_RESET_OFF                GPIO2_SET_HI
#endif
// HL001 end

#define GPIO52_FALLING_EDGE_SET          (pGPIOReg->GFER_y |=  GPIO_52)  
#define GPIO52_FALLING_EDGE_CLEAR        (pGPIOReg->GFER_y &= ~GPIO_52)  
#define GPIO52_RISING_EDGE_SET           (pGPIOReg->GRER_y |=  GPIO_52)  
#define GPIO52_RISING_EDGE_CLEAR         (pGPIOReg->GRER_y &= ~GPIO_52) 
#define GPIO52_EDGE_DETECT_STATUS_CLEAR  (pGPIOReg->GEDR_y |= GPIO_52)   
#define GPIO52_EDGE_DETECT_STATUS        (pGPIOReg->GEDR_y & GPIO_52) 

#define YAMAHA_762B_INT_STATUS           GPIO52_EDGE_DETECT_STATUS
#define YAMAHA_762B_INT_CLR              GPIO52_EDGE_DETECT_STATUS_CLEAR
#define YAMAHA_762B_RISING_EDGE_CLR      GPIO52_RISING_EDGE_CLEAR
#define YAMAHA_762B_RISING_EDGE_SET      GPIO52_RISING_EDGE_SET
#define YAMAHA_762B_FALLING_EDGE_SET     GPIO52_FALLING_EDGE_SET
#define YAMAHA_762B_FALLING_EDGE_CLR     GPIO52_FALLING_EDGE_CLEAR 
//jacky01 end

/* ----------------------------------------------------------------------------
    Audio
*/
#define GPIO58_SET_OUTPUT           (pGPIOReg->GPDR_y |=  GPIO_58)  //jawadebug
#define GPIO_AMP_PWR_ON             GPIO58_SET_HI                   //Xy14  
#define GPIO_AMP_PWR_OFF            GPIO58_SET_LOW                  //Xy14
#define GPIO75_SET_OUTPUT           (pGPIOReg->GPDR_z |=  GPIO_75)  //zhao01
#define GPIO_AUD_PWR_ON             GPIO75_SET_HI                   //zhao01  
#define GPIO_AUD_PWR_OFF            GPIO75_SET_LOW                  //zhao01

//Xy01 start: add for phone jack detection
#define AUDIO_HDS_DETCT             GPIO14_LEVEL 
#define AUDIO_EXT_INT_STATUS        GPIO14_EDGE_DETECT_STATUS       // Read edge-detect status
#define AUDIO_EXT_INT_EDGE_CLR      GPIO14_EDGE_DETECT_STATUS_CLEAR // Clear edge-detect status by write bit 1.
#define AUDIO_EXT_RISING_EDGE_SET   GPIO14_RISING_EDGE_SET          // Enable Rising-edge trigger.  
#define AUDIO_EXT_RISING_EDGE_CLR   GPIO14_RISING_EDGE_CLEAR        // Disable Rising-edge trigger. 
#define AUDIO_EXT_FALLING_EDGE_SET  GPIO14_FALLING_EDGE_SET         // Enable Falling-edge trigger. 
#define AUDIO_EXT_FALLING_EDGE_CLR  GPIO14_FALLING_EDGE_CLEAR       // Disable Falling-edge trigger.
#define AUDIO_EXT_STATUS            GPIO14_LEVEL                    // Read level-detect status
//Xy01 end

//Xy03 start:
#define GPIO_PTT_DET                GPIO63_LEVEL                    // for ptt detecting    
#define AUDIO_PTT_INT_STATUS        GPIO63_EDGE_DETECT_STATUS       // Read edge-detect status
#define AUDIO_PTT_INT_EDGE_CLR      GPIO63_EDGE_DETECT_STATUS_CLEAR // Clear edge-detect status by write bit 1.
#define AUDIO_PTT_RISING_EDGE_SET   GPIO63_RISING_EDGE_SET          // Enable Rising-edge trigger.  
#define AUDIO_PTT_RISING_EDGE_CLR   GPIO63_RISING_EDGE_CLEAR        // Disable Rising-edge trigger. 
#define AUDIO_PTT_FALLING_EDGE_SET  GPIO63_FALLING_EDGE_SET         // Enable Falling-edge trigger. 
#define AUDIO_PTT_FALLING_EDGE_CLR  GPIO63_FALLING_EDGE_CLEAR       // Disable Falling-edge trigger.
#define AUDIO_PTT_STATUS            GPIO63_LEVEL                    // Read level-detect status
//Xy03 end.

//Xy48 start:
#define AUDIO_EXT_MIC_SET               GPIO64_SET_LOW                       // set to car mic.
#define AUDIO_INT_MIC_SET               GPIO64_SET_HI                      // set to internal mic.( default)

#define GPIO_CAR_DET                         GPIO13_LEVEL                    // for CARKIT detecting    
#define AUDIO_CAR_INT_STATUS         GPIO13_EDGE_DETECT_STATUS       // Read edge-detect status
#define AUDIO_CAR_INT_EDGE_CLR     GPIO13_EDGE_DETECT_STATUS_CLEAR // Clear edge-detect status by write bit 1.
#define AUDIO_CAR_RISING_EDGE_SET   GPIO13_RISING_EDGE_SET          // Enable Rising-edge trigger.  
#define AUDIO_CAR_RISING_EDGE_CLR   GPIO13_RISING_EDGE_CLEAR        // Disable Rising-edge trigger. 
#define AUDIO_CAR_FALLING_EDGE_SET  GPIO13_FALLING_EDGE_SET         // Enable Falling-edge trigger. 
#define AUDIO_CAR_FALLING_EDGE_CLR  GPIO13_FALLING_EDGE_CLEAR       // Disable Falling-edge trigger.
#define AUDIO_CAR_STATUS                  GPIO13_LEVEL                    // Read level-detect status
//Xy48 end.

/* ----------------------------------------------------------------------------
    USB
*/
#define  GPIO66_SET_OUTPUT            (pGPIOReg->GPDR_z |=  GPIO_66)  //hy01
#define  GPIO66_SET_INPUT             (pGPIOReg->GPDR_z &= ~GPIO_66)  //hy01
#define  USB_CABLE_IN_STATUS          (!(pGPIOReg->GPLR_x & GPIO_3))  //Paul01

/* ----------------------------------------------------------------------------
    FLASH
*/

/*-----------------------------------------------------------------------------
    RIL
*/
#define  GPIO70_SET_OUTPUT                 (pGPIOReg->GPDR_z |=  GPIO_70)  //hy02
#define  GPIO70_SET_INPUT                  (pGPIOReg->GPDR_z &= ~GPIO_70)  //hy02

/* ----------------------------------------------------------------------------
    Camera
*/
// Ryn01 Start
#ifndef HITACHI_CAMERA 
//bin01 start
#define     GPIO_CAM_POWER_ON              GPIO56_SET_LOW
#define     GPIO_CAM_POWER_OFF             GPIO56_SET_HI
#define     GPIO_CAM_PWDN_ON               GPIO50_SET_HI
#define     GPIO_CAM_PWDN_OFF              GPIO50_SET_LOW
#define     GPIO_CAM_RST_ON                GPIO51_SET_HI
#define     GPIO_CAM_RST_OFF               GPIO51_SET_LOW

#else
// For Hitachi Camera
#define     GPIO_CAM_POWER_ON              GPIO56_SET_HI
#define     GPIO_CAM_POWER_OFF             GPIO56_SET_LOW
#define     GPIO_CAM_PWDN_ON               GPIO50_SET_HI
#define     GPIO_CAM_PWDN_OFF              GPIO50_SET_LOW
#define     GPIO_CAM_RST_ON                GPIO51_SET_LOW
#define     GPIO_CAM_RST_OFF               GPIO51_SET_HI

#endif
// Ryn01 End
//bin01 end
//bin02 start
#define   GPIO51_SET_OUTPUT                              (pGPIOReg->GPDR_y |= GPIO_51)
#define   GPIO51_SET_AS_GPIO_PIN          (pGPIOReg->GAFR1_y &= ~GPIO_51_AF3)
#define   GPIO50_SET_OUTPUT                              (pGPIOReg->GPDR_y |= GPIO_50)
#define   GPIO50_SET_AS_GPIO_PIN          (pGPIOReg->GAFR1_y &= ~GPIO_50_AF3)
#define   GPIO56_SET_OUTPUT                              (pGPIOReg->GPDR_y |= GPIO_56)
#define   GPIO56_SET_AS_GPIO_PIN          (pGPIOReg->GAFR1_y &= ~GPIO_56_AF3) 
//bin02 end     

// Ryn01 Start
#define GPIO_86_AF1 0x00001000
#define GPIO_87_AF1 0x00004000

#define GPIO_86_AF3 0x00003000
#define GPIO_87_AF3 0x0000C000

#define  GPIO86_SET_GPIO                pGPIOReg->GAFR1_z &= ~GPIO_86_AF3;pGPIOReg->GAFR1_z |= GPIO_86_AF1
#define  GPIO87_SET_GPIO                pGPIOReg->GAFR1_z &= ~GPIO_87_AF3;pGPIOReg->GAFR1_z |= GPIO_87_AF1

#define  GPIO86_SET_OUT                 pGPIOReg->GPDR_z &= ~GPIO_86
#define  GPIO87_SET_OUT                 pGPIOReg->GPDR_z &= ~GPIO_87

#define  GPIO86_SET_IN                  pGPIOReg->GPDR_z |= GPIO_86
#define  GPIO87_SET_IN                  pGPIOReg->GPDR_z |= GPIO_87
// Ryn01 End

#endif
