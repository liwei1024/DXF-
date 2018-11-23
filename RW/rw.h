#pragma once

DRIVER_INITIALIZE DriverEntry;

enum PROTO_MESSAGE
{
	PROTO_GET_BASEADDR = 0,
	PROTO_NORMAL_READ = 1,
	PROTO_NORMAL_WRITE = 2,
	PROTO_Protect_Process = 3,
	PROTO_UnProtect_Process = 4
};

DWORD64 GetSectionBaseAddress(HANDLE);
PEPROCESS GetEpGameIdByProcess(HANDLE);

typedef struct ReadStruct
{
	ULONGLONG UserBufferAdress;
	ULONGLONG GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	UINT32 ProtocolMsg;
} ReadStruct, *pReadStruct;

LONGLONG PUBase;
HANDLE PUID;

PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;
PEPROCESS PUBGepGame = NULL;

PVOID DriverBuffer;