#pragma once
typedef struct _POS
{
	int x, y, z = 0;
}POS, *PPOS;

typedef struct _WINDOW_INFO
{
	int top = 0;
	int bottom = 0;
	int left = 0;
	int right = 0;
	int width = 0;
	int height = 0;
} WINDOW_INFO, *PWINDOW_INFO;