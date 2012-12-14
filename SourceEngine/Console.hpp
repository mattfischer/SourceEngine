#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <windows.h>

class Console {
public:
	Console();

	void clear();
	void printf(const char *fmt, ...);

	static Console *instance();

private:
	HWND mHWnd;
	int mStart;
	int mHeight;
};

#endif