#include "pch.h"
#include "utils.h"

HANDLE msdk_handle;
WINDOW_INFO game_window_info;


void utils::myprintf(const char *_Format,WORD Color,...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),Color);
	va_list argList;
	char buffer[0x1024];
	va_start(argList, Color);
	vsprintf_s(buffer, _Format, argList);
	std::cout << buffer << std::endl;
	va_end(argList);
}

HWND utils::getWindowHandle()
{
	std::wstring wstr(_T("地下城与勇士"));
	return FindWindow(wstr.c_str(), wstr.c_str());
}

WINDOW_INFO utils::getWindowInfo(HWND hWnd)
{
	RECT rect;
	WINDOW_INFO window_info;
	GetWindowRect(hWnd, &rect);
	window_info.bottom = rect.bottom;
	window_info.left = rect.left;
	window_info.right = rect.right;
	window_info.top = rect.top;
	window_info.width = window_info.right - window_info.left;
	window_info.height = window_info.bottom - window_info.top;
	return window_info;
}

int utils::createRandom(int min, int max)
{
	int Random = 0;
	if (max > min) {
		srand((unsigned)time(NULL));
		Random = (rand() % (min - max + 1) + max);
	}
	else {
		srand((unsigned)time(NULL));
		Random = (rand() % (max - min + 1) + min);
	}
	return Random;
}

void utils::randomSleep(int min, int max)
{
	Sleep(createRandom(min,max));
}

void utils::outputDbebugWString(const wchar_t *lpcszFormat, ...)
{
	va_list argList;
	wchar_t buffer[0x1024];
	wchar_t temp_buffer[0x1024];
	va_start(argList, lpcszFormat);
	vswprintf_s(buffer, lpcszFormat, argList);
	swprintf_s(temp_buffer, L"DXF一体 %s", buffer);
	OutputDebugStringW(temp_buffer);
	va_end(argList);
}

void utils::outputDbebugString(const char *lpcszFormat, ...)
{
	va_list argList;
	char buffer[0x1024];
	char temp_buffer[0x1024];
	va_start(argList, lpcszFormat);
	vsprintf_s(buffer, lpcszFormat, argList);
	sprintf_s(temp_buffer, "DXF一体 %s", buffer);
	OutputDebugStringA(temp_buffer);
	va_end(argList);
}

void utils::msdkInit()
{
	msdk_handle = M_Open_VidPid(0xc310, 0xc007);
	if (msdk_handle == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"", L"端口打开失败，请确认您的USB设备已经插上电脑", MB_OK);
	}
	/*utils::myprintf("msdk_handle %d", RED, msdk_handle);
	utils::moveMousePos(500, 500);
	utils::myprintf("random %d", RED, M_RandDomNbr(1, 10));*/
}

void utils::upAllKey()
{
	M_ReleaseAllKey(msdk_handle);
}

void utils::keyDown(int keyCode)
{
	if (M_KeyState2(msdk_handle,keyCode) == 0)
	{
		M_KeyDown2(msdk_handle, keyCode);
	}
	
}

void utils::keyUp(int keyCode)
{
	if (M_KeyState2(msdk_handle, keyCode) == 1)
	{
		M_KeyUp2(msdk_handle, keyCode);
	}
}

void utils::doKeyPress(int keyCode, int s)
{
	keyDown(keyCode);
	Sleep(s + createRandom(1, 10));
	keyUp(keyCode);
}

void utils::mouseClick()
{
	M_LeftClick(msdk_handle,1);
}

void utils::mouseDoubleClick(int s)
{
	mouseClick();
	Sleep(s + createRandom(0, 10));
	mouseClick();
}

void utils::moveMousePos(int x,int y)
{
	M_MoveTo2(msdk_handle,x, y);
}

void utils::setMousePos(int x, int y)
{
	M_MoveTo3(msdk_handle, x, y);
}

void utils::getMousePos(int *x, int *y)
{
	M_GetCurrMousePos2(x,y);
}

void utils::processExit()
{
	M_Close(msdk_handle);
	exit(0);
}

void utils::windowInitialize()
{
	RECT rect;
	int cx, cy;
	int width, height;
	cx = GetSystemMetrics(SM_CXFULLSCREEN);
	cy = GetSystemMetrics(SM_CYFULLSCREEN);
	HWND g_self_window_handle = GetConsoleWindow();
	GetWindowRect(g_self_window_handle, &rect);
	MoveWindow(g_self_window_handle, 0, 0, 800, 600, TRUE);
	GetWindowRect(g_self_window_handle, &rect);
	width = (rect.right - rect.left);
	height = (rect.bottom - rect.top);
	MoveWindow(g_self_window_handle, cx - width, cy - height, width, height, TRUE);

	EnableMenuItem(GetSystemMenu(g_self_window_handle, FALSE), SC_CLOSE, MF_GRAYED);
	SetConsoleTitle(_T("丶"));
	//SetLayeredWindowAttributes(g_self_window_handle, 0, 200, 3);//透明度设置
}