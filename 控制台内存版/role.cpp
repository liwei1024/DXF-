#include "pch.h"
#include "role.h"
#include "function.h"

int role::getRoleLevel()
{
	return memory::read<int>(__角色等级);
}
std::wstring role::getRoleJobName()
{
	return memory::readWString(memory::read<DWORD>(__职业名称) + 0x0, 100);
}
int role::getCurrentRoleFatigueValue()
{
	return function::decrypt(__最大疲劳) - function::decrypt(__当前疲劳);
}
ROLE_POS role::getRolePos()
{
	ROLE_POS RolePos;
	RolePos.room = function::getCurrentRoomPos();
	RolePos.x = (int)memory::read<float>(__角色坐标);
	RolePos.y = (int)memory::read<float>(__角色坐标 + 4);
	RolePos.z = (int)memory::read<float>(__角色坐标 + 8);
	return RolePos;
}
int role::getRoleStatus()
{
	return memory::read<int>(memory::read<int>(__角色状态) + __角色状态偏移);
}
void role::releaseSkillByKey(int keyCode, int s)
{
	if (function::isOpenDoor() == false)
	{
		msdk.doKeyPress(keyCode, s);
		// 释放技能的延时
		while (getRoleStatus() != 0)
		{
			Sleep(100);
		}
	}
}

