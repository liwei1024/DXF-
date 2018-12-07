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


void role::moveRoleToPos(ROLE_POS targetPos)
{
	// 时间变量
	DWORD t1,t2 = utils::getTime();
	// 游戏状态
	int gameStatus = function::getGameStatus();
	utils::myprintf("gameStatus->%d",GREEN, gameStatus);
	// 卡点列表
	//std::map<const char*,bool> cardPointList;
	// 位置变量
	ROLE_POS currentPos,cardPointPos = getRolePos();
	utils::myprintf("目标位置 房间x->:%d,房间y->:%d | x->:%d,y->:%d", RED, targetPos.room.x, targetPos.room.y, targetPos.x, targetPos.y);
	while (true)
	{
		currentPos = getRolePos();
		t1 = utils::getTime();
		utils::myprintf("当前位置 房间x->:%d,房间y->:%d | x->:%d,y->:%d",YELLOW, currentPos.room.x, currentPos.room.y, currentPos.x, currentPos.y);
		if (
			currentPos.room.x != targetPos.room.x ||
			currentPos.room.y != targetPos.room.y ||
			(abs(currentPos.x - targetPos.x)<50 && abs(currentPos.y - targetPos.y) < 30)
			)
		{
			msdk.upAllKey();//还原所有按键
			utils::myprintf("成功到达指定位置");
			break;
		}

		if ((currentPos.y - targetPos.y) >= 30 && msdk.getKeyState(VK_NUMPAD5) == 0)
		{
			msdk.keyDown(VK_NUMPAD5);
		}

		if ((targetPos.y - currentPos.y) >= 30 && msdk.getKeyState(VK_NUMPAD2) == 0)
		{
			msdk.keyDown(VK_NUMPAD2);
		}

		if ((currentPos.x - targetPos.x) >= 50 && msdk.getKeyState(VK_NUMPAD1) == 0)
		{
			printf("左\n");
			if (gameStatus == 3)
			{
				msdk.keyDown(VK_NUMPAD1);
				Sleep(100);
				msdk.keyUp(VK_NUMPAD1);
				Sleep(100);
				msdk.keyDown(VK_NUMPAD1);
			}
			else {
				msdk.keyDown(VK_NUMPAD1);
			}
		}

		if ((targetPos.x - currentPos.x) >= 50 && msdk.getKeyState(VK_NUMPAD3) == 0)
		{
			printf("右\n");
			if (gameStatus == 3)
			{
				msdk.keyDown(VK_NUMPAD3);
				Sleep(100);
				msdk.keyUp(VK_NUMPAD3);
				Sleep(100);
				msdk.keyDown(VK_NUMPAD3);
			}
			else {
				msdk.keyDown(VK_NUMPAD3);
			}
		}

		


		if (abs(currentPos.y - targetPos.y) < 30)
		{
			if (msdk.getKeyState(VK_NUMPAD5) == 1) {
				msdk.keyUp(VK_NUMPAD5);
				utils::myprintf("keyUp 下");
			}
			if (msdk.getKeyState(VK_NUMPAD2) == 1)
			{
				msdk.keyUp(VK_NUMPAD2);
				utils::myprintf("keyUp 下");
			}
			
		}

		if (abs(currentPos.x - targetPos.x) < 50)
		{
			if (msdk.getKeyState(VK_NUMPAD1) == 1) {
				msdk.keyUp(VK_NUMPAD1);
			}
			if (msdk.getKeyState(VK_NUMPAD3) == 1)
			{
				msdk.keyUp(VK_NUMPAD3);
			}
			utils::myprintf("keyUp 左右");
		}

		// 卡点处理
		if ((t1 - t2) > 3)
		{
			t2 = utils::getTime();
			if (
				abs(currentPos.x - cardPointPos.x) < 3 && 
				abs(currentPos.y - cardPointPos.y) < 3
				)
			{

				if (msdk.getKeyState(VK_NUMPAD1) == 1)
				{
					msdk.keyUp(VK_NUMPAD1);
					msdk.doKeyPress(VK_NUMPAD3, 500);
				}

				if (msdk.getKeyState(VK_NUMPAD2) == 1)
				{
					msdk.keyUp(VK_NUMPAD2);
					msdk.doKeyPress(VK_NUMPAD5, 500);
				}

				if (msdk.getKeyState(VK_NUMPAD3) == 1)
				{
					msdk.keyUp(VK_NUMPAD3);
					msdk.doKeyPress(VK_NUMPAD1, 500);
				}

				if (msdk.getKeyState(VK_NUMPAD5) == 1)
				{
					msdk.keyUp(VK_NUMPAD5);
					msdk.doKeyPress(VK_NUMPAD2, 500);
				}
			}
			cardPointPos = getRolePos();
		}
	}
}

