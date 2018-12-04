#include "rw3.h"
NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess, ClientProcess;
	HANDLE ClientProcessId;
	KAPC_STATE apc_state;
	PVOID DriverBuffer = NULL;
	PMDL pMdl;
	PUCHAR SrcAddress;
	

	ClientProcessId = PsGetCurrentProcessId();
	DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, rvms->Size, 0);

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status)) {
		__try {
			KeStackAttachProcess(TargetProcess, &apc_state);
			ProbeForRead((PVOID)rvms->Address, rvms->Size, sizeof(CHAR));
			if (MmIsAddressValid((PVOID)rvms->Address))
			{
				pMdl = IoAllocateMdl((PVOID)rvms->Address, (ULONG)rvms->Size, FALSE, FALSE, NULL);

				if (MmIsAddressValid(pMdl))
				{
					MmBuildMdlForNonPagedPool(pMdl);
					SrcAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
					if (MmIsAddressValid(SrcAddress)) {
						RtlCopyMemory(DriverBuffer, SrcAddress, rvms->Size);
					}
					IoFreeMdl(pMdl);
				}
			}
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			KeUnstackDetachProcess(&apc_state);
			Status = STATUS_ABANDONED;
		}
		ObDereferenceObject(TargetProcess);
	}
	Status = PsLookupProcessByProcessId(ClientProcessId, &ClientProcess);
	if (NT_SUCCESS(Status))
	{
		
		__try
		{
			KeStackAttachProcess(ClientProcess, &apc_state);
			ProbeForRead((void*)rvms->Response, rvms->Size, sizeof(CHAR));
			RtlCopyMemory((void*)rvms->Response, DriverBuffer, rvms->Size);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			Status = STATUS_ABANDONED;
		}
		ObDereferenceObject(ClientProcess);
	}
	ExFreePool(DriverBuffer);
	return Status;
}

NTSTATUS WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess, ClientProcess;
	KAPC_STATE apc_state;
	PVOID DriverBuffer = NULL;


	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, wvms->Size, 0);
		ClientProcess = PsGetCurrentProcess();
		__try
		{
			KeStackAttachProcess(ClientProcess, &apc_state);
			ProbeForRead((PVOID)wvms->Value, wvms->Size, sizeof(CHAR));
			RtlCopyMemory(DriverBuffer, (PVOID)wvms->Value, wvms->Size);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			Status = STATUS_ABANDONED;
		}
		__try
		{
			KeStackAttachProcess(TargetProcess, &apc_state);
			ProbeForRead((PVOID)wvms->Address, wvms->Size, sizeof(CHAR));
			RtlCopyMemory((PVOID)wvms->Address, DriverBuffer, wvms->Size);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			Status = STATUS_ABANDONED;
		}
		ExFreePool(DriverBuffer);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}