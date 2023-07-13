#include "pch.h"

#define XCD_DLL extern "C" __declspec(dllexport)
#include "XCDDll.h"
#include "oid.h"
#include "wlanIntf.h"
#include <windowsx.h>
#include <setupapi.h>
#include <tchar.h>
#include <cfgmgr32.h>
//=======================================================================
//	dll local functions
//=======================================================================
bool
dll_IoPacketRequest(
	LPADAPTER			AdapterObject,
	bool				Set,
	PPACKET_OID_DATA	OidData
    )
{
	UINT	BytesReturned;
	BOOL	bResult;
	
	bResult = DeviceIoControl(
		AdapterObject->hFile,
		(DWORD) Set ? IOCTL_PROTOCOL_SET_OID : IOCTL_PROTOCOL_QUERY_OID,
		OidData,
		sizeof(PACKET_OID_DATA)-1+OidData->Length,
		OidData,
		sizeof(PACKET_OID_DATA)-1+OidData->Length,
		(ULONG*)&BytesReturned,
		NULL
		);
	
	if(Set==true) 
		OidData->Length = BytesReturned;

	if(bResult)
		return true;
	else
		return false;
}


bool
dll_QueryOidIoctl(
	LPADAPTER	AdapterObject,
	ULONG		Oid,
	ULONG		oidSize,
	PVOID		pOidData
	)
{
    bool bResult=false;
    ULONG IoCtlBufferLength=(sizeof(PACKET_OID_DATA)+oidSize-1);
    PPACKET_OID_DATA OidData;

    OidData=(PPACKET_OID_DATA)GlobalAllocPtr(
        GMEM_MOVEABLE | GMEM_ZEROINIT,
        IoCtlBufferLength
        );

    if (OidData == NULL) {
        return bResult;
    }

    OidData->Oid = Oid;
    OidData->Length = oidSize;
	memcpy(OidData->Data, pOidData, oidSize);

    bResult = dll_IoPacketRequest(
        AdapterObject,
        false,
        OidData
        );

	if(bResult)
	{
		memcpy(pOidData, OidData->Data, oidSize);
	}

    GlobalFreePtr(OidData);
	
    return bResult;
}

XCD_DLL bool
DLL_QueryOid(
	CONTROL_INFO* pCtrlInfo,
	ULONG			Oid,
	ULONG			oidSize,
	PVOID			pOidData,
	bool			bByProDrv
)
{
	LPADAPTER	AdapterObject = pCtrlInfo->pCtrlAdapter;
	int index = pCtrlInfo->bindIndex;
	GUID tmpGuid = pCtrlInfo->AdapterList.adapter[index].nicGuid;

	if (bByProDrv)
		return dll_QueryOidIoctl(pCtrlInfo->pCtrlAdapter, Oid, oidSize, pOidData);
	else
		return QueryOID(tmpGuid, Oid, oidSize, pOidData);
}

bool
dll_QueryDeviceInfo(
    LPADAPTER   AdapterObject,
    ULONG*      ChipID,
    ULONG*      ChipVer,
	ULONG*		busType
    )
{
	PRT_DEVICE_ID_HEADER pDeviceHeader=NULL;
    bool bResult=false;
	char tmpBuf[100]={0};

	bResult = dll_QueryOidIoctl(AdapterObject, OID_RT_DEVICE_ID_INFO, RT_DEVICE_MAX_BUFFER, tmpBuf);

	if(bResult)
	{
		pDeviceHeader = (PRT_DEVICE_ID_HEADER)tmpBuf;

		*ChipID = pDeviceHeader->ChipID;
		*ChipVer = pDeviceHeader->ChipVer;
		*busType = pDeviceHeader->BusType;
	}
	
    return bResult;
}


