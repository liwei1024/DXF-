#include "pch.h"
#ifndef __ASTAR_H__
#include "../astar/astar.h"
#pragma comment(lib,"../x64/Release/library/astar.lib")
#endif // !__ASTAR_H__
#include "status_3.h"



DWORD status_3::getMapAddress()
{

	return memory::readOffset<int>(__人物基址, { __地图偏移 });
}

DWORD status_3::getMapStartAddress()
{
	return  memory::read<int>(getMapAddress() + __首地址);
}

int status_3::getMapObjectCount(DWORD map_start_address)
{
	return  (memory::read<int>(getMapAddress() + __尾地址) - map_start_address) / 4;
}

// 获取对象信息
MAP_OBJECT_STRUCT status_3::get_object_info(DWORD object_pointer)
{
	MAP_OBJECT_STRUCT object;
	DWORD pos_pointer;
	object.address = object_pointer;
	object.type = memory::read<DWORD>(object_pointer + __类型偏移);
	object.camp = memory::read<DWORD>(object_pointer + __阵营偏移);
	object.health_point = memory::read<DWORD>(object_pointer + __血量偏移);
	object.code = memory::read<DWORD>(object_pointer + __代码偏移);
	if (object.type == 289 && object.camp == 200)
	{
		object.name = memory::readWString(memory::read<DWORD>(memory::read<DWORD>(object_pointer + __地面物品名称偏移) + 0x24) + 0, 100);
	}
	else {
		object.name = memory::readWString(memory::read<DWORD>(object_pointer + __名称偏移), 100);
	}
	if (object.type == 273)
	{
		pos_pointer = memory::read<int>(object_pointer + __人物坐标偏移);
		object.x = (int)memory::read<float>(__角色坐标 + 0);
		object.y = (int)memory::read<float>(__角色坐标 + 4);
		object.z = (int)memory::read<float>(__角色坐标 + 8);
	}
	else {
		pos_pointer = memory::read<int>(object_pointer + __坐标偏移);
		object.x = (int)memory::read<float>(pos_pointer + 0x10);
		object.y = (int)memory::read<float>(pos_pointer + 0x14);
		object.z = (int)memory::read<float>(pos_pointer + 0x18);
	}
	return object;
}

// 打印副本对象信息
void status_3::outputMapObjectInfo()
{
	DWORD map_start_address = getMapStartAddress();
	utils::myprintf("map_start_address %x\n",RED, map_start_address);
	DWORD map_object_count = getMapObjectCount(map_start_address);
	utils::myprintf("map_object_count %d\n", RED, map_object_count);
	MAP_OBJECT_STRUCT _ObjectInfo;
	DWORD object_address;
	for (size_t i = 0; i < map_object_count; i++)
	{
		object_address = memory::read<int>(map_start_address + i * 4);
		if (object_address <= 0)continue;
		_ObjectInfo = get_object_info(object_address);
		utils::myprintf("=====================================");
		utils::myprintf("address 0x%x", RED, _ObjectInfo.address);
		utils::myprintf("code %d", RED, _ObjectInfo.code);
		utils::myprintf("type %d", RED, _ObjectInfo.type);
		utils::myprintf("camp %d", RED, _ObjectInfo.camp);
		utils::myprintf("health_point %d", RED, _ObjectInfo.health_point);
		utils::myprintf("pos %d,%d,%d", RED, _ObjectInfo.x, _ObjectInfo.y, _ObjectInfo.z);
		utils::myprintf("name %s", RED, _ObjectInfo.name.c_str());
	}
}