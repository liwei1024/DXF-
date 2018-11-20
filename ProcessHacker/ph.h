#pragma once
#include "ntos.h"
#include "phapi.h"

#pragma warning(disable:4311) //È«²¿¹Øµô

#ifndef dprintf
#define dprintf(Format, ...) DbgPrint("ProcessHacker: " Format "\n", __VA_ARGS__)
#endif // !dprintf

extern HANDLE GameProcessId;
extern PVOID GameImageBase;

typedef struct _KERNEL_READ_REQUEST
{
	ULONG ProcessId;

	ULONG Address;
	ULONG Response;
	ULONG Size;

} KERNEL_READ_REQUEST, *PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG ProcessId;

	ULONG Address;
	ULONG Value;
	ULONG Size;

} KERNEL_WRITE_REQUEST, *PKERNEL_WRITE_REQUEST;

NTSTATUS DispatchDeviceControl(
	_In_ PDEVICE_OBJECT DeviceObject,
	_Inout_ PIRP Irp
);

NTSTATUS ReadVirtualMemory(
	_In_ PEPROCESS Process,
	_In_ PVOID SourceAddress,
	_In_ PVOID TargetAddress,
	_In_ SIZE_T Size
);

NTSTATUS WriteVirtualMemory(
	_In_ PEPROCESS Process,
	_In_ PVOID SourceAddress,
	_In_ PVOID TargetAddress,
	_In_ SIZE_T Size
);