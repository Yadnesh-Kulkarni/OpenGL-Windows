#include <windows.h> 
#include <stdio.h> 

struct Resolution
{
	DWORD MyHeight;
	DWORD MyWidth;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	DEVMODE			dmode;
	BOOL			bRet = FALSE;
	LONG			CDSRET, SWLRET;
	static			FILE *fp = NULL;
	static struct Resolution MaxRes;
	int				iCnt = 0;


	memset((void*)&dmode, 0, sizeof(dmode));

	switch (iMsg)
	{

	case WM_CREATE:
		while (EnumDisplaySettings(NULL, iCnt, &dmode))
		{
			if (iCnt == 0)
			{
				MaxRes.MyWidth = dmode.dmPelsWidth;
				MaxRes.MyHeight = dmode.dmPelsHeight;
			}
			else
			{
				if (dmode.dmPelsWidth >= MaxRes.MyWidth)
				{
					MaxRes.MyWidth = dmode.dmPelsWidth;
					if (dmode.dmPelsHeight > MaxRes.MyHeight)
					{
						MaxRes.MyHeight = dmode.dmPelsHeight;
					}
				}
			}
			iCnt++;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{

		case 0x46:
			bRet = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmode);
			dmode.dmPelsWidth = MaxRes.MyWidth;
			dmode.dmPelsHeight = MaxRes.MyHeight;
			dmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			CDSRET = ChangeDisplaySettings(&dmode, CDS_RESET | CDS_FULLSCREEN);
			switch (CDSRET)
			{
			case DISP_CHANGE_SUCCESSFUL:
				MessageBox(hwnd, TEXT("DISP CHANGE SUCCESSFULL"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_RESTART:
				MessageBox(hwnd, TEXT("DISP CHANGE RESTART"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADFLAGS:
				MessageBox(hwnd, TEXT("DISP CHANGE BADFLAGS"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADPARAM:
				MessageBox(hwnd, TEXT("DISP CHANGE BADPARAM"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_FAILED:
				MessageBox(hwnd, TEXT("DISP CHANGE FAILED"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADMODE:
				MessageBox(hwnd, TEXT("DISP CHANGE BADMODE"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_NOTUPDATED:
				MessageBox(hwnd, TEXT("DISP CHANGE NOTUPDATED"), TEXT("Change Display Setting"), MB_OK);
				break;
			}

			SWLRET = SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
			ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			UpdateWindow(hwnd);
			break;

		case 0x1B:
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmode);

			CDSRET = ChangeDisplaySettings(NULL, 0);
			switch (CDSRET)
			{
			case DISP_CHANGE_SUCCESSFUL:
				MessageBox(hwnd, TEXT("DISP CHANGE SUCCESSFULL"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_RESTART:
				MessageBox(hwnd, TEXT("DISP CHANGE RESTART"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADFLAGS:
				MessageBox(hwnd, TEXT("DISP CHANGE BADFLAGS"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADPARAM:
				MessageBox(hwnd, TEXT("DISP CHANGE BADPARAM"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_FAILED:
				MessageBox(hwnd, TEXT("DISP CHANGE FAILED"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_BADMODE:
				MessageBox(hwnd, TEXT("DISP CHANGE BADMODE"), TEXT("Change Display Setting"), MB_OK);
				break;
			case DISP_CHANGE_NOTUPDATED:
				MessageBox(hwnd, TEXT("DISP CHANGE NOTUPDATED"), TEXT("Change Display Setting"), MB_OK);
				break;
			}
			SWLRET = SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			UpdateWindow(hwnd);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	}

return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}