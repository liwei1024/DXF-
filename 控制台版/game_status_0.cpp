#include "pch.h"
#include "game_status_0.h"
#include "memory.hpp"
#include "utils.h"
#include "function.h"


/*
1.判断角色是否刷完
2.选择角色
*/
void game_status_0::manage()
{
	if (get_role_count() == get_role_index()) {
		utils::myprintf("所有角色刷图完毕，自动关闭");
		g_auto_switch = false;
	}
	else {
		select_role();
	}

}

int game_status_0::get_role_index()
{
	int role_index = readOffset<int>(__遍历取值, { __角色下标偏移 });
	return role_index;

}

int game_status_0::get_role_count()
{
	int role_count = readOffset<int>(__遍历取值, { __当前角色数量,__角色数量偏移2 });
	return role_count;
}

int game_status_0::get_role_max_count()
{
	int role_max_count = readOffset<int>(__遍历取值, { __最大角色数量,__角色数量偏移2 });
	return role_max_count;
}



void game_status_0::select_role()
{
	int select_role_index = g_role_index + 1;
	while (function::getGameStatus() == 0)
	{
		if (select_role_index != get_role_index())
		{
			utils::doKeyPress(VK_RIGHT);
		}
		utils::setMousePos(game_window_info.left + 534, game_window_info.top + 550);
		utils::mouseClick();
		Sleep(3000);
	}
	g_role_index = select_role_index;
}



void game_status_0::create_role()
{

}