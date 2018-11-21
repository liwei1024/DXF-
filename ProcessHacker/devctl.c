#include "ph.h"

NTSTATUS DispatchDeviceControl(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS Status;
	ULONG BytesIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

	// Code received from user space
	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	if (ControlCode == READ_VIRTUAL_MEMORY)
	{
		// Get the input buffer & format it to our struct
		PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PKERNEL_READ_REQUEST ReadOutput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;

		PEPROCESS Process;
		// Get our process
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessId, &Process)))
			ReadVirtualMemory(Process, (PVOID)ReadInput->Address,&ReadInput->Response, ReadInput->Size);

		DbgPrintEx(0, 0, "Read Params:  %lu, %#010x \n", ReadInput->ProcessId, ReadInput->Address);
		DbgPrintEx(0, 0, "Value: %lu \n", ReadInput->Response);
		DbgPrintEx(0, 0, "Value: %lu \n", ReadOutput->Response);

		Status = STATUS_SUCCESS;
		BytesIO = sizeof(KERNEL_READ_REQUEST);
	}
	else if (ControlCode == WRITE_VIRTUAL_MEMORY)
	{
		// Get the input buffer & format it to our struct
		PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;

		PEPROCESS Process;
		// Get our process
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)WriteInput->ProcessId, &Process)))
			WriteVirtualMemory(Process, &WriteInput->Value,
			(PVOID)WriteInput->Address, WriteInput->Size);

		DbgPrintEx(0, 0, "Write Params:  %lu, %#010x \n", WriteInput->Value, WriteInput->Address);

		Status = STATUS_SUCCESS;
		BytesIO = sizeof(KERNEL_WRITE_REQUEST);
	}
	else if (ControlCode == GET_PROCESS_ID)
	{
		PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = (ULONG)GameProcessId;

		DbgPrintEx(0, 0, "id get %#010x", GameProcessId);

		Status = STATUS_SUCCESS;
		BytesIO = sizeof(*OutPut);
	}
	else if (ControlCode == Get_PROCESS_BASE_ADDRESS)
	{
		PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = (ULONG)GameImageBase;

		DbgPrintEx(0, 0, "Module get %#010x", GameImageBase);

		Status = STATUS_SUCCESS;
		BytesIO = sizeof(*OutPut);
	}
	else
	{
		// if the code is unknown
		Status = STATUS_INVALID_PARAMETER;
		BytesIO = 0;
	}

	// Complete the request
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = BytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}