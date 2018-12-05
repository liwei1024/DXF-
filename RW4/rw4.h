#ifndef __RW4_H__
#define __RW4_H__
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include "rw4api.h"

#ifndef dprintf
#define dprintf(Format, ...) DbgPrint("RW4: " Format "\n", __VA_ARGS__)
#endif // !dprintf

extern HANDLE TargetProcessId;
extern PVOID TargetProcessBaseAddress;

NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	CONST VOID * SourceAddress,
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

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE,MmCopyVirtualMemory)
#pragma alloc_text(PAGE,DispatchDeviceControl)
#pragma alloc_text(PAGE,ReadVirtualMemory)
#pragma alloc_text(PAGE,WriteVirtualMemory)
#endif

#endif // !1