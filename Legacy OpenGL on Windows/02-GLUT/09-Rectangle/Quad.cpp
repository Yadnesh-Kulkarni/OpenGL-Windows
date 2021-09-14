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
	//return(0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //to clear all pixel
	
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	  glVertex3f(-1.0f,0.0f,0.0f);
	  glVertex3f(1.0f,0.0f,0.0f);
	  glColor3f(0.0f,1.0f,0.0f);
	  glVertex3f(0.0f,1.0f,0.0f);
	  glVertex3f(0.0f,-1.0f,0.0f);
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES);
	  glColor3f(0.0f,0.0f,1.0f);
	  
	  //20 lines above
	   glVertex3f(-1.0f,0.05f,0.0f);
	  glVertex3f(1.0f,0.05f,0.0f);

	  glVertex3f(-1.0f,0.1f,0.0f);
	  glVertex3f(1.0f,0.1f,0.0f);

	  glVertex3f(-1.0f,0.15f,0.0f);
	  glVertex3f(1.0f,0.15f,0.0f);

	  glVertex3f(-1.0f,0.2f,0.0f);
	  glVertex3f(1.0f,0.2f,0.0f);

	  glVertex3f(-1.0f,0.25f,0.0f);
	  glVertex3f(1.0f,0.25f,0.0f);

	  glVertex3f(-1.0f,0.3f,0.0f);
	  glVertex3f(1.0f,0.3f,0.0f);

	  glVertex3f(-1.0f,0.35f,0.0f);
	  glVertex3f(1.0f,0.35f,0.0f);

	  glVertex3f(-1.0f,0.4f,0.0f);
	  glVertex3f(1.0f,0.4f,0.0f);

	  glVertex3f(-1.0f,0.45f,0.0f);
	  glVertex3f(1.0f,0.45f,0.0f);

	  glVertex3f(-1.0f,0.5f,0.0f);
	  glVertex3f(1.0f,0.5f,0.0f);

	  glVertex3f(-1.0f,0.55f,0.0f);
	  glVertex3f(1.0f,0.55f,0.0f);

	  glVertex3f(-1.0f,0.6f,0.0f);
	  glVertex3f(1.0f,0.6f,0.0f);

	  glVertex3f(-1.0f,0.65f,0.0f);
	  glVertex3f(1.0f,0.65f,0.0f);

	  glVertex3f(-1.0f,0.7f,0.0f);
	  glVertex3f(1.0f,0.7f,0.0f);

	  glVertex3f(-1.0f,0.75f,0.0f);
	  glVertex3f(1.0f,0.75f,0.0f);

	  glVertex3f(-1.0f,0.8f,0.0f);
	  glVertex3f(1.0f,0.8f,0.0f);

	  glVertex3f(-1.0f,0.85f,0.0f);
	  glVertex3f(1.0f,0.85f,0.0f);

	  glVertex3f(-1.0f,0.9f,0.0f);
	  glVertex3f(1.0f,0.9f,0.0f);

	  glVertex3f(-1.0f,0.95f,0.0f);
	  glVertex3f(1.0f,0.95f,0.0f);

	  glVertex3f(-1.0f,1.0f,0.0f);
	  glVertex3f(1.0f,1.0f,0.0f);

	  //20 lines below

	    glVertex3f(-1.0f,-0.05f,0.0f);
	  glVertex3f(1.0f,-0.05f,0.0f);

	  glVertex3f(-1.0f,-0.1f,0.0f);
	  glVertex3f(1.0f,-0.1f,0.0f);

	  glVertex3f(-1.0f,-0.15f,0.0f);
	  glVertex3f(1.0f,-0.15f,0.0f);

	  glVertex3f(-1.0f,-0.2f,0.0f);
	  glVertex3f(1.0f,-0.2f,0.0f);

	  glVertex3f(-1.0f,-0.25f,0.0f);
	  glVertex3f(1.0f,-0.25f,0.0f);

	  glVertex3f(-1.0f,-0.3f,0.0f);
	  glVertex3f(1.0f,-0.3f,0.0f);

	  glVertex3f(-1.0f,-0.35f,0.0f);
	  glVertex3f(1.0f,-0.35f,0.0f);

	  glVertex3f(-1.0f,-0.4f,0.0f);
	  glVertex3f(1.0f,-0.4f,0.0f);

	  glVertex3f(-1.0f,-0.45f,0.0f);
	  glVertex3f(1.0f,-0.45f,0.0f);

	  glVertex3f(-1.0f,-0.5f,0.0f);
	  glVertex3f(1.0f,-0.5f,0.0f);

	  glVertex3f(-1.0f,-0.55f,0.0f);
	  glVertex3f(1.0f,-0.55f,0.0f);

	  glVertex3f(-1.0f,-0.6f,0.0f);
	  glVertex3f(1.0f,-0.6f,0.0f);

	  glVertex3f(-1.0f,-0.65f,0.0f);
	  glVertex3f(1.0f,-0.65f,0.0f);

	  glVertex3f(-1.0f,-0.7f,0.0f);
	  glVertex3f(1.0f,-0.7f,0.0f);

	  glVertex3f(-1.0f,-0.75f,0.0f);
	  glVertex3f(1.0f,-0.75f,0.0f);

	  glVertex3f(-1.0f,-0.8f,0.0f);
	  glVertex3f(1.0f,-0.8f,0.0f);

	  glVertex3f(-1.0f,-0.85f,0.0f);
	  glVertex3f(1.0f,-0.85f,0.0f);

	  glVertex3f(-1.0f,-0.9f,0.0f);
	  glVertex3f(1.0f,-0.9f,0.0f);

	  glVertex3f(-1.0f,-0.95f,0.0f);
	  glVertex3f(1.0f,-0.95f,0.0f);

	  glVertex3f(-1.0f,-1.0f,0.0f);
	  glVertex3f(1.0f,-1.0f,0.0f);
	  //20 lines of left side
	  glVertex3f(-0.05f, 1.0f, 0.0f);
	  glVertex3f(-0.05f, -1.0f, 0.0f);

	  glVertex3f(-0.1f, 1.0f, 0.0f);
	  glVertex3f(-0.1f, -1.0f, 0.0f);

	   glVertex3f(-0.15f, 1.0f, 0.0f);
	  glVertex3f(-0.15f, -1.0f, 0.0f);

	   glVertex3f(-0.2f, 1.0f, 0.0f);
	  glVertex3f(-0.2f, -1.0f, 0.0f);

	   glVertex3f(-0.25f, 1.0f, 0.0f);
	  glVertex3f(-0.25f, -1.0f, 0.0f);

	   glVertex3f(-0.3f, 1.0f, 0.0f);
	  glVertex3f(-0.3f, -1.0f, 0.0f);

	   glVertex3f(-0.35f, 1.0f, 0.0f);
	  glVertex3f(-0.35f, -1.0f, 0.0f);

	   glVertex3f(-0.4f, 1.0f, 0.0f);
	  glVertex3f(-0.4f, -1.0f, 0.0f);

	   glVertex3f(-0.45f, 1.0f, 0.0f);
	  glVertex3f(-0.45f, -1.0f, 0.0f);

	   glVertex3f(-0.5f, 1.0f, 0.0f);
	  glVertex3f(-0.5f, -1.0f, 0.0f);

	   glVertex3f(-0.55f, 1.0f, 0.0f);
	  glVertex3f(-0.55f, -1.0f, 0.0f);

	   glVertex3f(-0.6f, 1.0f, 0.0f);
	  glVertex3f(-0.6f, -1.0f, 0.0f);

	   glVertex3f(-0.65f, 1.0f, 0.0f);
	  glVertex3f(-0.65f, -1.0f, 0.0f);

	   glVertex3f(-0.7f, 1.0f, 0.0f);
	  glVertex3f(-0.7f, -1.0f, 0.0f);

	   glVertex3f(-0.75f, 1.0f, 0.0f);
	  glVertex3f(-0.75f, -1.0f, 0.0f);

	   glVertex3f(-0.8f, 1.0f, 0.0f);
	  glVertex3f(-0.8f, -1.0f, 0.0f);

	   glVertex3f(-0.85f, 1.0f, 0.0f);
	  glVertex3f(-0.85f, -1.0f, 0.0f);

	   glVertex3f(-0.9f, 1.0f, 0.0f);
	  glVertex3f(-0.9f, -1.0f, 0.0f);

	   glVertex3f(-0.95f, 1.0f, 0.0f);
	  glVertex3f(-0.95f, -1.0f, 0.0f);

	   glVertex3f(-0.1f, 1.0f, 0.0f);
	  glVertex3f(-0.1f, -1.0f, 0.0f);
	    
	  //20 vertical lines on right

	    glVertex3f(0.05f, 1.0f, 0.0f);
	  glVertex3f(0.05f, -1.0f, 0.0f);

	  glVertex3f(0.1f, 1.0f, 0.0f);
	  glVertex3f(0.1f, -1.0f, 0.0f);

	   glVertex3f(0.15f, 1.0f, 0.0f);
	  glVertex3f(0.15f, -1.0f, 0.0f);

	   glVertex3f(0.2f, 1.0f, 0.0f);
	  glVertex3f(0.2f, -1.0f, 0.0f);

	   glVertex3f(0.25f, 1.0f, 0.0f);
	  glVertex3f(0.25f, -1.0f, 0.0f);

	   glVertex3f(0.3f, 1.0f, 0.0f);
	  glVertex3f(0.3f, -1.0f, 0.0f);

	   glVertex3f(0.35f, 1.0f, 0.0f);
	  glVertex3f(0.35f, -1.0f, 0.0f);

	   glVertex3f(0.4f, 1.0f, 0.0f);
	  glVertex3f(0.4f, -1.0f, 0.0f);

	   glVertex3f(0.45f, 1.0f, 0.0f);
	  glVertex3f(0.45f, -1.0f, 0.0f);

	   glVertex3f(0.5f, 1.0f, 0.0f);
	  glVertex3f(0.5f, -1.0f, 0.0f);

	   glVertex3f(0.55f, 1.0f, 0.0f);
	  glVertex3f(0.55f, -1.0f, 0.0f);

	   glVertex3f(0.6f, 1.0f, 0.0f);
	  glVertex3f(0.6f, -1.0f, 0.0f);

	   glVertex3f(0.65f, 1.0f, 0.0f);
	  glVertex3f(0.65f, -1.0f, 0.0f);

	   glVertex3f(0.7f, 1.0f, 0.0f);
	  glVertex3f(0.7f, -1.0f, 0.0f);

	   glVertex3f(0.75f, 1.0f, 0.0f);
	  glVertex3f(0.75f, -1.0f, 0.0f);

	   glVertex3f(0.8f, 1.0f, 0.0f);
	  glVertex3f(0.8f, -1.0f, 0.0f);

	   glVertex3f(0.85f, 1.0f, 0.0f);
	  glVertex3f(0.85f, -1.0f, 0.0f);

	   glVertex3f(0.9f, 1.0f, 0.0f);
	  glVertex3f(0.9f, -1.0f, 0.0f);

	   glVertex3f(0.95f, 1.0f, 0.0f);
	  glVertex3f(0.95f, -1.0f, 0.0f);

	   glVertex3f(0.1f, 1.0f, 0.0f);
	  glVertex3f(0.1f, -1.0f, 0.0f);

	   	    //square
	  glColor3f(1.0f,1.0f,0.0f);
	  glVertex3f(0.5f,0.5f,0.0f);
	  glVertex3f(-0.5f,0.5f,0.0f);

	  glVertex3f(-0.5f,0.5f,0.0f);
	  glVertex3f(-0.5f,-0.5f,0.0f);

	  glVertex3f(-0.5f,-0.5f,0.0f);
	   glVertex3f(0.5f,-0.5f,0.0f);

	   glVertex3f(0.5f,-0.5f,0.0f);
	   glVertex3f(0.5f,0.5f,0.0f);

	  glEnd();
	//to process buffered ogl routine
	glutSwapBuffers();
}

void initialize(void)
{
	//to select clearing ckgrnd clear
	glClearColor(0.0f,0.0f,0.0f,0.0f); 
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
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}
void uninitialize(void)
{
}