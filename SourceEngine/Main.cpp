#include <windows.h>
#include <GL/gl.h>

#include "File/FileReaderFactory.hpp"
#include "File/BSP.hpp"
#include "Renderer.hpp"
#include "File/VPKReaderFactory.hpp"
#include "File/MultiReaderFactory.hpp"

HWND hWnd;
HDC hDC;
HGLRC hglRC;

File::BSP *bspFile;
Renderer *renderer;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
		case WM_CREATE:
			{
				hDC = GetDC(hWnd);

				PIXELFORMATDESCRIPTOR pfd = { 
					sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
					1,                     // version number  
					PFD_DRAW_TO_WINDOW |   // support window  
					PFD_SUPPORT_OPENGL |   // support OpenGL  
					PFD_DOUBLEBUFFER,      // double buffered  
					PFD_TYPE_RGBA,         // RGBA type  
					24,                    // 24-bit color depth  
					0, 0, 0, 0, 0, 0,      // color bits ignored  
					0,                     // no alpha buffer  
					0,                     // shift bit ignored  
					0,                     // no accumulation buffer  
					0, 0, 0, 0,            // accum bits ignored  
					32,                    // 32-bit z-buffer  
					0,                     // no stencil buffer  
					0,                     // no auxiliary buffer  
					PFD_MAIN_PLANE,        // main layer  
					0,                     // reserved  
					0, 0, 0                // layer masks ignored  
				};

				int pf = ChoosePixelFormat(hDC, &pfd);
				SetPixelFormat(hDC, pf, &pfd);

				hglRC = wglCreateContext(hDC);
				wglMakeCurrent(hDC, hglRC);

				File::MultiReaderFactory *factory = new File::MultiReaderFactory();
				factory->addFactory(new File::FileReaderFactory("portal2"));
				factory->addFactory(new File::VPKReaderFactory("portal2/pak01_dir.vpk"));
				Map *map = new Map(factory, "sp_a1_intro1");
				renderer = new Renderer(map, SCREEN_WIDTH, SCREEN_HEIGHT);

				return 0;
			}

		case WM_DESTROY:
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hglRC);
			ReleaseDC(hWnd, hDC);

			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASSEX cls;
	memset(&cls, 0, sizeof(cls));
	cls.cbSize = sizeof(cls);
	cls.cbClsExtra = 0;
	cls.cbWndExtra = 0;
	cls.lpszClassName = "SourceEngine";
	cls.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	cls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	cls.hCursor = LoadCursor(NULL, IDC_ARROW);
	cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	cls.hInstance = hInst;
	cls.lpfnWndProc = WndProc;

	RegisterClassEx(&cls);

	hWnd = CreateWindowEx(0, "SourceEngine", "Source Engine", WS_VISIBLE | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInst, NULL);

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	dm.dmPelsWidth = SCREEN_WIDTH;
	dm.dmPelsHeight = SCREEN_HEIGHT;
	dm.dmBitsPerPel = 32;
	ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

	MSG msg;
	DWORD clock = GetTickCount();
	ShowCursor(FALSE);
	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT) {
			break;
		}

		renderer->render();
		SwapBuffers(hDC);
		DWORD nextClock = GetTickCount();
		int elapsed = nextClock - clock;

		POINT point;
		GetCursorPos(&point);
		float mouseScale = 0.1f;
		renderer->rotate(((int)point.x - SCREEN_WIDTH/2) * elapsed * mouseScale, ((int)point.y - SCREEN_HEIGHT/2) * elapsed * mouseScale);
		SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

#define KEY_DOWN(x) (GetAsyncKeyState(x) & 0x80000000)

		float moveScale = 0.1f;
		if(KEY_DOWN(VK_UP)) {
			renderer->move(elapsed * moveScale);
		}

		if(KEY_DOWN(VK_DOWN)) {
			renderer->move(-elapsed * moveScale);
		}

		if(KEY_DOWN('A')) {
			renderer->rise(elapsed * moveScale);
		}

		if(KEY_DOWN('Z')) {
			renderer->rise(-elapsed * moveScale);
		}

		if(KEY_DOWN(VK_ESCAPE)) {
			break;
		}

		clock = nextClock;
	}

    return 0;
}