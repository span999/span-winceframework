/*
	header file for spLog2txt
	spCElogCtrl.c will be main file
*/

#ifndef __SPCELOGCTRL_H__
#define __SPCELOGCTRL_H__

#define		SPCELOGCTRL_EVENT					TEXT("named spCElog control event")

#define		SPCELOGCTRL_CTRLCMD_BASE			0x80000000
#define		SPCELOGCTRL_CTRLCMD_GETSTATUS		(SPCELOGCTRL_CTRLCMD_BASE|0x00000001)
#define		SPCELOGCTRL_CTRLCMD_HOSTSINIT		(SPCELOGCTRL_CTRLCMD_BASE|0x00000002)
#define		SPCELOGCTRL_CTRLCMD_GETCURRPKGNUM	(SPCELOGCTRL_CTRLCMD_BASE|0x00000010)
#define		SPCELOGCTRL_CTRLCMD_LOG_START		(SPCELOGCTRL_CTRLCMD_BASE|0x00020000)
#define		SPCELOGCTRL_CTRLCMD_LOG_STOP		(SPCELOGCTRL_CTRLCMD_BASE|0x00040000)
#define		SPCELOGCTRL_CTRLCMD_LOG_ON_SD		(SPCELOGCTRL_CTRLCMD_BASE|0x00080000)
#define		SPCELOGCTRL_CTRLCMD_LOG_ON_DEV		(SPCELOGCTRL_CTRLCMD_BASE|0x00100000)
#define		SPCELOGCTRL_CTRLCMD_LOG_SPY			(SPCELOGCTRL_CTRLCMD_BASE|0x00200000)
#define		SPCELOGCTRL_CTRLCMD_LOG_STANDALONE	(SPCELOGCTRL_CTRLCMD_BASE|0x00400000)
#define		SPCELOGCTRL_CTRLCMD_LOG_DEBUGLOG	(SPCELOGCTRL_CTRLCMD_BASE|0x60000000)
#define		SPCELOGCTRL_CTRLCMD_TIMER_RESET		(SPCELOGCTRL_CTRLCMD_BASE|0x80000000)


#define		SPCELOGCTRL_CTRLRSP_BASE			0x60000000
#define		SPCELOGCTRL_CTRLRSP_OK				(SPCELOGCTRL_CTRLRSP_BASE|0x00000001)
#define		SPCELOGCTRL_CTRLRSP_FAIL			(SPCELOGCTRL_CTRLRSP_BASE|0x00000002)
#define		SPCELOGCTRL_CTRLRSP_TIMEOUT			(SPCELOGCTRL_CTRLRSP_BASE|0x00000004)
#define		SPCELOGCTRL_CTRLRSP_NOT_SUPPORT		(SPCELOGCTRL_CTRLRSP_BASE|0x00000008)
#define		SPCELOGCTRL_CTRLRSP_LOG_RUNNING		(SPCELOGCTRL_CTRLRSP_BASE|0x00010000)
#define		SPCELOGCTRL_CTRLRSP_LOG_STOP		(SPCELOGCTRL_CTRLRSP_BASE|0x00020000)

#define		SPCELOGCTRL_CTRLRSP_VALUE_BASE		0xC0000000

#endif	///#ifndef __SPCELOGCTRL_H__