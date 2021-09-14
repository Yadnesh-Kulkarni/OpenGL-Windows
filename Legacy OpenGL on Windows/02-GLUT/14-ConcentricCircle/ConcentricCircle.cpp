#include <GL/freeglut.h>
#include<math.h>
bool bFullScreen=false;
#define pi 3.14
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
	void displaycircle(void);
	void drawLine(float x,float y,float z);
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	glPointSize(3);
	displaycircle();
	
	//to process buffered ogl routine
	glutSwapBuffers();
}

void displaycircle(void)
{
	float x=0.1f;
	float angle;

	glBegin(GL_POINTS);

	for (int c = 0; c < 10; c++)
	{
		if (c == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (c == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (c == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (c == 3)
			glColor3f(0.0f, 1.0f, 1.0f);
		else if (c == 4)
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (c == 5)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (c == 6)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (c == 7)
			glColor3f(0.98f, 0.625, 0.12f);
		else if (c == 8)
			glColor3f(0.5f, 0.5f, 0.5f);
		else if (c == 9)
			glColor3f(0.60f, 0.40f, 0.12f);

	
		for (angle = 0.0f; angle< 2.0f*pi; angle = angle + 0.01f)
		{
			glVertex2f(x*cos(angle), x*sin(angle));
			
		}
		x = x + 0.1f;
	}
	glEnd();
	
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