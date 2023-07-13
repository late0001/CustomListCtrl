#pragma once

#include "xcmd.h"

typedef enum _DBG_OPCODE {
    /*0 */  DbgOpDriver,                //OpCode used in driver
    /*1 */  DbgOpFirm,                  //OpCode used in firmware
    /*3 */  DbgOpMax,
    /*4 */  DbgOpEnd                    //End of command table
}DBG_OPCODE;

typedef struct _INTF_DBG_SEND {
    UINT    dbgOpCode;
    UINT    dbgLength;
    UINT    dbgCmdIndex;
    UINT    dbgArgc;
#define MaxDbgArgvNum       15
#define MaxDbgArgvLen       16
    char    dbgArgv[MaxDbgArgvNum * MaxDbgArgvLen];
} INTF_DBG_SEND;

#define MaxTokenNum		(MaxDbgArgvNum+1)

typedef struct _INTF_DBG_RETURN {
    UINT    dbgOpCode;
    UINT    dbgStatus;
    UINT    dbgSegment;
    UINT    dbgLength;
    char    dbgRtnBuf[MaxDbgArgvNum * MaxDbgArgvLen];
} INTF_DBG_RETURN;

void dcmd_ParseToDriver(int argc, char* argv[], UINT Type, int CmdIndex);