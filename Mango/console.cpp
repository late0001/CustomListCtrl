#include "pch.h"
#include "DbgCmd.h"
#include <conio.h>
#include "xcmd.h"

void moveLeft();
void moveRight(int step);
void gotoxy(int x, int y);
void restorexy();
void backupxy();
void clrlength(int dellen);
void getxy(int* x, int* y);

void showPrompt()
{
	LOGDA("%s", FieldDATA.Prompt);
}



void reset_prompt()
{
	FieldDATA.TotalCharNum = 0;
	FieldDATA.CurrCharIndex = 0;
	
	INC_CurrCMD;
	INC_TotalCMD;
	FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex] = 0;
	FieldDATA.HistoryIndex = FieldDATA.CurrCMD;
	
	LOGDA("\n\r");
	showPrompt();
}

void reset_prompt1()
{
	FieldDATA.TotalCharNum = 0;
	FieldDATA.CurrCharIndex = 0;

	//INC_CurrCMD;
	//INC_TotalCMD;
	FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex] = 0;
	//FieldDATA.HistoryIndex = FieldDATA.CurrCMD;
	LOGDA("\n\r");
	showPrompt();
}

void show_available_cmds(CLI_CMD* INcmd)
{
	CLI_CMD* cmd = INcmd;
	int ori_x, ori_y, len;
	getxy(&ori_x, &ori_y);

	LOGDA("\n\r Avaliable commands   :\n\r");
	while (cmd->name != 0)
	{
		LOGDA(" %-20s : %s\n\r", cmd->name, cmd->help);
		cmd++;
	}
	LOGDA("\n\r");
	showPrompt();
	LOGDA("%s", FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);

	len = FieldDATA.CurrCharIndex;
	FieldDATA.CurrCharIndex = 0;
	moveRight(len);
}

void gotoxy(int x, int y)
{
	COORD point;

	point.X = x; point.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void backupxy()
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	FieldDATA.CursorX = SBInfo.dwCursorPosition.X;
	FieldDATA.CursorY = SBInfo.dwCursorPosition.Y;
}

void restorexy()
{
	gotoxy(FieldDATA.CursorX, FieldDATA.CursorY);
}

void getxyWithRange(int* x, int* y, int* max_x, int* max_y)
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	*x = SBInfo.dwCursorPosition.X;
	*y = SBInfo.dwCursorPosition.Y;

	*max_x = SBInfo.dwMaximumWindowSize.X;
	*max_y = SBInfo.dwMaximumWindowSize.Y;
}

void getxy(int* x, int* y)
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	*x = SBInfo.dwCursorPosition.X;
	*y = SBInfo.dwCursorPosition.Y;
}

void moveLeft()
{
	int x, y, max_x, max_y;
	int total = 0, line = 0, prompt_len;

	if (FieldDATA.CurrCharIndex)
	{
		prompt_len = strlen(FieldDATA.Prompt);
		getxyWithRange(&x, &y, &max_x, &max_y);

		total = FieldDATA.CurrCharIndex + prompt_len;
		line = total / max_x;
		if (line && x == 0)
			gotoxy(max_x - 1, y - 1);
		else
			gotoxy(x - 1, y);
		FieldDATA.CurrCharIndex--;
	}
}

void moveRight(int step)
{
	int x, y, max_x, max_y;
	int total = 0, line = 0, prompt_len, pos = 0, line1 = 0;

	if (FieldDATA.CurrCharIndex + step <= FieldDATA.TotalCharNum)
	{
		prompt_len = strlen(FieldDATA.Prompt);
		getxyWithRange(&x, &y, &max_x, &max_y);

		total = FieldDATA.CurrCharIndex + prompt_len + step;// new position
		line = total / max_x;
		pos = total % max_x;

		total = FieldDATA.CurrCharIndex + prompt_len;		// old position
		line1 = total / max_x;

		gotoxy(pos, y + (line - line1));
		FieldDATA.CurrCharIndex += step;
	}
}

void clrlength(int dellen)
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	coordScreen.X = csbi.dwCursorPosition.X;
	coordScreen.Y = csbi.dwCursorPosition.Y;
	dwConSize = dellen;
	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ',
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void show_cmd_not_available()
{
	int ori_x, ori_y, len;
	getxy(&ori_x, &ori_y);

	LOGDA("\n\r Not available command\n\r\n\r");
	showPrompt();
	LOGDA("%s", FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);

	len = FieldDATA.CurrCharIndex;
	FieldDATA.CurrCharIndex = 0;
	moveRight(len);
}

