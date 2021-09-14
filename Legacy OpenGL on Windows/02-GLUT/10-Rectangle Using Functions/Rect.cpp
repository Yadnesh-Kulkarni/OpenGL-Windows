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
	glutInitWindowSize(800,600);
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
	void drawLine(float x,float y,float z);
	void drawGrid(void);
	void drawRect(float x, float y, float z);
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	glLineWidth(3);
	drawLine(1.0f,1.0f,0.0f);
	glLineWidth(1);
	drawGrid();
	drawRect(0.5f,0.5f,0.0f);
	//to process buffered ogl routine
	glutSwapBuffers();
}

void drawLine(float x,float y,float z)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(-x, 0.0f, 0.0f);
	glVertex3f(x, 0.0f, 0.0f);
	glVertex3f(0.0f,y, 0.0f);
	glVertex3f(0.0f,-y, 0.0f);
	glEnd();
}

void drawGrid(void)
{
	int i;
	float c=0.0f;
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (i = 1; i <= 20; i++)
	{
		c = c + 0.05f;
		glVertex3f(1.0f, c, 0.0f);
		glVertex3f(-1.0f, c, 0.0f);
		glVertex3f(-1.0f, -c, 0.0f);
		glVertex3f(1.0f, -c, 0.0f);

		glVertex3f(c, 1.0f, 0.0f);
		glVertex3f(c, -1.0f, 0.0f);
		glVertex3f(-c, 1.0f, 0.0f);
		glVertex3f(-c, -1.0f, 0.0f);
		
	}
	
	glEnd();
}

void drawRect(float x,float y,float z)
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(-x, y, z);
	glVertex3f(-x, -y, z);

	glVertex3f(-x, -y, z);
	glVertex3f(x, -y, z);

	glVertex3f(x, -y, z);
	glVertex3f(x, y, z);

	glVertex3f(x, y, z);
	glVertex3f(-x, y, z);
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