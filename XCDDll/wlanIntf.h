#pragma once

#include <vector>
#include <wlanapi.h>
#include <winioctl.h>

#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib, "Setupapi.lib")

typedef struct _PACKET_OID_DATA {

    ULONG           Oid;
    ULONG           Length;
    UCHAR	        Data[10];
	UCHAR			StringData[256];
}   PACKET_OID_DATA, *PPACKET_OID_DATA;

#define FILE_DEVICE_PROTOCOL        0x8000

#define IOCTL_PROTOCOL_SET_OID      CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_QUERY_OID    CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_RESET        CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)


using namespace std;

typedef unsigned char       CCXBOOLEAN;
typedef signed char         CCXINT8,    *PCCXINT8;
typedef unsigned char       CCXUINT8,   *PCCXUINT8;
typedef signed short        CCXINT16,   *PCCXINT16;
typedef unsigned short      CCXUINT16,  *PCCXUINT16;
typedef signed long         CCXINT32,   *PCCXINT32;
typedef unsigned long       CCXUINT32,  *PCCXUINT32;
typedef signed __int64      CCXINT64,   *PCCXINT64;
typedef unsigned __int64    CCXUINT64,  *PCCXUINT64;
typedef CCXUINT32           CCX_OID,    *PCCX_OID;
typedef CCXINT32            CCX_ERROR,  *PCCX_ERROR;

typedef struct _RT_DIAG_DATA
{
	UCHAR			OUI[4];
	ULONG          messageType;
	ULONG			bufferLen;
	UCHAR			buffer[1];
} RT_DIAG_DATA, *PRT_DIAG_DATA;

typedef enum _CCX_REQUEST_TYPE
{
    CCX_REQUEST_SET,
    CCX_REQUEST_QUERY,
    CCX_REQUEST_METHOD,
} CCX_REQUEST_TYPE, *PCCX_REQUEST_TYPE;

typedef union _CCX_REQUEST_DATA
{
    struct _QUERY_INFO
    {
        CCX_OID                  Oid;
        CCXUINT32               BytesWritten;
        CCXUINT32               BytesNeeded;
        CCXUINT32               InformationBufferLength;
        CCXUINT8                 InformationBuffer[1];
    } QUERY_INFO;

    struct _SET_INFO
    {
        CCX_OID                 Oid;
        CCXUINT32               BytesRead;
        CCXUINT32               BytesNeeded;
        CCXUINT32               InformationBufferLength;
        CCXUINT8                InformationBuffer[1];
    } SET_INFO;

    struct _METHOD_INFO
    {
        CCX_OID                 Oid;
        CCXUINT32               BytesWritten;
        CCXUINT32               BytesNeeded;
        CCXUINT32               BytesRead;
        CCXUINT32               InputBufferLength;
        CCXUINT32               InformationBufferLength;
        CCXUINT8                InformationBuffer[1];
    } METHOD_INFO;

} CCX_REQUEST_DATA, *PCCX_REQUEST_DATA;
typedef struct _CCX_REQUEST
{
    CCX_REQUEST_TYPE            ccxRequestType;
    CCX_REQUEST_DATA            ccxData;
} CCX_REQUEST, *PCCX_REQUEST;


#pragma pack( 4 )

//---------------------------------------------------------------------------
// Format of TLV in CCX_NIC_SPECIFIC_EXTENSION structure.
//---------------------------------------------------------------------------
typedef struct _IHV_CCX_TLV {
    ULONG           type;       // TLV type code (see defined type codes below)
    ULONG           length;     // length of the data in value array (in bytes)
    UCHAR           value[1];   // data associated with the specified type code
} IHV_CCX_TLV, *PIHV_CCX_TLV;

#pragma pack()


//---------------------------------------------------------------------------
// Structure defining a count followed by array of TLV.
//---------------------------------------------------------------------------
typedef struct _IHV_CCX_TLV_DATA {
    ULONG           tlvCount;   // number of TLV in tlv array.
    IHV_CCX_TLV     tlv[1];     // variable number of variable length tlv
} IHV_CCX_TLV_DATA, *PIHV_CCX_TLV_DATA;

//---------------------------------------------------------------------------
// Format of CCX data passed to OID_DOT11_NIC_SPECIFIC_EXTENSION.
//---------------------------------------------------------------------------
typedef struct _CCX_NIC_SPECIFIC_EXTENSION {
    ULONG                   len;        // length of entire structure (excluding this field)
    UCHAR                   oui[4];     // vendor specific OUI value
    ULONG                   event;      // event code
    IHV_CCX_TLV_DATA        tlvData;    // tlv data
} CCX_NIC_SPECIFIC_EXTENSION, *PCCX_NIC_SPECIFIC_EXTENSION;

const UCHAR RTL_OUI[] = { 0x00, 0xe0, 0x4c, 0x81};

bool SetOID(GUID guid,ULONG Oid , ULONG oidSize ,PVOID pOidData );
bool QueryOID(GUID guid,ULONG Oid , ULONG oidSize ,PVOID pOidData );

#define	WLAN_IHV_CONTROL_TYPE_SERVICE	0
#define	WLAN_IHV_CONTROL_TYPE_MP_DRIVER	1

class CWlanBase {
public:
	CWlanBase();
	~CWlanBase();
	DWORD GetErrorState(){return errorState;}
	void SetErrorState(DWORD err);
protected:
	HANDLE hClientHandle;
	DWORD errorState;
};

class CWlanInterfaces : public CWlanBase{
public:
	CWlanInterfaces();
	~CWlanInterfaces();
	int MatchGuid(GUID Guid);  // matches the guid passed in with a interface context guid. returns the index of the guid or -1 if non-existent
	int GetNumberOfInterfaces();
	GUID* GetInterfaceGuid(int index); // returns the interface guid for that index. NULL is returned if there is no interface for the index.
	WCHAR *GetDescription(int index);
	WLAN_INTERFACE_STATE GetInterfaceState(int index);
	DWORD GetConnectionAttributes(IN GUID Guid, OUT PWLAN_CONNECTION_ATTRIBUTES *ppConnAttr);
	DWORD GetChannel(IN GUID Guid, OUT PULONG pulChannel);
	
private:
	vector <TCHAR *> guidArray;
	vector <GUID *> guidValueArray;
	vector <WCHAR *> ifcDescriptionArray;
	vector <int *> connectionArray;
	vector <char *> ifNameArray;

};

class CWlanIHVControl : public CWlanBase {
public:
    CWlanIHVControl();
    ~CWlanIHVControl();
    void SetControlType(WLAN_IHV_CONTROL_TYPE ct){controlType = ct;}
    DWORD Transmit(GUID Guid, int controlType, DWORD inBufsize, PVOID outbuf, DWORD outBufsize, PVOID inbuf,  DWORD *bytesReturned);

protected:
    WLAN_IHV_CONTROL_TYPE controlType;
    DWORD inbufSize;
    WCHAR inbuf[512];
    DWORD outbufSize;
    WCHAR outbuf[512];
    DWORD bytesReturned;
};


