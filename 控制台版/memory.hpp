#pragma once

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

static std::wstring readWString(DWORD_PTR dwBaseAddress, SIZE_T Size)
{
	wchar_t *buffer = new wchar_t[Size];
	rw2.readVirtualMemory((ULONG)dwBaseAddress, buffer, Size);
	std::wstring wstr(buffer, Size);
	delete[]buffer;
	return wstr;
}

static std::string readString(DWORD_PTR dwBaseAddress, SIZE_T Size)
{
	char *buffer = new char[Size];
	rw2.readVirtualMemory((ULONG)dwBaseAddress, buffer, Size);
	std::string str(buffer, Size);
	delete[]buffer;
	return str;
}

static std::vector<byte> readBytes(DWORD_PTR dwBaseAddress, SIZE_T Size)
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

static BOOL writeBytes(DWORD_PTR dwBaseAddress, std::vector<byte> Bytes)
{
	byte * buffer = new byte[Bytes.size()];
	for (size_t i = 0; i < Bytes.size(); i++)
	{
		buffer[i] = Bytes[i];
	}
	BOOL result = rw2.writeVirtualMemory((ULONG)dwBaseAddress, buffer, Bytes.size());
	delete[]buffer;
	if (result == FALSE)
	{
		printf("write_bytes false");
	}
	return result;
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