#ifndef __MSDK_H__
#define __MSDK_H__
typedef HANDLE(WINAPI*M_OPEN_VIDPID)(int, int);
typedef int (WINAPI*M_RELEASEALLKEY)(HANDLE);
typedef int (WINAPI*M_KEYSTATE2)(HANDLE, int);
typedef int (WINAPI*M_KEYDOWN2)(HANDLE, int);
typedef int (WINAPI*M_KEYUP2)(HANDLE, int);
typedef int (WINAPI*M_LEFTCLICK)(HANDLE, int);
typedef int (WINAPI*M_MOVETO2)(HANDLE, int, int);
typedef int (WINAPI*M_MOVETO3)(HANDLE, int, int);
typedef int (WINAPI*M_GETCURMOUSEPOS2)(int*, int*);
typedef int (WINAPI*M_CLOSE)(HANDLE);
class Msdk
{
public:
	HANDLE msdk_handle;
	Msdk();
	~Msdk();
	void openHandle();
	void closeHandle();
	void upAllKey();
	void keyDown(int keyCode);
	void keyUp(int keyCode);
	void doKeyPress(int keyCode, int s = 100);
	void mouseClick();
	void mouseDoubleClick(int s = 100);
	void moveMousePos(int x, int y);
	void setMousePos(int x, int y);
	void getMousePos(int * x, int * y);
};
#endif // !__MSDK_H__



