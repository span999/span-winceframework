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


struct ProcStatSets
{
	struct ProcStatNums cpu;
	int					cpuNUM;
	struct ProcStatNums cpu0;
	struct ProcStatNums cpu1;
	struct ProcStatNums cpu2;
	struct ProcStatNums cpu3;
	struct ProcStatNums cpu4;
	struct ProcStatNums cpu5;
	struct ProcStatNums cpu6;
	struct ProcStatNums cpu7;
};


struct ProcMeminfoNums
{
	_TYPNUM memtotalNUM;
	_TYPNUM memfreeNUM;
	_TYPNUM memusedNUM;	/* memtotalNUM - memfreeNUM */
	_TYPNUM buffersNUM;
	_TYPNUM cachedNUM;
	_TYPNUM vmalloctotalNUM;
	_TYPNUM vmallocusedNUM;
	_TYPNUM vmallocchunkNUM;
};


int getProcStat( struct ProcStatNums *pIn, int cpuIdx );
int getProcStatSet( struct ProcStatSets *pIn );
int updateNUM( struct ProcStatNums *pOld, struct ProcStatNums *pNew, struct ProcStatNums *pDiff );
int updateSetsNUM( struct ProcStatSets *pOld, struct ProcStatSets *pNew, struct ProcStatSets *pDiff );
int getProcMeminfo( struct ProcMeminfoNums *pIn );

