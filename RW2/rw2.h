#pragma once
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include "rw2api.h"

#ifndef dprintf
#define dprintf(Format, ...) DbgPrint("RW2: " Format "\n", __VA_ARGS__)
#endif // !dprintf

extern HANDLE TargetProcessId;
extern PVOID TargetProcessBaseAddress;

typedef struct _REPROTECT_CONTEXT
{
	PMDL   Mdl;
	PUCHAR LockedVa;
} REPROTECT_CONTEXT, *PREPROTECT_CONTEXT;

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


NTSTATUS DispatchDeviceControl(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
);

NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
);

NTSTATUS WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
);

NTSTATUS
MmLockVaForWrite(
	PVOID Va,
	ULONG Length,
	PREPROTECT_CONTEXT ReprotectContext
);

NTSTATUS
MmUnlockVaForWrite(
	PREPROTECT_CONTEXT ReprotectContext
);