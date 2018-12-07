#include "rw4.h"

typedef struct _REPROTECT_CONTEXT
{
	PMDL   Mdl;
	PUCHAR LockedVa;
} REPROTECT_CONTEXT, *PREPROTECT_CONTEXT;

NTSTATUS
MmLockVaForWrite(
	__in PVOID Va,
	__in ULONG Length,
	__out PREPROTECT_CONTEXT ReprotectContext
)
{
	NTSTATUS Status;

	Status = STATUS_SUCCESS;

	ReprotectContext->Mdl = 0;
	ReprotectContext->LockedVa = 0;

	ReprotectContext->Mdl = IoAllocateMdl(
		Va,
		Length,
		FALSE,
		FALSE,
		0
	);

	if (!ReprotectContext->Mdl)
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	//
	// Retrieve a locked VA mapping.
	//

	__try
	{
		MmProbeAndLockPages(
			ReprotectContext->Mdl,
			KernelMode,
			IoModifyAccess
		);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return GetExceptionCode();
	}

	ReprotectContext->LockedVa = (PUCHAR)MmMapLockedPagesSpecifyCache(
		ReprotectContext->Mdl,
		KernelMode,
		MmCached,
		0,
		FALSE,
		NormalPagePriority
	);

	if (!ReprotectContext->LockedVa)
	{


		IoFreeMdl(
			ReprotectContext->Mdl
		);

		ReprotectContext->Mdl = 0;

		return STATUS_ACCESS_VIOLATION;
	}

	//
	// Reprotect.
	//

	Status = MmProtectMdlSystemAddress(
		ReprotectContext->Mdl,
		PAGE_EXECUTE_READWRITE
	);

	if (!NT_SUCCESS(Status))
	{


		MmUnmapLockedPages(
			ReprotectContext->LockedVa,
			ReprotectContext->Mdl
		);
		MmUnlockPages(
			ReprotectContext->Mdl
		);
		IoFreeMdl(
			ReprotectContext->Mdl
		);

		ReprotectContext->LockedVa = 0;
		ReprotectContext->Mdl = 0;
	}

	return Status;
}

NTSTATUS
MmUnlockVaForWrite(
	__in PREPROTECT_CONTEXT ReprotectContext
)
{
	if (ReprotectContext->LockedVa)
	{
		MmUnmapLockedPages(
			ReprotectContext->LockedVa,
			ReprotectContext->Mdl
		);
		MmUnlockPages(
			ReprotectContext->Mdl
		);
		IoFreeMdl(
			ReprotectContext->Mdl
		);

		ReprotectContext->LockedVa = 0;
		ReprotectContext->Mdl = 0;
	}

	return STATUS_SUCCESS;
}


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
		Status = MmCopyVirtualMemory(TargetProcess, (CONST PVOID)rvms->Address, PsGetCurrentProcess(), rvms->Response, rvms->Size, KernelMode, &Bytes);
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
		/*PMDL pMdl = NULL;
		PVOID pSafeAddress = NULL;
		if (!MmIsAddressValid((PVOID)wvms->Address) || !MmIsAddressValid((PVOID)wvms->Address))
			return Status;
		pMdl = IoAllocateMdl((PVOID)wvms->Address, (ULONG)wvms->Size, FALSE, FALSE, NULL);
		if (!pMdl)
			return Status;
		__try
		{
			MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			IoFreeMdl(pMdl);
			return Status;
		}
		pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
		if (!pSafeAddress)
			return Status;*/
		REPROTECT_CONTEXT ReprotectContext;
		MmLockVaForWrite((PVOID)wvms->Address, (ULONG)wvms->Size,&ReprotectContext);
		Status = MmCopyVirtualMemory(PsGetCurrentProcess(), wvms->Value, TargetProcess, (PVOID)ReprotectContext.LockedVa, wvms->Size, KernelMode, &Bytes);
		/*MmUnlockPages(pMdl);
		IoFreeMdl(pMdl);*/
		MmUnlockVaForWrite(&ReprotectContext);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}

HANDLE DriverAllocMemory(ULONG RegionSize)
{

	NTSTATUS Status;
	PEPROCESS TargetProcess;
	HANDLE BaseAddr = NULL;
	KAPC_STATE ApcState;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);

	if (NT_SUCCESS(Status))
	{

		KeStackAttachProcess((PKPROCESS)TargetProcess, &ApcState);
		__try
		{
			ZwAllocateVirtualMemory(NtCurrentProcess(),(PVOID*)&BaseAddr,0,(PSIZE_T)&RegionSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
			MmProtectMdlSystemAddress(BaseAddr, PAGE_EXECUTE_READWRITE);
			RtlZeroMemory(BaseAddr, RegionSize); //关键点
			//这里是关键点,内存清理后,然后ring0下再调用 VirtualProtectEx() 修改内存属性,这样申请到的内存,用之前的代码,就莫名其妙的就可以访问了
			//原因我不懂,之前因为申请到的内存,不能访问,我就用CE查看内存,然后发现用CE访问过后的内存就可以驱动读写了
			//后来我就胡乱测试,发现这个方法可以,ring0申请内存,然后清零,然后ring3修改内存属性,就可以访问
			KeUnstackDetachProcess(&ApcState);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&ApcState);
		}
		ObDereferenceObject(TargetProcess);
	}

	return BaseAddr;
}
HANDLE GetProcessHandle()
{
	NTSTATUS Status;
	PEPROCESS TargetProcess;
	HANDLE hProcess = NULL;
	UNICODE_STRING Unicode;
	PVOID PsProcessType;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);

	if (NT_SUCCESS(Status)) {
		RtlInitUnicodeString(&Unicode,L"PsProcessType");
		PsProcessType = MmGetSystemRoutineAddress(&Unicode);
		if (PsProcessType)
		{
			Status = ObOpenObjectByPointer(
				TargetProcess,
				0,
				NULL,
				PROCESS_ALL_ACCESS,
				*(POBJECT_TYPE*)PsProcessType,
				KernelMode,
				&hProcess
			);
		}
		ObDereferenceObject(TargetProcess);
	}
	return hProcess;
}