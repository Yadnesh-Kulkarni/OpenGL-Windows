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
	void displaytriangle(void);
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	glPointSize(3);
	displaytriangle();
	
	//to process buffered ogl routine
	glutSwapBuffers();
}

void displaytriangle(void)
{
	 float x = 0.1f;
	 float y = 0.1f;
	//glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glLineWidth(5);
	glBegin(GL_LINES);
	//glColor3f(1.0f, 1.0f, 1.0f);
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

		glVertex3f(0.0f, y, 0.0f);
		glVertex3f(-x, -y, 0.0f);
		glVertex3f(-x, -y, 0.0f);
		glVertex3f(x,-y, 0.0f);
		glVertex3f(x, -y, 0.0f);
		glVertex3f(0.0f, y, 0.0f);
		x = x + 0.1f;
		y = y + 0.1f;
	}
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