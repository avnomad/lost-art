#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <numeric>
using std::numeric_limits;

#include <cmath>
using std::log;

#include <tuple>
using std::get;

#include <GL/glew.h>
#include <GL/glut.h>

#include <boost/rational.hpp>

#include "eigen-rational interface code.h"
using boost::rational;
using Eigen::Matrix;

#include "linear system solving.h"
//#include "symbolic computation.h"
#include "symbol table.h"
#include "geometry.h"
#include "graphene.h"
#include "gui model.h"

float pixelWidth, pixelHeight; // in milimetres
graphene::Controls::Button<geometry::Rectangle<float>> button(10,10,50,50,5);

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	button.render();

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, pixelWidth*windowWidth, 0, pixelHeight*windowHeight);
} // end function reshape


int main(int argc, char **argv)
{
	// run test suites
	LinearSystem::runTestSuite();
	//Symbolic::runTestSuite();
	Symbolic::Common::testSymbolTable();
	geometry::runTestSuite();
	graphene::runTestSuite();
	gui::runTestSuite();

	//using geometry::RectangleSide;

	//gui::Model<int> model1;

	//model1.controls.emplace_back(0,0,100,100);
	//model1.controls.emplace_back(20,20,80,80);

	//model1.constraints.emplace_back("a",0,RectangleSide::LEFT,1,RectangleSide::LEFT);
	//model1.constraints.emplace_back("a",0,RectangleSide::RIGHT,1,RectangleSide::RIGHT);
	//model1.constraints.emplace_back("2a",1,RectangleSide::LEFT,1,RectangleSide::RIGHT);

	//model1.constraints.emplace_back("1/2b",0,RectangleSide::BOTTOM,1,RectangleSide::BOTTOM);
	//model1.constraints.emplace_back("1/2b",0,RectangleSide::TOP,1,RectangleSide::TOP);
	//model1.constraints.emplace_back("b",1,RectangleSide::BOTTOM,1,RectangleSide::TOP);

	//model1.save("test1.las");

	//auto symbols = std::make_shared<Symbolic::Common::SymbolTable<>>();
	//for(auto constraint : model1.constraints)
	//	auto result = constraint.parse<boost::rational<long long>>(symbols);

	//model1.compile<boost::rational<long long>,float,int,std::string>("application customization.h");
	//model1.run("\"..\\Debug\\Win32\\Generated Application.exe\"");

	//gui::Model<int> model2;

	//model2.load("test2.las");

	// glut initialization
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(700,160);
	glutCreateWindow("LostArt");

	// glew initialization
	glewInit();

	// OpenGL initialization
	glColor3f(1.0,0.75,0.0);	// gold
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
