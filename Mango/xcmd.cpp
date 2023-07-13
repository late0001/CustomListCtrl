#include "pch.h"
#include "DbgCmd.h"
#include "xcmd.h"
#include "rlrelative.h"

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
