#pragma once
#include "rlrelative.h"

int cli_bind_adapter(int argc, char* argv[]);
int cli_refresh_adapter(int argc, char* argv[]);
int cli_exit(int argc, char* argv[]);
int cli_read_efuse(int argc, char* argv[]);
int cli_read_efuse();
int	parseToken(int* dargc, char** dargv, char* inBuf, int bufLen);
void initFieldData();
typedef struct par_hint {
    char* name;			/* parameter name */
    char* help_string;	/* parameter help string */
}CLI_PAR;

typedef enum _CMD_ATTR {
    /*0 */	ATTR_NONE,
    /*1 */	ATTR_EXEC_ONCE,
    /*2 */  ATTR_MONITOR_ALLOW,
}CMD_ATTR;

typedef struct cmd {
    char* name;
    char* help;		/* command help string */
    CLI_PAR* help_par;	/* parameter help structure */
    struct cmd* next_level;/* sub command table of this command group */
    CMD_ATTR	 attribute;	/* attributes */
    int			(*handler)(int, char**);
}CLI_CMD;

extern CLI_CMD cmd_table[];

typedef struct {
    CLI_CMD* cmd;                       	//pointer to the current command table
#define MaxCLIHistory       11					//can buffer MaxCLIHistory-1 commands
#define MaxCLILen			500
    char		CLI_Buffer[MaxCLIHistory][MaxCLILen];  //command history
    char* Prompt;                     	//command prompt
    int			TotalCharNum;					//the total num of chars
    int			CurrCharIndex;					//cur index to the cli buffer.
    int         CurrCMD;                    	//pointer to current command buffer
    int         TotalCMD;                   	//total cmd in buffer
    int         HistoryIndex;               	//pointer to the history buffer
    int			CursorX;						//backup cursor X
    int			CursorY;						//backup cursor Y
}CLI_DATA;
extern CLI_DATA FieldDATA;

#define INC_CurrCMD         FieldDATA.CurrCMD = \
                            ((FieldDATA.CurrCMD+MaxCLIHistory+1)%MaxCLIHistory)

#define INC_TotalCMD        if(FieldDATA.TotalCMD++ >= MaxCLIHistory)  \
                            FieldDATA.TotalCMD=MaxCLIHistory

#define MaxCLIBuf			300

extern	bool	bSaveToFile;
extern	bool	bShowTime;
extern	char	saveFileName[];
extern	bool	bUseFileName;