#pragma once
#include "msdk.h"
#pragma comment(lib,"msdk.lib") 
#define RED FOREGROUND_INTENSITY | FOREGROUND_RED
#define CYAN FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define YELLOW FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define PINK FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define GREEN FOREGROUND_INTENSITY | FOREGROUND_GREEN



namespace utils {

	void myprintf(const char * _Format, WORD Color = CYAN, ...);
	HWND getWindowHandle();
	WINDOW_INFO getWindowInfo(HWND hWnd);
	int createRandom(int min, int max);
	void randomSleep(int min, int max);
	void outputDbebugWString(const wchar_t * lpcszFormat, ...);
	void outputDbebugString(const char * lpcszFormat, ...);
	void msdkInit();
	void upAllKey();
	void keyDown(int keyCode);
	void keyUp(int keyCode);
	void doKeyPress(int keyCode, int s = 100);
	void mouseClick();
	void mouseDoubleClick(int s = 100);
	void moveMousePos(int x, int y);
	void setMousePos(int x, int y);
	void getMousePos(int * x, int * y);
	void processExit();
	void windowInitialize();
}

