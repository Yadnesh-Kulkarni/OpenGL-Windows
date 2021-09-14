#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool gbFullScreen = false;
void ToggleFullScreen(void);
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
HWND ghWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	TCHAR className[] = TEXT("Window Class");
	HWND hwnd;
	MSG msg;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hIconSm = NULL;
	wndclass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.lpszClassName = className;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("ERROR REGISTERING CLASS"), TEXT("EXITING"), MB_OK);
		return 0;
	}

	hwnd = CreateWindow(className,
		TEXT("OpenGL First Assignment"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghWnd = hwnd;
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rc;
	static int iPaint ;
	HBRUSH hbr;
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT :
		HDC hdc;
		
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd,&rc);
		if (iPaint == 1)
		{
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 2)
		{
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 3)
		{
			hbr = CreateSolidBrush(RGB(0, 0, 255));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 4)
		{
			hbr = CreateSolidBrush(RGB(0, 255, 255));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 5)
		{
			hbr = CreateSolidBrush(RGB(255, 0, 255));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 6)
		{
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 7)
		{
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(hdc, &rc, hbr);
		}
		if (iPaint == 8)
		{
			hbr = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(hdc, &rc, hbr);
		}
		
		EndPaint(hwnd,&ps);
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 0x52 :
			iPaint = 1;
			InvalidateRect(hwnd,NULL,TRUE);
			break;
		case 0x47:
			iPaint = 2;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x42:
			iPaint = 3;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x43:
			iPaint = 4;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x4D:
			iPaint = 5;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x59:
			iPaint = 6;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x4B:
			iPaint = 7;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x57:
			iPaint = 8;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 0x46:
			if (gbFullScreen == false)
			{
				ToggleFullScreen();
				gbFullScreen = true;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = false;
			}
			break;
		}
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ToggleFullScreen(void)
{
	MONITORINFO mi;

	if (gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghWnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			HMONITOR hm;
			hm = MonitorFromWindow(ghWnd, MONITORINFOF_PRIMARY);
			if (GetWindowPlacement(ghWnd, &wpPrev) && GetMonitorInfo(hm, &mi))
			{
				SetWindowLong(ghWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghWnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}
		}

		ShowCursor(FALSE);
	}
	else
	{
		SetWindowLong(ghWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghWnd, &wpPrev);
		SetWindowPos(ghWnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED
		);

		ShowCursor(TRUE);
	}
}