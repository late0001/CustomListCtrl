#include "pch.h"
#include <Rpcdce.h> // for UuidToStringW
#include <iostream>
#include "wlanintf.h"

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "Wlanui.lib")
#pragma comment( lib, "Rpcrt4.lib") // for UuidToStringW

using namespace std;


CWlanBase::CWlanBase()
{
    DWORD sVer; // version for open handle output. We don't use ir for anything.
    errorState = ERROR_SUCCESS;
    errorState = WlanOpenHandle(WLAN_API_VERSION, NULL, &sVer, &hClientHandle);
    if (ERROR_SUCCESS != errorState )
    {
        if (hClientHandle != NULL)
        {
            WlanCloseHandle(hClientHandle, NULL);
		//	TRACE("WLAN CLose for open fail %d \n",hClientHandle);
			hClientHandle = NULL;
        }
        return;
    }
//	TRACE("Open OK for handle %d\n", hClientHandle);
}

CWlanBase::~CWlanBase()
{
    if (hClientHandle != NULL)
    {
        WlanCloseHandle(hClientHandle, NULL);
		//TRACE("WLANClose handle %d for destructure \n",hClientHandle);
        hClientHandle = NULL;
    }
}


GUID* CWlanInterfaces::GetInterfaceGuid(int index)
{
    if (index < (int)guidValueArray.size())
    {
        return guidValueArray.at(index);
    } else
    {
        return NULL;
    }
}
WCHAR *CWlanInterfaces::GetDescription(int index)
{
	int size=ifcDescriptionArray.size();

    if (index < size)
    {
        return ifcDescriptionArray[index];
    } else
    {
        return NULL;
    }
}

WLAN_INTERFACE_STATE CWlanInterfaces::GetInterfaceState(int index)
{
    if (index < (int)connectionArray.size())
    {
        return(WLAN_INTERFACE_STATE)*connectionArray[index];
    } else
    {
        return(WLAN_INTERFACE_STATE)-1;
    }
}

DWORD CWlanInterfaces::GetConnectionAttributes(
	IN GUID Guid, 
	OUT PWLAN_CONNECTION_ATTRIBUTES *ppConnAttr
	)
{
	DWORD dwDataSize = 0;
	DWORD dwRet = ERROR_SUCCESS;
	errorState = WlanQueryInterface(
			hClientHandle,
			&Guid,
			wlan_intf_opcode_current_connection,
			NULL, // reserved
			&dwDataSize,
			(PVOID*)ppConnAttr,
			NULL
			);
	dwRet = errorState;
	return dwRet;
}

DWORD CWlanInterfaces::GetChannel(
	IN GUID Guid, 
	OUT PULONG pulChannel
	)
{
	DWORD dwDataSize = 0;
	PULONG pulData;
	errorState = WlanQueryInterface(
			hClientHandle,
			&Guid,
			wlan_intf_opcode_channel_number,
			NULL, // reserved
			&dwDataSize,
			(PVOID*)&pulData,
			NULL
			);
	if(errorState == ERROR_SUCCESS)
	{
		*pulChannel = *pulData;
	}
	return errorState;
}

int CWlanInterfaces::MatchGuid(GUID Guid)
{
    if (GetNumberOfInterfaces() == 0)
    {
        return -1;
    }
    for (int i = 0; i < GetNumberOfInterfaces(); i++)
    {
        GUID *tmp = GetInterfaceGuid(i);
        if (*tmp == Guid)
        {
            return i;
        }
    }
    return -1;
}

int CWlanInterfaces::GetNumberOfInterfaces()
{
    vector <TCHAR *>::size_type count;
    count = guidArray.size();
    //int i = (int)count;

    return(int)count;
}

