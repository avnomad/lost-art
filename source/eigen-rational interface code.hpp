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

#ifndef EIGEN_RATIONAL_INTERFACE_CODE_H
#define EIGEN_RATIONAL_INTERFACE_CODE_H

#include <boost/rational.hpp>
#include <Eigen/Dense>

namespace Eigen
{
	template<typename I>
	struct NumTraits<boost::rational<I>>
	{
		using Real       = boost::rational<I>;
		using NonInteger = boost::rational<I>; // perhaps double would be better?
		using Nested     = boost::rational<I>;

		enum{
			IsComplex = 0,
			IsInteger = 0,
			ReadCost = 2,
			AddCost = 10,
			MulCost = 10,
			IsSigned = std::numeric_limits<I>::is_signed,
			RequireInitialization = 1,
		}; // end enum

		static constexpr Real epsilon()
		{
			return boost::rational<I>(std::numeric_limits<I>::max(),std::numeric_limits<I>::max()-1);
		} // end function epsilon

		static constexpr Real dummy_precision()
		{
			return epsilon()*10000;
		} // end function dummy_precision

		static constexpr Real highest()
		{
			return boost::rational<I>(std::numeric_limits<I>::max(),1);
		} // end function highest

		static constexpr Real lowest()
		{
			return boost::rational<I>(std::numeric_limits<I>::min(),1);
		} // end function lowest

		static constexpr int digits10()
		{
			return std::numeric_limits<boost::rational<I>>::digits10;
		} // end function digits10
	}; // end struct NumTraits

	// Eigen didn't document it but I still needed to specialize this class...
	namespace internal
	{
		template<typename I, typename NewType>
		struct cast_impl<boost::rational<I>,NewType>
		{
			static inline constexpr NewType run(const boost::rational<I>& x)
			{
				return boost::rational_cast<NewType>(x);
			} // end function run
		}; // end struct cast_impl
	} // end namespace internal
} // end namespace Eigen


namespace boost
{
	template<typename I>
	constexpr double log(boost::rational<I> r)
	{
		return std::log(boost::rational_cast<double>(r));
	} // end function log
} // end namespace boost

#endif /* EIGEN_RATIONAL_INTERFACE_CODE_H */