PVOID
dll_OpenDUT(
	LPTSTR      AdapterName
)
{
	LPADAPTER   lpAdapter;
	BOOL        Result;

	lpAdapter = (LPADAPTER)GlobalAllocPtr(
		GMEM_MOVEABLE | GMEM_ZEROINIT,
		sizeof(ADAPTER)
	);

	if (lpAdapter == NULL)
	{
		return NULL;
	}

	wsprintf(lpAdapter->SymbolicLink, TEXT("\\\\.\\%s%s"), DOSNAMEPREFIX, &AdapterName[8]);

	lpAdapter->hFile = CreateFile(lpAdapter->SymbolicLink,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0
	);

	if (lpAdapter->hFile != INVALID_HANDLE_VALUE)
	{
		return lpAdapter;
	}
	else
	{
		Result = DefineDosDevice(DDD_RAW_TARGET_PATH, &lpAdapter->SymbolicLink[4], AdapterName);
		if (Result)
		{
			lpAdapter->hFile = CreateFile(lpAdapter->SymbolicLink,
				GENERIC_WRITE | GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				0
			);
			if (lpAdapter->hFile != INVALID_HANDLE_VALUE)
			{
				return lpAdapter;
			}
		}
	}

	GlobalFreePtr(lpAdapter);

	return NULL;
}

bool
dll_CloseDUT(
	LPADAPTER   lpAdapter
)
{
	BOOL bStatus = FALSE;

	bStatus = CloseHandle(lpAdapter->hFile);

	if (bStatus == TRUE)
	{
		GlobalFreePtr(lpAdapter);
	}

	if (bStatus)
		return true;
	else
		return false;
}