static int unicodeToMbcs(char *szDest,const WCHAR* zWideFilename) {

	int nByte;
	char* zFilename;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = static_cast<char*>(malloc(nByte));
	if (zFilename == 0) {

		return 0;

	}
	nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, zFilename, nByte,
		0, 0);
	if (nByte == 0) {

		free(zFilename);
		zFilename = 0;
		return -1;
	}
	strcpy(szDest, zFilename);
	free(zFilename);
	zFilename = 0;
	return 0;

}
// enumerate wireless interfaces
CWlanInterfaces::CWlanInterfaces()
{
	PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
    RPC_WSTR strGuid = NULL;
    UINT i = 0;
    int NumberOfInterfaces = 0;
    if (ERROR_SUCCESS != errorState)
    {
        return;
    }
    // enumerate wireless interfaces
    if ((errorState = WlanEnumInterfaces(hClientHandle, NULL, &pIntfList)) != ERROR_SUCCESS)
    {
        return;
    }
    NumberOfInterfaces = pIntfList->dwNumberOfItems;
    for (i = 0; i < pIntfList->dwNumberOfItems; i++)
    {
        if (UuidToStringW(&pIntfList->InterfaceInfo[i].InterfaceGuid, &strGuid) == RPC_S_OK)
        {
        	
		TCHAR * tmp = NULL;
		GUID * ptmpGuidValue = NULL;
		WCHAR * desc = NULL;
		int * connectiontype = NULL;
		
        	try
		{
			tmp = new TCHAR[_tcsclen ((TCHAR*)strGuid)+1];
			ptmpGuidValue = new GUID;
			desc = new WCHAR[256];
			connectiontype = new int;
			//CString tempStr;

			_tcscpy_s(tmp, _tcsclen((TCHAR *)strGuid)+1, (TCHAR *)strGuid);
			*ptmpGuidValue =  pIntfList->InterfaceInfo[i].InterfaceGuid;
			//tempStr = pIntfList->InterfaceInfo[i].strInterfaceDescription;
			//_tcscpy_s(desc, 256, (const char *)pIntfList->InterfaceInfo[i].strInterfaceDescription);
			wcscpy_s(desc, 256, pIntfList->InterfaceInfo[i].strInterfaceDescription);
		    //unicodeToMbcs(desc, pIntfList->InterfaceInfo[i].strInterfaceDescription);
			//memcpy(desc ,pIntfList->InterfaceInfo[i].strInterfaceDescription , 256 );
			*connectiontype = pIntfList->InterfaceInfo[i].isState;
			guidArray.push_back(tmp);
			guidValueArray.push_back(ptmpGuidValue);
			ifcDescriptionArray.push_back(desc);
			connectionArray.push_back(connectiontype);
	
			RpcStringFreeW(&strGuid);
		}
		catch(...)
		{
			if(tmp)
			{
				delete [] tmp;
				tmp = NULL;
			}
			if( ptmpGuidValue)
			{
				delete ptmpGuidValue;
				ptmpGuidValue = NULL;
			}
			if(desc)
			{
				delete [] desc;
				desc = NULL;
			}
			if(connectiontype)
			{
				delete connectiontype;
				connectiontype = NULL;
			}
		}
        }
    }
    if (pIntfList != NULL)
    {
        WlanFreeMemory(pIntfList);
    }
}

CWlanInterfaces::~CWlanInterfaces()
{
    TCHAR * tmp;
    int * iptr;

    for (int i = (int)guidArray.size() -1; i >= 0; i--)
    {
        tmp = (TCHAR *)guidArray.at(i);
        delete []tmp;
        guidArray.erase(guidArray.end() -1);
    }
    for (int i = (int)guidValueArray.size() -1; i >= 0; i--)
    {
        GUID* tmpGuid = (GUID *)guidValueArray.at(i);
        delete tmpGuid;
        guidValueArray.erase(guidValueArray.end() -1);
    }
    for (int i = (int)ifcDescriptionArray.size() -1; i >= 0; i--)
    {
        WCHAR *tmp = ifcDescriptionArray.at(i);
        delete [] tmp;
        ifcDescriptionArray.erase(ifcDescriptionArray.end() -1);
    }
    for (int i = (int)connectionArray.size() -1; i >= 0 ; i--)
    {
        iptr = connectionArray.at(i);
        delete iptr;
        connectionArray.erase(connectionArray.end() - 1);
    }
}


