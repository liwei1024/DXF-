

#include "memory.h"

RW2 rw2;

std::wstring memory::readWString(DWORD_PTR dwBaseAddress, SIZE_T Size)
{
	wchar_t *buffer = new wchar_t[Size];
	rw2.readVirtualMemory((ULONG)dwBaseAddress, buffer, Size);
	std::wstring wstr(buffer, Size);
	delete[]buffer;
	return wstr;
}

std::string memory::readString(DWORD_PTR dwBaseAddress, SIZE_T Size)
{
	char *buffer = new char[Size];
	rw2.readVirtualMemory((ULONG)dwBaseAddress, buffer, Size);
	std::string str(buffer, Size);
	delete[]buffer;
	return str;
}

std::vector<byte> memory::readBytes(DWORD_PTR dwBaseAddress, SIZE_T Size)
{
	std::vector<byte> bytes;
	byte * buffer = new byte[Size];
	rw2.readVirtualMemory((ULONG)dwBaseAddress, buffer, Size);
	for (size_t i = 0; i < Size; i++)
	{
		bytes.insert(bytes.end(), buffer[i]);
	}
	delete[]buffer;
	return bytes;
}

BOOL memory::writeBytes(DWORD_PTR dwBaseAddress, std::vector<byte> Bytes)
{
	byte * buffer = new byte[Bytes.size()];
	BOOL result = TRUE;
	for (size_t i = 0; i < Bytes.size(); i++)
	{
		buffer[i] = Bytes[i];
	}
	result = rw2.writeVirtualMemory((ULONG)dwBaseAddress, buffer, Bytes.size());
	delete[]buffer;
	return result;
}

