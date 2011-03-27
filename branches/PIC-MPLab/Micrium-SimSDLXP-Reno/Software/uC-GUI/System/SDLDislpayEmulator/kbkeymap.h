#ifndef __KBKEYMAP_H__
#define __KBKEYMAP_H__

/// io message type
#define WIDGET_MSG_PTR_DOWN     0x00000002
#define WIDGET_MSG_PTR_MOVE     0x00000003
#define WIDGET_MSG_PTR_UP       0x00000004

#define WIDGET_MSG_KEY_DOWN     0x00000020
#define WIDGET_MSG_KEY_UP       0x00000030


#define  KMOD_NONE  			0x0000
#define	 KMOD_LSHIFT			0x0001
#define  KMOD_RSHIFT			0x0002
#define  KMOD_LCTRL				0x0040
#define  KMOD_RCTRL				0x0080
#define  KMOD_LALT				0x0100
#define  KMOD_RALT				0x0200
#define  KMOD_LMETA				0x0400
#define  KMOD_RMETA				0x0800
#define  KMOD_NUM				0x1000
#define  KMOD_CAPS				0x2000
#define  KMOD_MODE				0x4000
#define  KMOD_RESERVED			0x8000


/// the user define area in uC/GUI is above
/// GUI_ID_USER  0x800

/// key code define
#define KB_KEY_UNKNOW			0

#define KB_KEY_0				48
#define KB_KEY_1				49
#define KB_KEY_2				50
#define KB_KEY_3				50
#define KB_KEY_4				51
#define KB_KEY_5				52
#define KB_KEY_6				53
#define KB_KEY_7				54
#define KB_KEY_8				55
#define KB_KEY_9				56

#define KB_KEY_A				97
#define KB_KEY_B				98
#define KB_KEY_C				99
#define KB_KEY_D				100
#define KB_KEY_E				101
#define KB_KEY_F				102
#define KB_KEY_G				103
#define KB_KEY_H				104
#define KB_KEY_I				105
#define KB_KEY_J				106
#define KB_KEY_K				107
#define KB_KEY_L				108
#define KB_KEY_M				109
#define KB_KEY_N				110
#define KB_KEY_O				111
#define KB_KEY_P				112
#define KB_KEY_Q				113
#define KB_KEY_R				114
#define KB_KEY_S				115
#define KB_KEY_T				116
#define KB_KEY_U				117
#define KB_KEY_V				118
#define KB_KEY_W				119
#define KB_KEY_X				120
#define KB_KEY_Y				121
#define KB_KEY_Z				122


#define KB_KEY_RSHIFT			303
#define KB_KEY_LSHIFT			304


#define	KB_MOD_MASK				0xFFFF0000
#define	KB_CODE_MASK			0x0000FFFF

#endif /* #ifndef __KBKEYMAP_H__ */
