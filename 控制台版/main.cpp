// 控制台版.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"


int main()
{
	//driversss.Init();

	DWORD value = RVM<DWORD>(0x00AEB570);
	utils::myprintf("value %d",CYAN, value);
	system("pause");
}


