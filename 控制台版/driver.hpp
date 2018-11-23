#pragma once

#include "../ProcessHacker/phapi.h"
extern HANDLE hDriver;

static HANDLE GetDriverHandle() {
	HANDLE DriverHandle = NULL;
	DriverHandle = CreateFileW(
		L"\\\\.\\" SYMBOLIC_LINK_SHORT_NAME,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		0
	);
	if (DriverHandle == INVALID_HANDLE_VALUE)
	{
		utils::myprintf("驱动句柄创建失败\n");
	}
	return DriverHandle;
}
static DWORD GetTargetProcessId()
{
	ULONG TargetProcessId = 0;
	if (DeviceIoControl(hDriver, IO_GET_PROCESS_ID, &TargetProcessId, sizeof(TargetProcessId), &TargetProcessId, sizeof(TargetProcessId), NULL, NULL) == FALSE)
		utils::myprintf("获取进程ID失败\n");
	return TargetProcessId;
}
static DWORD GetTargetProcessBaseAddress()
{
	ULONG TargetProcessBaseAddress;
	if (DeviceIoControl(hDriver, IO_GET_PROCESS_BASE_ADDRESS, &TargetProcessBaseAddress, sizeof(TargetProcessBaseAddress), &TargetProcessBaseAddress, sizeof(TargetProcessBaseAddress), NULL, NULL) == FALSE)
		utils::myprintf("获取进程基址失败\n");
	return TargetProcessBaseAddress;
}
template<typename T>static T RVM(DWORD_PTR dwBaseAddress)
{
	READ_VIRTUAL_MEMORY ReadVirtualMemoryStruct;

	ReadVirtualMemoryStruct.ProcessId = GetTargetProcessId();
	ReadVirtualMemoryStruct.Address = (ULONG)dwBaseAddress;
	ReadVirtualMemoryStruct.Size = sizeof(T);

	// send code to our driver with the arguments
	if (DeviceIoControl(hDriver, IO_READ_VIRTUAL_MEMORY, &ReadVirtualMemoryStruct, sizeof(ReadVirtualMemoryStruct), &ReadVirtualMemoryStruct, sizeof(ReadVirtualMemoryStruct), 0, 0))
		return (T)ReadVirtualMemoryStruct.Response;
	else
		return T();
}
template<typename T>static bool WVM(DWORD_PTR dwBaseAddress, T Value)
{
	WRITE_VIRTUAL_MEMORY  WriteVirtualMemoryStruct;

	WriteVirtualMemoryStruct.ProcessId = GetTargetProcessId();
	WriteVirtualMemoryStruct.Address = dwBaseAddress;
	WriteVirtualMemoryStruct.Value = Value;
	WriteVirtualMemoryStruct.Size = sizeof(T);

	if (DeviceIoControl(hDriver, IO_WRITE_VIRTUAL_MEMORY, &WriteVirtualMemoryStruct, sizeof(WriteVirtualMemoryStruct), 0, 0, NULL, NULL))
		return true;
	else
		return false;
}

