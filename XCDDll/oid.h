#pragma once


#define OID_RT_DEVICE_ID_INFO					0xFF0101CB

#define	OID_RT_PRO_DBGCMD_SEND					0xFF818040
#define	OID_RT_PRO_DBGCMD_CHECK					0xFF818041
#define	OID_RT_PRO_DBGCMD_RETURN				0xFF818042

typedef struct _RT_DEVICE_ID_HEADER {
	//
	// Identify whether this is a Realtek WLAN device.
	// RT_DEVICE_ID_INFO_TAG means Realtek WLAN NIC device, other values are not valid.
	//
	ULONG	RtWlanDevTag;

	//
	// Identify which IC.
	// Examples of (ChipID, ChipVer):
	// (0x8185, 0x1)	=> 8185 
	// (0x8187, 0x1)	=> 8187 
	// (0x8185, 0x2)	=> 8185B 
	// (0x8187, 0x2)	=> 8187B 
	//
	ULONG	ChipID;
	ULONG	ChipVer;

	//
	// BusType is used to identify BUS type of the device and corresponding data type, 
	// for example:
	// RT_PCI_DEVICE => struc _RT_PCI_ID_INFO
	// RT_USB_DEVICE => struc _RT_USB_ID_INFO
	//
	ULONG	BusType;
}RT_DEVICE_ID_HEADER, * PRT_DEVICE_ID_HEADER;


typedef struct _RT_PCI_ID_INFO {
	RT_DEVICE_ID_HEADER	DevIDHeader;

	// 
	// Vendor ID and Device ID from PCI configuration space. 
	//
	USHORT	VID;
	USHORT	DID;

	// 
	// Sub Vendor ID and Subsystem ID from PCI configuration space. 
	//
	USHORT	SVID;
	USHORT	SMID;

	//
	// Revision ID from PCI configuration space.
	//
	UCHAR	RevID;

	//
	// Customer ID.
	//
	USHORT	CustomerID;
}RT_PCI_ID_INFO, * PRT_PCI_ID_INFO;

typedef struct _RT_USB_ID_INFO {
	RT_DEVICE_ID_HEADER	DevIDHeader;

	// 
	// Vendor ID and Product ID from USB Device Descriptor.
	//
	USHORT	VID;
	USHORT	PID;

	//
	// bcdDevice from USB Device Descriptor.
	//
	USHORT	RevID;

	//
	// Interface index.
	//
	USHORT	InterfaceIdx;
}RT_USB_ID_INFO, * PRT_USB_ID_INFO;

typedef struct _RT_SDIO_ID_INFO {
	RT_DEVICE_ID_HEADER	DevIDHeader;

	// 
	// Vendor ID and Product ID from SDIO Device.
//
	USHORT	VID;
	USHORT	PID;

	USHORT	RevID;

	//
	// Interface index.
	//
	USHORT	InterfaceIdx;
}RT_SDIO_ID_INFO, * PRT_SDIO_ID_INFO;

#define RT_DEVICE_MAX_BUFFER			\
	((sizeof(RT_PCI_ID_INFO) > sizeof(RT_USB_ID_INFO)) ? sizeof(RT_PCI_ID_INFO) : sizeof(RT_USB_ID_INFO))
