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
	vector<Matrix<double,Dynamic,Dynamic>> vin_d, vout_d;
	vector<Matrix<Rational,Dynamic,Dynamic>> vin_r, vout_r;
	Matrix<double,Dynamic,Dynamic> in_d, out_d;
	Matrix<Rational,Dynamic,Dynamic> in_r,out_r;

	// trivial cases
	in_d.resize(0,0); out_d.resize(0,0);
	vin_d.push_back(in_d); vout_d.push_back(out_d);
	in_r.resize(0,0); out_r.resize(0,0);
	vin_r.push_back(in_r); vout_r.push_back(out_r);

	in_d.resize(1,1); out_d.resize(1,1);
	in_d << 0; out_d << 0;
	vin_d.push_back(in_d); vout_d.push_back(out_d);
	in_d << 1.2; out_d << 1.0;
	vin_d.push_back(in_d); vout_d.push_back(out_d);

	in_r.resize(1,1); out_r.resize(1,1);
	in_r << 0; out_r << 0;
	vin_r.push_back(in_r); vout_r.push_back(out_r);
	in_r << Rational(12,10); out_r << 1;
	vin_r.push_back(in_r); vout_r.push_back(out_r);

	// small cases
	in_d.resize(4,4); out_d.resize(4,4);
	in_d << 1,0,0,0,
			0,1,2,2,
			0,0,0,2,
			0,0,0,1;
	out_d << 1,0,0,0,
			 0,1,2,0,
			 0,0,0,1,
			 0,0,0,0;
	vin_d.push_back(in_d); vout_d.push_back(out_d);
	in_r.resize(4,4); out_r.resize(4,4);
	in_r << 1,0,0,0,
			0,1,2,2,
			0,0,0,2,
			0,0,0,1;
	out_r << 1,0,0,0,
			 0,1,2,0,
			 0,0,0,1,
			 0,0,0,0;
	vin_r.push_back(in_r); vout_r.push_back(out_r);


	// do the actual checking
	assert(vin_d.size() == vout_d.size() && vin_d.size() == vin_r.size() && vin_r.size() == vout_r.size());
	for(size_t i = 0 ; i < vin_d.size() ; ++i)
	{
		assert(numericReducedRowEchelonFormNoPivot(vin_d[i]) == vout_d[i]);
		assert(numericReducedRowEchelonFormNoPivot(vin_r[i]) == vout_r[i]);
	} // end for
} // end function runLinearSystemSolvingTestSuite
