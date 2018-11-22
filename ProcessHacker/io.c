#include "ph.h"

NTSTATUS IoDispatchDeviceControl(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	ULONG BytesIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	if (stack)
	{
		PREAD_VIRTUAL_MEMORY ReadVirtualMemoryStruct;
		PWRITE_VIRTUAL_MEMORY WriteVirtualMemoryStruct;
		PEPROCESS TargetProcess;

		switch (stack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IO_READ_VIRTUAL_MEMORY:
			ReadVirtualMemoryStruct = (PREAD_VIRTUAL_MEMORY)Irp->AssociatedIrp.SystemBuffer;
			if (ReadVirtualMemoryStruct && ReadVirtualMemoryStruct->Size > 0)
			{
				Status = PsLookupProcessByProcessId((HANDLE)ReadVirtualMemoryStruct->ProcessId, &TargetProcess);
				if (NT_SUCCESS(Status))
				{
					Status = ReadVirtualMemory(TargetProcess, (PVOID)ReadVirtualMemoryStruct->Address, &ReadVirtualMemoryStruct->Response, ReadVirtualMemoryStruct->Size);
					ObDereferenceObject(TargetProcess);
				}
			}
			else {
				Status = STATUS_NOT_SUPPORTED;
			}
			BytesIO = sizeof(READ_VIRTUAL_MEMORY);
			break;
		case IO_WRITE_VIRTUAL_MEMORY:
			WriteVirtualMemoryStruct = (PWRITE_VIRTUAL_MEMORY)Irp->AssociatedIrp.SystemBuffer;
			if (WriteVirtualMemoryStruct && WriteVirtualMemoryStruct->Size > 0)
			{
				Status = PsLookupProcessByProcessId((HANDLE)WriteVirtualMemoryStruct->ProcessId, &TargetProcess);
				if (NT_SUCCESS(Status))
				{
					Status = WriteVirtualMemory(TargetProcess, &WriteVirtualMemoryStruct->Value, (PVOID)WriteVirtualMemoryStruct->Address, WriteVirtualMemoryStruct->Size);
					ObDereferenceObject(TargetProcess);
				}
			}
			else {
				Status = STATUS_NOT_SUPPORTED;
			}
			BytesIO = sizeof(WRITE_VIRTUAL_MEMORY);
			break;
		case IO_GET_PROCESS_ID:
			*(ULONG*)Irp->AssociatedIrp.SystemBuffer = (ULONG)TargetProcessId;
			BytesIO = sizeof(ULONG);
			break;
		case IO_GET_PROCESS_BASE_ADDRESS:
			*(ULONG*)Irp->AssociatedIrp.SystemBuffer = (ULONG)TargetProcessBaseAddress;
			BytesIO = sizeof(ULONG);
			break;
		default:
			break;
		}
	}
	// Complete the request
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = BytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

NTSTATUS IoReadRequest(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}

NTSTATUS IoWriteRequest(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}