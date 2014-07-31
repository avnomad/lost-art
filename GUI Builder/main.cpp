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


	system("pause");
	return 0;
} // end function main
