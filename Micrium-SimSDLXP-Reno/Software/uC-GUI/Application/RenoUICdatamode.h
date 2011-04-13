
/*
	header of common UI frame page
*/

#ifndef _RENOUICDATAMODE_H_
#define	_RENOUICDATAMODE_H_

#include "RenoGUI.h"
#include "RenoUIframe.h"


/*
	frame page data of activity data set
*/
typedef struct
{
	int						iIndex;
	const GUI_ConstString	sDataName;
	const GUI_ConstString	sDataUnit;
	const GUI_ConstString	sDataValue;
} DATASET_HEADER;

typedef struct
{
	int						iDataSetNum;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	DATASET_HEADER*			pDataSets;
	FRAMEPAGE_HEADER** 		pDataFrame;
	int*					pDataParam;
} FP_DATASETS_HEADER;

typedef struct
{
	int 					iTotal;
	int						iCurrIdx;
	FRAMEPAGE_HEADER**		pDataSetFrame;
} DATASETS_FRAME_LIST;


void cbDataModeWindow(WM_MESSAGE* pMsg);
void DataModeWindow( int iOption );





#include "RenoUIframepage.h"


#endif	/* _RENOUICDATAMODE_H_ */