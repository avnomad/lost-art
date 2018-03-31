#include <vector>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>

#include "geometry.h"


#include "application customization.h"

float pixelWidth, pixelHeight; // in millimetres


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for(const auto &control : controls)
		glRectf(control.left(),control.bottom(),control.right(),control.top());

	glutSwapBuffers();
} // end function display


void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:	// escape key
			std::exit(0);
	} // end switch
} // end function keyboard


void reshape(int windowWidth, int windowHeight)
{
	glViewport(0, 0, windowWidth, windowHeight);
	glLoadIdentity();
	gluOrtho2D(0, pixelWidth*windowWidth, 0, pixelHeight*windowHeight);
	updateCoordinates(pixelWidth*windowWidth,pixelHeight*windowHeight,pixelWidth,pixelHeight);
} // end function reshape


int main(int argc, char **argv)
{
	// glut initialization
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(700,160);
	glutCreateWindow("LostArt Test Application");

	// glew initialization
	glewInit();

	// OpenGL initialization
	glColor3f(1.0,0.75,0.0);	// gold
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_LINE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // consider adding alpha planes to framebuffer
	glEnable(GL_BLEND); // and change factors for optimum polygon antialiasing.
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	// application initialization
	pixelWidth = (float)glutGet(GLUT_SCREEN_WIDTH_MM) / glutGet(GLUT_SCREEN_WIDTH);
	pixelHeight = (float)glutGet(GLUT_SCREEN_HEIGHT_MM) / glutGet(GLUT_SCREEN_HEIGHT);

	// event handling initialization
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
} // end function main
