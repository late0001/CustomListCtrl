#pragma once

#ifdef XCD_DLL
#else
#define XCD_DLL extern "C" __declspec(dllimport)
#endif

#define	MAX_LINK_NAME_LENGTH		64		///< Define the max adapter name length.
#define	MAX_DESCRIPTION_LENGTH		256	///< Define the max adapter description length.
#define	MAX_ADAPTER_NUM				8



typedef struct _ADAPTER {
	HANDLE		hFile;									///< The handle that application can use to access the lower actual adapter.
	char		SymbolicLink[MAX_LINK_NAME_LENGTH];		///< The adapter name.
	TCHAR		Description[MAX_DESCRIPTION_LENGTH];	///< The adapter description.
	GUID		nicGuid;
	UINT		ChipID;
	UINT		ChipVer;
	UINT		busType;
} ADAPTER, * LPADAPTER;

typedef struct _ADAPTER_LIST {
	int			Cnt;
	ADAPTER		adapter[MAX_ADAPTER_NUM];
} ADAPTER_LIST, * LPADAPTER_LIST;

typedef struct _CONTROL_INFO {
	int				bindIndex;
	LPADAPTER		pCtrlAdapter;
	ADAPTER_LIST	AdapterList;
} CONTROL_INFO, * PCONTROL_INFO;

XCD_DLL bool
DLL_findRealtekWlanNics(
	ADAPTER_LIST* pAdapterList,
	bool			bByProDrv
);
XCD_DLL bool
DLL_QueryOid(
	CONTROL_INFO* pCtrlInfo,
	ULONG			Oid,
	ULONG			oidSize,
	PVOID			pOidData,
	bool			bByProDrv
);

XCD_DLL bool
DLL_bindCtrlAdapter(
	CONTROL_INFO* pCtrlInfo,
	bool			bByProDrv
);

XCD_DLL bool
DLL_unbindCtrlAdapter(
	CONTROL_INFO* pCtrlInfo,
	bool			bByProDrv
);