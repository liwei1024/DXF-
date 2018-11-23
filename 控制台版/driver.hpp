#pragma once
//using std::string;
using byte = BYTE;
using uint64_t = ULONGLONG;
using int64_t = LONGLONG;
using uint32_t = UINT32;
using int32_t = INT32;
using uint16_t = USHORT;
using int16_t = SHORT;

enum PROTO_MESSAGE
{
	PROTO_GET_Base = 0,
	PROTO_NORMAL_READ = 1,
	PROTO_NORMAL_WRITE = 2,
	PROTO_Protect_Process = 3,
	PROTO_UnProtect_Process = 4
};

typedef struct WriteStruct
{
	DWORD_PTR UserBufferAdress;
	DWORD_PTR GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	UINT32 ProtocolMsg;
} WriteStruct, *pWriteStruct;

typedef struct ReadStruct
{
	DWORD_PTR UserBufferAdress;
	DWORD_PTR GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	UINT32 ProtocolMsg;
} ReadStruct, *pReadStruct;

typedef struct Struct
{
	DWORD_PTR UserBufferAdress; //缓冲区地址//传入进程的指针写进去
	DWORD_PTR GameAddressOffset; //游戏地址
	ULONGLONG ReadWriteSize; //读写大小
	ULONG UserPID; //进程ID(自己的)
	ULONG GamePID; //游戏进程ID
	ULONG uStatusCode; //状态码
	UINT32 ProtocolMsg; //消息格式
} IoStruct, *pIoStruct;

class Driver
{
public:
	Driver() : hDriver(0)
	{
		hDriver = CreateFile(
			L"\\\\.\\CHINAHuaiye",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			0,
			0
		);
	}

	~Driver() {}

	int64_t 取进程主模块地址()
	{
		DWORD_PTR writeMe = 0;
		DWORD dw;
		ReadStruct rStruct{ (DWORD_PTR)&writeMe, NULL, sizeof(DWORD_PTR), (uint32_t)GetCurrentProcessId(), 0, NULL, PROTO_GET_Base };
		WriteFile(hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dw, NULL);
		return writeMe;
	}

	template<typename T> bool WPM(DWORD_PTR w_write, T value) const
	{
		T readMe = value;
		DWORD dw;
		WriteStruct rStruct{ (DWORD_PTR)&readMe, (DWORD_PTR)w_write, sizeof(T), (uint32_t)GetCurrentProcessId(), 0, NULL, PROTO_NORMAL_WRITE };
		WriteFile(hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dw, NULL);
		return true;
	}

	template<typename T> T RPM(DWORD_PTR addr) const
	{
		T writeMe;
		DWORD dw;
		ReadStruct rStruct{ (DWORD_PTR)&writeMe, (DWORD_PTR)addr, sizeof(T), (uint32_t)GetCurrentProcessId(), 0, NULL, PROTO_NORMAL_READ };
		WriteFile(hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dw, NULL);
		return writeMe;
	}

	bool WBT(DWORD_PTR w_write, byte* value, const int32_t w_readSize) const
	{
		byte* readMe = value;
		DWORD dw;
		ReadStruct rStruct{ (uint64_t)readMe, (uint64_t)w_write, ULONGLONG(w_readSize), (uint32_t)GetCurrentProcessId(), 0, TRUE, PROTO_NORMAL_WRITE };
		WriteFile(hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dw, NULL);
		return readMe;
	}

	byte* ReadSize(DWORD_PTR addr, const int32_t w_readSize) const
	{
		byte* writeMe = new byte[w_readSize];
		DWORD dw;
		ReadStruct rStruct{ (uint64_t)writeMe, (uint64_t)addr, ULONGLONG(w_readSize - 2), (uint32_t)GetCurrentProcessId(), 0, TRUE, PROTO_NORMAL_READ };
		WriteFile(hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dw, NULL);
		return writeMe;
	}

	void ReadStr(DWORD_PTR addr, wchar_t* buff, DWORD bufSize) const
	{
		DWORD dw;
		ReadStruct luku{ (uint64_t)buff, (uint64_t)addr, bufSize - 2, (uint32_t)GetCurrentProcessId(), 0, TRUE, PROTO_NORMAL_READ };
		WriteFile(hDriver, (LPCVOID)&luku, sizeof(ReadStruct), &dw, NULL);
	}

	HANDLE GetDriverHandle() const
	{
		return hDriver;
	}

private:
	HANDLE hDriver;
};


