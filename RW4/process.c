#include "rw4.h"
NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	SIZE_T Bytes;
	PEPROCESS TargetProcess;
	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		Status = MmCopyVirtualMemory(TargetProcess, (PVOID)rvms->Address, PsGetCurrentProcess(), rvms->Response, rvms->Size, KernelMode, &Bytes);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
	
}

NTSTATUS WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	SIZE_T Bytes;
	PEPROCESS TargetProcess;
	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		Status = MmCopyVirtualMemory(PsGetCurrentProcess(), wvms->Value, TargetProcess, (PVOID)wvms->Address, wvms->Size, KernelMode, &Bytes);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}