bool
dll_FindRealtekNicsIoctl(
	ADAPTER_LIST* pAdapterList
)
{
	int 		i;
	LPADAPTER AdapterFound = NULL;
	BOOL	result;
	BOOL	bFound = FALSE;
	GUID ClassId;
	ULONG ulIdNum;
	DWORD dwStatus, dwProblem;
	int nFail, nHidden;
	SP_DEVINFO_DATA DeviceInfoData = { sizeof(SP_DEVINFO_DATA) };
	HKEY hKey;
	TCHAR szBuf[MAX_PATH];
	DWORD cbBuf, dwType;
	TCHAR	szExportName[128];
	ULONG	tmpChipID, tmpChipVer, tmpBusType;
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;

	// Reset list counters.
	pAdapterList->Cnt = 0;

	// Retrieves Class GUID for "Net".
	SetupDiClassGuidsFromName(_T("Net"), &ClassId, 1, &ulIdNum);

	// Retrieves device info set for NICs.
	hDevInfo = SetupDiGetClassDevs(&ClassId, NULL, NULL, DIGCF_PRESENT);
	nFail = nHidden = 0;
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
	{
		//
		// Filter out non-active ones.
		//
		if (CR_SUCCESS != CM_Get_DevNode_Status(
			&dwStatus, &dwProblem, DeviceInfoData.DevInst, 0))
		{
			nFail++;
			continue;
		}
		//
		// Filter out hidden ones.
		//
		if (dwStatus & DN_NO_SHOW_IN_DM)
		{
			nHidden++;
			continue;
		}
		//
		// Collect information in NDIS miniport's driver registry.
		//
		// Open registry.
		hKey = SetupDiOpenDevRegKey(hDevInfo, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_ALL_ACCESS);
		if (hKey == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		// Get NetCfgInstanceId.
		cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);	// You have to reset the cbBuf size before the query
		if (RegQueryValueEx(hKey, _T("NetCfgInstanceId"), NULL, &dwType, (BYTE*)szBuf, &cbBuf) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}

		_stprintf_s(szExportName, _T("%s%s"), PROT_DRV_PREFIX, szBuf);

		//strcpy_s((char*)pAdapterList->adapter[pAdapterList->Cnt].SymbolicLink, sizeof(szExportName), szExportName);
		_tcscpy_s(pAdapterList->adapter[pAdapterList->Cnt].SymbolicLink, sizeof(szExportName), szExportName);
		cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);	// You have to reset the cbBuf size before the query
		if (RegQueryValueEx(hKey, _T("DriverDesc"), NULL, &dwType, (BYTE*)szBuf, &cbBuf) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}

		// translate char * to wchar_t *
		swprintf((wchar_t*)pAdapterList->adapter[pAdapterList->Cnt].Description, 200, L"%hs", szBuf);

		// Close registry key
		RegCloseKey(hKey);

		//
		// Retrive by ChipID and Ver
		//
		AdapterFound = (LPADAPTER)dll_OpenDUT(szExportName);

		if (AdapterFound != NULL)
		{
			// Check ChipID and ChipVer.
			result = dll_QueryDeviceInfo(
				AdapterFound,
				&tmpChipID,
				&tmpChipVer,
				&tmpBusType
			);

			if (!result)
			{
				dll_CloseDUT(AdapterFound);
				continue;
			}
			else
			{
				pAdapterList->adapter[pAdapterList->Cnt].busType = tmpBusType;
				pAdapterList->adapter[pAdapterList->Cnt].ChipID = tmpChipID;
				pAdapterList->adapter[pAdapterList->Cnt].ChipVer = tmpChipVer;
				pAdapterList->Cnt++;
				bFound = TRUE;
				dll_CloseDUT(AdapterFound);
			}
		}
		else
		{
			continue;
		}
	}

	if (hDevInfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
		hDevInfo = INVALID_HANDLE_VALUE;
	}

	if (!bFound)
	{
		AdapterFound = NULL;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

bool
dll_FindRealtekNicsWinApi(
	ADAPTER_LIST* pAdapterList
)
{
	bool bResult = false;
	CWlanInterfaces WlanIntif;
	int nInterfaces = WlanIntif.GetNumberOfInterfaces();
	WCHAR* pStrDesc;
	PRT_DEVICE_ID_HEADER pDeviceIdHeader;
	char	tmpBuf[100] = { 0 };

	pAdapterList->Cnt = 0;

	for (int i = 0; i < nInterfaces; i++)
	{
		GUID* pGuid = WlanIntif.GetInterfaceGuid(i);
		GUID  tmpGuid = *pGuid;
		pStrDesc = WlanIntif.GetDescription(i);

		bResult = QueryOID(tmpGuid, OID_RT_DEVICE_ID_INFO, 50, tmpBuf);

		if (bResult)
		{
			memcpy(pAdapterList->adapter[pAdapterList->Cnt].Description, pStrDesc, 256*sizeof(WCHAR));
			pAdapterList->adapter[pAdapterList->Cnt].nicGuid = *pGuid;
			pAdapterList->adapter[pAdapterList->Cnt].hFile = NULL;

			pDeviceIdHeader = (PRT_DEVICE_ID_HEADER)tmpBuf;
			pAdapterList->adapter[pAdapterList->Cnt].busType = pDeviceIdHeader->BusType;
			pAdapterList->adapter[pAdapterList->Cnt].ChipID = pDeviceIdHeader->ChipID;
			pAdapterList->adapter[pAdapterList->Cnt].ChipVer = pDeviceIdHeader->ChipVer;
			wsprintf(pAdapterList->adapter[pAdapterList->Cnt].SymbolicLink, TEXT(" "), "");
			pAdapterList->Cnt++;
		}
	}

	if (pAdapterList->Cnt)
		return true;
	else
		return false;
}

XCD_DLL bool
DLL_findRealtekWlanNics(
    ADAPTER_LIST* pAdapterList,
    bool			bByProDrv
)
{
    if (bByProDrv)
        return dll_FindRealtekNicsIoctl(pAdapterList);
    else
        return dll_FindRealtekNicsWinApi(pAdapterList);
}

XCD_DLL bool
DLL_bindCtrlAdapter(
	CONTROL_INFO* pCtrlInfo,
	bool			bByProDrv
)
{
	if (bByProDrv)
	{
		PVOID	pAdapter = NULL;
		pAdapter = dll_OpenDUT((LPTSTR)pCtrlInfo->AdapterList.adapter[pCtrlInfo->bindIndex].SymbolicLink);

		if (pAdapter != NULL)
		{
			pCtrlInfo->pCtrlAdapter = (LPADAPTER)pAdapter;
			return true;
		}
		else
		{
			pCtrlInfo->pCtrlAdapter = NULL;
			return false;
		}
	}
	else
	{
		if (pCtrlInfo->AdapterList.Cnt)
		{
			pCtrlInfo->pCtrlAdapter = &pCtrlInfo->AdapterList.adapter[pCtrlInfo->bindIndex];
			return true;
		}
		else
		{
			pCtrlInfo->pCtrlAdapter = NULL;
			return false;
		}
	}
}

XCD_DLL bool
DLL_unbindCtrlAdapter(
	CONTROL_INFO* pCtrlInfo,
	bool			bByProDrv
)
{
	if (bByProDrv)
	{
		if (pCtrlInfo->pCtrlAdapter)
		{
			dll_CloseDUT(pCtrlInfo->pCtrlAdapter);
			pCtrlInfo->pCtrlAdapter = NULL;
		}
	}
	else
	{

	}

	return true;
}