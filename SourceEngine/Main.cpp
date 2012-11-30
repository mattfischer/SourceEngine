#include <windows.h>
#include <GL/gl.h>

#include "FileReaderFactory.hpp"
#include "BSPFile.hpp"
#include "Renderer.hpp"
#include "VPKReaderFactory.hpp"

HWND hWnd;
HDC hDC;
HGLRC hglRC;

BSPFile *bspFile;
Renderer *renderer;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

				VPKReaderFactory *vpkFactory = new VPKReaderFactory("pak01_dir.vpk");
				FileReaderFactory *fileFactory = new FileReaderFactory();
				IReader *reader = fileFactory->open("sp_a1_intro1.bsp");
				bspFile = new BSPFile(reader);
				renderer = new Renderer(bspFile, SCREEN_WIDTH, SCREEN_HEIGHT);

				return 0;
			}

		case WM_KEYDOWN:
			switch(wParam) {
				case VK_LEFT:
					renderer->rotate(-1);
					break;

				case VK_RIGHT:
					renderer->rotate(1);
					break;

				case VK_UP:
					renderer->move(1);
					break;

				case VK_DOWN:
					renderer->move(-1);
					break;

				case 'A':
					renderer->rise(-1);
					break;

				case 'Z':
					renderer->rise(1);
					break;
			}
			return 0;

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

	hWnd = CreateWindowEx(0, "SourceEngine", "Source Engine", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 10, 10, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInst, NULL);

	MSG msg;
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
	}

    return 0;
}