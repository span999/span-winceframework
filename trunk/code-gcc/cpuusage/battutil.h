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


struct ThermalStatNums
{
	_TYPNUM temp;
};

struct ThermalStatSets
{
	int							thermalNUM;
	struct ThermalStatNums 		internal;	///for cpu thermal,
};



int getBatteryinfo( struct BattStatSets *pIn );
int getThermalinfo( struct ThermalStatSets *pIn );


#endif	/* __BATTUTIL_H__ */
