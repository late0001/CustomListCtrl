#include "pch.h"
#include "DbgCmd.h"
#include "xcmd.h"
#include "rlrelative.h"
#include "EfuseDlg.h"

CLI_DATA FieldDATA;
bool	bSaveToFile;
bool	bShowTime;
char	saveFileName[80] = "";
bool	bUseFileName;
//============================================================
char promptStr[] = { "Sigui# " };
char hint_cmd_exit[] = { "Exit Debug environment" };
char hint_cmd_adapter[] = { "adapter operation(bind/display/refresh..)." };
char hint_cmd_efuse[] = { "efuse operation" };
char hint_cmd_bind_adapter[] = { "bind adapter" };
char hint_cmd_refresh_adapter[] = { "refresh and bind default adapter" };
char hint_cmd_read_efuse[] = { "read efuse map/content" };
char hint_cmd_read_efuse_map[] = { "read efuse shadow map" };
char hint_cmd_read_efuse_content[] = { "read efuse real content" };
//============================================================
// Hint for parameter
//============================================================
CLI_PAR hint_par_return[] = {
	{"<cr>", "Click return or enter"},
	{0, 0},
};

CLI_PAR hint_par_bind_adapter[] = {
	{"<index>", "Adapter index"},
	{0, 0},
};

CLI_CMD read_efuse_table[] = {
	{"map", hint_cmd_read_efuse_map, hint_par_return, 0, ATTR_EXEC_ONCE, cli_read_efuse},
	{"content", hint_cmd_read_efuse_content, hint_par_return, 0, ATTR_EXEC_ONCE, cli_read_efuse},
	{0, "",	0, 0, ATTR_NONE, 0},
};

CLI_CMD adapter_table[] = {
	{"bind", hint_cmd_bind_adapter, hint_par_bind_adapter, 0, ATTR_EXEC_ONCE, cli_bind_adapter},
	{"refresh", hint_cmd_refresh_adapter, hint_par_return, 0, ATTR_EXEC_ONCE, cli_refresh_adapter},
	{0, "",	0, 0, ATTR_NONE, 0},
};

CLI_CMD efuse_table[] = {
		{"read", hint_cmd_read_efuse, 0, read_efuse_table, ATTR_NONE, 0},
	//	{"write", hint_cmd_write_efuse, 0, write_efuse_table, ATTR_NONE, 0},
		//{"update", hint_cmd_update_efuse, 0, 0, ATTR_EXEC_ONCE, cli_update_efuse},
		//{"loadmap", hint_cmd_load_efuse_map, hint_par_load_efuse_map, 0, ATTR_EXEC_ONCE, cli_efuse_loadmap},
		//{"reload", hint_cmd_reload_efuse, hint_par_return, 0, ATTR_EXEC_ONCE, cli_reload_efuse},
		//{"analyze", hint_cmd_analyze_efuse, 0, 0, ATTR_EXEC_ONCE, cli_efuse_analyze},
		//{"setMAC", hint_cmd_wifi_mac_addr, hint_par_setMAC, 0, ATTR_EXEC_ONCE, cli_set_WIFI_MACAddr},
		//{"fake", hint_cmd_fake_efuse, 0, fake_efuse_table, ATTR_NONE, 0},
		{0, "",	0, 0, ATTR_NONE, 0},
};


CLI_CMD cmd_table[] = {
	{"exit", hint_cmd_exit, hint_par_return, 0, ATTR_EXEC_ONCE, cli_exit},
	{"adapter", hint_cmd_adapter, 0, adapter_table, ATTR_NONE, 0},
	{"efuse", hint_cmd_efuse, 0, efuse_table, ATTR_NONE, 0},
	
	{0, "",	0, 0, ATTR_NONE, 0},
};


void initFieldData()
{
	memset(&FieldDATA, 0, sizeof(FieldDATA));
	FieldDATA.Prompt = promptStr;
	FieldDATA.cmd = cmd_table;
}

