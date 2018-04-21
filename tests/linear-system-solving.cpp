//	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>
/*
 *	This file is part of LostArt.
 *
 *	LostArt is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	LostArt is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linear system solving.hpp"

#include <vector>
using std::vector;

#include <tuple>
using std::get;

#include <algorithm>
using std::max;

#include "eigen-rational interface code.hpp"
using namespace boost;
using namespace Eigen;

typedef boost::rational<long long int> Rational;

#define BOOST_TEST_MODULE Linear System Solving
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Linear_System_Solvers)
{
	using namespace LinearSystem;

	const size_t nTestsNumeric = 11;
	const size_t nTestsSemiSymbolic = 2;
	vector<Matrix<Rational,Dynamic,Dynamic>> in_r, ech_r, base_r, semiSymIn_r, semiSymVarBase_r, semiSymConstBase_r;
	vector<Properties> prop_r, propU_r, semiSymProp_r;
	vector<Matrix<Rational,Dynamic,1>> offs_r, semiSymVarOffset_r, semiSymConstOffset_r;
	vector<size_t> semiSymNUnknowns;

	in_r.resize(nTestsNumeric);
	ech_r.resize(nTestsNumeric);
	base_r.resize(nTestsNumeric);
	offs_r.resize(nTestsNumeric);
	prop_r.resize(nTestsNumeric);
	propU_r.resize(nTestsNumeric);

	semiSymIn_r.resize(nTestsSemiSymbolic);
	semiSymVarBase_r.resize(nTestsSemiSymbolic);
	semiSymVarOffset_r.resize(nTestsSemiSymbolic);
	semiSymProp_r.resize(nTestsSemiSymbolic);
	semiSymConstBase_r.resize(nTestsSemiSymbolic);
	semiSymConstOffset_r.resize(nTestsSemiSymbolic);
	semiSymNUnknowns.resize(nTestsSemiSymbolic);

	// trivial cases
	in_r[0].resize(0,0);
	ech_r[0].resize(0,0);
	base_r[0].resize(0,1);
	offs_r[0].resize(0);
	prop_r[0] = Properties(0,0,0,false,true); // let's say the null (not zero) column vector is the unique solution...
	propU_r[0] = Properties(0,0,0,false,true,0);

	in_r[1].resize(1,1);
	in_r[1] << 0;
	ech_r[1].resize(1,1);
	ech_r[1] << 0;
	base_r[1].resize(0,1);
	offs_r[1].resize(0);
	prop_r[1] = Properties(1,0,0,false,true);
	propU_r[1] = Properties(1,0,0,false,true,0);

	in_r[2].resize(1,1);
	in_r[2] << Rational(12,10);
	ech_r[2].resize(1,1);
	ech_r[2] << 1;
	base_r[2].resize(0,0);
	offs_r[2].resize(0);
	prop_r[2] = Properties(1,0,0,true,false);
	propU_r[2] = Properties(1,0,0,true,false,0);

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
	prop_r[3] = Properties(4,3,2,true,false);
	prop_r[3].freeVariables.push_back(2);
	propU_r[3] = Properties(4,0,0,true,false,3);
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
	prop_r[4] = Properties(3,4,3,false,false);
	prop_r[4].freeVariables.push_back(3);
	propU_r[4] = Properties(3,0,0,false,true,4);
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
	prop_r[5] = Properties(4,5,3,false,false);
	prop_r[5].freeVariables.push_back(3);
	prop_r[5].freeVariables.push_back(4);
	propU_r[5] = Properties(4,0,0,false,true,5);
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
	prop_r[6] = Properties(4,3,3,false,true);
	propU_r[6] = Properties(4,0,0,false,true,3);
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
	prop_r[7] = Properties(4,3,1,false,false);
	prop_r[7].freeVariables.push_back(1);
	prop_r[7].freeVariables.push_back(2);
	propU_r[7] = Properties(4,0,0,false,true,3);
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
	prop_r[8] = Properties(4,3,3,false,true);
	propU_r[8] = Properties(4,0,0,false,true,3);
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
	prop_r[9] = Properties(4,3,2,false,false);
	prop_r[9].freeVariables.push_back(2);
	propU_r[9] = Properties(4,0,0,false,true,3);
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
	prop_r[10] = Properties(4,4,4,false,true);
	propU_r[10] = Properties(4,0,0,false,true,4);
	propU_r[10].boundUnknownConstants.push_back(0);
	propU_r[10].boundUnknownConstants.push_back(1);
	propU_r[10].boundUnknownConstants.push_back(2);
	propU_r[10].boundUnknownConstants.push_back(3);

	// examples from my linear algebra book for the semi-symbolic solver
	semiSymIn_r[0].resize(3,8);
	semiSymIn_r[0] << 3,-2,1,-1,-1, 0, 0,0,
					  1, 1,1, 1, 0,-1, 0,0,
					  0, 5,2, 4, 0, 0,-1,0;
	semiSymNUnknowns[0] = 3;
	semiSymProp_r[0] = Properties(3,4,2,false,false,3);
	semiSymProp_r[0].freeVariables.push_back(2);
	semiSymProp_r[0].freeVariables.push_back(3);
	semiSymProp_r[0].boundUnknownConstants.push_back(0);
	semiSymVarBase_r[0].resize(4,4);
	semiSymVarBase_r[0] << Rational(-3,5),Rational(-1,5),1,Rational(-1,5),
							Rational(-2,5),Rational(-4,5),0,Rational(1,5),
							1,0,0,0,
							0,1,0,0;
	semiSymVarOffset_r[0].resize(4);
	semiSymVarOffset_r[0] << 0,0,0,0;
	semiSymConstBase_r[0].resize(3,2);
	semiSymConstBase_r[0] << 3,-1,
							 1,0,
							 0,1;
	semiSymConstOffset_r[0].resize(3);
	semiSymConstOffset_r[0] << 0,0,0;

	// test cases I've run by hand
	semiSymIn_r[1].resize(6,9);
	semiSymIn_r[1] << -1,0,0,0,0,-1,1,0,0,
					   0,1,0,0,0,-1,0,0,0,
					   0,0,1,0,Rational(-1,2),0,0,0,0,
					   0,0,0,-1,Rational(-1,2),0,0,1,0,
					   0,0,-1,1,-1,0,0,0,0,
					   1,-1,0,0,0,-2,0,0,0;
	semiSymNUnknowns[1] = 2;
	semiSymProp_r[1] = Properties(6,6,6,false,true,2);
	semiSymVarBase_r[1].resize(6,2);
	semiSymVarBase_r[1] << Rational(3,4),0,
							Rational(1,4),0,
							0,Rational(1,4),
							0,Rational(3,4),
							0,Rational(1,2),
							Rational(1,4),0;
	semiSymVarOffset_r[1].resize(6);
	semiSymVarOffset_r[1] << 0,0,0,0,0,0;
	semiSymConstBase_r[1].resize(2,2);
	semiSymConstBase_r[1] << 1,0,
								0,1;
	semiSymConstOffset_r[1].resize(2);
	semiSymConstOffset_r[1] << 0,0;

	// do the actual checking
	BOOST_REQUIRE(in_r.size() == ech_r.size() && ech_r.size() == base_r.size() && base_r.size() == offs_r.size() && offs_r.size() == prop_r.size() && prop_r.size() == propU_r.size());
	for(size_t i = 0 ; i < in_r.size() ; ++i)
	{
		BOOST_CHECK_EQUAL(numericReducedRowEchelonFormNoPivot(in_r[i]), ech_r[i]);
		Matrix<double,Dynamic,Dynamic> in_d = in_r[i].cast<double>();
		BOOST_CHECK_EQUAL(numericReducedRowEchelonFormNoPivot(in_d), ech_r[i].cast<double>());

		BOOST_CHECK_EQUAL(numericInvestigate(ech_r[i]), prop_r[i]);
		BOOST_CHECK_EQUAL(numericSolve(ech_r[i]).first, base_r[i]);
		BOOST_CHECK_EQUAL(numericSolve(ech_r[i]).second, offs_r[i]);

		BOOST_CHECK_EQUAL(numericInvestigate(ech_r[i].cast<double>()), prop_r[i]);
		BOOST_CHECK_EQUAL(numericSolve(ech_r[i].cast<double>()).first, base_r[i].cast<double>());
		BOOST_CHECK_EQUAL(numericSolve(ech_r[i].cast<double>()).second, offs_r[i].cast<double>());

		BOOST_CHECK_EQUAL(semiSymbolicInvestigate(ech_r[i],0), prop_r[i]);
		BOOST_CHECK_EQUAL(get<0>(semiSymbolicSolve(ech_r[i],0)), base_r[i]);
		BOOST_CHECK_EQUAL(get<1>(semiSymbolicSolve(ech_r[i],0)), offs_r[i]);
		BOOST_CHECK_EQUAL(get<2>(semiSymbolicSolve(ech_r[i],0)), (Matrix<Rational,Dynamic,Dynamic>(prop_r[i].nUnknownConstants,!prop_r[i].isImpossible)));
		BOOST_CHECK_EQUAL(get<3>(semiSymbolicSolve(ech_r[i],0)), (Matrix<Rational,Dynamic,1>()));

		BOOST_CHECK_EQUAL(semiSymbolicInvestigate(ech_r[i],propU_r[i].nUnknownConstants), propU_r[i]);
		BOOST_CHECK_EQUAL(get<0>(semiSymbolicSolve(ech_r[i],propU_r[i].nUnknownConstants)), (Matrix<Rational,Dynamic,Dynamic>(propU_r[i].nVariables,
			propU_r[i].isImpossible?0:max(size_t(1),propU_r[i].freeVariables.size()+propU_r[i].nUnknownConstants-propU_r[i].boundUnknownConstants.size()))));
		BOOST_CHECK_EQUAL(get<1>(semiSymbolicSolve(ech_r[i],propU_r[i].nUnknownConstants)), (Matrix<Rational,Dynamic,1>()));
		BOOST_CHECK_EQUAL(get<2>(semiSymbolicSolve(ech_r[i],propU_r[i].nUnknownConstants)), base_r[i]);
		BOOST_CHECK_EQUAL(get<3>(semiSymbolicSolve(ech_r[i],propU_r[i].nUnknownConstants)), offs_r[i]);

		BOOST_CHECK_EQUAL(semiSymbolicInvestigate(ech_r[i].cast<double>(),0), prop_r[i]);
		BOOST_CHECK_EQUAL(get<0>(semiSymbolicSolve(ech_r[i].cast<double>(),0)), base_r[i].cast<double>());
		BOOST_CHECK_EQUAL(get<1>(semiSymbolicSolve(ech_r[i].cast<double>(),0)), offs_r[i].cast<double>());
		BOOST_CHECK_EQUAL(get<2>(semiSymbolicSolve(ech_r[i].cast<double>(),0)), (Matrix<double,Dynamic,Dynamic>(0,!prop_r[i].isImpossible)));
		BOOST_CHECK_EQUAL(get<3>(semiSymbolicSolve(ech_r[i].cast<double>(),0)), (Matrix<double,Dynamic,1>()));

		BOOST_CHECK_EQUAL(semiSymbolicInvestigate(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants), propU_r[i]);
		BOOST_CHECK_EQUAL(get<0>(semiSymbolicSolve(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)), (Matrix<double,Dynamic,Dynamic>(propU_r[i].nVariables,
			propU_r[i].isImpossible?0:max(size_t(1),propU_r[i].freeVariables.size()+propU_r[i].nUnknownConstants-propU_r[i].boundUnknownConstants.size()))));
		BOOST_CHECK_EQUAL(get<1>(semiSymbolicSolve(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)), (Matrix<double,Dynamic,1>()));
		BOOST_CHECK_EQUAL(get<2>(semiSymbolicSolve(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)), base_r[i].cast<double>());
		BOOST_CHECK_EQUAL(get<3>(semiSymbolicSolve(ech_r[i].cast<double>(),propU_r[i].nUnknownConstants)), offs_r[i].cast<double>());
	} // end for

	BOOST_REQUIRE(semiSymIn_r.size() == semiSymProp_r.size() && semiSymProp_r.size() == semiSymVarBase_r.size() && semiSymVarBase_r.size() == semiSymVarOffset_r.size()
		&& semiSymVarOffset_r.size() == semiSymConstBase_r.size() && semiSymConstBase_r.size() == semiSymConstOffset_r.size() && semiSymConstOffset_r.size() == semiSymNUnknowns.size());
	for(size_t i = 0 ; i < semiSymIn_r.size() ; ++i)
	{
		Matrix<Rational,Dynamic,Dynamic> rowEchelon_r = semiSymIn_r[i];
		numericReducedRowEchelonFormNoPivot(rowEchelon_r);
		BOOST_CHECK_EQUAL(semiSymbolicInvestigate(rowEchelon_r,semiSymNUnknowns[i]), semiSymProp_r[i]);
		BOOST_CHECK_EQUAL(get<0>(semiSymbolicSolve(rowEchelon_r,semiSymNUnknowns[i])), semiSymVarBase_r[i]);
		BOOST_CHECK_EQUAL(get<1>(semiSymbolicSolve(rowEchelon_r,semiSymNUnknowns[i])), semiSymVarOffset_r[i]);
		BOOST_CHECK_EQUAL(get<2>(semiSymbolicSolve(rowEchelon_r,semiSymNUnknowns[i])), semiSymConstBase_r[i]);
		BOOST_CHECK_EQUAL(get<3>(semiSymbolicSolve(rowEchelon_r,semiSymNUnknowns[i])), semiSymConstOffset_r[i]);

		// should fix comparison with zero in numericReducedRowEchelonFormNoPivot before
		// running these test cases on doubles because they produce different results.

		//Matrix<double,Dynamic,Dynamic> rowEchelon_d = semiSymIn_r[i].cast<double>();
		//numericReducedRowEchelonFormNoPivot(rowEchelon_d);
		//BOOST_CHECK_EQUAL(semiSymbolicInvestigate(rowEchelon_d,semiSymNUnknowns[i]), semiSymProp_r[i]);
	} // end for
} // end test case
