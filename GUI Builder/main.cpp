#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <numeric>
using std::numeric_limits;

#include <cmath>
using std::log;

#include <boost/rational.hpp>
using boost::rational;

#include <Eigen/Dense>
using Eigen::Matrix;
using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::Vector3d;


namespace Eigen
{
	template<typename I>
	struct NumTraits<boost::rational<I>>
	{
		typedef boost::rational<I> Real;
		typedef boost::rational<I> NonInteger; // perhaps double would be better?
		typedef boost::rational<I> Nested;

		enum{
			IsComplex = 0,
			IsInteger = 0,
			ReadCost = 2,
			AddCost = 10,
			MulCost = 10,
			IsSigned = std::numeric_limits<I>::is_signed,
			RequireInitialization = 1,
		}; // end enum

		static Real epsilon()
		{
			return boost::rational<I>(std::numeric_limits<I>::max(),std::numeric_limits<I>::max()-1);
		} // end function epsilon

		static Real dummy_precision()
		{
			return epsilon()*10000;
		} // end function dummy_precision

		static Real highest()
		{
			return boost::rational<I>(std::numeric_limits<I>::max(),1);
		} // end function highest

		static Real lowest()
		{
			return boost::rational<I>(std::numeric_limits<I>::min(),1);
		} // end function lowest
	}; // end struct NumTraits

	// Eigen didn't document it but I still needed to specialize this class...
	namespace internal
	{
		template<typename I, typename NewType>
		struct cast_impl<boost::rational<I>,NewType>
		{
			static inline NewType run(const boost::rational<I>& x)
			{
				return boost::rational_cast<NewType>(x);
			}
		};
	} // end namespace internal
} // end namespace Eigen


namespace boost
{
	template<typename I>
	double log(boost::rational<I> r)
	{
		return std::log(boost::rational_cast<double>(r));
	} // end function log
} // end namespace boost



int main()
{
	Matrix<rational<long long int>,3,3> A;
	Matrix<rational<long long int>,3,3> B;
	rational<long long int> r(5,3);

	long long int a = boost::rational_cast<long long int>(r);
	cout << a << endl;
	
	A << 1,2,3,
		 4,5,6,
		 7,8,9;

	cout << A << "\n\n" << A*3/10 << "\n\n" << A/3 << "\n\n" << endl;

	//A << 1, 2, 3,
	//	-1,-2,-3,
	//	 3, 2, 1;
	//B << 4, 1, 2,
	//	 0, 0, 0,
	//	 1, 2, 3;

	//cout << A << "\n\n" << B << "\n\n"  << (A.array() < B.array()) << "\n\n"  << (A.array() <= B.array()) << "\n\n"  << endl;
	//cout << (A.array() < B.array()).select(B,A) << "\n\n" << endl;
	//cout << A.cwiseMax(B) << "\n\n" << endl;
	//cout << (A==B) << endl;
	//cout << (A.array()==B.array()) << endl;

	system("pause");
	return 0;
} // end function main
