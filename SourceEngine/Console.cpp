#include "Console.hpp"

#include <stdarg.h>
#include <stdio.h>

#define WIDTH 200
#define HEIGHT 400

Console *Console::instance()
{
	static Console inst;

	return &inst;
}

Console::Console()
{
	mHWnd = CreateWindowEx(WS_EX_TOPMOST, "STATIC", "", WS_VISIBLE | WS_POPUP , 0, 0, WIDTH, HEIGHT, NULL, NULL, NULL, NULL);
	HDC hDC = GetDC(mHWnd);
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	ReleaseDC(mHWnd, hDC);
	mHeight = tm.tmHeight;
	mStart = 0;
}

void Console::clear()
{
	mStart = 0;
	RECT rect;
	rect.left = 0;
	rect.right = WIDTH;
	rect.top = 0;
	rect.bottom = HEIGHT;
	HDC hDC = GetDC(mHWnd);
	HBRUSH br = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hDC, &rect, br);
	ReleaseDC(mHWnd, hDC);
}

void Console::printf(const char *fmt, ...)
{
	va_list va;
	char buffer[1024];

	va_start(va, fmt);

	vsprintf_s(buffer, sizeof(buffer), fmt, va);

	RECT rect;
	rect.left = 0;
	rect.right = WIDTH;
	rect.top = mStart;
	rect.bottom = HEIGHT;

	HDC hDC = GetDC(mHWnd);
	DrawText(hDC, buffer, -1, &rect, DT_LEFT);
	ReleaseDC(mHWnd, hDC);

	mStart += mHeight;
}
