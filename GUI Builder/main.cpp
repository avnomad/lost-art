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
using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::Vector3d;





int main()
{
	Matrix<rational<long long int>,3,3> A;
	Matrix<rational<long long int>,3,3> B;
	rational<long long int> r(5,3);
	Matrix<rational<long long int>,0,0> null;
	cout << null.size() << ',' << null.rows() << ',' << null.cols() << endl;

	long long int a = boost::rational_cast<long long int>(r);
	cout << a << endl;
	
	A << 1,2,3,
		 4,5,6,
		 7,8,9;

	cout << A << "\n\n" << A*3/10 << "\n\n" << A/3 << "\n\n" << endl;

	A << 1, 2, 3,
		-1,-2,-4,
		 3, 2, 1;
	B << 4, 1, 2,
		 0, 0, 0,
		 1, 2, 3;

	size_t i1 = -1,j1 = -1, i2 = -1, j2 = -1;
	cout << A << "\n\n" << B << "\n\n" << endl;
	rational<long long int> m1, m2;

	m1 = A.bottomRightCorner<2,2>().maxCoeff(&i1,&j1);
	m2 = A.bottomRightCorner<2,2>().array().abs().maxCoeff(&i2,&j2);
	cout << m1 << '(' << i1+1 << ',' << j1+1 << ')' << "\n\n" << m2 << '(' << i2+1 << ',' << j2+1 << ')'  << "\n\n" << endl;

	//cout << A << "\n\n" << B << "\n\n"  << (A.array() < B.array()) << "\n\n"  << (A.array() <= B.array()) << "\n\n"  << endl;
	//cout << (A.array() < B.array()).select(B,A) << "\n\n" << endl;
	//cout << A.cwiseMax(B) << "\n\n" << endl;
	//cout << (A==B) << endl;
	//cout << (A.array()==B.array()) << endl;

	system("pause");
	return 0;
} // end function main
