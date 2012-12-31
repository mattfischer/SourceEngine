#include <windows.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "File/FileReaderFactory.hpp"
#include "File/VPKReaderFactory.hpp"
#include "File/MultiReaderFactory.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"
#include "Geo/Frustum.hpp"

#include "World/Map.hpp"

#include "Draw/MapDrawer.hpp"

#include "Console.hpp"

HWND hWnd;
HDC hDC;
HGLRC hglRC;

World::Map *map;
Draw::MapDrawer *mapDrawer;
Geo::Point position;
Geo::Orientation orientation;

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

				glewInit();

				glMatrixMode(GL_PROJECTION_MATRIX);
				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
				glLoadIdentity();
				gluPerspective(70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 50, 10000);

				glMatrixMode(GL_MODELVIEW_MATRIX);
				float matrix[16] = { 0.0f, 0.0f, -1.0f, 0.0f,   -1.0f, 0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f };
				glMultMatrixf(matrix);

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);

				glDepthFunc(GL_LEQUAL);

				File::MultiReaderFactory *factory = new File::MultiReaderFactory();
				factory->addFactory(new File::FileReaderFactory("portal2"));
				factory->addFactory(new File::VPKReaderFactory("portal2/pak01_dir.vpk"));
				map = new World::Map(factory, "maps/sp_a1_intro3.bsp");
				Geo::Frustum startFrustum(70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
				mapDrawer = new Draw::MapDrawer(map, startFrustum);
				position = map->playerStart()->position() + Geo::Vector(0, 0, 60);
				orientation = map->playerStart()->orientation();

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

	Console::instance();

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
	DWORD fpsClock = GetTickCount();
	int frames = 0;
	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT) {
			break;
		}

		mapDrawer->draw(position, orientation);
		SwapBuffers(hDC);

		frames++;
		if(GetTickCount() > fpsClock + 1000) {
			Console::instance()->clear();
			Console::instance()->printf("FPS: %i", frames);
			Console::instance()->printf("NumFacesDrawn: %i", mapDrawer->faceDrawer()->numFacesDrawn());
			Console::instance()->printf("NumFacesCulled: %i", mapDrawer->faceDrawer()->numFacesCulled());
			Console::instance()->printf("NumFrustumCulled: %i", mapDrawer->bspDrawer()->numFrustumCulled());
			Console::instance()->printf("NumVisLeaves: %i", mapDrawer->bspDrawer()->numVisLeaves());
			frames = 0;
			fpsClock = GetTickCount();
		}

		DWORD nextClock = GetTickCount();
		int elapsed = nextClock - clock;

		POINT point;
		GetCursorPos(&point);
		float pitch = orientation.pitch();
		float yaw = orientation.yaw();

		pitch += -((int)point.y - SCREEN_HEIGHT/2);
		yaw += -((int)point.x - SCREEN_WIDTH/2);

		pitch = min(pitch, 70.0f);
		pitch = max(pitch, -70.0f);

		if(yaw > 360) { yaw -= 360; }
		if(yaw < 0) { yaw += 360; }

		orientation = Geo::Orientation(pitch, yaw, 0);

		SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

#define KEY_DOWN(x) (GetAsyncKeyState(x) & 0x80000000)

		float speed = 30;
		if(KEY_DOWN('W')) {
			Geo::Vector delta(1, 0, 0);
			delta = Geo::Transformation::rotateZ(orientation.yaw()) * delta;
			position = position + speed * delta;
		}

		if(KEY_DOWN('S')) {
			Geo::Vector delta(-1, 0, 0);
			delta = Geo::Transformation::rotateZ(orientation.yaw()) * delta;
			position = position + speed * delta;
		}

		if(KEY_DOWN('A')) {
			Geo::Vector delta(0, 1, 0);
			delta = Geo::Transformation::rotateZ(orientation.yaw()) * delta;
			position = position + speed * delta;

		}

		if(KEY_DOWN('D')) {
			Geo::Vector delta(0, -1, 0);
			delta = Geo::Transformation::rotateZ(orientation.yaw()) * delta;
			position = position + speed * delta;
		}

		if(KEY_DOWN('C')) {
			position = position + speed * Geo::Vector(0, 0, -1);
		}

		if(KEY_DOWN('X')) {
			position = position + speed * Geo::Vector(0, 0, 1);
		}

		if(KEY_DOWN('F')) {
			mapDrawer->bspDrawer()->setFrustumCull(false);
		} else {
			mapDrawer->bspDrawer()->setFrustumCull(true);
		}

		if(KEY_DOWN('U')) {
			mapDrawer->setUpdateFrustum(false);
		} else {
			mapDrawer->setUpdateFrustum(true);
		}

		if(KEY_DOWN('T')) {
			mapDrawer->faceDrawer()->setDrawTextures(false);
		} else {
			mapDrawer->faceDrawer()->setDrawTextures(true);
		}

		if(KEY_DOWN('L')) {
			mapDrawer->faceDrawer()->setDrawLightmaps(false);
		} else {
			mapDrawer->faceDrawer()->setDrawLightmaps(true);
		}

		if(KEY_DOWN(VK_ESCAPE)) {
			break;
		}

		clock = nextClock;
	}

    return 0;
}