int cli_bind_adapter(int argc, char* argv[])
{
	long index = 0;
	wchar_t outName[128];

	LOGD(_T("\n\r"));
	if (argc < 3)
	{
		LOGD(_T(" Wrong argument number\n\r"));
		return 0;
	}

	sscanf(argv[2], "%d", &index);

	if (index <= 0)
	{
		_tprintf(_T(" Invalid bind index \n\r"));
		return 0;
	}

	if (index > g_CtrlInfo.AdapterList.Cnt)
	{
		_tprintf(_T(" Only %d adapter found!!\n\r"), g_CtrlInfo.AdapterList.Cnt);
		return 0;
	}
	else
	{
		g_CtrlInfo.bindIndex = index - 1;
		_stprintf(outName, _T(" %d. %s ***(binded adapter)\n"), g_CtrlInfo.bindIndex + 1, g_CtrlInfo.AdapterList.adapter[g_CtrlInfo.bindIndex].Description);
		_tprintf(outName);
	}
	return 0;
}

int cli_refresh_adapter(int argc, char* argv[])
{
	AdapterRefresh();

	return 0;
}

int cli_exit(int argc, char* argv[])
{
	return 1;
}

int	parseToken(int* dargc, char** dargv, char* inBuf, int bufLen)
{
	char buf[MaxCLIBuf];
	LPSTR current = buf, lptToken[MaxTokenNum];
	int total_token = 0;
	int len1 = 0, len2 = 0, i;

	memset(buf, 0, MaxCLIBuf);
	memcpy(buf, inBuf, bufLen);

	lptToken[0] = strtok(current, " ");
	if (lptToken[0] != NULL)
		total_token++;
	while (1)
	{
		lptToken[total_token] = strtok(NULL, " ");
		if (lptToken[total_token] != NULL)
			total_token++;
		else
			break;
	}
	*dargc = total_token;
	for (i = 0; i < total_token; i++)
	{
		strcpy(dargv[i], lptToken[i]);
	}

	return 0;
}

int cli_read_efuse(int argc, char* argv[])
{
	int dargc;
	char dargvBuf[MaxDbgArgvNum][MaxCLIBuf];
	char* dargv[MaxDbgArgvNum];
	char strRMap[] = "cmd 27 1236";
	char strRCon[] = "cmd 27 0 256";
	char strCmd[MaxCLIBuf] = "";
	int i;

	for (i = 0; i < MaxDbgArgvNum; i++)
		dargv[i] = &dargvBuf[i][0];

	if (memcmp(argv[2], "map", strlen(argv[2])) == 0)
	{
		strcpy(strCmd, strRMap);
	}
	else
	{
		strcpy(strCmd, strRCon);
	}

	parseToken(&dargc, dargv, strCmd, strlen(strCmd));
	dcmd_ParseToDriver(dargc, dargv, 0, 0);

	return 0;
}

void SaveEfuse(char *fileName, char* String)
{
	char* fpath;
	FILE* fp;

	fpath = "dbginfo.txt";
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[512];
	int i = 0;
	//tstruct = *localtime(&now);
	//strftime(buf, sizeof(buf), "(%Y-%m-%d, %X)", &tstruct);

	// Write from the last write pointer, Try designated filename first, if fail then use default filename
	if ((fp = fopen(fileName, "a")) != NULL)
	{
		//if (bShowTime)
		//	fprintf(fp, "\r\n\r\n .........%s..........\r\n", buf);
		//fprintf(fp, "%s", String);
		sprintf(buf, "%s", String);
		int len = strlen(buf);
		while (i <len) {
			if (buf[i] == '\r' && buf[i + 1] == '\n') { i++; continue; }
			fwrite(&buf[i], 1, 1, fp);
			i++;
		}
	}
	else if ((fp = fopen(fpath, "a")) != NULL)
	{
		//if (bShowTime)
		//	fprintf(fp, "\r\n\r\n .........%s..........\r\n", buf);
		fprintf(fp, "%s", String);
	}

	fclose(fp);

}

