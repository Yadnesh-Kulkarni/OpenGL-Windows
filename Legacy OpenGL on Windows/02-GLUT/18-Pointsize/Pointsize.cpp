#include <GL/freeglut.h>

bool bFullScreen=false;

int main(int argc,char** argv)
{
	void display(void);
	void resize(int,int);
	void keyboard(unsigned char,int,int);
	void mouse(int,int,int,int);
	void initialize(void);
	void uninitialize(void);

	//code

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL First Window :Hello World !!!");

	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();
	return(0);
}

void display(void)
{
	void displaypoint(int size);
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	displaypoint(10);
	
	//to process buffered ogl routine
	glutSwapBuffers();
}

void displaypoint(int size)
{
	 GLfloat psizes[2];
	GLfloat pstep;
	GLfloat pcurSize;
	//static int x = 0;

	// Get supported point size range and step size
	glGetFloatv(GL_POINT_SIZE_RANGE, psizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &pstep);


	// Set the initial point size
	pcurSize = psizes[0];
	glPointSize(size);
	glBegin(GL_POINTS);
	  glVertex3f(0.0f, 0.0f, 0.0f);
	  glEnd();
	glEnd();
	//SwapBuffers(ghdc);
}
void initialize(void)
{
	//to select clearing ckgrnd clear
	glClearColor(0.0f,0.0f,0.0f,0.0f); //blue
}

void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27: //escp
		glutLeaveMainLoop();
	case 'F':
	case 'f':
		if(bFullScreen==false)
		{
			glutFullScreen();
			bFullScreen=true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen=false;
		}
		break;
	default :
		break;
	}  
}

void mouse(int button,int state,int x,int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
			break;
		default:
			break;
	}
}

void resize(int width,int height)
{
	if(height==0)
		height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
}
void uninitialize(void)
{
}