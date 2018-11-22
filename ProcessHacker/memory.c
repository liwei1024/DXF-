#include "ph.h"

NTSTATUS ReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	SIZE_T ReturnSize = 0;
	/*dprintf("SourceAddress %x", SourceAddress);
	dprintf("TargetAddress %x", TargetAddress);
	dprintf("Size %x", Size);
	dprintf("PsGetCurrentProcess %x", PsGetCurrentProcess());
	dprintf("Process %x", Process);*/
	return MmCopyVirtualMemory(Process, SourceAddress, PsGetCurrentProcess(), TargetAddress, Size, KernelMode, &ReturnSize);
}

NTSTATUS WriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	SIZE_T ReturnSize = 0;
	/*dprintf("SourceAddress %x", SourceAddress);
	dprintf("TargetAddress %x", TargetAddress);
	dprintf("Size %x", Size);
	dprintf("PsGetCurrentProcess %x", PsGetCurrentProcess());
	dprintf("Process %x", Process);*/
	return MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, Process, TargetAddress, Size, KernelMode, &ReturnSize);
}