void processInChar(char inChar)
{
	if (inChar <= 0x7f)
	{
		if (FieldDATA.CurrCharIndex == FieldDATA.TotalCharNum)	// Replace mode
		{
			FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex] = inChar;
			FieldDATA.CurrCharIndex++;
			printf("%c", inChar);
		}
		else	// Insert mode
		{
			/*memmove(&FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex + 1],
				&FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex],
				FieldDATA.TotalCharNum - FieldDATA.CurrCharIndex);
			FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex] = inChar;

			backupxy();
			clrlength(FieldDATA.TotalCharNum - FieldDATA.CurrCharIndex);
			_tprintf("%s", &FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex]);
			restorexy();
			moveRight(1);*/
		}
		FieldDATA.TotalCharNum++;
		FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.TotalCharNum] = 0;
	}
}

void parseBackspace()
{
	if (FieldDATA.TotalCharNum && FieldDATA.CurrCharIndex)
	{
		if (FieldDATA.CurrCharIndex == FieldDATA.TotalCharNum)
		{
			FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex - 1] = 0;
		}
		else
		{
			//memmove(&FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex - 1],
			//	&FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex],
			//	FieldDATA.TotalCharNum - FieldDATA.CurrCharIndex);
			//FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.TotalCharNum - 1] = 0;
		}
		moveLeft();
		backupxy();
		clrlength(FieldDATA.TotalCharNum - FieldDATA.CurrCharIndex);
		FieldDATA.TotalCharNum--;
		printf("%s", &FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][FieldDATA.CurrCharIndex]);
		restorexy();
	}
}

void parseSpace()
{
	//CLI_CMD* cmd = 0;
	//int token_num = 0, cmd_level = 0;

	//cmd = cmd_matched(&token_num, &cmd_level);
	//if (cmd && (token_num == cmd_level))
	//{
	//	cmdStrReplace(cmd);
	//}
	processInChar(' ');
}

void NIC_binding_fail()
{
	_tprintf(_T("\n\r Binding fail!! Adapter not found!!\n\r"));
}

CLI_CMD* cmd_matched(int* token_num, int* level)
{
	CLI_CMD* cmd = &cmd_table[0];
	CLI_CMD* MatchedCmd = 0;
	char buf[MaxCLIBuf];
	LPSTR current = buf, lptToken[MaxTokenNum];
	int total_token = 0;

	*level = 0;
	memset(buf, 0, MaxCLIBuf);
	memcpy(buf, &FieldDATA.CLI_Buffer[FieldDATA.CurrCMD][0], FieldDATA.TotalCharNum);

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

	if (total_token == 0)
	{
		*token_num = 0;
		return 0;
	}

	*token_num = total_token;
	while (cmd && *level < total_token)
	{
		if ((memcmp(lptToken[*level], cmd->name, strlen(lptToken[*level])) == 0) &&
			(strlen(cmd->name) >= strlen(lptToken[*level])))
		{
			(*level)++;
			if (*level == total_token)
			{
				MatchedCmd = cmd;
				break;
			}
			else
			{
				MatchedCmd = cmd;
				if (cmd->next_level)
				{
					cmd = cmd->next_level;
					continue;
				}
				else
					break;
			}
		}
		else
		{
			cmd++;
			if (cmd->name == 0)
				break;
		}
	}
	return MatchedCmd;
}



