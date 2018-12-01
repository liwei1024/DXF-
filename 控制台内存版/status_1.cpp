#include "pch.h"
#include "status_1.h"
#include "role.h"
#include "function.h"
#include "call.h"
#include "send_packet.h"

void status_1::manage()
{
	moveToCopyDoor(104);
	enterCopy();
}

CITY_INFO status_1::getCityInfo()
{
	CITY_INFO city_info;
	city_info.room = function::getCurrentRoomPos();
	city_info.x = (int)memory::read<float>(__角色坐标);
	city_info.y = (int)memory::read<float>(__角色坐标 + 4);
	city_info.z = (int)memory::read<float>(__角色坐标 + 8);
	return city_info;
}

void status_1::moveToCopyDoor(int copy_id)
{
	CITY_INFO city_info;
	CITY_INFO current_city_info = getCityInfo();
	call::区域Call(&city_info,copy_id);
	if (city_info.room.x != current_city_info.room.x || city_info.room.y != current_city_info.room.y)
	{
		SendPacket().城镇瞬移(city_info);
	}
}

void status_1::enterCopy()
{
	SendPacket().进入选图();
	Sleep(1000);
}

