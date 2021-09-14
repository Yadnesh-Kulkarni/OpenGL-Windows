#include <GL/freeglut.h>
#include<math.h>
bool bFullScreen=false;
#define pi 3.1415926535898
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
	void drawTriangle(float x1, float y1, float z1,float x2,float y2,float z2,float x3,float y3,float z3);
	void drawCircle(float radius);
	float side1, side2, side3, x1, x2, x3, y1, y2, y3;
	float Area, median, radius;
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	
	drawTriangle(0.0f,0.5f,0.0f,-0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f);
	x1 = 0.0f; y1 = 0.5f; 
	x2 = -0.5f; y2 = -0.5f; 
	x3 = 0.5f; y3 = -0.5f; 
	side1 = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
	side2 = sqrt(((x3 - x2)*(x3 - x2)) + ((y3 - y2)*(y3 - y2)));
	side3 = sqrt(((x1 - x3)*(x1 - x3)) + ((y1 - y3)*(y1 - y3)));
	float peri = (side1 + side2 + side3) / 2;
	median = sqrt((side1*side1) - ((side2 / 2)*(side2 / 2)));
	Area = (side2 * median) / 2;
	radius = float(Area / peri);
	drawCircle(radius);
	
	//to process buffered ogl routine
	glutSwapBuffers();
}

void drawTriangle(float x1, float y1, float z1,float x2,float y2,float z2,float x3,float y3,float z3)
{
	float side1, side2, side3;
	float Area, median, radius;
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);

	glVertex3f(x2,y2,z2);
	glVertex3f(x3,y3,z3);

	glVertex3f(x3,y3,z3);
	glVertex3f(x1,y1,z1);

	glVertex3f(x1,y1,z1);
	glVertex3f(x1,y2,z2);
	
	glEnd();
}
void drawCircle(float radius)
{
	GLint circle_points = 100;
	float angle, step;
	step = -0.5f + radius;
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 0.0f);
	for (int i = 0; i < circle_points; i++)
	{
		angle = 2 * pi * i / circle_points;
		glVertex2f((radius*cos(angle)), (radius*sin(angle)) + step);
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