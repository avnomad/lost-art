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
#include "symbolic computation.h"



int main()
{
	try
	{
		// run test suites
		runLinearSystemSolvingTestSuite();
		Symbolic::runTestSuite();
	}
	catch(std::exception &e)
	{
		cout << e.what() << endl;
	}

	system("pause");
	return 0;
} // end function main
