#include "linear system solving.h"

#include <vector>
using std::vector;

#include <tuple>
using std::get;

#include <algorithm>
using std::max;

#include <cassert>

#include "eigen-rational interface code.h"
using namespace boost;
using namespace Eigen;

typedef boost::rational<long long int> Rational;

void runLinearSystemSolvingTestSuite()
{
	const size_t nTests = 11;
	vector<Matrix<Rational,Dynamic,Dynamic>> in_r, ech_r, base_r;
	vector<LinearSystemProperties> prop_r, propU_r;
	vector<Matrix<Rational,Dynamic,1>> offs_r;

	in_r.resize(nTests);
	ech_r.resize(nTests);
	base_r.resize(nTests);
	offs_r.resize(nTests);
	prop_r.resize(nTests);
	propU_r.resize(nTests);

	// trivial cases
	in_r[0].resize(0,0);
	ech_r[0].resize(0,0);
	base_r[0].resize(0,1);
	offs_r[0].resize(0);
	prop_r[0] = LinearSystemProperties(0,0,0,false,true); // let's say the null (not zero) column vector is the unique solution...
	propU_r[0] = LinearSystemProperties(0,0,0,false,true,0);

	in_r[1].resize(1,1);
	in_r[1] << 0;
	ech_r[1].resize(1,1);
	ech_r[1] << 0;
	base_r[1].resize(0,1);
	offs_r[1].resize(0);
	prop_r[1] = LinearSystemProperties(1,0,0,false,true);
	propU_r[1] = LinearSystemProperties(1,0,0,false,true,0);

	in_r[2].resize(1,1);
	in_r[2] << Rational(12,10);
	ech_r[2].resize(1,1);
	ech_r[2] << 1;
	base_r[2].resize(0,0);
	offs_r[2].resize(0);
	prop_r[2] = LinearSystemProperties(1,0,0,true,false);
	propU_r[2] = LinearSystemProperties(1,0,0,true,false,0);

	// small cases
	in_r[3].resize(4,4); ech_r[3].resize(4,4);
	in_r[3] <<  1,0,0,0,
				0,1,2,2,
				0,0,0,2,
				0,0,0,1;
	ech_r[3] << 1,0,0,0,
				0,1,2,0,
				0,0,0,1,
				0,0,0,0;
	base_r[3].resize(3,0);
	offs_r[3].resize(0);
	prop_r[3] = LinearSystemProperties(4,3,2,true,false);
	prop_r[3].freeVariables.push_back(2);
	propU_r[3] = LinearSystemProperties(4,0,0,true,false,3);
	propU_r[3].boundUnknownConstants.push_back(0);
	propU_r[3].boundUnknownConstants.push_back(1);

	// examples from my linear algebra book
	in_r[4].resize(3,5); ech_r[4].resize(3,5);
	in_r[4] << 1,4,1,3,-6,
				2,9,3,4,-4,
				2,6,3,4,-10;
	ech_r[4] << 1,0,0,5,-20,
				0,1,0,0,2,
				0,0,1,-2,6;
	base_r[4].resize(4,1);
	base_r[4] << -5,0,2,1;
	offs_r[4].resize(4);
	offs_r[4] << -20,2,6,0;
	prop_r[4] = LinearSystemProperties(3,4,3,false,false);
	prop_r[4].freeVariables.push_back(3);
	propU_r[4] = LinearSystemProperties(3,0,0,false,true,4);
	propU_r[4].boundUnknownConstants.push_back(0);
	propU_r[4].boundUnknownConstants.push_back(1);
	propU_r[4].boundUnknownConstants.push_back(2);

	in_r[5].resize(4,6); ech_r[5].resize(4,6);
	in_r[5] <<  1,-1,1,-1,1,1,
				2,-1,3,0,4,2,
				3,-2,2,1,1,1,
				1,0,1,2,1,0;
	ech_r[5] << 1,0,0,3,-1,-1,
				0,1,0,3,0,-1,
				0,0,1,-1,2,1,
				0,0,0,0,0,0;
	base_r[5].resize(5,2);
	base_r[5] << -3,1,
				 -3,0,
				 1,-2,
				 1,0,
				 0,1;
	offs_r[5].resize(5);
	offs_r[5] << -1,-1,1,0,0;
	prop_r[5] = LinearSystemProperties(4,5,3,false,false);
	prop_r[5].freeVariables.push_back(3);
	prop_r[5].freeVariables.push_back(4);
	propU_r[5] = LinearSystemProperties(4,0,0,false,true,5);
	propU_r[5].boundUnknownConstants.push_back(0);
	propU_r[5].boundUnknownConstants.push_back(1);
	propU_r[5].boundUnknownConstants.push_back(2);

	in_r[6].resize(4,4); ech_r[6].resize(4,4);
	in_r[6] <<  1,1,1,0,
				1,1,0,0,
				0,-1,-1,0,
				1,0,1,0;
	ech_r[6] << 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,0;
	base_r[6].resize(3,1);
	base_r[6] << 0,0,0;
	offs_r[6].resize(3);
	offs_r[6] << 0,0,0;
	prop_r[6] = LinearSystemProperties(4,3,3,false,true);
	propU_r[6] = LinearSystemProperties(4,0,0,false,true,3);
	propU_r[6].boundUnknownConstants.push_back(0);
	propU_r[6].boundUnknownConstants.push_back(1);
	propU_r[6].boundUnknownConstants.push_back(2);

	in_r[7].resize(4,4); ech_r[7].resize(4,4);
	in_r[7] <<  1,1,1,0,
				1,1,1,0,
				-1,-1,-1,0,
				1,1,1,0;
	ech_r[7] << 1,1,1,0,
				0,0,0,0,
				0,0,0,0,
				0,0,0,0;
	base_r[7].resize(3,2);
	base_r[7] << -1,-1,
				  1, 0,
				  0, 1;
	offs_r[7].resize(3);
	offs_r[7] << 0,0,0;
	prop_r[7] = LinearSystemProperties(4,3,1,false,false);
	prop_r[7].freeVariables.push_back(1);
	prop_r[7].freeVariables.push_back(2);
	propU_r[7] = LinearSystemProperties(4,0,0,false,true,3);
	propU_r[7].boundUnknownConstants.push_back(0);

	in_r[8].resize(4,4); ech_r[8].resize(4,4);
	in_r[8] <<  1,1,1,0,
				1,1,-2,0,
				2,-1,-1,0,
				1,-1,1,0;
	ech_r[8] << 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,0;
	base_r[8].resize(3,1);
	base_r[8] << 0,0,0;
	offs_r[8].resize(3);
	offs_r[8] << 0,0,0;
	prop_r[8] = LinearSystemProperties(4,3,3,false,true);
	propU_r[8] = LinearSystemProperties(4,0,0,false,true,3);
	propU_r[8].boundUnknownConstants.push_back(0);
	propU_r[8].boundUnknownConstants.push_back(1);
	propU_r[8].boundUnknownConstants.push_back(2);

	in_r[9].resize(4,4); ech_r[9].resize(4,4);
	in_r[9] <<  1,-1,2,1,
				0,4,-3,9,
				0,0,0,0,
				0,0,0,0;
	ech_r[9] << 1,0,Rational(5,4),Rational(13,4),
				0,1,Rational(-3,4),Rational(9,4),
				0,0,0,0,
				0,0,0,0;
	base_r[9].resize(3,1);
	base_r[9] << Rational(-5,4),Rational(3,4),1;
	offs_r[9].resize(3);
	offs_r[9] << Rational(13,4),Rational(9,4),0;
	prop_r[9] = LinearSystemProperties(4,3,2,false,false);
	prop_r[9].freeVariables.push_back(2);
	propU_r[9] = LinearSystemProperties(4,0,0,false,true,3);
	propU_r[9].boundUnknownConstants.push_back(0);
	propU_r[9].boundUnknownConstants.push_back(1);

	in_r[10].resize(4,5); ech_r[10].resize(4,5);
	in_r[10] << 0,1,-2,3,0,
				1,-3,2,1,4,
				-2,4,1,2,-2,
				3,-5,4,2,1;
	ech_r[10] << 1,0,0,0,Rational(-306,73),
				 0,1,0,0,Rational(-205,73),
				 0,0,1,0,Rational(-32,73),
				 0,0,0,1,Rational(47,73);
	base_r[10].resize(4,1);
	base_r[10] << 0,0,0,0;
	offs_r[10].resize(4);
	offs_r[10] << Rational(-306,73),Rational(-205,73),Rational(-32,73),Rational(47,73);
	prop_r[10] = LinearSystemProperties(4,4,4,false,true);
	propU_r[10] = LinearSystemProperties(4,0,0,false,true,4);
	propU_r[10].boundUnknownConstants.push_back(0);
	propU_r[10].boundUnknownConstants.push_back(1);
	propU_r[10].boundUnknownConstants.push_back(2);
	propU_r[10].boundUnknownConstants.push_back(3);

	// do the actual checking
	assert(in_r.size() == ech_r.size() && ech_r.size() == base_r.size() && base_r.size() == offs_r.size() && offs_r.size() == prop_r.size());
	for(size_t i = 0 ; i < in_r.size() ; ++i)
	{
		assert(numericReducedRowEchelonFormNoPivot(in_r[i]) == ech_r[i]);
		Matrix<double,Dynamic,Dynamic> in_d = in_r[i].cast<double>();
		assert(numericReducedRowEchelonFormNoPivot(in_d) == ech_r[i].cast<double>());

		assert(numericInvestigateLinearSystem(ech_r[i]) == prop_r[i]);
		assert(numericSolveLinearSystem(ech_r[i]).first == base_r[i]);
		assert(numericSolveLinearSystem(ech_r[i]).second == offs_r[i]);

		assert(numericInvestigateLinearSystem(ech_r[i].cast<double>()) == prop_r[i]);
		assert(numericSolveLinearSystem(ech_r[i].cast<double>()).first == base_r[i].cast<double>());
		assert(numericSolveLinearSystem(ech_r[i].cast<double>()).second == offs_r[i].cast<double>());

		assert(semiSymbolicInvestigateLinearSystem(ech_r[i],0) == prop_r[i]);
		assert(get<0>(semiSymbolicSolveLinearSystem(ech_r[i],0)) == base_r[i]);
		assert(get<1>(semiSymbolicSolveLinearSystem(ech_r[i],0)) == offs_r[i]);
		assert(get<2>(semiSymbolicSolveLinearSystem(ech_r[i],0)) == (Matrix<Rational,Dynamic,Dynamic>(prop_r[i].nUnknownConstants,!prop_r[i].isImpossible)));
		assert(get<3>(semiSymbolicSolveLinearSystem(ech_r[i],0)) == (Matrix<Rational,Dynamic,1>()));

		assert(semiSymbolicInvestigateLinearSystem(ech_r[i],propU_r[i].nUnknownConstants) == propU_r[i]);
		assert(get<0>(semiSymbolicSolveLinearSystem(ech_r[i],propU_r[i].nUnknownConstants)) == (Matrix<Rational,Dynamic,Dynamic>(propU_r[i].nVariables,
			propU_r[i].isImpossible?0:max(size_t(1),propU_r[i].freeVariables.size()+propU_r[i].nUnknownConstants-propU_r[i].boundUnknownConstants.size()))));
		assert(get<1>(semiSymbolicSolveLinearSystem(ech_r[i],propU_r[i].nUnknownConstants)) == (Matrix<Rational,Dynamic,1>()));
		assert(get<2>(semiSymbolicSolveLinearSystem(ech_r[i],propU_r[i].nUnknownConstants)) == base_r[i]);
		assert(get<3>(semiSymbolicSolveLinearSystem(ech_r[i],propU_r[i].nUnknownConstants)) == offs_r[i]);

		assert(semiSymbolicInvestigateLinearSystem(ech_r[i].cast<double>(),0) == prop_r[i]);
		assert(get<0>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),0)) == base_r[i].cast<double>());
		assert(get<1>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),0)) == offs_r[i].cast<double>());
		assert(get<2>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),0)) == (Matrix<double,Dynamic,Dynamic>(0,!prop_r[i].isImpossible)));
		assert(get<3>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),0)) == (Matrix<double,Dynamic,1>()));

		assert(semiSymbolicInvestigateLinearSystem(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants) == propU_r[i]);
		assert(get<0>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)) == (Matrix<double,Dynamic,Dynamic>(propU_r[i].nVariables,
			propU_r[i].isImpossible?0:max(size_t(1),propU_r[i].freeVariables.size()+propU_r[i].nUnknownConstants-propU_r[i].boundUnknownConstants.size()))));
		assert(get<1>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)) == (Matrix<double,Dynamic,1>()));
		assert(get<2>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)) == base_r[i].cast<double>());
		assert(get<3>(semiSymbolicSolveLinearSystem(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)) == offs_r[i].cast<double>());
	} // end for
} // end function runLinearSystemSolvingTestSuite
