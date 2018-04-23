#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "uxtheme")

#include <windows.h>
#include <uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCheck;
	static HFONT hFont;
	static DOUBLE dControlHeight = 32.0;
	switch (msg)
	{
	case WM_CREATE:
		{
			HTHEME hTheme = OpenThemeData(hWnd, VSCLASS_AEROWIZARD);
			LOGFONT lf = { 0 };
			GetThemeFont(hTheme, NULL, AW_HEADERAREA, 0, TMT_FONT, &lf);
			hFont = CreateFontIndirectW(&lf);
			dControlHeight = lf.lfHeight * 1.8;
			if (dControlHeight < 0.0)
			{
				dControlHeight = -dControlHeight;
			}
			CloseThemeData(hTheme);
		}
		hCheck = CreateWindow(TEXT("Button"), TEXT("スリープ状態を回避(&S)"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		SendMessage(hCheck, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hCheck, BM_SETCHECK, 1, 0);
		break;
	case WM_SIZE:
		MoveWindow(hCheck, 10, 10, 256, (int)dControlHeight, TRUE);
		break;
	case WM_POWERBROADCAST:
		if (BST_CHECKED == SendMessage(hCheck, BM_GETCHECK, 0, 0))
		{
			return BROADCAST_QUERY_DENY;
		}
		return DefDlgProc(hWnd, msg, wParam, lParam);
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("スリープ状態を回避する"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
