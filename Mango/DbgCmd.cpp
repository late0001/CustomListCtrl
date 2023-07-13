#include "pch.h"
#include "DbgCmd.h"
#include "rlrelative.h"
#include <time.h>

INTF_DBG_SEND		ToDriver;
INTF_DBG_RETURN		FromDriver;


bool
dcmd_SendDebugCmd(
	CONTROL_INFO* pCtrlInfo,
	int				DbgCmdLength,
	UCHAR* InData,
	bool			bByApi
)
{
	return DLL_QueryOid(pCtrlInfo, OID_RT_PRO_DBGCMD_SEND, DbgCmdLength, InData, bByApi);
}

bool
dcmd_CheckDebugCmd(
	CONTROL_INFO* pCtrlInfo,
	int* ready,
	bool			bByApi
)
{
	bool bResult = false;
	char tmpBuf[10] = { 0 };

	bResult = DLL_QueryOid(pCtrlInfo, OID_RT_PRO_DBGCMD_CHECK, sizeof(ULONG), tmpBuf, bByApi);

	if (bResult)
		*ready = (int)(*((PULONG)tmpBuf));
	else
		*ready = 0;

	return bResult;
}

#define MAX_DBG_BUF_SIZE    256
bool
dcmd_ReturnDebugCmd(
	CONTROL_INFO* pCtrlInfo,
	UCHAR* OutData,
	bool			bByApi
)
{
	bool bResult = false;
	char tmpBuf[MAX_DBG_BUF_SIZE] = { 0 };

	bResult = DLL_QueryOid(pCtrlInfo, OID_RT_PRO_DBGCMD_RETURN, MAX_DBG_BUF_SIZE, tmpBuf, bByApi);

	if (bResult)
	{
		memcpy(OutData, tmpBuf, MAX_DBG_BUF_SIZE);
	}

	return bResult;
}

void dcmd_SaveCmdStr(char* String)
{
	char* fpath;
	FILE* fp;

	fpath = "dbginfo.txt";
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "(%Y-%m-%d, %X)", &tstruct);

	// Write from the last write pointer, Try designated filename first, if fail then use default filename
	if (bUseFileName && (fp = fopen(saveFileName, "a")) != NULL)
	{
		if (bShowTime)
			fprintf(fp, "\r\n\r\n .........%s..........\r\n", buf);
		fprintf(fp, "%s", String);
	}
	else if ((fp = fopen(fpath, "a")) != NULL)
	{
		if (bShowTime)
			fprintf(fp, "\r\n\r\n .........%s..........\r\n", buf);
		fprintf(fp, "%s", String);
	}

	fclose(fp);

}

void dcmd_ParseToDriver(int argc, char* argv[], UINT Type, int CmdIndex)
{
	int		i, wait_cnt = 0;
	BOOL	bResult;
	int		start_idx = 0;
	char	monitor = FALSE;

	//
	// Description :
	// Type == 0 We  call debug command once
	// Type == 1 We must call debug command periodically
	// Type == 2 We must call debug command by cmd string once
	// Type == 3 We must call debug command by cmd string periodically
	// Type == Type+0x10000000 We must save dbg info into a file
	//
	// Input Format type:
	// 0. d cmd CMD_IDX [argv1] [argv2]...
	// 1. d m cmd CMD_IDX [argv1] [argv2]...
	// 2. d CMD_STRING [argv1] [argv2]...
	// 3. d m CMD_STRING [argv1] [argv2]...
	//
	// Define constamt to replace later



	//
	// Decide input command type and start argv address.
	//
	if (Type == 0)
	{
		start_idx = 1;//2;	
		monitor = FALSE;	// Stop monitor
	}
	else if (Type == 1)
	{
		start_idx = 3;
		monitor = TRUE;	// Enable monitor
	}
	else if (Type == 2)
	{
		start_idx = 1;
		monitor = FALSE;	// Stop monitor
	}
	else if (Type == 3)
	{
		start_idx = 2;
		monitor = TRUE;	// Enable monitor
	}


	// Clear Cmd buffer between UI and driver
	memset(&ToDriver, 0, sizeof(INTF_DBG_SEND));
	memset(&FromDriver, 0, sizeof(INTF_DBG_RETURN));

	// Send command to driver. We can support command to firmware later
	ToDriver.dbgOpCode = DbgOpDriver;
	//ToDriver.dbgOpCode = DbgOpFirm;

	// Decide driver visible argument counter.
	ToDriver.dbgArgc = argc - start_idx;
	// 
	ToDriver.dbgLength = 8 + (ToDriver.dbgArgc) * MaxDbgArgvLen;

	// We can insert commadn index from upper layer or transfer by ATOL. 
	if (Type < 2)
		ToDriver.dbgCmdIndex = (UINT)atol(argv[start_idx]);
	else
		ToDriver.dbgCmdIndex = (UINT)CmdIndex;

	// Copy argv to command send buffer.
	for (i = 0; i < (argc - (start_idx + 1)); i++)
		memcpy(&ToDriver.dbgArgv[i * MaxDbgArgvLen], argv[i + start_idx + 1], strlen(argv[i + start_idx + 1]));

	// Do while loop when user enable monitor ability
	do
	{
		// Send command to drive or firmware
		bResult = dcmd_SendDebugCmd(&g_CtrlInfo,
			8 + ToDriver.dbgLength,
			(UCHAR*)&ToDriver,
			GLByProDrv
		);

		if (bResult == TRUE)
		{
			bShowTime = TRUE; // Only Show Time On The First Time, Because Driver Msgs may be divided in segments
			int ready = 0;
			// Wait 50*100=5000ns for command execute flag
			while (wait_cnt++ <= 50)
			{
				bResult = dcmd_CheckDebugCmd(&g_CtrlInfo, &ready, GLByProDrv);
				if (bResult && ready)
					break;
				else
					Sleep(100);
			}
			// Print debug info from driver or firmawre
			while (wait_cnt++ < 100)
			{
				bResult = dcmd_ReturnDebugCmd(&g_CtrlInfo, (UCHAR*)&FromDriver, GLByProDrv);

				if (bResult)
				{
					wait_cnt = 0;
					LOGDA(&FromDriver.dbgRtnBuf[0]);





					// Save to file	
					if (bSaveToFile)
						dcmd_SaveCmdStr(&FromDriver.dbgRtnBuf[0]);

					bShowTime = FALSE;

					if (FromDriver.dbgStatus == 1 ||	//success
						FromDriver.dbgStatus == 0)		//fail
					{
						break;
					}
				}
				else
					break;
			}
		}
		else
		{
			// Send command fail, we stop monitor
			monitor = FALSE;
		}

		if (monitor)
		{
			Sleep(1000);		// ms uint
			system("cls.exe");
		}
	} while (monitor);
}