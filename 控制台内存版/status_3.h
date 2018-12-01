#pragma once

struct MAP_OBJECT_STRUCT
{
	DWORD address;
	std::wstring name;
	DWORD type;
	DWORD camp;
	DWORD health_point;
	DWORD code;
	int x;
	int y;
	int z;
};

namespace status_3
{
	DWORD getMapAddress();
	DWORD getMapStartAddress();
	int getMapObjectCount(DWORD map_start_address);
	MAP_OBJECT_STRUCT get_object_info(DWORD object_pointer);
	void outputMapObjectInfo();
};

