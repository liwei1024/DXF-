#pragma once
#include "pch.h"
#include "call.h"
/*
void Send_缓冲Call(DWORD cdov, DWORD len)
{
//DWORD parameter[] = { 100, 人物基址, cdov, len, 缓冲CALL };
//汇编远程执行(Send_缓冲, parameter, sizeof(parameter));
char Code[] = { 0x51,                       //push ecx
0x68, 0xFF, 0x00, 0x00, 0x00,      //push len
0x68, 0xFF, 0x00, 0x00, 0x00,      //push cdov
0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00,//mov ecx, dword ptr  [发包基址]
0xE8, 0x5F, 0x1C, 0x06, 0x00,      //call 缓冲CALL
0x59,  0xC3 };                        //  pop ecx ret 
LPVOID CodeAddr = VirtualAllocEx(hProcess, 0, sizeof(Code), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

*(int *)(Code + 2) = len;
*(int *)(Code + 7) = cdov;
*(int *)(Code + 13) = 发包基址;
*(int *)(Code + 18) = 缓冲CALL - ((int)CodeAddr + 17) - 5;
WriteProcessMemory(hProcess, CodeAddr, &Code, sizeof(Code), NULL);
//公告log(L"Send_缓冲Call%X", CodeAddr);
FARPROC  CallWindowProcA_addr = ::GetProcAddress(GetModuleHandleA("user32.dll"), "CallWindowProcW");
HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CallWindowProcA_addr, CodeAddr, 0, 0);
//等待线程结束
WaitForSingleObject(hRemoteThread, 0xFF);
// 清理工作
VirtualFreeEx(hProcess, CodeAddr, sizeof(Code), MEM_DECOMMIT);
CloseHandle(hRemoteThread);

}


*/

#define __CALL地址 0x400500
#define __参数地址 0x400800

void call::技能Call(int pointer,int code,int damage,int x,int y,int z)
{
	int skill_struct[26] = {};
	skill_struct[0] = pointer;//触发指针;
	skill_struct[2] = code; //代码
	skill_struct[3] = damage; //伤害
	skill_struct[6] = x;//x
	skill_struct[7] = y;//y
	skill_struct[8] = z;//z
	skill_struct[22] = 0; //大小
	skill_struct[23] = 0;
	skill_struct[24] = 65535;
	skill_struct[25] = 65535;

	byte shell_code[] = {
		0xb9,0x00,0x00,0x00,0x00,
		0xb8,0x00,0x00,0x00,0x00,
		0xff,0xd0,
		0xc3
	};
	*(int*)(shell_code + 1) = __参数地址;
	*(int*)(shell_code + 6) = __技能CALL;
	rw2.writeVirtualMemory(__CALL地址, shell_code, sizeof(shell_code));
	rw2.writeVirtualMemory(__参数地址, skill_struct,sizeof(skill_struct));
	//SendMessageW(hWnd,10024,0x400500,0);

}