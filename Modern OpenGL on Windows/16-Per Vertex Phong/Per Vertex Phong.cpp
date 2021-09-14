#include<Windows.h>
#include<stdio.h>
#include<GL\glew.h>
#include<gl\GL.h>
#include"vmath.h"
#include"Sphere.h"

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Sphere.lib")

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

GLuint gVao_sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint gModelMatrixUniform,gViewMatrixUniform,gProjectionMatrixUniform;
GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint gLightPositionUniform;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

GLuint keyPressedUniform;

mat4 gPerspectiveProjectionMatrix;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = {100.0f,100.0f,100.0f,1.0f};

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

float sphere_vertices[1146];
float sphere_normal[1146];
float sphere_texture[764];
unsigned short sphere_elements[2280];

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;
bool gbAnimate = false;
bool gbLight = false;

int gNumVertices;
int gNumElements;

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

	static bool Akeyispressed = false;
	static bool Lkeyispressed = false;
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
		case 0x41:
			if (Akeyispressed == false)
			{
				gbAnimate = true;
				Akeyispressed = true;
			}
			else
			{
				gbAnimate = false;
				Akeyispressed = false;
			}
			break;
		case 0x4C:
			if (Lkeyispressed == false)
			{
				gbLight = true;
				Lkeyispressed = true;
			}
			else
			{
				gbLight = false;
				Lkeyispressed = false;
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
		"in vec3 vNormal;"\
		"uniform mat4 u_model_matrix;"\
		"uniform mat4 u_view_matrix;"\
		"uniform mat4 u_projection_matrix;"\
		"uniform int u_Lkeyispressed;"\
		"uniform vec3 u_La;"\
		"uniform vec3 u_Ld;"\
		"uniform vec3 u_Ls;"\
		"uniform vec3 u_Ka;"\
		"uniform vec3 u_Kd;"\
		"uniform vec3 u_Ks;"\
		"uniform float u_material_shininess;"\
		"uniform vec4 u_light_position;"\
		"out vec3 phong_ads_color;"\
		"void main(void)"\
		"{"\
		"if(u_Lkeyispressed == 1)"\
		"{"\
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"vec3 tnorm = normalize(mat3(u_view_matrix * u_model_matrix)*vNormal);"\
		"vec3 s = normalize(vec3(u_light_position) - eyeCoordinates.xyz);"\
		"float tn_dot_ld = max(dot(tnorm,s),0.0);"\
		"vec3 ambient = u_La * u_Ka;"\
		"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;"\
		"vec3 reflection_vector = reflect(-s,tnorm);"\
		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"\
		"vec3 specular = u_Ls * u_Ks *pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininess);"\
		"phong_ads_color = ambient+diffuse+specular;"\
		"}"\
		"else"\
		"{"\
		"phong_ads_color = vec3(1.0f,1.0,1.0);"
		"}"\
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"\
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
		"in vec3 phong_ads_color;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{"\
		"FragColor = vec4(phong_ads_color,1.0);"\
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
	glBindAttribLocation(gShaderProgramObject,YDK_ATTRIBUTE_NORMAL,"vNormal");

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
	gModelMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_model_matrix");
	gViewMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_projection_matrix");
	keyPressedUniform = glGetUniformLocation(gShaderProgramObject,"u_Lkeyispressed");
	La_uniform = glGetUniformLocation(gShaderProgramObject,"u_La");
	Ld_uniform = glGetUniformLocation(gShaderProgramObject,"u_Ld");
	Ls_uniform = glGetUniformLocation(gShaderProgramObject,"u_Ls");
	Ka_uniform = glGetUniformLocation(gShaderProgramObject,"u_Ka");
	Kd_uniform = glGetUniformLocation(gShaderProgramObject,"u_Kd");
	Ks_uniform = glGetUniformLocation(gShaderProgramObject,"u_Ks");
	material_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");
	gLightPositionUniform = glGetUniformLocation(gShaderProgramObject,"u_light_position");

	getSphereVertexData(sphere_vertices,sphere_normal,sphere_texture,sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	glGenVertexArrays(1,&gVao_sphere);
	glBindVertexArray(gVao_sphere);

	glGenBuffers(1,&gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_sphere_position);

	glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_vertices),sphere_vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(YDK_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(YDK_ATTRIBUTE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);

	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normal), sphere_normal, GL_STATIC_DRAW);
	glVertexAttribPointer(YDK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(YDK_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gPerspectiveProjectionMatrix = mat4::identity();

	gbAnimate = false;
	gbLight = false;

	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void uninitialize();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Execute the linked program
	glUseProgram(gShaderProgramObject);

	if (gbLight == true)
	{
		glUniform1i(keyPressedUniform,1);

		glUniform3fv(La_uniform,1,lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);

		glUniform4fv(gLightPositionUniform,1,lightPosition);

		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);

		glUniform1f(material_shininess_uniform,material_shininess);
	}
	else
	{
		glUniform1i(keyPressedUniform,0);
	}

	//Create Model-View and Model-View-Projection Matrix as we did in Fixed Function Pipeline
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	//Translate Perspective Matrix
	modelMatrix = translate(0.0f, 0.0f, -1.75f);
	
	glUniformMatrix4fv(gModelMatrixUniform,1,GL_FALSE,modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform,1,GL_FALSE,viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform,1,GL_FALSE,gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES,gNumElements,GL_UNSIGNED_SHORT,0);

	glBindVertexArray(0);

	//Stop Using Program
	glUseProgram(0);
	SwapBuffers(ghDC);
}

void update()
{

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

	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1,&gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

	if(gVbo_sphere_normal)
	{
		glDeleteBuffers(1,&gVbo_sphere_normal);
		gVbo_sphere_normal = 0;
	}

	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1,&gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	if (gVao_sphere)
	{
		glDeleteVertexArrays(1,&gVao_sphere);
		gVao_sphere = 0;
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

