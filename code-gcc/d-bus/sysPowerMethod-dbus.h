#ifndef __SYSPOWERMETHOD_DBUS_H__
#define __SYSPOWERMETHOD_DBUS_H__


#define DBUSSRV_METHOD_SERVER		"test.method.server"
#define DBUSSRV_METHOD_OBJECT		"/test/method/Object"
#define DBUSSRV_METHOD_TYPE			"test.method.Type"


struct sysPowerCallMethod
{
	int				iID;			/* id */
	int				iType;			/* method type */
	char*			userName;		/* user function name */
	char*			dbusName;		/* dbus function name */
};

static struct sysPowerCallMethod methodList[] = {
	{	0,	0,	"sPSgetCPUActivatedNum", "getCPUcoreActivatedNumber"	},
	{	1,	0,	"sPSsetCPUActivatedNum", "setCPUcoreActivatedNumber"	},
	{	2,	0,	"sPSsetCPUDVFS", "setCPUdvfs"	},
	{	3,	0,	"sPSsetCPUspeed", "setCPUspeed"	},
	{	4,	0,	"sPSsetCPUsuspend", "setCPUsuspend"	},
	{	-1,	-1,	NULL, NULL	}
};





#endif /* ##ifndef __SYSPOWERMETHOD_DBUS_H__ */