CWlanIHVControl::CWlanIHVControl()
{
}

CWlanIHVControl::~CWlanIHVControl()
{
}

DWORD
CWlanIHVControl::Transmit(
	IN	GUID 	Guid,
	IN	int		controlType,
	IN	DWORD	inBufsize,
	IN	PVOID	inbuf,
	IN	DWORD	outBufsize,
	OUT	PVOID	outbuf,
	OUT	DWORD 	*bytesReturned
	)
{
	errorState = ERROR_SUCCESS;
    WLAN_IHV_CONTROL_TYPE ct;

	switch(controlType)
	{
	case WLAN_IHV_CONTROL_TYPE_SERVICE:
		ct = wlan_ihv_control_type_service;
		break;

	case WLAN_IHV_CONTROL_TYPE_MP_DRIVER:
		ct = wlan_ihv_control_type_driver;
		break;

	default:
		//RT_TRACE_F(COMP_IHV, DBG_WARNING, ("Invalid WLAN_IHV_CONTROL_TYPE (%d)\n", controlType));
		return ERROR_INVALID_PARAMETER;
	}

	errorState = WlanIhvControl(hClientHandle, &Guid, ct, inBufsize, inbuf, outBufsize, outbuf, bytesReturned);

	// Error Handling
	if(errorState != ERROR_SUCCESS)
	{
		//RT_TRACE_F(COMP_IHV, DBG_WARNING, ("WlanIhvControl = 0x%x\n", errorState));

		//if(pGuid)
		//	RT_PRINT_DATA(COMP_IHV | COMP_INIT, DBG_WARNING, "Adapter GUID = ", pGuid, sizeof(GUID));

		// Bruce_Test
		//switch(errorState)
		//{
		//case ERROR_NOT_FOUND:
		//case ERROR_NOT_SUPPORTED:
		//	bProtocolDriverInitialized = false;
		//	break;
		//}
	}
    return  errorState;
}


#define CCX_EVENT_OID                               0x00000004L     // to driver
#define CCX_TLV_SET_DATA                            0x00000008L
#define CCX_TLV_QUERY_DATA                          0x00000007L

typedef struct _IHV_SET_INFO
{
    UINT32                  Oid;
    UINT32                  BytesRead;
    UINT32                  BytesNeeded;
    UINT32                  InformationBufferLength;
    UINT8                   InformationBuffer[1];
} IHV_SET_INFO, *PIHV_SET_INFO;

typedef struct _IHV_QUERY_INFO
{
    UINT32                  Oid;
    UINT32                  BytesWritten;
    UINT32                  BytesNeeded;
    UINT32                  InformationBufferLength;
    UINT8                   InformationBuffer[1];
} IHV_QUERY_INFO, *PIHV_QUERY_INFO;

