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




int main()
{
	// run test suites
	runLinearSystemSolvingTestSuite();

	Matrix<rational<long long int>,3,8> A;
	A << 3,-2,1,-1,-1,0,0,0,
		 1,1,1,1,0,-1,0,0,
		 0,5,2,4,0,0,-1,0;

	numericReducedRowEchelonFormNoPivot(A);
	LinearSystemProperties sp = semiSymbolicInvestigateLinearSystem(A,3);

	cout << A << "\n\n" << "\nfree: " << sp.freeVariables.size() << "\nunique: " << sp.hasUniqueSolution
		<< "\nimpossible: " << sp.isImpossible << "\nequations: " << sp.nEquations
		<< "\nindependent: " << sp.nIndependentEquations << "\nvariables: " << sp.nVariables 
		<< "\nunknown: " << sp.nUnknownConstants << "\nbound: " << sp.boundUnknownConstants.size() << "\n\n" << endl;

	auto solution = semiSymbolicSolveLinearSystem(A,3);
	cout << get<0>(solution) << "\n\n" << get<1>(solution) << "\n\n" << get<2>(solution) << "\n\n" << get<3>(solution) << "\n\n" << endl;
	system("pause");
	return 0;
} // end function main
