/*


*/

#ifndef __BATTUTIL_H__
#define __BATTUTIL_H__



#define _USE_NO_GREP_




/* 
Parameters in /sys/class/power_supply/internal_battery/
*/


typedef  long	_TYPNUM;

struct BattStatNums
{
	_TYPNUM adc;
	_TYPNUM voltage;
	_TYPNUM percentage;
	_TYPNUM online;
	char 	status[32];
};


struct BattStatSets
{
	int					battNUM;
	struct BattStatNums internal;
	struct BattStatNums vehicle;
};


int getBatteryinfo( struct BattStatSets *pIn );



#endif	/* __BATTUTIL_H__ */
