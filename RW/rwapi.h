#pragma once
#define TARGET_PROCESS_NAME L"YoudaoDict.exe"
#define DEVICE_SHOST_NAME L"rw"
#define DEVICE_NAME (L"\\Device\\" DEVICE_SHOST_NAME)
#define SYMBOLIC_LINK_SHORT_NAME L"link_rw"
#define SYMBOLIC_LINK_NAME (L"\\DosDevices\\" SYMBOLIC_LINK_SHORT_NAME)

//#define RW_CTL_CODE(x) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800 + x, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
//
//#define IO_READ_VIRTUAL_MEMORY RW_CTL_CODE(1)
//#define IO_WRITE_VIRTUAL_MEMORY RW_CTL_CODE(2)
//#define IO_GET_PROCESS_ID RW_CTL_CODE(3)
//#define IO_GET_PROCESS_BASE_ADDRESS RW_CTL_CODE(4)

typedef enum _PROTO_MESSAGE
{
	PROTO_GET_BASEADDR = 0,
	PROTO_NORMAL_READ = 1,
	PROTO_NORMAL_WRITE = 2,
	PROTO_Protect_Process = 3,
	PROTO_UnProtect_Process = 4
}PROTO_MESSAGE;

//typedef struct ReadStruct
//{
//	PVOID UserBufferAdress;
//	ULONG GameAddressOffset;
//	ULONG ReadSize;
//	ULONG UserPID;
//	ULONG GamePID;
//	BOOLEAN WriteOrRead;
//	PROTO_MESSAGE ProtocolMsg;
//} ReadStruct, *pReadStruct;

typedef struct ReadStruct
{
	ULONGLONG UserBufferAdress;
	ULONGLONG GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	PROTO_MESSAGE ProtocolMsg;
} ReadStruct, *pReadStruct;