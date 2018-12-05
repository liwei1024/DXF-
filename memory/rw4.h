#ifndef __RW4_h__
#include <windows.h>
#include "../RW4/rw4api.h"
class RW4
{
private:
	HANDLE hDriver = NULL;
public:
	RW4();
	~RW4();
	DWORD dwProcessId;
	DWORD dwProcessBaseAddress;
	void Init();
	void closeHandle();
	DWORD getProcessId();
	DWORD getProcessBaseAddress();
	BOOL readVirtualMemory(ULONG Address, PVOID Response, SIZE_T Size);
	BOOL writeVirtualMemory(ULONG Address, PVOID Value, SIZE_T Size);
};
#endif // !__RW2_h__