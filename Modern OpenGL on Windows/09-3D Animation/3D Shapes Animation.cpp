#include<Windows.h>
#include<stdio.h>
#include<GL\glew.h>
#include<gl\GL.h>
#include"vmath.h"

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")

#define WIN_WIDTH	800
#define WIN_HEIGHT	600

using namespace vmath;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Declaring enum for binding GPU and CPU data.
enum {
	YDK_ATTRIBUTE_VERTEX = 0,
	YDK_ATTRIBUTE_COLOR,
	YDK_ATTRIBUTE_NORMAL,
	YDK_ATTRIBUTE_TEXTURE0,
};

FILE *g_fp = NULL;
HWND ghWnd = NULL;
HDC ghDC = NULL;
HGLRC ghGLRC = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

GLuint gVertexShaderObject = 0;
GLuint gFragmentShaderObject = 0;
GLuint gShaderProgramObject = 0;

GLuint gVao_pyramid;
GLuint gVao_cube;
GLuint gVbo_pyramid_position;
GLuint gVbo_pyramid_color;
GLuint gVbo_cube_position;
GLuint gVbo_cube_color;
GLuint gMVPUniform;

GLfloat angle_Pyramid = 0.0f;
GLfloat angle_Cube = 0.0f;

mat4 gPerspectiveProjectionMatrix;

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototypes
	void initialize(void);
	void display(void);
	void uninitialize(void);
	void ToggleFullScreen();
	void update(void);

	WNDCLASSEX wndclass;
	HWND hWnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MyOpenGLCLASS");
	bool bDone = false;

	fopen_s(&g_fp, "PP-Log.txt", "w");
	if (!g_fp)
	{
		MessageBox(NULL,TEXT("File Cannot be Created!!!Exiting!"),TEXT("ERROR"),MB_OK);
		return 0;
	}

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
		TEXT("OpenGL Native Windowing : Programmable Pipeline : 2 2D Animation"),
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
	void uninitialize(void);

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

	GLenum gl_error = glewInit();
	if (gl_error != GLEW_OK)
	{
		wglDeleteContext(ghGLRC);
		ghGLRC = NULL;
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	//******VERTEX SHADER*************
	//Create Shader Object(Vertex)
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//Write Source Code in a String
	const GLchar *vertexShaderSourceCode =
		"#version 450 core"\
		"\n"\
		"in vec4 vPosition;"\
		"in vec4 vColor;"\
		"uniform mat4 u_mvp_matrix;"\
		"out vec4 out_color;"\
		"void main(void)"\
		"{"\
		"gl_Position = u_mvp_matrix * vPosition;"\
		"out_color = vColor;"\
		"}";

	//Give Source to Shader Object
	glShaderSource(gVertexShaderObject,1,(const GLchar **)&vertexShaderSourceCode,NULL);

	//Compile Shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject,iInfoLogLength,&written,szInfoLog);
				fprintf(g_fp,"Vertex Shader Compilation Log : %s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//*************FRAGMENT SHADER*********
	//Create Shader Object (Fragment)
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//Write Source Code in a string
	const GLchar *fragmentShaderSourceCode =
		"#version 450 core"\
		"\n"\
		"in vec4 out_color;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{"\
		"FragColor = out_color;"\
		"}";
	
	//Give Source to Shader Object
	glShaderSource(gFragmentShaderObject,1,(const GLchar **)&fragmentShaderSourceCode,NULL);

	//Compile Shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = NULL;
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject,iInfoLogLength,&written,szInfoLog);
				fprintf(g_fp,TEXT("Fragment Shader Compilation Log : %s\n"),szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//Create Shader Program
	gShaderProgramObject = glCreateProgram();

	//Attach Both Compiled Shaders to this program
	glAttachShader(gShaderProgramObject,gVertexShaderObject);
	glAttachShader(gShaderProgramObject,gFragmentShaderObject);

	//Skip this call if tracing the flow after decades just go on and come back to this when specified in glVertexAttribPointer
	//This call specifically Binds CPUs particular address to GPU memory returned address.This is required because Shader cannot know the address of data in GPU and GPU does not provide programmer with the address in GPU.
	//Hence we bind one of our memory location to GPU memory which makes it easy to acces and at the same time we can tell shader how to use that data
	//1st Param : For which program object i.e. to which shaders are linked,this call is applied.
	//2nd Param : What is the CPU RAM memory location
	//3rd Param : By which name shader knows it in its program
	//This call only creates a link but actual data is sent in glBufferData to a memory location in GPU and this memory location is bound (using named objects) to CPUs YDK_ATTRIBUTE_VERTEX in glVertexAttribPointer call.
	glBindAttribLocation(gShaderProgramObject,YDK_ATTRIBUTE_VERTEX,"vPosition");
	glBindAttribLocation(gShaderProgramObject,YDK_ATTRIBUTE_COLOR,"vColor");

	//Link Program
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject,GL_LINK_STATUS,&iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = NULL;
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject,iInfoLogLength,&written,szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//We will get location of Uniform matrices required for shaders which we will send to shaders in display().
	gMVPUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");

	const GLfloat pyramidVertices[] = {
		0.0f, 1.0f, 0.0f,
	   -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f };
										

	const GLfloat pyramidColor[] = { 
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
    	0.0f, 0.0f, 1.0f,
	
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
	  
	    1.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 
	
		1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f,
	    0.0f, 1.0f, 0.0f };

	const GLfloat cubeVertices[] = 
	{ 
		1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, -1.0f, 1.0f,
	   -1.0f, -1.0f, 1.0f,
	   -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f,
	   -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
	                         
		1.0f, -1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
	   -1.0f, 1.0f, -1.0f,
	    1.0f, 1.0f, -1.0f,
									 
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
	                                
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
	    1.0f, -1.0f, -1.0f };

	const GLfloat cubeColor[] =
	{   1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};
	                          

	//********************TRIANGLE VAO*******************
	//Generate Vertex Array Objects
	//1st Param : How Many Array Objects are to Be generated
	//2nd Param : Address of the variable in which object is to be stored
	glGenVertexArrays(1,&gVao_pyramid);
	glBindVertexArray(gVao_pyramid);//Bind to the Vertex Array Object

	//Generate Vertex Buffer Objects
	//1st Param : How many buffer objects are to generated
	//2nd Param : Address of variable
	glGenBuffers(1,&gVbo_pyramid_position);
	//Bind to Vertex Buffer Object
	//This call binds the data buffer in GPU memory but GPU has multiple buffers in its memory hence
	//1st Param : Which Memory Buffer this call is working on
	//2nd Param : Which Buffer object is to be bound
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_pyramid_position);

	//Send Data which is currently in CPU RAM to recently allocated memory in GPU RAM
	//1st Param : Where should the data be sent
	//2nd Param : what is the size of data
	//3rd Param : Actual Data
	//4th Param : When the Data should be copied/sent
	glBufferData(GL_ARRAY_BUFFER,sizeof(pyramidVertices),pyramidVertices,GL_STATIC_DRAW);

	//Sets Vertex Attribute pointers which are mapped in glBindAttrib at line number 380.This call specifically tells OpenGL how the data which is sent is to be used.
	//1st Param : Which is the identifier at CPU RAM for the given VBO and Data
	//2nd Param : Property of the attribute in term of its dimensions
				  //For Co-ordinates it will be 3 as for (x,y,z)
				  //For Color it may be 3 or 4 (Depending on RGB or RGBA)
				  //For Texture it maybe 2
				  //For Normals it maybe 3
	//3rd Param : Type of data which is to be used
	//4th Param : Should the data be normalized or not
	//5th Param : Strides/Jumps.This Param is used if we are using interleaved arrays and require to specify offset of each attribute i.e. Vertex or Color or Texture or Normal
	//6th Param : Data to which this applies.In our case it is NULL because we are already bound with a Vbo to which we have passed data
	glVertexAttribPointer(YDK_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,NULL);

	//Enable the attribute which we mapped in above data
	glEnableVertexAttribArray(YDK_ATTRIBUTE_VERTEX);

	//Unbinding
	glBindBuffer(GL_ARRAY_BUFFER , 0);

	glGenBuffers(1,&gVbo_pyramid_color);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_pyramid_color);

	glBufferData(GL_ARRAY_BUFFER,sizeof(pyramidColor),pyramidColor,GL_STATIC_DRAW);
	glVertexAttribPointer(YDK_ATTRIBUTE_COLOR,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(YDK_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);


	//******************SQAURE VAO***********************************

	glGenVertexArrays(1,&gVao_cube);
	glBindVertexArray(gVao_cube);

	glGenBuffers(1,&gVbo_cube_position);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_cube_position);

	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(YDK_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(YDK_ATTRIBUTE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&gVbo_cube_color);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_cube_color);

	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeColor),cubeColor,GL_STATIC_DRAW);
	glVertexAttribPointer(YDK_ATTRIBUTE_COLOR,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(YDK_ATTRIBUTE_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	

	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gPerspectiveProjectionMatrix = mat4::identity();
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void uninitialize();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Execute the linked program
	glUseProgram(gShaderProgramObject);

	//Create Model-View and Model-View-Projection Matrix as we did in Fixed Function Pipeline 
	mat4 modelViewmatrix = mat4::identity();
	mat4 modelViewProjectionmatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	//Translate Perspective Matrix
	modelViewmatrix = translate(-1.5f,0.0f,-6.0f);
	//Rotate
	rotationMatrix = rotate(angle_Pyramid,0.0f,1.0f,0.0f);
	//Multiply to modelview matrix
	modelViewmatrix = modelViewmatrix * rotationMatrix;
	//Create ModelView Projection Matrix
	modelViewProjectionmatrix = gPerspectiveProjectionMatrix * modelViewmatrix;
	
	//Set mvp matrix as uniform which we have acquired in initialize
	glUniformMatrix4fv(gMVPUniform,1,GL_FALSE,modelViewProjectionmatrix);

	glBindVertexArray(gVao_pyramid);
	
	glDrawArrays(GL_TRIANGLES,0,12);

	glBindVertexArray(0);

	modelViewmatrix = mat4::identity();
	modelViewProjectionmatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	//Translate Perspective Matrix
	modelViewmatrix = translate(1.5f, 0.0f, -6.0f);
	scaleMatrix = scale(0.75f,0.75f,0.75f);
	modelViewmatrix = modelViewmatrix * scaleMatrix;

	//Rotate
	rotationMatrix = rotate(angle_Cube,angle_Cube,angle_Cube);
	//Multiply to modelview matrix
	modelViewmatrix = modelViewmatrix * rotationMatrix;
	//Create ModelView Projection Matrix
	modelViewProjectionmatrix = gPerspectiveProjectionMatrix * modelViewmatrix;
	glUniformMatrix4fv(gMVPUniform, 1, GL_FALSE, modelViewProjectionmatrix);

	glBindVertexArray(gVao_cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	glBindVertexArray(0);

	//Stop Using Program
	glUseProgram(0);
	SwapBuffers(ghDC);
}

void update()
{
	angle_Pyramid = angle_Pyramid + 1.0f;
	if (angle_Pyramid >= 360.0f)
	{
		angle_Pyramid = 0.0f;
	}

	angle_Cube = angle_Cube + 1.0f;
	if (angle_Cube >= 360.0f)
	{
		angle_Cube = 0.0f;
	}
}


void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Setting Perspective Matrix
	gPerspectiveProjectionMatrix = perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
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

	if (gVbo_cube_color)
	{
		glDeleteBuffers(1, &gVbo_cube_color);
		gVbo_cube_color = 0;
	}
	if (gVbo_cube_position)
	{
		glDeleteBuffers(1,&gVbo_cube_position);
		gVbo_cube_position = 0;
	}

	if (gVao_cube)
	{
		glDeleteVertexArrays(1,&gVao_cube);
		gVao_cube = 0;
	}

	if (gVbo_pyramid_color)
	{
		glDeleteBuffers(1,&gVbo_pyramid_color);
		gVbo_pyramid_color = 0;
	}

	if (gVbo_pyramid_position)
	{
		glDeleteBuffers(1,&gVbo_pyramid_position);
		gVbo_pyramid_position = 0;
	}

	if (gVao_pyramid)
	{
		glDeleteVertexArrays(1,&gVao_pyramid);
		gVao_pyramid = 0;
	}
	//Detach Shader Programs
	glDetachShader(gShaderProgramObject,gFragmentShaderObject);
	glDetachShader(gShaderProgramObject,gVertexShaderObject);

	//Delete Shader Objects
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	//Delete Program Object
	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;

	//Unlink Shader Program
	glUseProgram(0);

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghGLRC);
	ghGLRC = NULL;

	ReleaseDC(ghWnd, ghDC);
	ghDC = NULL;

	DestroyWindow(ghWnd);

	if (g_fp)
	{
		fprintf(g_fp, "Log File Is Successfully Closed.\n");
		fclose(g_fp);
		g_fp = NULL;
	}
}

