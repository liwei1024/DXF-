// 控制台内存版.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "call.h"
#include "send_packet.h"
#include "status_3.h"
#include "function.h"
#include "role.h"

int HotKeyId_END = 0;
int HotKeyId_HOME = 0;
int HotKeyId_F1 = 0;
SendPacket sp;

void exitProcess()
{
	int s = 3;
	UnregisterHotKey(NULL, HotKeyId_END);
	UnregisterHotKey(NULL, HotKeyId_HOME);
	UnregisterHotKey(NULL, HotKeyId_F1);
	msdk.closeHandle();
	utils::printString("msdk_handle close ...");
	rw4.closeHandle();
	utils::printString("rw4 close ...");
	//UN_PROTECT_FILE();
	
	while (s > 0) {
		utils::printString("程序将在 %d 秒后结束...",RED, s);
		s--;
		Sleep(1000);
	}
	exit(0);
}

void test()
{
	Sleep(2000);
	utils::myprintf("开始移动");
	ROLE_POS pos = role::getRolePos();
	pos.x += 600;
	pos.y += 200;
	role::moveRoleToPos(pos);
	return;
}

VOID ThreadMessage(
	MSG* msg
)
{
	switch (msg->message)
	{
	case WM_HOTKEY:
		if (msg->wParam == HotKeyId_END)
		{
			//utils::printString("HotKeyId_END");
			_beginthreadex(NULL, 0, (_beginthreadex_proc_type)exitProcess, NULL, 0, 0);
		
		}
		else if (msg->wParam == HotKeyId_HOME) {
			utils::printString("HotKeyId_HOME");
			rw4.Init();
			utils::printString("ProcessId->:%d\nProcessBaseAddress->:%x", GREEN, rw4.dwProcessId, rw4.dwProcessBaseAddress);
			msdk.openHandle();
			utils::printString("msdk_handle->:%x", GREEN, msdk.msdk_handle);
			
		}
		else if (msg->wParam == HotKeyId_F1) {
			//sp.返回角色();
			//status_3::outputMapObjectInfo();
			//utils::printString("status->:%d",RED,memory::read<int>(__游戏状态));
			
			_beginthreadex(NULL, 0, (_beginthreadex_proc_type)test, NULL, 0, 0);
			
			
		}
	default:
		break;
	}
}

void RegHotKeys()
{
	HotKeyId_END = GlobalAddAtom(L"HotKeyId_END");
	HotKeyId_HOME = GlobalAddAtom(L"HotKeyId_HOME");
	HotKeyId_F1 = GlobalAddAtom(L"HotKeyId_F1");
	if (!RegisterHotKey(NULL, HotKeyId_END, NULL, VK_END)) 
		utils::myprintf("END 注册失败 HotKeyId_END->:%d",RED, HotKeyId_END);
	
	if (!RegisterHotKey(NULL, HotKeyId_HOME, NULL, VK_HOME)) 
		utils::myprintf("END 注册失败 HotKeyId_HOME->:%d", RED, HotKeyId_HOME);
	
	if(!RegisterHotKey(NULL, HotKeyId_F1, NULL, VK_F1))
		utils::myprintf("F1 注册失败 HotKeyId_F1->:%d", RED, HotKeyId_F1);
}


int main()
{
	utils::windowInitialize();
	RegHotKeys();
	//PROTECT_FILE();
	
	MSG msg = { 0 };//消息指针
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);//等待信号
		ThreadMessage(&msg);//线程消息处理
		DispatchMessage(&msg);//处理信号
	}
}


