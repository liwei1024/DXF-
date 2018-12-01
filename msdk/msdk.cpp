
#ifndef __UTILS_H__
#include "../utils/utils.h"
#pragma comment(lib,"../x64/Release/library/utils.lib")
#endif

#ifndef __MEMORY_LOADER_H__
#include "../memory_loader/memory_loader.h"
#pragma comment(lib,"../x64/Release/library/memory_loader.lib")
#endif // !__MEMORY_LOADER_H__

#include "msdk.h"

M_OPEN_VIDPID M_Open_VidPid;
M_RELEASEALLKEY M_ReleaseAllKey;
M_KEYSTATE2 M_KeyState2;
M_KEYDOWN2 M_KeyDown2;
M_KEYUP2 M_KeyUp2;
M_LEFTCLICK M_LeftClick;
M_MOVETO2 M_MoveTo2;
M_MOVETO3 M_MoveTo3;
M_GETCURMOUSEPOS2 M_GetCurrMousePos2;
M_CLOSE M_Close;


Msdk::Msdk() {
	FILE *fp;
	unsigned char *data = NULL;
	size_t size;
	HMEMORYMODULE module;
	fopen_s(&fp, "./dll/msdk.dat", "rb");
	if (fp == NULL)
	{
		printf("打开文件失败\n");
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	data = (unsigned char *)malloc(size);
	fseek(fp, 0, SEEK_SET);
	fread(data, 1, size, fp);
	fclose(fp);

	module = MemoryLoadLibrary(data);
	if (module == NULL)
	{
		printf("获取模块失败\n");
	}

	M_Open_VidPid = (M_OPEN_VIDPID)MemoryGetProcAddress(module, "M_Open_VidPid");
	M_ReleaseAllKey = (M_RELEASEALLKEY)MemoryGetProcAddress(module, "M_ReleaseAllKey");
	M_KeyState2 = (M_KEYSTATE2)MemoryGetProcAddress(module, "M_KeyState2");
	M_KeyDown2 = (M_KEYDOWN2)MemoryGetProcAddress(module, "M_KeyDown2");
	M_KeyUp2 = (M_KEYUP2)MemoryGetProcAddress(module, "M_KeyUp2");
	M_LeftClick = (M_LEFTCLICK)MemoryGetProcAddress(module, "M_LeftClick");
	M_MoveTo2 = (M_MOVETO2)MemoryGetProcAddress(module, "M_MoveTo2");
	M_MoveTo3 = (M_MOVETO3)MemoryGetProcAddress(module, "M_MoveTo3");
	M_GetCurrMousePos2 = (M_GETCURMOUSEPOS2)MemoryGetProcAddress(module, "M_GetCurrMousePos2");
	M_Close = (M_CLOSE)MemoryGetProcAddress(module, "M_Close");
}
Msdk::~Msdk() {

}

void Msdk::openHandle()
{
	if (msdk_handle == NULL)
	{
		msdk_handle = M_Open_VidPid(0xc310, 0xc007);
		if (msdk_handle == INVALID_HANDLE_VALUE) {
			MessageBox(NULL, L"", L"端口打开失败，请确认您的USB设备已经插上电脑", MB_OK);
		}
	}
}

void Msdk::closeHandle()
{
	if (msdk_handle != NULL)
	{
		M_Close(msdk_handle);
		msdk_handle = NULL;
	}
	
}

void Msdk::upAllKey()
{
	M_ReleaseAllKey(msdk_handle);
}
void Msdk::keyDown(int keyCode)
{
	if (M_KeyState2(msdk_handle, keyCode) == 0)
	{
		M_KeyDown2(msdk_handle, keyCode);
	}

}
void Msdk::keyUp(int keyCode)
{
	if (M_KeyState2(msdk_handle, keyCode) == 1)
	{
		M_KeyUp2(msdk_handle, keyCode);
	}
}
void Msdk::doKeyPress(int keyCode, int s)
{
	keyDown(keyCode);
	Sleep(s + utils::createRandom(1, 10));
	keyUp(keyCode);
}
void Msdk::mouseClick()
{
	M_LeftClick(msdk_handle, 1);
}
void Msdk::mouseDoubleClick(int s)
{
	mouseClick();
	Sleep(s + utils::createRandom(0, 10));
	mouseClick();
}
void Msdk::moveMousePos(int x, int y)
{
	M_MoveTo2(msdk_handle, x, y);
}
void Msdk::setMousePos(int x, int y)
{
	M_MoveTo3(msdk_handle, x, y);
}
void Msdk::getMousePos(int *x, int *y)
{
	M_GetCurrMousePos2(x, y);
}