int parseEnter()
{
	CLI_CMD *cmd=0;
	int token_num=0, cmd_level=0;
	int dargc;
	char dargvBuf[MaxDbgArgvNum][MaxCLIBuf];
	char *dargv[MaxDbgArgvNum];
	int i, ret=0;
	int cnt=0, stop=0;
	
	
	for(i=0; i<MaxDbgArgvNum; i++)
		dargv[i] = &dargvBuf[i][0];

	cmd = cmd_matched(&token_num, &cmd_level);
	if(token_num)
	{
		if(cmd)
		{
			if(cmd->next_level)
				show_available_cmds(cmd->next_level);
			else if(cmd->handler)
			{
	//			if(cmd->handler == &cli_exit)
	//			{
	//				parseToken(&dargc, dargv, FieldDATA.CLI_Buffer[FieldDATA.CurrCMD], FieldDATA.TotalCharNum);
	//				ret = cmd->handler(dargc, dargv);
	//			}
	//			else if(cmd->handler == &cli_script)
	//			{
	//				parseToken(&dargc, dargv, FieldDATA.CLI_Buffer[FieldDATA.CurrCMD], FieldDATA.TotalCharNum);
	//				reset_prompt();
	//				ret = cmd->handler(dargc, dargv);
	//			}
	//			else if( (cmd->handler == &cli_bind_adapter) ||
	//					(cmd->handler == &cli_refresh_adapter) ||
	//					(cmd->handler == &cli_set_ioctl) )
	//			{
	//				parseToken(&dargc, dargv, FieldDATA.CLI_Buffer[FieldDATA.CurrCMD], FieldDATA.TotalCharNum);
	//				ret = cmd->handler(dargc, dargv);
	//				reset_prompt();
	//			}
	//			else
	//			{
					parseToken(&dargc, dargv, FieldDATA.CLI_Buffer[FieldDATA.CurrCMD], FieldDATA.TotalCharNum);
					
					// Check save to file mode
					//if(isparamexist(dargv, dargc, "-f"))
					//	bsavetofile = true;
					//else
						bSaveToFile = FALSE;

	//				if((cmd->attribute == ATTR_MONITOR_ALLOW)	&&
	//					isParamExist(dargv, dargc, "-m") )
	//				{	// monitor mode

	//					while(!stop)
	//					{
	//						cls();
	//						_tprintf(_T(" Press ESC to quit!\n\r"));

	//						// get adapter control
	//						if(DLL_bindCtrlAdapter(&g_CtrlInfo, GLByProDrv))
	//						{
	//							ret = cmd->handler(dargc-1, dargv);
	//							DLL_unbindCtrlAdapter(&g_CtrlInfo, GLByProDrv);
	//						}
	//						else
	//						{
	//							NIC_binding_fail();
	//						}
	//						if(ESC_Pushed_Detect(2))
	//							stop=1;
	//					}
	//				}
	//				else
	//				{

						// get adapter control
						if(DLL_bindCtrlAdapter(&g_CtrlInfo, GLByProDrv))
						{
							ret = cmd->handler(dargc, dargv);
							DLL_unbindCtrlAdapter(&g_CtrlInfo, GLByProDrv);
						}
						else
						{
							NIC_binding_fail();
						}
	//				}
					reset_prompt();
	//			}
			}
	    }
		else
			show_cmd_not_available();
	}
	else
	{
		if(strlen(FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]))
			reset_prompt();
		//else
		//	reset_prompt1();
	}
	return ret;
	//return 0;
}
void CommandLine(int init)
{
	int inChar;
	int line = 0, ch_index = 0;
	int len_line = 0;
	TCHAR strBuf[200];

	if (init)
	{
		initFieldData();
		//_tprintf(_T(" [Release Date 2023/03/26] \n\r"));
		//_tprintf(_T(" [Tips] \n\r"));
		//sprintf(strBuf, " %-15s : %s\n\r", "1. Help", "\"Shift\" + \"?\"");
		//_tprintf(_T(strBuf));
		//sprintf(strBuf, " %-15s : %s\n\r", "2. Cmd name", "\"white space key\"");
		//_tprintf(_T(strBuf));
		//sprintf(strBuf, " %-15s : %s\n\r", "3. Save to file", "\"Command\" -f [filename]");
		//_tprintf(_T(strBuf));
		_tprintf(_T(" \n\r"));
		showPrompt();
	}


	while (1)
	{
		
		inChar = _getch();
		

		switch (inChar)
		{
		case 0x08:      /* \b': */
			parseBackspace();
			break;
		case 0x09:		/* tab	*/
		case 0x1b:		/* ESC */
			break;
		case 0x0a:      /* '\r': */
		case 0x0d:      /* '\n': */
			if (parseEnter())
				return;
			break;
		case 0x20:      /* ' ': */
			parseSpace();
			break;
		case 0x3f:      /* '?': */
			//parseHelp();
			break;
		case 0xe0:
			inChar = _getch();
			//parse0xE0(inChar);
			break;
		case 0x03: // Ctrl-C
			reset_prompt();
			break;
		default:
			processInChar(inChar);
			break;
		}
	}
	return;
}