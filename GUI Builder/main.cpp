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

#include <boost/rational.hpp>

#include "eigen-rational interface code.h"
using boost::rational;
using Eigen::Matrix;

#include "linear system solving.h"
//#include "symbolic computation.h"
#include "symbol table.h"
#include "geometry.h"
#include "gui model.h"

int main()
{
	// run test suites
	runLinearSystemSolvingTestSuite();
	//Symbolic::runTestSuite();
	Symbolic::Common::testSymbolTable();
	geometry::runTestSuite();
	gui::runTestSuite();

	using geometry::RectangleSide;

	gui::Model<int> model1;

	model1.controls.emplace_back(0,0,100,100);
	model1.controls.emplace_back(20,20,80,80);

	model1.constraints.emplace_back("a",0,RectangleSide::LEFT,1,RectangleSide::LEFT);
	model1.constraints.emplace_back("a",0,RectangleSide::RIGHT,1,RectangleSide::RIGHT);
	model1.constraints.emplace_back("2a",1,RectangleSide::LEFT,1,RectangleSide::RIGHT);

	model1.constraints.emplace_back("1/2b",0,RectangleSide::BOTTOM,1,RectangleSide::BOTTOM);
	model1.constraints.emplace_back("1/2b",0,RectangleSide::TOP,1,RectangleSide::TOP);
	model1.constraints.emplace_back("b",1,RectangleSide::BOTTOM,1,RectangleSide::TOP);

	model1.save("test1.las");

	auto symbols = std::make_shared<Symbolic::Common::SymbolTable<>>();
	for(auto constraint : model1.constraints)
		auto result = constraint.parse<boost::rational<long long>>(symbols);

	gui::Model<int> model2;

	model2.load("test2.las");

	system("pause");
	return 0;
} // end function main
