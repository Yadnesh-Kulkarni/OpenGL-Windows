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
bool bLight1 = false;

GLboolean bPyramid = GL_TRUE;
GLboolean bCube = GL_FALSE;
GLboolean bSphere = GL_FALSE;

GLfloat angle_triangle = 0.0f;
GLfloat angle_cube = 0.0f;
GLfloat angle_sphere = 0.0f;

GLUquadric *quadric = NULL;

//Light0
GLfloat light0Ambient[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat light0Diffuse[] = {1.0f,0.0f,0.0f,0.0f};
GLfloat light0Specular[] = {1.0f,0.0f,0.0f,0.0f};
GLfloat light0Position[] = {2.0f,1.0f,1.0f,0.0f};

//Light1
GLfloat light1Ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light1Diffuse[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light1Specular[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light1Position[] = { -2.0f,1.0f,1.0f,0.0f };

//Material
GLfloat matAmbient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat matDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat matSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat matShininess[] = { 50.0f };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototypes
	void initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);
	void ToggleFullScreen();

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
		TEXT("OpenGL Native Windowing : Pyramid And Cube"),
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

		case 0x46://For FullScreen(F)
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
		case 0x4C://For Light(L)
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
		case 0x50://Pyramid
			if (bPyramid == GL_FALSE)
			{
				bPyramid = GL_TRUE;
				bCube = GL_FALSE;
				bSphere = GL_FALSE;
			}
			break;
		case 0x53://Sphere
			if (bSphere == GL_FALSE)
			{
				bSphere = GL_TRUE;
				bCube = GL_FALSE;
				bPyramid = GL_FALSE;
			}
			break;
		case 0x43://Cube
			if (bCube == GL_FALSE)
			{
				bCube = GL_TRUE;
				bSphere = GL_FALSE;
				bPyramid = GL_FALSE;
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

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	quadric = gluNewQuadric();

	//Light0
	glLightfv(GL_LIGHT0,GL_AMBIENT,light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
//	glEnable(GL_LIGHT0);

	//Light1
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
//	glEnable(GL_LIGHT1);

	//Material
	glMaterialfv(GL_FRONT,GL_AMBIENT,matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void DrawPyramid(void);
	void DrawCube(void);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (bPyramid == GL_TRUE)
	{
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(angle_triangle, 0.0f, 1.0f, 0.0f);
		DrawPyramid();
	}
	glLoadIdentity();

	if (bCube == GL_TRUE)
	{
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(angle_cube, 0.0f, 1.0f, 0.0f);
		glScalef(0.75f, 0.75f, 0.75f);
		DrawCube();
	}

	glLoadIdentity();

	if (bSphere == GL_TRUE)
	{
		glTranslatef(0.0f,0.0f,-2.0f);
		glRotatef(angle_sphere,0.0f,1.0f,0.0f);
		if (bFill == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (bLine == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		}
		else if (bPoint == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
		}
		gluSphere(quadric, 0.75, 30, 30);
	}
	SwapBuffers(ghDC);
}

void DrawCube(void)
{
	glBegin(GL_QUADS);
	// Top Face
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //right-top corner
	glVertex3f(-1.0f, 1.0f, -1.0f); //left-top corner
	glVertex3f(-1.0f, 1.0f, 1.0f); //left-bottom corner
	glVertex3f(1.0f, 1.0f, 1.0f); //right-bottom corner

								  // Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Right Face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

void DrawPyramid(void)
{
	glBegin(GL_TRIANGLES);
	// Front Face
	glNormal3f(0.0f, 0.447214f, 0.894427f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//Right Face
	glNormal3f(0.894427f, 0.447214f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back Face
	glNormal3f(0.0f, 0.447214f, -0.894427f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left Face
	glNormal3f(-0.894427f, 0.447214f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
}

void update(void)
{
	if (bPyramid == GL_TRUE)
	{
		angle_triangle = angle_triangle + 0.5f;
		if (angle_triangle >= 360.0f)
		{
			angle_triangle = 0.0f;
		}
	}

	if (bCube == GL_TRUE)
	{
		angle_cube = angle_cube + 0.5f;
		if (angle_cube >= 360.0f)
		{
			angle_cube = 0.0f;
		}
	}

	if (bSphere == GL_TRUE)
	{
		angle_sphere = angle_sphere + 0.5f;
			if (angle_sphere >= 360.0f)
			{
				angle_sphere = 0.0f;
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

