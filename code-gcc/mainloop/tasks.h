/*


*/
#ifndef __TASKS_H__
#define __TASKS_H__


typedef int (*fnTaskMeber)(int);

typedef struct {
    fnTaskMeber begin;
    fnTaskMeber duty;
    fnTaskMeber end;
    fnTaskMeber ISR;
    fnTaskMeber IST;
} TaskMembers;

typedef struct {
    int			valid[16];
    int			priority[16];
    int			requested[16];
    TaskMembers tasks[16];
} TaskList;

#define TASK_VALID			1
#define TASK_INVALID		(-1)

#define TASK_PRIOR_NONE		(-1)
#define TASK_PRIOR_01		(15)
#define TASK_PRIOR_02		(14)
#define TASK_PRIOR_03		(13)
#define TASK_PRIOR_04		(12)
#define TASK_PRIOR_05		(11)
#define TASK_PRIOR_06		(10)
#define TASK_PRIOR_07		(9)
#define TASK_PRIOR_08		(8)
#define TASK_PRIOR_09		(7)
#define TASK_PRIOR_10		(6)
#define TASK_PRIOR_11		(5)
#define TASK_PRIOR_12		(4)
#define TASK_PRIOR_13		(3)
#define TASK_PRIOR_14		(2)
#define TASK_PRIOR_15		(1)
#define TASK_PRIOR_MAX		TASK_PRIOR_01
#define TASK_PRIOR_HIGH		TASK_PRIOR_03
#define TASK_PRIOR_NORNAL	TASK_PRIOR_08
#define TASK_PRIOR_LOW		(0)

#define TASK_IST_REQUEST	1
#define TASK_IST_NONREQ		(-1)


void TasksDump( TaskList *ptasklist );
int TasksStarter( TaskList *ptasklist );
int TasksPicker( TaskList *ptasklist );




#endif /* #ifndef __TASKS_H__ */


