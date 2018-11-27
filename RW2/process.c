#include "rw2.h"


NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess,ClientProcess;
	KAPC_STATE apc_state;
	PVOID DriverBuffer;
	PMDL pMdl;
	PUCHAR SrcAddress;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	
	if (NT_SUCCESS(Status))
	{
		DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, rvms->Size, 0);
		ClientProcess = PsGetCurrentProcess();
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
		__except (EXCEPTION_EXECUTE_HANDLER){
			KeUnstackDetachProcess(&apc_state);
			Status = STATUS_ABANDONED;
		}
		if (NT_SUCCESS(Status))
		{
			__try
			{
				KeStackAttachProcess(ClientProcess, &apc_state);
				ProbeForRead((PVOID)rvms->Response, rvms->Size, sizeof(CHAR));
				RtlCopyMemory((PVOID)rvms->Response, DriverBuffer, rvms->Size);
				KeUnstackDetachProcess(&apc_state);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				KeUnstackDetachProcess(&apc_state);
				Status = STATUS_ABANDONED;
			}
		}
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}

NTSTATUS WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess, ClientProcess;
	KAPC_STATE apc_state;
	PVOID DriverBuffer;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, wvms->Size, 0);
		ClientProcess = PsGetCurrentProcess();
		__try
		{
			KeStackAttachProcess(ClientProcess, &apc_state);
			ProbeForRead(wvms->Value, wvms->Size, sizeof(CHAR));
			RtlCopyMemory(DriverBuffer, wvms->Value, wvms->Size);
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
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}

