#ifndef __RW2_h__
#include <windows.h>
#include "../RW2/rw2api.h"
class RW2
{
private:
	HANDLE hDriver;
public:
	RW2();
	~RW2();
	DWORD dwProcessId;
	DWORD dwProcessBaseAddress;
	void Init();
	DWORD getProcessId();
	DWORD getProcessBaseAddress();
	BOOL readVirtualMemory(ULONG Address, PVOID Response, SIZE_T Size);
	BOOL writeVirtualMemory(ULONG Address, PVOID Value, SIZE_T Size);
};
#endif // !__RW2_h__


