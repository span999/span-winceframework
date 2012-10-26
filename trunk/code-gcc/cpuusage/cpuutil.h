/*


*/



/* 
Parameters in /proc/stat
*/

typedef  long	_TYPNUM;

struct ProcStatNums
{
	_TYPNUM userNUM;
	_TYPNUM niceNUM;
	_TYPNUM systemNUM;
	_TYPNUM idleNUM;
	_TYPNUM iowaitNUM;
	_TYPNUM irqNUM;
	_TYPNUM softirqNUM;
	_TYPNUM sumNUM;
};


int getProcStat( struct ProcStatNums *pIn, int cpuIdx );
int updateNUM( struct ProcStatNums *pOld, struct ProcStatNums *pNew, struct ProcStatNums *pDiff );

