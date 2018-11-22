#pragma once
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include "phapi.h"

#pragma warning(disable:4311) //È«²¿¹Øµô

#ifndef dprintf
#define dprintf(Format, ...) DbgPrint("ProcessHacker: " Format "\n", __VA_ARGS__)
#endif // !dprintf

extern HANDLE TargetProcessId;
extern PVOID TargetProcessBaseAddress;


NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

NTSTATUS IoDispatchDeviceControl(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
);

NTSTATUS IoReadRequest(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
);

NTSTATUS IoWriteRequest(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
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