#ifndef __SPLIBNUMPADPARCER_H__
#define __SPLIBNUMPADPARCER_H__




typedef enum _TOUCHHOOKEXT_AREA_SET
{
	AREA_ZERO = 0,
	AREA_ONE,
	AREA_TWO,
	AREA_THREE,
	AREA_FOUR,
	AREA_FIVE,
	AREA_SIX,
	AREA_SEVEN,
	AREA_EIGHT,
	AREA_NINE,
	AREA_STAR,
	AREA_POUND,
} TOUCHHOOKEXT_AREA_SET;


BOOL spLibParseAreaInit( void );
BOOL spLibParseAreaDeInit( void );
TOUCHHOOKEXT_AREA_SET spLibParseAreaCondition( INT X, INT Y );
BOOL spLibParseSequenceCondition( TOUCHHOOKEXT_AREA_SET ThisHit );





#endif	///#ifndef __SPLIBNUMPADPARCER_H__