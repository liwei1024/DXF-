#include "ph.h"

NTSTATUS ReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	SIZE_T ReturnSize = 0;
	if (NT_SUCCESS(MmCopyVirtualMemory(Process, SourceAddress, PsGetCurrentProcess(),TargetAddress, Size, KernelMode, &ReturnSize)))
		return STATUS_SUCCESS;
	else
		return STATUS_ACCESS_DENIED;
}

NTSTATUS WriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	SIZE_T ReturnSize = 0;
	if (NT_SUCCESS(MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, Process,TargetAddress, Size, KernelMode, &ReturnSize)))
		return STATUS_SUCCESS;
	else
		return STATUS_ACCESS_DENIED;
}