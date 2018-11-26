#include <ntdef.h>
#include <ntifs.h>
#include <time.h>
#include "rw.h"

//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye
//CHINAHuaiye CHINAHuaiye CHINAHuaiye

NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(__in PEPROCESS Process);

VOID NotifyImageLoadCallback(PUNICODE_STRING w_FullImageName, HANDLE w_ProcessId, PIMAGE_INFO w_ImageInfo)
{
	if (wcsstr(w_FullImageName->Buffer, L"YoudaoDict.exe") && wcsstr(w_FullImageName->Buffer, L"Device"))
	{
		PUID = w_ProcessId;
		PUBase = GetSectionBaseAddress(w_ProcessId);
		PUBGepGame = GetEpGameIdByProcess(w_ProcessId);
		//DbgPrint("PUBGepGame %x", PUBGepGame);
		//DbgPrint("w_ImageInfo->ImageBase %p", w_ImageInfo->ImageBase);

	}
}

DWORD64 GetSectionBaseAddress(HANDLE w_pid)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PEPROCESS targetProcess;
	PVOID value = 0;

	ntStatus = PsLookupProcessByProcessId((HANDLE)w_pid, &targetProcess);

	if (ntStatus != STATUS_SUCCESS || !targetProcess)
		return 0;

	__try
	{
		KeAttachProcess((PKPROCESS)targetProcess);
		value = PsGetProcessSectionBaseAddress(targetProcess);
		KeDetachProcess();
	}
	__except (GetExceptionCode())
	{
		return 0;
	}

	return (DWORD64)value;
}

PEPROCESS GetEpGameIdByProcess(HANDLE w_pid)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PEPROCESS targetProcess;

	ntStatus = PsLookupProcessByProcessId((HANDLE)w_pid, &targetProcess);

	if (ntStatus != STATUS_SUCCESS || !targetProcess)
		return 0;
	return targetProcess;
}

VOID Unload(PDRIVER_OBJECT  DriverObject)
{
	PsRemoveLoadImageNotifyRoutine(NotifyImageLoadCallback);

	UNICODE_STRING usDosDeviceName;

	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\CHINAHuaiye");
	IoDeleteSymbolicLink(&usDosDeviceName);

	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS ReadMemOutputClient(pReadStruct w_poReadStruct)
{
	PEPROCESS hClient, hGame;

	KAPC_STATE apc_state;
	NTSTATUS NtStatus = STATUS_SUCCESS;

	if (PUBase == 0 && w_poReadStruct->ProtocolMsg != PROTO_GET_BASEADDR)
	{
		return STATUS_UNSUCCESSFUL;
	}

	PsLookupProcessByProcessId(PUID, &hGame);
	PsLookupProcessByProcessId((HANDLE)w_poReadStruct->UserPID, &hClient);

	DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, w_poReadStruct->ReadSize, 0);

	if (DriverBuffer == NULL)
	{
		return STATUS_MEMORY_NOT_ALLOCATED;
	}

	if (w_poReadStruct->ProtocolMsg == PROTO_GET_BASEADDR)
	{
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((PVOID)w_poReadStruct->UserBufferAdress, &PUBase, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
	}

	else if (w_poReadStruct->ProtocolMsg == PROTO_NORMAL_READ)
	{
		__try
		{
			KeStackAttachProcess(hGame, &apc_state);
			ProbeForRead((PVOID)w_poReadStruct->GameAddressOffset, w_poReadStruct->ReadSize, sizeof(CHAR));
			PMDL pMdl;
			PUCHAR SrcAddress;
			if (MmIsAddressValid((PVOID)w_poReadStruct->GameAddressOffset))
			{
				pMdl = IoAllocateMdl((PVOID)w_poReadStruct->GameAddressOffset, (ULONG)w_poReadStruct->ReadSize, FALSE, FALSE, NULL);

				if (MmIsAddressValid(pMdl))
				{
					MmBuildMdlForNonPagedPool(pMdl);
					SrcAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
					if (MmIsAddressValid(SrcAddress))
						RtlCopyMemory(DriverBuffer, SrcAddress, w_poReadStruct->ReadSize);
					IoFreeMdl(pMdl);
				}
			}

			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((PVOID)w_poReadStruct->UserBufferAdress, DriverBuffer, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
	}
	else if (w_poReadStruct->ProtocolMsg == PROTO_NORMAL_WRITE)
	{
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory(DriverBuffer, (PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
		__try
		{
			KeStackAttachProcess(hGame, &apc_state);
			ProbeForRead((PVOID)w_poReadStruct->GameAddressOffset, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((PVOID)w_poReadStruct->GameAddressOffset, DriverBuffer, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}

	}
	ExFreePool(DriverBuffer);
	return NtStatus;
}

NTSTATUS WriteBufferedIO(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS NtStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	pReadStruct readStruct;

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);

	if (pIoStackIrp)
	{
		readStruct = (pReadStruct)Irp->AssociatedIrp.SystemBuffer;
		if (readStruct)
		{
			if (readStruct->ReadSize)
			{
				ReadMemOutputClient(readStruct);
			}
		}
	}

	return NtStatus;
}

NTSTATUS UnSupportedIrpFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS NtStatus = STATUS_NOT_SUPPORTED;
	return NtStatus;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  pDriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	PUBase = 0;

	NTSTATUS status = STATUS_SUCCESS;

	RtlInitUnicodeString(&dev, L"\\Device\\CHINAHuaiye");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\CHINAHuaiye");

	status = IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);

	if (status == STATUS_SUCCESS)
	{
		for (UINT32 uiIndex = 0; uiIndex < IRP_MJ_MAXIMUM_FUNCTION; uiIndex++)
		{
			pDriverObject->MajorFunction[uiIndex] = UnSupportedIrpFunction;
		}

		pDriverObject->MajorFunction[IRP_MJ_WRITE] = WriteBufferedIO;
		pDriverObject->DriverUnload = Unload;

		pDeviceObject->Flags |= DO_BUFFERED_IO;
		pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

		IoCreateSymbolicLink(&dos, &dev);
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}

	PsSetLoadImageNotifyRoutine(NotifyImageLoadCallback);

	return STATUS_SUCCESS;
}