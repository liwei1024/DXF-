#include "rw2.h"


VOID ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess,ClientProcess;
	KAPC_STATE apc_state;
	PVOID DriverBuffer;
	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, rvms->Size, 0);
	if (NT_SUCCESS(Status))
	{
		dprintf("PsLookupProcessByProcessId Success1");
		ClientProcess = PsGetCurrentProcess();
		__try {
			KeStackAttachProcess(TargetProcess, &apc_state);
			ProbeForRead((CONST PVOID)rvms->Address, rvms->Size, sizeof(CHAR));
			PMDL pMdl;
			PUCHAR SrcAddress;
			if (MmIsAddressValid((CONST PVOID)rvms->Address))
			{
				dprintf("MmIsAddressValid Success1");
				pMdl = IoAllocateMdl((CONST PVOID)rvms->Address, rvms->Size, FALSE, FALSE, NULL);

				if (MmIsAddressValid(pMdl))
				{
					dprintf("MmIsAddressValid Success2");
					MmBuildMdlForNonPagedPool(pMdl);
					SrcAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
					if (MmIsAddressValid(SrcAddress)) {
						dprintf("MmIsAddressValid Success3");
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
		__try
		{
			KeStackAttachProcess(ClientProcess, &apc_state);
			ProbeForRead((CONST PVOID)rvms->Response, rvms->Size, sizeof(CHAR));
			RtlCopyMemory(rvms->Response, DriverBuffer, rvms->Size);
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

VOID WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
)
{
	/*NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess, ClientProcess;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		ClientProcess = PsGetCurrentProcess();
		Status = MmCopyVirtualMemory(TargetProcess, wvms->Response, PsGetCurrentProcess(), wvms->Address, wvms->Size, KernelMode, NULL);
		ObDereferenceObject(TargetProcess);
	}*/
}

