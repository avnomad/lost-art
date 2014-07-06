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

	Matrix<rational<long long int>,4,6> A;
	A << 1,-1,1,-1,1,1,
		2,-1,3,0,4,2,
		3,-2,2,1,1,1,
		1,0,1,2,1,0;

	numericReducedRowEchelonFormNoPivot(A);
	LinearSystemProperties sp = numericInvestigateLinearSystem(A);

	cout << A << "\n\n" << "\nfree: " << sp.freeVariables.size() << "\nunique: " << sp.hasUniqueSolution
		<< "\nimpossible: " << sp.isImpossible << "\nequations: " << sp.nEquations
		<< "\nindependent: " << sp.nIndependentEquations << "\nvariables: " << sp.nVariables << "\n\n" << endl;

	auto solution = numericSolveLinearSystem(A);
	cout << solution.first << "\n\n" << solution.second << "\n\n" << endl;

	system("pause");
	return 0;
} // end function main
