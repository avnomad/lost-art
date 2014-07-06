#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <numeric>
using std::numeric_limits;

#include <cmath>
using std::log;

#include "eigen-rational interface code.h"
using boost::rational;
using Eigen::Matrix;

#include "linear system solving.h"




int main()
{
	// run test suites
	runLinearSystemSolvingTestSuite();


	system("pause");
	return 0;
} // end function main
