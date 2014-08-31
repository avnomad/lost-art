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

#include <list>
using std::list;

#include <memory>
using std::unique_ptr;

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

int main(int argc, char **argv)
{
	// run test suites
	LinearSystem::runTestSuite();
	//Symbolic::runTestSuite();
	Symbolic::Common::testSymbolTable();
	geometry::runTestSuite();
	graphene::runTestSuite();
	GUIModel::runTestSuite();

	// glut initialization
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800,800);
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
	typedef graphene::EventAdaptors::GLUT<GUIModel::Controls::Model<float>> AdaptorType;
	AdaptorType::initialize();

	// event handling initialization
	glutIdleFunc(AdaptorType::idle);
	glutDisplayFunc(AdaptorType::display);
	glutKeyboardFunc(AdaptorType::keyboard);
	glutKeyboardUpFunc(AdaptorType::keyboardUp);
	glutSpecialFunc(AdaptorType::special);
	glutSpecialUpFunc(AdaptorType::specialUp);
	glutMouseFunc(AdaptorType::mouse);
	glutMotionFunc(AdaptorType::motion);
	glutPassiveMotionFunc(AdaptorType::passiveMotion);
	glutReshapeFunc(AdaptorType::reshape);
	glutMainLoop();

	return 0;
} // end function main
