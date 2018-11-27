#include "rw2.h"

NTSTATUS DispatchDeviceControl(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PIO_STACK_LOCATION Stack;
	PREAD_VIRTUAL_MEMORY_STRUCT rvms;
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms;
	

	Stack = IoGetCurrentIrpStackLocation(Irp);

	if (Stack)
	{
		switch (Stack->Parameters.DeviceIoControl.IoControlCode)
		{
		case READ_VIRTUAL_MEMORY:
			{
				rvms = (PREAD_VIRTUAL_MEMORY_STRUCT)Irp->AssociatedIrp.SystemBuffer;
				if (rvms)
				{
					Status = ReadVirtualMemory(rvms);
				}
				Irp->IoStatus.Information = sizeof(READ_VIRTUAL_MEMORY_STRUCT);
			}
			break;
		case WRITE_VIRTUAL_MEMORY:
			{
				wvms = (PWRITE_VIRTUAL_MEMORY_STRUCT)Irp->AssociatedIrp.SystemBuffer;
				if (wvms)
				{
					Status = WriteVirtualMemory(wvms);
				}
				Irp->IoStatus.Information = sizeof(WRITE_VIRTUAL_MEMORY_STRUCT);
			}
			break;
		case GET_PROCESS_ID:
			{
				*(ULONG*)Irp->AssociatedIrp.SystemBuffer = (ULONG)(ULONG_PTR)TargetProcessId;
				Irp->IoStatus.Information = sizeof(ULONG);
			}
			break;
		case GET_PROCESS_BASE_ADDRESS:
			{
				*(ULONG*)Irp->AssociatedIrp.SystemBuffer = (ULONG)(ULONG_PTR)TargetProcessBaseAddress;
				Irp->IoStatus.Information = sizeof(ULONG);
			}
			break;
		default:
			Status = STATUS_INVALID_DEVICE_REQUEST;
			break;
		}
		
	}
	else {
		Status = STATUS_NOT_SUPPORTED;
	}
	Irp->IoStatus.Status = Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}