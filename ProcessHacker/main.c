#include "ph.h"

UNICODE_STRING SymbolicLinkName;
HANDLE GameProcessId;
PVOID GameImageBase;


NTSTATUS DefaultDispatchFunction(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID NotifyImageLoadCallback(
	_In_opt_ PUNICODE_STRING FullImageName,
	_In_ HANDLE ProcessId,                // pid into which image is being mapped
	_In_ PIMAGE_INFO ImageInfo
)
{
	if (wcsstr(FullImageName->Buffer, L"YoudaoDict.exe") && wcsstr(FullImageName->Buffer, L"Device"))
	{
		dprintf("GameName %ls\n", FullImageName->Buffer);
		dprintf("GameProcessId %d\n", ProcessId);
		dprintf("GameImageBase %x\n", ImageInfo->ImageBase);
		GameProcessId = ProcessId;
		GameImageBase = ImageInfo->ImageBase;
	}
}

VOID DriverUnload(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	if (!NT_SUCCESS(PsRemoveLoadImageNotifyRoutine(NotifyImageLoadCallback)))
	{
		dprintf("Delete NotifyImageLoadCallback Failure");
	}
	if (!NT_SUCCESS(IoDeleteSymbolicLink(&SymbolicLinkName)))
	{
		dprintf("Delete SymbolicLinkName Failure");
	}
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName;

	RtlInitUnicodeString(&DeviceName,DEVICE_NAME);
	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		dprintf("Create DeviceObject Failure");
		return status;
	}
	RtlInitUnicodeString(&SymbolicLinkName, SYMBOLIC_LINK_NAME);
	status = IoCreateSymbolicLink(&SymbolicLinkName,&DeviceName);
	if (!NT_SUCCESS(status))
	{
		dprintf("Create SymbolicLink Failure");
		IoDeleteDevice(DeviceObject);
		return status;
	}

	for (size_t i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatchFunction;
	}
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->Flags &= ~DO_DEVICE_INITIALIZING;

	PsSetLoadImageNotifyRoutine(NotifyImageLoadCallback);

	return status;
}


