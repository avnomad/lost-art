#include "linear system solving.h"

#include <vector>
using std::vector;

#include <cassert>

#include "eigen-rational interface code.h"
using namespace boost;
using namespace Eigen;

typedef boost::rational<long long int> Rational;

void runLinearSystemSolvingTestSuite()
{
	vector<Matrix<Rational,Dynamic,Dynamic>> in_r, out_r;
	in_r.resize(11); out_r.resize(11);

	// trivial cases
	in_r[0].resize(0,0); out_r[0].resize(0,0);

	in_r[1].resize(1,1); out_r[1].resize(1,1);
	in_r[1] << 0; out_r[1] << 0;
	in_r[2].resize(1,1); out_r[2].resize(1,1);
	in_r[2] << Rational(12,10); out_r[2] << 1;

	// small cases
	in_r[3].resize(4,4); out_r[3].resize(4,4);
	in_r[3] <<  1,0,0,0,
				0,1,2,2,
				0,0,0,2,
				0,0,0,1;
	out_r[3] << 1,0,0,0,
				0,1,2,0,
				0,0,0,1,
				0,0,0,0;

	// examples from my linear algebra book
	in_r[4].resize(3,5); out_r[4].resize(3,5);
	in_r[4] << 1,4,1,3,-6,
				2,9,3,4,-4,
				2,6,3,4,-10;
	out_r[4] << 1,0,0,5,-20,
				0,1,0,0,2,
				0,0,1,-2,6;

	in_r[5].resize(4,6); out_r[5].resize(4,6);
	in_r[5] <<  1,-1,1,-1,1,1,
				2,-1,3,0,4,2,
				3,-2,2,1,1,1,
				1,0,1,2,1,0;
	out_r[5] << 1,0,0,3,-1,-1,
				0,1,0,3,0,-1,
				0,0,1,-1,2,1,
				0,0,0,0,0,0;

	in_r[6].resize(4,3); out_r[6].resize(4,3);
	in_r[6] <<  1,1,1,
				1,1,0,
				0,-1,-1,
				1,0,1;
	out_r[6] << 1,0,0,
				0,1,0,
				0,0,1,
				0,0,0;

	in_r[7].resize(4,3); out_r[7].resize(4,3);
	in_r[7] <<  1,1,1,
				1,1,1,
				-1,-1,-1,
				1,1,1;
	out_r[7] << 1,1,1,
				0,0,0,
				0,0,0,
				0,0,0;

	in_r[8].resize(4,3); out_r[8].resize(4,3);
	in_r[8] <<  1,1,1,
				1,1,-2,
				2,-1,-1,
				1,-1,1;
	out_r[8] << 1,0,0,
				0,1,0,
				0,0,1,
				0,0,0;

	in_r[9].resize(4,4); out_r[9].resize(4,4);
	in_r[9] <<  1,-1,2,1,
				0,4,-3,9,
				0,0,0,0,
				0,0,0,0;
	out_r[9] << 1,0,Rational(5,4),Rational(13,4),
				0,1,Rational(-3,4),Rational(9,4),
				0,0,0,0,
				0,0,0,0;

	in_r[10].resize(4,5); out_r[10].resize(4,5);
	in_r[10] << 0,1,-2,3,0,
				1,-3,2,1,4,
				-2,4,1,2,-2,
				3,-5,4,2,1;
	out_r[10] << 1,0,0,0,Rational(-306,73),
				 0,1,0,0,Rational(-205,73),
				 0,0,1,0,Rational(-32,73),
				 0,0,0,1,Rational(47,73);

	// do the actual checking
	assert(in_r.size() == out_r.size());
	for(size_t i = 0 ; i < in_r.size() ; ++i)
	{
		assert(numericReducedRowEchelonFormNoPivot(in_r[i]) == out_r[i]);
		Matrix<double,Dynamic,Dynamic> in_d = in_r[i].cast<double>();
		assert(numericReducedRowEchelonFormNoPivot(in_d) == out_r[i].cast<double>());
	} // end for
} // end function runLinearSystemSolvingTestSuite
