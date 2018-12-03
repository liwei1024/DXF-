#include "rw2.h"

////去掉页面保护
//void  WPOFF(void)
//{
//
//#ifdef _WIN64
//
//	_disable();
//	DWORD64 cr0 = __readcr0();
//	cr0 &= 0xfffffffffffeffff;
//	__writecr0(cr0);
//	//	_enable();
//
//#else
//	__asm
//	{
//		cli
//		mov eax, cr0
//		and eax, not 10000h
//		mov cr0, eax
//	}
//#endif
//}
//
////设置页面保护
//void  WPON(void)
//{
//#ifdef _WIN64
//	_disable();
//	DWORD64 cr0 = __readcr0();
//	cr0 |= 0x10000;
//	__writecr0(cr0);
//#else
//	__asm
//	{
//		mov eax, cr0
//		or eax, 10000h
//		mov cr0, eax
//		sti
//	}
//#endif
//}

NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS TargetProcess,ClientProcess;
	KAPC_STATE apc_state;
	PVOID DriverBuffer = NULL;
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
		ExFreePool(DriverBuffer);
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
	PVOID DriverBuffer = NULL;
	
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
		ExFreePool(DriverBuffer);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}


////修改内存属性（有问题）
//NTSTATUS
//MmLockVaForWrite(
//	__in PVOID Va,
//	__in ULONG Length,
//	__out PREPROTECT_CONTEXT ReprotectContext
//)
//{
//	NTSTATUS Status;
//
//	Status = STATUS_SUCCESS;
//
//	ReprotectContext->Mdl = 0;
//	ReprotectContext->LockedVa = 0;
//
//	ReprotectContext->Mdl = IoAllocateMdl(
//		Va,
//		Length,
//		FALSE,
//		FALSE,
//		0
//	);
//
//	if (!ReprotectContext->Mdl)
//	{
//		return STATUS_INSUFFICIENT_RESOURCES;
//	}
//
//	//  
//	// Retrieve a locked VA mapping.  
//	//  
//
//	__try
//	{
//		MmProbeAndLockPages(
//			ReprotectContext->Mdl,
//			KernelMode,
//			IoModifyAccess
//		);
//	}
//	__except (EXCEPTION_EXECUTE_HANDLER)
//	{
//		return GetExceptionCode();
//	}
//
//	ReprotectContext->LockedVa = (PUCHAR)MmMapLockedPagesSpecifyCache(
//		ReprotectContext->Mdl,
//		KernelMode,
//		MmCached,
//		0,
//		FALSE,
//		NormalPagePriority
//	);
//
//	if (!ReprotectContext->LockedVa)
//	{
//		IoFreeMdl(
//			ReprotectContext->Mdl
//		);
//
//		ReprotectContext->Mdl = 0;
//
//		return STATUS_ACCESS_VIOLATION;
//	}
//	//  
//	// Reprotect.  
//	//  
//
//	Status = MmProtectMdlSystemAddress(
//		ReprotectContext->Mdl,
//		PAGE_EXECUTE_READWRITE
//	);
//
//	if (!NT_SUCCESS(Status))
//	{
//		MmUnmapLockedPages(
//			ReprotectContext->LockedVa,
//			ReprotectContext->Mdl
//		);
//		MmUnlockPages(
//			ReprotectContext->Mdl
//		);
//		IoFreeMdl(
//			ReprotectContext->Mdl
//		);
//
//		ReprotectContext->LockedVa = 0;
//		ReprotectContext->Mdl = 0;
//	}
//
//	return Status;
//}
////还原内存属性（有问题）
//NTSTATUS
//MmUnlockVaForWrite(
//	__in PREPROTECT_CONTEXT ReprotectContext
//)
//{
//	if (ReprotectContext->LockedVa)
//	{
//		MmUnmapLockedPages(
//			ReprotectContext->LockedVa,
//			ReprotectContext->Mdl
//		);
//		MmUnlockPages(
//			ReprotectContext->Mdl
//		);
//		IoFreeMdl(
//			ReprotectContext->Mdl
//		);
//
//		ReprotectContext->LockedVa = 0;
//		ReprotectContext->Mdl = 0;
//	}
//
//	return STATUS_SUCCESS;
//}




