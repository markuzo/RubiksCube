/*********************************************************************************/
/* Title:		WinMain.cpp																									         */
/* Author:	Markus Schlafli																							         */
/* Date:		27-Jul-2013																								           */
/*********************************************************************************/

#include "stdafx.h"
#include "WinMain.h"
#include "Context.h"
#include "OpenGLContext.h"
#include "AppSettings.h"
#include <assert.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <Windowsx.h>

HINSTANCE hinstance;
Context * context;

bool mouseDown = false;
int xPosPrev = std::numeric_limits<int>::max();
int yPosPrev = std::numeric_limits<int>::max();

/*
------------------------------------------------------------------------------------
Standard Win32 callback
------------------------------------------------------------------------------------
*/
LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wd, LPARAM lng) {

	switch (msg) {
	case WM_SIZE:
	{
		context->resizeWindow(LOWORD(lng), HIWORD(lng));
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wd == MK_LBUTTON) {
			int xPos = GET_X_LPARAM(lng);
			int yPos = GET_Y_LPARAM(lng);

			int dx;
			int dy;
			if (xPosPrev == std::numeric_limits<int>::max()) {
				xPosPrev = xPos;
				break;
			}
			else {
				dx = xPos - xPosPrev;
			}

			if (yPosPrev == std::numeric_limits<int>::max()) {
				yPosPrev = yPos;
				break;
			} 
			else {
				dy = yPos - yPosPrev;
			}

			context->rotateView(dx, dy);

			yPosPrev = yPos;
			xPosPrev = xPos;
		}
		else {
			xPosPrev = std::numeric_limits<int>::max();
			yPosPrev = std::numeric_limits<int>::max();
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		mouseDown = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		mouseDown = false;
		break;
	}
	case WM_KEYDOWN:
	{

		glm::mat4 mat;

		switch (wd) {
		case VK_DOWN:
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
			context->rotateView(mat);
			break;
		case VK_UP:
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
			context->rotateView(mat);
			break;
		case VK_RIGHT:
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			context->rotateView(mat);
			break;
		case VK_LEFT:
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			context->rotateView(mat);
			break;
		}
		break;
	}
	case WM_CHAR:
	{
		char key = static_cast<char>((wchar_t)wd);
		if (key == '\\') {
			context->loadShaders();
		}
		else {
			context->rotateCube(key);
		}
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wd, lng);
}

void
CreateContext(HWND hwnd) {

	// currently we are only interested in creating an OpenGL context 
	// as no option is available
	context = new OpenGLContext(hwnd);

	if (!context->initialized())
		exit(1);
}

void
OpenConsole() {
	AllocConsole();

	HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)outHandle, _O_TEXT);
	FILE * outfile = _fdopen(hCrt, "w");
	setvbuf(outfile, NULL, _IONBF, 1);
	*stdout = *outfile;

	HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)inHandle, _O_TEXT);
	FILE * infile = _fdopen(hCrt, "r");
	setvbuf(infile, NULL, _IONBF, 128);
	*stdin = *infile;
}

bool
CreateApplicationWindow() {
	const char * titleChar = AppSettings::Title();
	size_t titleSize = strlen(titleChar) + 1;
	size_t convertedTitle = 0;
	wchar_t wcstring[100];
	mbstowcs_s(&convertedTitle, wcstring, titleSize, titleChar, _TRUNCATE);

	LPCWSTR title = wcstring;

	WNDCLASS windowclass;
	HWND hwnd;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	hinstance = GetModuleHandle(NULL);

	windowclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowclass.lpfnWndProc = (WNDPROC)WndProc;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 0;
	windowclass.hInstance = hinstance;
	windowclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclass.hbrBackground = NULL;
	windowclass.lpszMenuName = NULL;
	windowclass.lpszClassName = title;

	if (!RegisterClass(&windowclass)) {
		return false;
	}

	//hwnd = CreateWindowEx( dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL );

	hwnd = CreateWindowEx(dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, hinstance, NULL);

	CreateContext(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	context->setupScene();

	return true;
}

int
MainLoop() {
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			context->render();
		}
	}

	return (int)msg.wParam;
}

/*
------------------------------------------------------------------------------------
Entry point for Windows
------------------------------------------------------------------------------------
*/
int APIENTRY
_tWinMain(_In_ HINSTANCE hInstance,
_In_opt_ HINSTANCE hPrevInstance,
_In_ LPTSTR    lpCmdLine,
_In_ int       nCmdShow)
{
	bool created = CreateApplicationWindow();
	if (!created)
		return 1;

	//OpenConsole();

	int msgWParam = MainLoop();

	return msgWParam;
}