#include "pch.h"
#include "status_0.h"
#include "function.h"

void status_0::manage()
{
	if (getRoleCount() == getRoleIndox()) {
		utils::printString("所有角色刷图完毕，自动关闭");
		autoSwitch = false;
	}
	else {
		selectRole(getRoleIndox() + 1);
	}
}

int status_0::getRoleIndox()
{
	return memory::readOffset<int>(__遍历取值, { __角色下标偏移 });
}
int status_0::getRoleCount()
{
	return memory::readOffset<int>(__遍历取值, { __当前角色数量,__角色数量偏移2 });
}

int status_0::getRoleMaxCount()
{
	return memory::readOffset<int>(__遍历取值, { __最大角色数量,__角色数量偏移2 });
}

void status_0::selectRole(int select_role_index)
{
	while (function::getGameStatus() == 0)
	{
		if (select_role_index != getRoleIndox())
		{
			msdk.doKeyPress(VK_RIGHT);
		}
		msdk.setMousePos(game_window_info.left + 534, game_window_info.top + 550);
		msdk.mouseClick();
		Sleep(3000);
	}
}