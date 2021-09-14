#include<Windows.h>
#include<math.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")

#define WIN_WIDTH	1600
#define WIN_HEIGHT	900
#define GL_PI 3.1428

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND ghWnd = NULL;
HDC ghDC = NULL;
HGLRC ghGLRC = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

GLboolean gbAnimate = FALSE;
GLboolean gbAnimateI = TRUE;
GLboolean gbAnimateN = FALSE;
GLboolean gbAnimateD = FALSE;
GLboolean gbAnimateIS = FALSE;
GLboolean gbAnimateA = FALSE;
GLboolean gbAnimateLine = FALSE;
GLboolean gbAnimateCircle = FALSE;
GLboolean IncrementDstep = TRUE;
GLboolean gbAnimateLast = FALSE;
GLfloat Istep = 0.05f;
GLfloat Nstep = 0.05f;
GLfloat I_step = 0.05f;
GLfloat AHstep = 0.05f;
GLfloat AVstep = 0.05f;
GLfloat LineX1 = -4.5f;
GLfloat LineX2 = -4.4f;
GLfloat Dstep = 0.0f;
GLfloat CircleStep = 0.0f;
GLfloat Fade = 1.0f;
GLfloat LastStep = 0.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototypes
	void initialize(void);
	void display(void);
	void uninitialize(void);
	void ToggleFullScreen();
	void Animate();

	WNDCLASSEX wndclass;
	HWND hWnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MyOpenGLCLASS");
	bool bDone = false;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("OpenGL Native Windowing"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghWnd = hWnd;

	initialize();



	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	//message loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			display();
			Animate();
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}


	uninitialize();
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void display(void);
	void resize(int, int);
	void ToggleFullScreen(void);
	void uninitialize(void);

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;

	case WM_ERASEBKGND:
		return(0);
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			if (gbEscapeKeyIsPressed == false)
				gbEscapeKeyIsPressed = true;
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

		case 's':
		case 'S':
			if (gbAnimate == TRUE)
			{
				gbAnimate = FALSE;
			}
			else
			{
				gbAnimateI = TRUE;
				gbAnimateN = FALSE;
				gbAnimateD = FALSE;
				gbAnimateIS = FALSE;
				gbAnimateA = FALSE;
				gbAnimateLine = FALSE;
				gbAnimateCircle = FALSE;
				IncrementDstep = TRUE;
				gbAnimateLast = FALSE;
				gbAnimate = TRUE;
				Istep = 0.05f;
				Nstep = 0.05f;
				I_step = 0.05f;
				AHstep = 0.05f;
				AVstep = 0.05f;
				LineX1 = -4.5f;
				LineX2 = -4.4f;
				Dstep = 0.0f;
				CircleStep = 0.0f;
			    Fade = 1.0f;
			    LastStep = 0.0f;
				mciSendString(TEXT("play 1.mp3"), NULL, 0, 0);
			}
			break;
		default:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_CLOSE:
		uninitialize();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
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

