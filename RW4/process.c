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
		Status = MmCopyVirtualMemory(PsGetCurrentProcess(), wvms->Value, TargetProcess, (PVOID)wvms->Address, wvms->Size, KernelMode, &Bytes);
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