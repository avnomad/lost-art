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

#ifndef STD_OVERLOADS_H
#define STD_OVERLOADS_H

#include <algorithm>
#include <type_traits>

namespace utility
{
/**
 * Returns the smallest of a and b. If both are equivalent, a is returned.
 *
 * Wraps std::min converting all arguments to a common type,
 * so that there is no ambiguity as to which instantiation to call.
 */
template<typename T, typename S>
inline constexpr const auto &min(const T &a, const S &b)
{
	return std::min<typename std::common_type<T, S>::type>(a, b);
}

/**
 * Returns the largest of a and b. If both are equivalent, a is returned.
 *
 * Wraps std::max converting all arguments to a common type,
 * so that there is no ambiguity as to which instantiation to call.
 */
template<typename T, typename S>
inline constexpr const auto &max(const T &a, const S &b)
{
	return std::max<typename std::common_type<T, S>::type>(a, b);
}

} // end namespace utility

#endif // STD_OVERLOADS_H
