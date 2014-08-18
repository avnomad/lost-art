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

float lastX, lastY;
float pixelWidth, pixelHeight; // in milimetres

graphene::Controls::Button<geometry::Rectangle<float>,std::ratio<1>> button1(10,10,90,50,1);
graphene::Controls::Button<geometry::Rectangle<float>,std::ratio<1>> button2(10,60,90,100,1);
decltype(button1) *lastContaining = nullptr;

list<graphene::Controls::Control<geometry::Rectangle<float>,std::ratio<1>>> controls;
unique_ptr<graphene::Controls::IShapePart<float>> selectedPart;
graphene::Controls::Control<geometry::Rectangle<float>,std::ratio<1>> *selectedControl = nullptr;
graphene::Controls::Control<geometry::Rectangle<float>,std::ratio<1>> *highlightedControl = nullptr;

void idle()
{
	glutPostRedisplay();
} // end function idle


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	button1.render();
	button2.render();

	for(auto &control : controls)
		control.render();

	if(selectedPart)
		selectedPart->render();

	glutSwapBuffers();
} // end function display


void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:	// escape key
			std::exit(0);
	} // end switch

	float fx = x*pixelWidth;
	float fy = (glutGet(GLUT_WINDOW_HEIGHT)-1 - y)*pixelHeight;

	button1.keyboardAscii(key,true,fx,fy);
	button2.keyboardAscii(key,true,fx,fy);
} // end function keyboard


void mouse(int button, int state, int x, int y)
{
	float fx = x*pixelWidth;
	float fy = (glutGet(GLUT_WINDOW_HEIGHT)-1 - y)*pixelHeight;

	if(button == GLUT_LEFT_BUTTON)
		if(button1.target != nullptr)
			static_cast<decltype(button1)*>(button1.target)->mouseButton(0,state==GLUT_DOWN,fx,fy);
		else
		{
			if(button1.contains(fx,fy))
				button1.mouseButton(0,state==GLUT_DOWN,fx,fy);
			if(button1.target == nullptr)
				if(button2.contains(fx,fy))
					button2.mouseButton(0,state==GLUT_DOWN,fx,fy);
		} // end else

	if(button == GLUT_LEFT_BUTTON)	// TODO: add bring-to-front
		if(state == GLUT_DOWN)
		{
			lastX = fx;
			lastY = fy;
			if(selectedControl)
				selectedControl->deselect();
			for(auto &control : controls)
				if(selectedPart = control.partUnderPoint(fx,fy))
				{
					selectedControl = &control.select();
					break;
				} // end if
		}
		else
		{
			selectedPart = nullptr;
		} // end else
} // end function mouse


void motion(int x, int y)
{
	float fx = x*pixelWidth;
	float fy = (glutGet(GLUT_WINDOW_HEIGHT)-1 - y)*pixelHeight;

	if(button1.target != nullptr)
	{
		if(static_cast<decltype(button1)*>(button1.target)->contains(fx,fy))
		{
			if(static_cast<decltype(button1)*>(button1.target) == lastContaining)
				static_cast<decltype(button1)*>(button1.target)->mouseMove(fx,fy);
			else
				(lastContaining = static_cast<decltype(button1)*>(button1.target))->mouseEnter(fx,fy);
		}
		else
		{
			if(static_cast<decltype(button1)*>(button1.target) == lastContaining)
			{
				lastContaining = nullptr;
				static_cast<decltype(button1)*>(button1.target)->mouseExit(fx,fy);
			}
			else
				static_cast<decltype(button1)*>(button1.target)->mouseMove(fx,fy);
		} // end else
	}
	else
	{
		if(button1.contains(fx,fy))
		{
			if(&button1 == lastContaining)
				button1.mouseMove(fx,fy);
			else
				(lastContaining = &button1)->mouseEnter(fx,fy);
		}
		else if(&button1 == lastContaining)
		{
			lastContaining = nullptr;
			button1.mouseExit(fx,fy);
		} // end else

		if(button2.contains(fx,fy))
		{
			if(&button2 == lastContaining)
				button2.mouseMove(fx,fy);
			else
				(lastContaining = &button2)->mouseEnter(fx,fy);
		}
		else if(&button2 == lastContaining)
		{
			lastContaining = nullptr;
			button2.mouseExit(fx,fy);
		} // end else
	} // end else

	if(selectedPart)
		selectedPart->move(fx-lastX,fy-lastY);
	lastX = fx;
	lastY = fy;
} // end function motion


void passiveMotion(int x, int y)
{
	float fx = x*pixelWidth;
	float fy = (glutGet(GLUT_WINDOW_HEIGHT)-1 - y)*pixelHeight;

	if(button1.target != nullptr)
	{
		if(static_cast<decltype(button1)*>(button1.target)->contains(fx,fy))
		{
			if(static_cast<decltype(button1)*>(button1.target) == lastContaining)
				static_cast<decltype(button1)*>(button1.target)->mouseMove(fx,fy);
			else
				(lastContaining = static_cast<decltype(button1)*>(button1.target))->mouseEnter(fx,fy);
		}
		else
		{
			if(static_cast<decltype(button1)*>(button1.target) == lastContaining)
			{
				lastContaining = nullptr;
				static_cast<decltype(button1)*>(button1.target)->mouseExit(fx,fy);
			}
			else
				static_cast<decltype(button1)*>(button1.target)->mouseMove(fx,fy);
		} // end else
	}
	else
	{
		if(button1.contains(fx,fy))
		{
			if(&button1 == lastContaining)
				button1.mouseMove(fx,fy);
			else
				(lastContaining = &button1)->mouseEnter(fx,fy);
		}
		else if(&button1 == lastContaining)
		{
			lastContaining = nullptr;
			button1.mouseExit(fx,fy);
		} // end else

		if(button2.contains(fx,fy))
		{
			if(&button2 == lastContaining)
				button2.mouseMove(fx,fy);
			else
				(lastContaining = &button2)->mouseEnter(fx,fy);
		}
		else if(&button2 == lastContaining)
		{
			lastContaining = nullptr;
			button2.mouseExit(fx,fy);
		} // end else
	} // end else

	if(highlightedControl)
		highlightedControl->dehighlight();
	for(auto &control : controls)
		if(control.contains(fx,fy))
		{
			highlightedControl = &control.highlight();
			break;
		} // end if
} // end function motion


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
	glutInitWindowSize(500,600);
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
	controls.emplace_back(100.0f, 10.0f,150.0f, 40.0f,1.0f);
	controls.emplace_back(100.0f, 50.0f,150.0f, 80.0f,1.0f);
	controls.emplace_back(100.0f, 90.0f,140.0f,130.0f,1.0f);
	controls.emplace_back(100.0f,140.0f,140.0f,180.0f,1.0f);

	// event handling initialization
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
} // end function main
