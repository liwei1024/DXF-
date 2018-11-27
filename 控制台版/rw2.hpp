#pragma once
#include "../RW2/rw2api.h"

class RW2
{
private:
	HANDLE hDriver;
public:
	DWORD dwProcessId;
	DWORD_PTR dwProcessBaseAddress;
	void Init()
	{
		hDriver = CreateFile(
			L"\\\\.\\" SYMBOLIC_LINK_SHORT_NAME,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			0,
			0
		);
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, L"#驱动链接失败,解决办法如下\n\n#家庭用户请关闭安全软件\n#网吧请干掉网吧防火墙回调\n#重新获取云驱动并绑定独立云驱动\n#把辅助放置非系统盘目录下管理员身份运行\n#如果以上方法没有解决您的问题请重启电脑管理员身份运行辅助", NULL, NULL);
			exit(0);
		}
		dwProcessId = getProcessId();
		printf("dwProcessId %d\n", dwProcessId);
		dwProcessBaseAddress = getProcessBaseAddress();
		printf("dwProcessBaseAddress %llx\n", dwProcessBaseAddress);
	}
	
	DWORD getProcessId()
	{
		ULONG pid;
		DeviceIoControl(hDriver, GET_PROCESS_ID, &pid, sizeof(pid), &pid, sizeof(pid), 0, 0);
		return pid;
	}

	DWORD_PTR getProcessBaseAddress()
	{
		ULONG baseAddress;
		DeviceIoControl(hDriver, GET_PROCESS_BASE_ADDRESS, &baseAddress, sizeof(baseAddress), &baseAddress, sizeof(baseAddress), 0, 0);
		return baseAddress;
	}

	template<typename T>
	T rvm(DWORD_PTR dwBaseAddress)
	{
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		T Response;
		rvms.Response = &Response;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = sizeof(T);
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		return Response;
	}

	template<typename T>
	BOOL wvm(DWORD_PTR dwBaseAddress, T Value)
	{
		WRITE_VIRTUAL_MEMORY_STRUCT wvms;
		wvms.Address = (ULONG)dwBaseAddress;
		wvms.Value = &Value;
		wvms.Size = sizeof(T);
		return DeviceIoControl(hDriver, WRITE_VIRTUAL_MEMORY, &wvms, sizeof(wvms), &wvms, sizeof(wvms), 0, 0);
	}

	std::wstring read_wstring(DWORD_PTR dwBaseAddress,SIZE_T length)
	{
		wchar_t *buffer = new wchar_t[length];
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		rvms.Response = buffer;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = length - 2;
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		return std::wstring(buffer, rvms.Size);
	}
};