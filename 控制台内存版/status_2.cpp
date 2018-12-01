#include "pch.h"
#include "status_2.h"
#include "function.h"
#include "role.h"

void status_2::manage()
{
	if (role::getCurrentRoleFatigueValue() == 0)
	{
		msdk.doKeyPress(VK_ESCAPE);
		Sleep(1000);
	}
	else {
		//按键选择副本(g_dungeon_id, g_dungeon_rank);
		selectCopy(104,0);
	}
}

void status_2::selectCopy(int copy_id,int copy_rand)
{
	int cur_copy_id, cur_copy_rand;
	int flag = true;
	while (flag && function::getGameStatus() == 2)
	{
		cur_copy_id = memory::read<int>(__副本ID);
		cur_copy_rand = memory::read<int>(memory::read<int>(__副本难度) + __副本难度偏移);
		Sleep(500);
		if (copy_id == cur_copy_id && cur_copy_rand == copy_rand)
		{
			flag = false;
			while (true)
			{
				if (function::getGameStatus() == 3)
				{
					break;
				}
				msdk.doKeyPress(VK_SPACE);
				Sleep(3000);
			}
			if (memory::read<int>(__对话基址) == 1)
			{
				msdk.doKeyPress(VK_RETURN);
			}
			break;
		}
		if (copy_id != cur_copy_id)
		{
			msdk.doKeyPress(VK_NUMPAD2);
			Sleep(500);
			continue;
		}
		if (copy_rand != cur_copy_rand && copy_rand > cur_copy_rand)
		{
			msdk.doKeyPress(VK_NUMPAD3);
			Sleep(500);
			continue;
		}
		else if (copy_rand != cur_copy_rand && copy_rand < cur_copy_rand)
		{
			msdk.doKeyPress(VK_NUMPAD1);
			Sleep(500);
			continue;
		}
	}
}