void initialize(void)
{
	void resize(int, int);

	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	ghDC = GetDC(ghWnd);

	iPixelFormatIndex = ChoosePixelFormat(ghDC, &pfd);

	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	if (SetPixelFormat(ghDC, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	ghGLRC = wglCreateContext(ghDC);
	if (ghGLRC == NULL)
	{
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	if (wglMakeCurrent(ghDC, ghGLRC) == false)
	{
		wglDeleteContext(ghGLRC);
		ghGLRC = NULL;
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat size[2];
	GLfloat pointsize[2];
	GLfloat curSize;
	GLfloat curPointSize;
	GLfloat xcircle = 0.0f;
	GLfloat ycircle = 0.0f;
	GLfloat angle = 0.0f;
	GLfloat radius = 0.525f;
	GLfloat x = -4.7f;
	GLfloat y = 2.5f;
	glClear(GL_COLOR_BUFFER_BIT);
	glGetFloatv(GL_LINE_WIDTH_RANGE, size);

	curSize = size[0];

	curSize = curSize + 3.0f;

	glLineWidth(curSize);
	glLoadIdentity();
	glTranslatef(0.3f, 0.0f, -6.0f);

	if (gbAnimate == TRUE)
	{
		//DRAW I
		if (gbAnimateI == TRUE)
		{
			glTranslatef(Istep, 0.0f, 0.0f);
			glBegin(GL_LINES);
			glColor4f(1.0f, 0.5f, 0.0f, Fade);
			glVertex3f(-4.5, 1.5f, 0.0f);
			glColor4f(0.0f, 1.0f, 0.0f, Fade);
			glVertex3f(-4.5, -1.5f, 0.0f);
			glEnd();
			//Drawing N
			if (gbAnimateN == TRUE)
			{
				glTranslatef(-Istep, 0.0f, 0.0f);
				glTranslatef(0.0f, -Nstep, 0.0f);

				glBegin(GL_LINES);

				glColor4f(1.0f, 0.5f, 0.0f, Fade);
				glVertex3f(-2.8f, 4.5f, 0.0f);
				glColor4f(0.0f, 1.0f, 0.0f, Fade);
				glVertex3f(-2.8f, 1.5f, 0.0f);

				glColor4f(1.0f, 0.5f, 0.0f, Fade);
				glVertex3f(-2.8f, 4.5f, 0.0f);
				glColor4f(0.0f, 1.0f, 0.0f, Fade);
				glVertex3f(-1.8f, 1.5f, 0.0f);

				glColor4f(1.0f, 0.5f, 0.0f, Fade);
				glVertex3f(-1.8f, 4.5f, 0.0f);
				glColor4f(0.0f, 1.0f, 0.0f, Fade);
				glVertex3f(-1.8f, 1.5f, 0.0f);
				glEnd();
				//Drawing D
				if (gbAnimateD == TRUE)
				{
					glTranslatef(0.0f, Nstep, 0.0f);
					glBegin(GL_LINES);

					glColor4f(1.0f, 0.5f, 0.0f, Dstep);
					glVertex3f(-1.0f, 1.5f, 0.0f);
					glColor4f(0.0f, 1.0f, 0.0f, Dstep);
					glVertex3f(-1.0f, -1.5f, 0.0f);

					glColor4f(1.0f, 0.5f, 0.0f, Dstep);;
					glVertex3f(0.2f, 1.5f, 0.0f);
					glColor4f(0.0f, 1.0f, 0.0f, Dstep);
					glVertex3f(0.2f, -1.5f, 0.0f);

					glColor4f(1.0f, 0.5f, 0.0f, Dstep);
					glVertex3f(-1.05f, 1.5f, 0.0f);
					glVertex3f(0.2f, 1.5f, 0.0f);

					glColor4f(0.0f, 1.0f, 0.0f, Dstep);
					glVertex3f(-1.05f, -1.5f, 0.0f);
					glVertex3f(0.2f, -1.5f, 0.0f);
					glEnd();

					if (gbAnimateIS == TRUE)
					{
						glTranslatef(0.0f, I_step, 0.0f);
						glBegin(GL_LINES);
						//Drawing I
						glColor4f(1.0f, 0.5f, 0.0f, Fade);
						glVertex3f(1.1f, -2.5f, 0.0f);
						glColor4f(0.0f, 1.0f, 0.0f, Fade);
						glVertex3f(1.1f, -5.5f, 0.0f);
						glEnd();
						if (gbAnimateA == TRUE)
						{
							glTranslatef(0.0f, -I_step, 0.0f);
							glTranslatef(-AHstep, AVstep, 0.0f);
							glBegin(GL_LINES);
							//Drawing A
							glColor4f(1.0f, 0.5f, 0.0f, Fade);
							glVertex3f(4.6f, -1.5f, 0.0f);
							glColor4f(0.0f, 1.0f, 0.0f, Fade);
							glVertex3f(3.6f, -4.5f, 0.0f);

							glColor4f(1.0f, 0.5f, 0.0f, Fade);
							glVertex3f(4.6f, -1.5f, 0.0f);
							glColor4f(0.0f, 1.0f, 0.0f, Fade);
							glVertex3f(5.6f, -4.5f, 0.0f);

							glEnd();
							if (gbAnimateLine == TRUE)
							{
								glTranslatef(AHstep, -AVstep, 0.0f);
								curSize = curSize - 1.5f;
								glLineWidth(curSize);

								glBegin(GL_LINES);
								glColor4f(0.0f, 1.0f, 0.0f, Fade);
								glVertex3f(LineX1, -0.01f, 0.0f);
								glVertex3f(LineX2, -0.01f, 0.0f);

								glColor4f(1.0f, 1.0f, 1.0f, Fade);
								glVertex3f(LineX1, 0.0f, 0.0f);
								glVertex3f(LineX2, 0.0f, 0.0f);

								glColor4f(1.0f, 0.5f, 0.0f, Fade);
								glVertex3f(LineX1, 0.01f, 0.0f);
								glVertex3f(LineX2, 0.01f, 0.0f);
								glEnd();

								if (gbAnimateCircle == TRUE)
								{
									if (gbAnimateLast == TRUE)
									{
										glBegin(GL_QUADS);
										glColor4f(1.0f, 0.5f, 0.0f, LastStep);
										glVertex3f(-4.7f,2.5f,0.0f);
										glColor4f(1.0f, 1.0f, 1.0f, LastStep);
										glVertex3f(-4.7f,0.0f,0.0f);
										glColor4f(1.0f, 1.0f, 1.0f, LastStep);
										glVertex3f(4.9f, 0.0f, 0.0f);
										glColor4f(1.0f, 0.5f, 0.0f, LastStep);
										glVertex3f(4.9f, 2.5f, 0.0f);
										glEnd();
										glBegin(GL_QUADS);
										glColor4f(1.0f, 1.0f, 1.0f, LastStep);
										glVertex3f(-4.7f, 0.0f, 0.0f);
										glColor4f(0.0f, 1.0f, 0.0f, LastStep);
										glVertex3f(-4.7f, -2.5f, 0.0f);
										glColor4f(0.0f, 1.0f, 0.0f, LastStep);
										glVertex3f(4.9f, -2.5f, 0.0f);
										glColor4f(1.0f, 1.0f, 1.0f, LastStep);
										glVertex3f(4.9f, 0.0f, 0.0f);
										glEnd();
									}
									glGetFloatv(GL_POINT_SIZE_RANGE, pointsize);

									glColor4f(0.0f, 0.0f, 0.9f, CircleStep / 2);
									curPointSize = pointsize[0];
									curPointSize = curPointSize + 1.0f;
									glPointSize(curPointSize);
									glBegin(GL_POINTS);
									for (angle = 0.0f; angle <= (2 * GL_PI); angle = angle + 0.01f)
									{
										xcircle = radius*sin(angle);
										ycircle = radius*cos(angle);

										glVertex3f(xcircle - 0.400f, ycircle, 0.0f);

									}
									glEnd();
									glBegin(GL_LINES);
									for (angle = 0.0f; angle <= (2 * GL_PI); angle = angle + 0.2619f)
									{
										x = radius*sin(angle);
										y = radius*cos(angle);

										glVertex3f(-0.400f, 0.0f, 0.0f);
										glVertex3f(x - 0.400, y, 0.0f);
									}
									glEnd();
								}
							}
						}
					}
				}
			}
		}

	}






	SwapBuffers(ghDC);

}


void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void uninitialize(void)
{
	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghWnd, GWL_STYLE);
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

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghGLRC);
	ghGLRC = NULL;

	ReleaseDC(ghWnd, ghDC);
	ghDC = NULL;

	DestroyWindow(ghWnd);
}

void Animate()
{
	if (gbAnimate == TRUE)
	{
		if (Istep < 0.8f)
		{
			Istep = Istep + 0.005f;
		}
		if (Istep >= 0.8f)
		{
			gbAnimateN = TRUE;
		}

		if (gbAnimateN == TRUE && Nstep < 3.0f)
		{
			Nstep = Nstep + 0.005f;
		}
		if (Nstep >= 3.0f)
		{
			gbAnimateD = TRUE;
		}
		if (gbAnimateD == TRUE)
		{
			if (IncrementDstep == TRUE)
			{
				Dstep = Dstep + 0.005f;
			}
		}
		if (Dstep >= 1.0f)
		{
			gbAnimateIS = TRUE;
			IncrementDstep = FALSE;
		}
		if (gbAnimateIS == TRUE && I_step < 4.0f)
		{
			I_step = I_step + 0.005f;
		}
		if (I_step >= 4.0f)
		{
			gbAnimateA = TRUE;
		}

		if (gbAnimateA == TRUE && AVstep < 3.0f)
		{
			AVstep = AVstep + 0.005f;
		}

		if (gbAnimateA == TRUE && AVstep >= 3.0f && AHstep < 1.8f)
		{
			AHstep = AHstep + 0.005f;
		}
		if (AHstep > 1.8f)
		{
			gbAnimateLine = TRUE;
		}
		if (gbAnimateLine == TRUE && LineX2 < 3.3f)
		{
			LineX2 = LineX2 + 0.009f;
		}

		if (gbAnimateLine == TRUE && LineX2 >= 3.3f && LineX1 <= 2.3f)
		{
			LineX1 = LineX1 + 0.01f;
		}

		if (LineX1 > 2.3f)
		{
			gbAnimateCircle = TRUE;
		}

		if (gbAnimateCircle == TRUE)
		{
			CircleStep = CircleStep + 0.003f;
		}

		if (CircleStep > 1.0f)
		{
			Fade = Fade - 0.005f;
			if (IncrementDstep == FALSE)
			{
				Dstep = Dstep - 0.005f;
			}
		}

		if (Fade < 0.0f && Dstep < 0.0f)
		{
			gbAnimateLast = TRUE;
		}

		if (gbAnimateLast == TRUE)
		{
			LastStep = LastStep + 0.002f;
		}
	}
}