void ReadEfuseFromFile(char* fileName, CEfuseDlg* pDlg)
{
	char* fpath;
	FILE* fp;

	fpath = "dbginfo.txt";
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[1024];
	int i = 0;
	int addr;
	UINT ebyte[16];
	//tstruct = *localtime(&now);
	//strftime(buf, sizeof(buf), "(%Y-%m-%d, %X)", &tstruct);

	// Write from the last write pointer, Try designated filename first, if fail then use default filename
	if ((fp = fopen(fileName, "r")) != NULL)
	{
		char* ret = NULL;
		pDlg->ListCtrlDeleteAllItem();
		//if (bShowTime)
		//	fprintf(fp, "\r\n\r\n .........%s..........\r\n", buf);
		//fprintf(fp, "%s", String);
		fgets(buf, 1024 * sizeof(char), fp);
		while (buf[0] == ' '|| buf[0] == '\n'|| buf[0] == '\r')  ret = fgets(buf, 1024 * sizeof(char), fp);
		while (buf[0] != ' ' &&  ret  != NULL) {
			sscanf(buf, "0x%08x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x\r\n",
				&addr, &ebyte[1], &ebyte[0], &ebyte[3], &ebyte[2], &ebyte[5], &ebyte[4], &ebyte[7], &ebyte[6], &ebyte[9], &ebyte[8], &ebyte[0xB], &ebyte[0xA], &ebyte[0xD], &ebyte[0xC], &ebyte[0xF], &ebyte[0xE]);
			pDlg->ListCtrlInsertItem(ebyte);
			ret = fgets(buf, 1024 * sizeof(char), fp);
		}
		
		
	}
	

	fclose(fp);

}
void ParseToDriver(int argc, char* argv[])
{
	int		i, wait_cnt = 0;
	BOOL	bResult;
	int		start_idx = 0;

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
	
		start_idx = 1;//2;	
	
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
	
		ToDriver.dbgCmdIndex = (UINT)atol(argv[start_idx]);

	// Copy argv to command send buffer.
	for (i = 0; i < (argc - (start_idx + 1)); i++)
		memcpy(&ToDriver.dbgArgv[i * MaxDbgArgvLen], argv[i + start_idx + 1], strlen(argv[i + start_idx + 1]));

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
			FILE* fp = NULL;
			// Wait 50*100=5000ns for command execute flag
			while (wait_cnt++ <= 50)
			{
				bResult = dcmd_CheckDebugCmd(&g_CtrlInfo, &ready, GLByProDrv);
				if (bResult && ready)
					break;
				else
					Sleep(100);
			}
			fp = fopen("hello.txt", "w");
			fclose(fp);
			// Print debug info from driver or firmawre
			while (wait_cnt++ < 100)
			{
				bResult = dcmd_ReturnDebugCmd(&g_CtrlInfo, (UCHAR*)&FromDriver, GLByProDrv);

				if (bResult)
				{
					wait_cnt = 0;
					LOGDA(&FromDriver.dbgRtnBuf[0]);

					// Save to file	
					//if (bSaveToFile)
						SaveEfuse("hello.txt", &FromDriver.dbgRtnBuf[0]);

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

}

int cli_read_efuse(CEfuseDlg *pDlg)
{
	int dargc;
	char dargvBuf[MaxDbgArgvNum][MaxCLIBuf];
	char* dargv[MaxDbgArgvNum];
	char strRCon[] = "cmd 27 0 256";
	char strCmd[MaxCLIBuf] = "";
	int i;

	for (i = 0; i < MaxDbgArgvNum; i++)
		dargv[i] = &dargvBuf[i][0];
	
	strcpy(strCmd, strRCon);
	
	parseToken(&dargc, dargv, strCmd, strlen(strCmd));
	ParseToDriver(dargc, dargv);
	ReadEfuseFromFile("hello.txt", pDlg);
	return 0;
}
