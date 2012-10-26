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
int updateNUM( struct ProcStatNums *pOld, struct ProcStatNums *pNew, struct ProcStatNums *pDiff );
int getProcMeminfo( struct ProcMeminfoNums *pIn );

