#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")

#define WIN_WIDTH	800
#define WIN_HEIGHT	600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND ghWnd = NULL;
HDC ghDC = NULL;
HGLRC ghGLRC = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;
bool gbLight = false;
bool bFill = true;
bool bLine = false;
bool bPoint = false;
bool bLight0 = true;
bool bLight1 = true;
bool bLight2 = true;

GLUquadric *quadric = NULL;

//Light0
GLfloat light0Ambient[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat light0Diffuse[] = {1.0f,0.0f,0.0f,0.0f};
GLfloat light0Specular[] = {1.0f,0.0f,0.0f,0.0f};
GLfloat light0Position[] = {0.0f,0.0f,0.0f,0.0f};

//Light1
GLfloat light1Ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light1Diffuse[] = { 0.0f,1.0f,0.0f,0.0f };
GLfloat light1Specular[] = { 0.0f,1.0f,0.0f,0.0f };
GLfloat light1Position[] = { 0.0f,0.0f,0.0f,0.0f };


//Light2
GLfloat light2Ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light2Diffuse[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light2Specular[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light2Position[] = { 0.0f,0.0f,0.0f,0.0f };


//Material
GLfloat matrialSpecular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat materialShininess[] = {50.0f};

GLfloat RedLightAngle = 0.0f;
GLfloat GreenLightAngle = 0.0f;
GLfloat BlueLightAngle = 0.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototypes
	void initialize(void);
	void display(void);
	void uninitialize(void);
	void ToggleFullScreen();
	void update();

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
		TEXT("OpenGL Native Windowing : Sphere Lighting"),
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
			
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;
				update();
				display();
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
		case 0x4C:
			if (gbLight == false)
			{
				glEnable(GL_LIGHTING);
				gbLight = true;
			}
			else
			{
				glDisable(GL_LIGHTING);
				gbLight = false;
			}
			break;
		case 0x31://Fill Mode
			if (bFill == false)
			{
				bFill = true;
				bPoint = false;
				bLine = false;
			}
			break;
		case 0x32://Point Mode
			if (bPoint == false)
			{
				bPoint = true;
				bFill = false;
				bLine = false;
			}
			break;
		case 0x33://Line Mode
			if (bLine == false)
			{
				bLine = true;
				bFill = false;
				bPoint = false;
			}
			break;
		case 0x30://Light0
			if (bLight0 == false)
			{
				glEnable(GL_LIGHT0);
				bLight0 = true;
			}
			else
			{
				glDisable(GL_LIGHT0);
				bLight0 = false;
			}
			break;
		case 0x39://Light1
			if (bLight1 == false)
			{
				glEnable(GL_LIGHT1);
				bLight1 = true;
			}
			else
			{
				glDisable(GL_LIGHT1);
				bLight1 = false;
			}
			break;
		case 0x38:
			if (bLight2 == false)
			{
				glEnable(GL_LIGHT2);
				bLight2 = true;
			}
			else
			{
				glDisable(GL_LIGHT2);
				bLight2 = false;
			}
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
	pfd.cDepthBits = 24;

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

	quadric = gluNewQuadric();
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Light0
	glLightfv(GL_LIGHT0,GL_AMBIENT,light0Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0Specular);
	glEnable(GL_LIGHT0);
	//Light1
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
	glEnable(GL_LIGHT1);
	//Light2
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2Ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);
	glEnable(GL_LIGHT2);

	//Material
	glMaterialfv(GL_FRONT,GL_SPECULAR,matrialSpecular);
	glMaterialfv(GL_FRONT,GL_SHININESS,materialShininess);


	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void drawSphere();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		gluLookAt(0.0f,0.0f,0.1f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
		glPushMatrix();
			glRotatef(RedLightAngle,1.0f,0.0f,0.0f);
			light0Position[1] = RedLightAngle;
			glLightfv(GL_LIGHT0,GL_POSITION,light0Position);
		glPopMatrix();
		glPushMatrix();
			glRotatef(GreenLightAngle,0.0f,1.0f,0.0f);
			light1Position[0] = GreenLightAngle;
			glLightfv(GL_LIGHT1,GL_POSITION,light1Position);
		glPopMatrix();
		glPushMatrix();
			glRotatef(BlueLightAngle, 0.0f, 1.0f, 0.0f);
			light2Position[0] = BlueLightAngle;
			glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
		glPopMatrix();
		glPushMatrix();
			drawSphere();
		glPopMatrix();
	glPopMatrix();
	SwapBuffers(ghDC);
}

void drawSphere()
{
	glTranslatef(0.0f, 0.0f, -2.0f);
	if (bFill == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (bLine == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (bPoint == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	gluSphere(quadric, 0.75, 100, 100);
}

void update()
{
	if (bLight0 == true)
	{
		RedLightAngle = RedLightAngle + 0.5f;
		if (RedLightAngle >= 360.0f)
		{
			RedLightAngle = 0.0f;
		}
	}

	if (bLight1 == true)
	{
		GreenLightAngle = GreenLightAngle + 0.5f;
		if (GreenLightAngle >= 360.0f)
		{
			GreenLightAngle = 0.0f;
		}
	}

	if (bLight2 == true)
	{
		BlueLightAngle = BlueLightAngle + 0.5f;
		if (BlueLightAngle >= 360.0f)
		{
			BlueLightAngle = 0.0f;
		}
	}
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

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