bool SetOID(GUID guid,ULONG Oid , ULONG oidSize ,PVOID pOidData )
{
   	PCCX_NIC_SPECIFIC_EXTENSION     ccxInData   = NULL;
	CWlanIHVControl				ictl;
	DWORD 						err = ERROR_SUCCESS;
	ULONG                           			totalSize   = 0;
	DWORD 						bytesReturned;

 	 if (NULL != pOidData) 
	{
		// 
		// Base size (oui + event + tlvData->tlvCount)
		// 
		totalSize  = FIELD_OFFSET(CCX_NIC_SPECIFIC_EXTENSION, tlvData) +  FIELD_OFFSET(IHV_CCX_TLV_DATA, tlv) ;

		int nLen=FIELD_OFFSET(CCX_REQUEST_DATA, SET_INFO.InformationBuffer) + oidSize;

		ULONG totalTlvLen = 0;
		totalTlvLen += FIELD_OFFSET(IHV_CCX_TLV, value) + nLen;

		totalSize += totalTlvLen;

		//Allocate contiguous data buffer
		ccxInData = (PCCX_NIC_SPECIFIC_EXTENSION)(new char[totalSize]);
		if (NULL != ccxInData) 
		{
			ccxInData->len              = totalSize - sizeof(ULONG);
			memcpy(ccxInData->oui, RTL_OUI, 4);
			ccxInData->event            = CCX_EVENT_OID;
			ccxInData->tlvData.tlvCount = 1;
			ccxInData->tlvData.tlv[0].type=CCX_TLV_SET_DATA;
			ccxInData->tlvData.tlv[0].length=totalSize;

			PIHV_SET_INFO info = (PIHV_SET_INFO)&ccxInData->tlvData.tlv[0].value[0];

			info->Oid = Oid;
			info->BytesNeeded = totalTlvLen;
			info->BytesRead = totalTlvLen;
			info->InformationBufferLength = oidSize;
			memcpy(info->InformationBuffer,pOidData,oidSize);

			err = ictl.Transmit(guid, 
							WLAN_IHV_CONTROL_TYPE_MP_DRIVER,  // to IHV or miniport
							totalSize,
							ccxInData,
							0, //outleng
							NULL,  //outdata
							&bytesReturned);

			if(err !=ERROR_SUCCESS)
			{
				delete(ccxInData);
				return false;
			}

			memcpy(pOidData , info->InformationBuffer ,oidSize);
			delete(ccxInData);

		}
		else
			return false;
	}
	else
		return false;

    return true;
}


bool QueryOID(GUID guid,ULONG Oid , ULONG oidSize ,PVOID pOidData )
{
	PCCX_NIC_SPECIFIC_EXTENSION     ccxInData   = NULL;
	CWlanIHVControl				ictl;
	DWORD 						err = ERROR_SUCCESS;
	ULONG                           			totalSize   = 0;
	DWORD 						bytesReturned;

 	 if (NULL != pOidData) 
	{
		// 
		// Base size (oui + event + tlvData->tlvCount)
		// 
		totalSize  = FIELD_OFFSET(CCX_NIC_SPECIFIC_EXTENSION, tlvData) +  FIELD_OFFSET(IHV_CCX_TLV_DATA, tlv) ;

		int nLen=FIELD_OFFSET(CCX_REQUEST_DATA, QUERY_INFO.InformationBuffer) + oidSize;

		ULONG totalTlvLen = 0;
		totalTlvLen += FIELD_OFFSET(IHV_CCX_TLV, value) + nLen;

		totalSize += totalTlvLen;

		//Allocate contiguous data buffer
		ccxInData = (PCCX_NIC_SPECIFIC_EXTENSION)(new char[totalSize]);
		if (NULL != ccxInData) 
		{
			ccxInData->len              = totalSize - sizeof(ULONG);
			memcpy(ccxInData->oui, RTL_OUI, 4);
			ccxInData->event            = CCX_EVENT_OID;
			ccxInData->tlvData.tlvCount = 1;
			ccxInData->tlvData.tlv[0].type=CCX_TLV_QUERY_DATA;
			ccxInData->tlvData.tlv[0].length=totalSize;

			PIHV_QUERY_INFO info = (PIHV_QUERY_INFO)&ccxInData->tlvData.tlv[0].value[0];

			info->Oid = Oid;
			info->BytesNeeded = totalTlvLen;
			info->BytesWritten = totalTlvLen;
			info->InformationBufferLength = oidSize;
			memcpy(info->InformationBuffer,pOidData,oidSize);
			DWORD	outsize = totalSize;

			err = ictl.Transmit(guid, 
							WLAN_IHV_CONTROL_TYPE_MP_DRIVER,  // to IHV or miniport
							totalSize,
							ccxInData,
							totalSize, //outleng
							ccxInData,  //outdata
							&bytesReturned);

			if(err !=ERROR_SUCCESS)
			{
				delete(ccxInData);
				return false;
			}

			memcpy(pOidData , info->InformationBuffer ,oidSize);
			delete(ccxInData);

		}
		else
			return false;
	}
	else
		return false;

    return true;
}