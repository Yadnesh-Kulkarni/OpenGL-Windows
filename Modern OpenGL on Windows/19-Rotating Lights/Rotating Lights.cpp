#include<Windows.h>
#include<stdio.h>
#include"glew.h"
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
GLuint gShaderProgramObjectFragmentPhong = 0;

GLuint gPhongVertexShaderObject = 0;
GLuint gPhongFragmentShaderObject = 0;
GLuint gShaderProgramObjectVertexPhong = 0;

GLuint gVao_sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint gModelMatrixUniform,gViewMatrixUniform,gProjectionMatrixUniform;
GLuint gRotationMatrixUniformRed;
GLuint La_uniform_red;
GLuint Ld_uniform_red;
GLuint Ls_uniform_red;
GLuint gLightPositionUniformRed;

GLuint gRotationMatrixUniformBlue;
GLuint La_uniform_blue;
GLuint Ld_uniform_blue;
GLuint Ls_uniform_blue;
GLuint gLightPositionUniformBlue;

GLuint gRotationMatrixUniformGreen;
GLuint La_uniform_green;
GLuint Ld_uniform_green;
GLuint Ls_uniform_green;
GLuint gLightPositionUniformGreen;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

GLuint keyPressedUniform;

mat4 gPerspectiveProjectionMatrix;

GLfloat redLightAngle;
GLfloat blueLightAngle;
GLfloat greenLightAngle;

