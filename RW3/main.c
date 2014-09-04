#include "rw3.h"
UNICODE_STRING SymbolicLinkName;
HANDLE TargetProcessId = 0;
PVOID TargetProcessBaseAddress = NULL;

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
	if (wcsstr(FullImageName->Buffer, TARGET_PROCESS_NAME) && wcsstr(FullImageName->Buffer, L"Device"))
	{
		dprintf("TargetName %ls\n", FullImageName->Buffer);
		dprintf("TargetProcessId %d\n", ProcessId);
		dprintf("TargetImageBase %x\n", ImageInfo->ImageBase);
		TargetProcessId = ProcessId;
		TargetProcessBaseAddress = ImageInfo->ImageBase;
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
	NTSTATUS Status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName;
	// 创建虚拟设备
	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
	Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create DeviceObject Failure");
		return Status;
	}
	// 创建设备的符号链接
	RtlInitUnicodeString(&SymbolicLinkName, SYMBOLIC_LINK_NAME);
	Status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create SymbolicLink Failure");
		IoDeleteDevice(DeviceObject);
		return Status;
	}

	// 处理Irp请求
	for (size_t i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatchFunction;
	}
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = DriverUnload;

	DriverObject->Flags |= DO_BUFFERED_IO;
	DeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
	// 镜像加载通知回调函数
	Status = PsSetLoadImageNotifyRoutine(NotifyImageLoadCallback);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create LoadImageNotifyRoutine Failure");
		IoDeleteSymbolicLink(&SymbolicLinkName);
		IoDeleteDevice(DeviceObject);
	}
	return Status;
}