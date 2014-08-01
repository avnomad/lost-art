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


#include "eigen-rational interface code.h"
using boost::rational;
using Eigen::Matrix;

#include "linear system solving.h"
//#include "symbolic computation.h"
#include "gui model.h"

int main()
{
	// run test suites
	runLinearSystemSolvingTestSuite();
	//Symbolic::runTestSuite();
	gui::runTestSuite();

	gui::Model<int> model1;

	model1.controls.emplace_back(0,0,100,100);
	model1.controls.emplace_back(20,20,80,80);

	model1.constraints.emplace_back("a",0,gui::RectangleSide::LEFT,1,gui::RectangleSide::LEFT);
	model1.constraints.emplace_back("a",0,gui::RectangleSide::RIGHT,1,gui::RectangleSide::RIGHT);
	model1.constraints.emplace_back("2a",1,gui::RectangleSide::LEFT,1,gui::RectangleSide::RIGHT);

	model1.constraints.emplace_back("1/2b",0,gui::RectangleSide::BOTTOM,1,gui::RectangleSide::BOTTOM);
	model1.constraints.emplace_back("1/2b",0,gui::RectangleSide::TOP,1,gui::RectangleSide::TOP);
	model1.constraints.emplace_back("b",1,gui::RectangleSide::BOTTOM,1,gui::RectangleSide::TOP);

	model1.save("test1.las");

	gui::Model<int> model2;

	model2.load("test2.las");

	system("pause");
	return 0;
} // end function main