GLfloat lightAmbientRed[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseRed[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightSpecularRed[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightPositionRed[] = { 0.0f, 100.0f , 100.0f , 1.0f };

GLfloat lightAmbientBlue[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseBlue[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightSpecularBlue[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightPositionBlue[] = { 100.0f, 0.0f , 100.0f , 1.0f };

GLfloat lightAmbientGreen[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseGreen[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightSpecularGreen[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightPositionGreen[] = { 100.0f, 100.0f , 0.0f , 1.0f };

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
bool gbFragmentPhong = false;
bool gbVertexPhong = true;

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

		case 0x46:
			if (gbFragmentPhong == false)
			{
				gbFragmentPhong = true;
				gbVertexPhong = false;
			}
			break;
		case 0x51:
			if (gbEscapeKeyIsPressed == false)
				gbEscapeKeyIsPressed = true;
			break;
		case 0x56:
			if (gbVertexPhong == false)
			{
				gbVertexPhong = true;
				gbFragmentPhong = false;
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

	//PHONG SHADING IN FRAGMENT SHADER
	//******VERTEX SHADER*************
	//Create Shader Object(Vertex)


	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//Write Source Code in a String
	const GLchar *vertexShaderSourceCode =
		"#version 460 core"\
		"\n"\
		"in vec4 vPosition;"\
		"in vec3 vNormal;"\
		"uniform mat4 u_rotation_matrix_red;"\
		"uniform mat4 u_rotation_matrix_blue;"\
		"uniform mat4 u_rotation_matrix_green;"\
		"uniform mat4 u_model_matrix;"\
		"uniform mat4 u_view_matrix;"\
		"uniform mat4 u_projection_matrix;"\
		"uniform vec4 u_light_position_red;"\
		"uniform vec4 u_light_position_blue;"\
		"uniform vec4 u_light_position_green;"\
		"uniform int u_vKeyisPressed;"\
		"uniform int u_FKeyisPressed;"\
		"out vec3 transformed_normals;"\
		"out vec3 light_direction_red;"\
		"out vec3 light_direction_blue;"\
		"out vec3 light_direction_green;"\
		"out vec3 viewer_vector;"\
		"void main(void)"\
		"{"\
		"vec4 lightPosRed = u_rotation_matrix_red*u_light_position_red;"\
		"vec4 lightPosBlue = u_rotation_matrix_blue*u_light_position_blue;"\
		"vec4 lightPosGreen = u_rotation_matrix_green*u_light_position_green;"\
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix)*vNormal);"\
		"light_direction_red = normalize(vec3(lightPosRed) - eyeCoordinates.xyz);"\
		"light_direction_blue = normalize(vec3(lightPosBlue) - eyeCoordinates.xyz);"\
		"light_direction_green = normalize(vec3(lightPosGreen) - eyeCoordinates.xyz);"\
		"viewer_vector = normalize(-eyeCoordinates.xyz);"\
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
		"#version 460 core"\
		"\n"\
		"in vec3 transformed_normals;"\
		"in vec3 light_direction_red;"\
		"in vec3 light_direction_blue;"\
		"in vec3 light_direction_green;"\
		"in vec3 viewer_vector;"\
		"uniform vec3 u_La_red;"\
		"uniform vec3 u_Ld_red;"\
		"uniform vec3 u_Ls_red;"\
		"uniform vec3 u_La_blue;"\
		"uniform vec3 u_Ld_blue;"\
		"uniform vec3 u_Ls_blue;"\
		"uniform vec3 u_La_green;"\
		"uniform vec3 u_Ld_green;"\
		"uniform vec3 u_Ls_green;"\
		"uniform vec3 u_Ka;"\
		"uniform vec3 u_Kd;"\
		"uniform vec3 u_Ks;"\
		"uniform float u_material_shininess;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{"\
		"vec3 phong_ads_color;"\
		"vec3 normalized_transformed_normals = normalize(transformed_normals);"\
		"vec3 normalized_light_direction_red = normalize(light_direction_red);"\
		"vec3 normalized_light_direction_blue = normalize(light_direction_blue);"\
		"vec3 normalized_light_direction_green = normalize(light_direction_green);"\
		"vec3 normalized_viewer_vector = normalize(viewer_vector);"\
		"float tn_dot_ld_red = max(dot(normalized_transformed_normals,normalized_light_direction_red),0.0);"\
		"float tn_dot_ld_blue = max(dot(normalized_transformed_normals,normalized_light_direction_blue),0.0);"\
		"float tn_dot_ld_green = max(dot(normalized_transformed_normals,normalized_light_direction_green),0.0);"\
		"vec3 ambientRed = u_La_red * u_Ka;"\
		"vec3 diffuseRed = u_Ld_red * u_Kd * tn_dot_ld_red;"\
		"vec3 reflection_vector_red = reflect(-normalized_light_direction_red,normalized_transformed_normals);"\
		"vec3 specularRed = u_Ls_red * u_Ks *pow(max(dot(reflection_vector_red,normalized_viewer_vector),0.0),u_material_shininess);"\
		"vec3 ambientBlue = u_La_blue * u_Ka;"\
		"vec3 diffuseBlue = u_Ld_blue * u_Kd * tn_dot_ld_blue;"\
		"vec3 reflection_vector_blue = reflect(-normalized_light_direction_blue,normalized_transformed_normals);"\
		"vec3 specularBlue = u_Ls_blue * u_Ks *pow(max(dot(reflection_vector_blue,normalized_viewer_vector),0.0),u_material_shininess);"\
		"vec3 ambientGreen = u_La_green * u_Ka;"\
		"vec3 diffuseGreen = u_Ld_green * u_Kd * tn_dot_ld_green;"\
		"vec3 reflection_vector_green = reflect(-normalized_light_direction_green,normalized_transformed_normals);"\
		"vec3 specularGreen = u_Ls_green * u_Ks *pow(max(dot(reflection_vector_green,normalized_viewer_vector),0.0),u_material_shininess);"\
		"vec3 ambient = ambientRed + ambientBlue + ambientGreen;"\
		"vec3 diffuse = diffuseRed + diffuseBlue + diffuseGreen;"\
		"vec3 specular = specularRed + specularBlue + specularGreen;"\
		"phong_ads_color = ambient+diffuse+specular;"\
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
	gShaderProgramObjectFragmentPhong = glCreateProgram();

	//Attach Both Compiled Shaders to this program
	glAttachShader(gShaderProgramObjectFragmentPhong,gVertexShaderObject);
	glAttachShader(gShaderProgramObjectFragmentPhong,gFragmentShaderObject);

	glBindAttribLocation(gShaderProgramObjectFragmentPhong,YDK_ATTRIBUTE_VERTEX,"vPosition");
	glBindAttribLocation(gShaderProgramObjectFragmentPhong,YDK_ATTRIBUTE_NORMAL,"vNormal");

	//Link Program
	glLinkProgram(gShaderProgramObjectFragmentPhong);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObjectFragmentPhong,GL_LINK_STATUS,&iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObjectFragmentPhong,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = NULL;
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObjectFragmentPhong,iInfoLogLength,&written,szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}
	/*------------------------------------------------------------------------------------------------------------------------------
	--------------------------------------------------------------------------------------------------------------------------------
	--------------------------------------------------------------------------------------------------------------------------------*/
	//PHONG SHADING IN VERTEX SHADER
	gPhongVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	const char *phongVertexShading =
		"#version 450 core"\
		"\n"\
		"in vec4 vPosition;"\
		"in vec3 vNormal;"\
		"uniform mat4 u_rotation_matrix_red;"\
		"uniform mat4 u_rotation_matrix_blue;"\
		"uniform mat4 u_rotation_matrix_green;"\
		"uniform mat4 u_model_matrix;"\
		"uniform mat4 u_view_matrix;"\
		"uniform mat4 u_projection_matrix;"\
		"uniform vec3 u_La_red;"\
		"uniform vec3 u_Ld_red;"\
		"uniform vec3 u_Ls_red;"\
		"uniform vec3 u_La_blue;"\
		"uniform vec3 u_Ld_blue;"\
		"uniform vec3 u_Ls_blue;"\
		"uniform vec3 u_La_green;"\
		"uniform vec3 u_Ld_green;"\
		"uniform vec3 u_Ls_green;"\
		"uniform vec3 u_Ka;"\
		"uniform vec3 u_Kd;"\
		"uniform vec3 u_Ks;"\
		"uniform float u_material_shininess;"\
		"uniform vec4 u_light_position_red;"\
		"uniform vec4 u_light_position_blue;"\
		"uniform vec4 u_light_position_green;"\
		"out vec3 phong_ads_color;"\
		"void main(void)"\
		"{"\
		"vec4 lightPosRed = u_rotation_matrix_red*u_light_position_red;"\
		"vec4 lightPosBlue = u_rotation_matrix_blue*u_light_position_blue;"\
		"vec4 lightPosGreen = u_rotation_matrix_green*u_light_position_green;"\
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"vec3 tnorm = normalize(mat3(u_view_matrix * u_model_matrix)*vNormal);"\
		"vec3 sred = normalize(vec3(lightPosRed) - eyeCoordinates.xyz);"\
		"vec3 sblue = normalize(vec3(lightPosBlue) - eyeCoordinates.xyz);"\
		"vec3 sgreen = normalize(vec3(lightPosGreen) - eyeCoordinates.xyz);"\
		"float tn_dot_ld_red = max(dot(tnorm,sred),0.0);"\
		"float tn_dot_ld_blue = max(dot(tnorm,sblue),0.0);"\
		"float tn_dot_ld_green = max(dot(tnorm,sgreen),0.0);"\
		"vec3 ambientRed = u_La_red * u_Ka;"\
		"vec3 ambientBlue = u_La_blue * u_Ka;"\
		"vec3 ambientGreen = u_La_green * u_Ka;"\
		"vec3 diffuseRed = u_Ld_red * u_Kd * tn_dot_ld_red;"\
		"vec3 diffuseBlue = u_Ld_blue * u_Kd * tn_dot_ld_blue;"\
		"vec3 diffuseGreen = u_Ld_green * u_Kd * tn_dot_ld_green;"\
		"vec3 reflection_vector_red = reflect(-sred,tnorm);"\
		"vec3 reflection_vector_blue = reflect(-sblue,tnorm);"\
		"vec3 reflection_vector_green = reflect(-sgreen,tnorm);"\
		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"\
		"vec3 specularRed = u_Ls_red * u_Ks *pow(max(dot(reflection_vector_red,viewer_vector),0.0),u_material_shininess);"\
		"vec3 specularBlue = u_Ls_blue * u_Ks *pow(max(dot(reflection_vector_blue,viewer_vector),0.0),u_material_shininess);"\
		"vec3 specularGreen = u_Ls_blue * u_Ks *pow(max(dot(reflection_vector_green,viewer_vector),0.0),u_material_shininess);"\
		"vec3 ambient = ambientRed + ambientBlue + ambientGreen;"\
		"vec3 diffuse = diffuseRed + diffuseBlue + diffuseGreen;"\
		"vec3 specular = specularRed + specularBlue + specularGreen;"\
		"phong_ads_color = ambient+diffuse+specular;"\
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"\
		"}";

	glShaderSource(gPhongVertexShaderObject, 1, (const GLchar **)&phongVertexShading, NULL);

	//Compile Shader
	glCompileShader(gPhongVertexShaderObject);
	iInfoLogLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gPhongVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gPhongVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gPhongVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(g_fp, "Phong Vertex - Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}
	//FRAGMENT SHADER
	gPhongFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	const char *phongVertexFragmentShading =
		"#version 450 core"\
		"\n"\
		"in vec3 phong_ads_color;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{"\
		"FragColor = vec4(phong_ads_color,1.0);"\
		"}";
	glShaderSource(gPhongFragmentShaderObject, 1, (const GLchar **)&phongVertexFragmentShading, NULL);

	//Compile Shader
	glCompileShader(gPhongFragmentShaderObject);
	iInfoLogLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gPhongFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gPhongFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gPhongFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(g_fp, "Phong Vertex - Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	gShaderProgramObjectVertexPhong = glCreateProgram();

	//Attach Both Compiled Shaders to this program
	glAttachShader(gShaderProgramObjectVertexPhong, gPhongVertexShaderObject);
	glAttachShader(gShaderProgramObjectVertexPhong, gPhongFragmentShaderObject);

	glBindAttribLocation(gShaderProgramObjectVertexPhong, YDK_ATTRIBUTE_VERTEX, "vPosition");
	glBindAttribLocation(gShaderProgramObjectVertexPhong, YDK_ATTRIBUTE_NORMAL, "vNormal");

	//Link Program
	glLinkProgram(gShaderProgramObjectVertexPhong);
	iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObjectVertexPhong, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObjectVertexPhong, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = NULL;
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObjectVertexPhong, iInfoLogLength, &written, szInfoLog);
				fprintf(g_fp, "Phong Vertex - Link Error Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//We will get location of Uniform matrices required for shaders which we will send to shaders in display().

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


	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void uninitialize();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Execute the linked program
	if (gbFragmentPhong == true)
	{
		gModelMatrixUniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_model_matrix");
		gViewMatrixUniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_view_matrix");
		gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_projection_matrix");
		La_uniform_red = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_La_red");
		Ld_uniform_red = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ld_red");
		Ls_uniform_red = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ls_red");
		La_uniform_blue = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_La_blue");
		Ld_uniform_blue = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ld_blue");
		Ls_uniform_blue = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ls_blue");
		La_uniform_green = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_La_green");
		Ld_uniform_green = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ld_green");
		Ls_uniform_green = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ls_green");
		Ka_uniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ka");
		Kd_uniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Kd");
		Ks_uniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_Ks");
		material_shininess_uniform = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_material_shininess");
		gLightPositionUniformRed = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_light_position_red");
		gLightPositionUniformBlue = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_light_position_blue");
		gLightPositionUniformGreen = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_light_position_green");
		gRotationMatrixUniformRed = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_rotation_matrix_red");
		gRotationMatrixUniformBlue = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_rotation_matrix_blue");
		gRotationMatrixUniformGreen = glGetUniformLocation(gShaderProgramObjectFragmentPhong, "u_rotation_matrix_green");
		glUseProgram(gShaderProgramObjectFragmentPhong);

	}
	if(gbVertexPhong == true)
	{
		gModelMatrixUniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_model_matrix");
		gViewMatrixUniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_view_matrix");
		gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_projection_matrix");
		La_uniform_red = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_La_red");
		Ld_uniform_red = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ld_red");
		Ls_uniform_red = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ls_red");
		La_uniform_blue = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_La_blue");
		Ld_uniform_blue = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ld_blue");
		Ls_uniform_blue = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ls_blue");
		La_uniform_green = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_La_green");
		Ld_uniform_green = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ld_green");
		Ls_uniform_green = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ls_green");
		Ka_uniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ka");
		Kd_uniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Kd");
		Ks_uniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_Ks");
		material_shininess_uniform = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_material_shininess");
		gLightPositionUniformRed = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_light_position_red");
		gLightPositionUniformBlue = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_light_position_blue");
		gLightPositionUniformGreen = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_light_position_green");
		gRotationMatrixUniformRed = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_rotation_matrix_red");
		gRotationMatrixUniformBlue = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_rotation_matrix_blue");
		gRotationMatrixUniformGreen = glGetUniformLocation(gShaderProgramObjectVertexPhong, "u_rotation_matrix_green");
		glUseProgram(gShaderProgramObjectVertexPhong);
	}


	//Create Model-View and Model-View-Projection Matrix as we did in Fixed Function Pipeline
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrixRed = mat4::identity();
	mat4 rotationMatrixBlue = mat4::identity();
	mat4 rotationMatrixGreen = mat4::identity();
	//Translate Perspective Matrix
	modelMatrix = translate(0.0f, 0.0f, -1.75f);
	
	rotationMatrixRed = rotate(redLightAngle,0.0f,1.0f,0.0f);
	rotationMatrixBlue = rotate(blueLightAngle,1.0f,0.0f,0.0f);
	rotationMatrixGreen = rotate(greenLightAngle,0.0f,0.0f,-1.0f);
	
	glUniform3fv(La_uniform_red, 1, lightAmbientRed);
	glUniform3fv(Ld_uniform_red, 1, lightDiffuseRed);
	glUniform3fv(Ls_uniform_red, 1, lightSpecularRed);
	glUniformMatrix4fv(gRotationMatrixUniformRed, 1, GL_FALSE, rotationMatrixRed);
	glUniform4fv(gLightPositionUniformRed, 1, lightPositionRed);

	glUniform3fv(La_uniform_blue, 1, lightAmbientBlue);
	glUniform3fv(Ld_uniform_blue, 1, lightDiffuseBlue);
	glUniform3fv(Ls_uniform_blue, 1, lightSpecularBlue);
	glUniformMatrix4fv(gRotationMatrixUniformBlue, 1, GL_FALSE, rotationMatrixBlue);
	glUniform4fv(gLightPositionUniformBlue, 1, lightPositionBlue);

	glUniform3fv(La_uniform_green, 1, lightAmbientGreen);
	glUniform3fv(Ld_uniform_green, 1, lightDiffuseGreen);
	glUniform3fv(Ls_uniform_green, 1, lightSpecularGreen);
	glUniformMatrix4fv(gRotationMatrixUniformGreen, 1, GL_FALSE, rotationMatrixGreen);
	glUniform4fv(gLightPositionUniformGreen, 1, lightPositionGreen);

	glUniform3fv(Ka_uniform, 1, material_ambient);
	glUniform3fv(Kd_uniform, 1, material_diffuse);
	glUniform3fv(Ks_uniform, 1, material_specular);

	glUniform1f(material_shininess_uniform, material_shininess);

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
	redLightAngle = redLightAngle + 0.75f;
	if (redLightAngle >= 360.0f)
	{
		redLightAngle = 0.0f;
	}

	blueLightAngle = blueLightAngle + 0.75f;
	if (blueLightAngle >= 360.0f)
	{
		blueLightAngle = 0.0f;
	}

	greenLightAngle = greenLightAngle + 0.75f;
	if (greenLightAngle >= 360.0f)
	{
		greenLightAngle = 0.0f;
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
	glDetachShader(gShaderProgramObjectFragmentPhong,gFragmentShaderObject);
	glDetachShader(gShaderProgramObjectFragmentPhong,gVertexShaderObject);

	//Delete Shader Objects
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	//Delete Program Object
	glDeleteProgram(gShaderProgramObjectFragmentPhong);
	gShaderProgramObjectFragmentPhong = 0;

	glDetachShader(gShaderProgramObjectVertexPhong,gPhongVertexShaderObject);
	glDetachShader(gShaderProgramObjectVertexPhong,gPhongFragmentShaderObject);

	glDeleteShader(gPhongVertexShaderObject);
	gPhongVertexShaderObject = 0;
	glDeleteShader(gPhongFragmentShaderObject);
	gPhongFragmentShaderObject = 0;

	glDeleteProgram(gShaderProgramObjectVertexPhong);
	gShaderProgramObjectVertexPhong = 0;

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

