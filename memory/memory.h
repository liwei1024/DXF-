#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifndef __RW2_h__
#include "rw2.h"
#endif
extern RW2 rw2;

namespace memory
{
	template<typename T>
	static T read(DWORD_PTR dwBaseAddress)
	{
		T Value;
		rw2.readVirtualMemory((ULONG)dwBaseAddress, &Value, sizeof(T));
		return Value;
	}

	template<typename T>
	static BOOL write(DWORD_PTR dwBaseAddress, T Value)
	{
		return rw2.writeVirtualMemory((ULONG)dwBaseAddress, &Value, sizeof(T));
	}

	template <typename T>
	static T readOffset(DWORD_PTR base_address, std::vector<int> offset)
	{
		DWORD ofset_address = read<DWORD>(base_address);
		T value;
		for (size_t i = 0; i < offset.size(); i++)
		{
			if (ofset_address > 0)
			{
				if ((i + 1) < offset.size())
				{
					ofset_address = read<DWORD>(ofset_address + offset[i]);
				}
				else {
					value = read<T>(ofset_address + offset[i]);
				}
			}
		}
		return value;
	}

	template <typename T>
	static bool writeOffset(DWORD_PTR base_address, std::vector<int> offset, T value)
	{
		DWORD ofset_address = read<DWORD>(base_address);
		bool result = false;

		for (size_t i = 0; i < offset.size(); i++)
		{
			if (ofset_address)
			{
				if ((i + 1) < offset.size())
				{
					ofset_address = read<DWORD>(ofset_address + offset[i]);
				}
				else {
					result = write<T>(ofset_address + offset[i], value);
				}
			}
		}

		return result;
	}

	std::wstring readWString(DWORD_PTR dwBaseAddress, SIZE_T Size);
	

	std::string readString(DWORD_PTR dwBaseAddress, SIZE_T Size);
	

	std::vector<byte> readBytes(DWORD_PTR dwBaseAddress, SIZE_T Size);
	

	BOOL writeBytes(DWORD_PTR dwBaseAddress, std::vector<byte> Bytes);
	
};
#endif // !__MEMORY_H__



