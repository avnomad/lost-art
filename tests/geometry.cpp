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

#include "geometry.hpp"

#include <cassert>

namespace geometry
{
	void runTestSuite()
	{
		// test RectangleSide
		assert(!isHorizontal(RectangleSide::LEFT));
		assert(isHorizontal(RectangleSide::BOTTOM));
		assert(!isHorizontal(RectangleSide::RIGHT));
		assert(isHorizontal(RectangleSide::TOP));
		assert(isVertical(RectangleSide::LEFT));
		assert(!isVertical(RectangleSide::BOTTOM));
		assert(isVertical(RectangleSide::RIGHT));
		assert(!isVertical(RectangleSide::TOP));

		// test Rectangle
		Rectangle<int> recti(-2,5,3,8);
		assert(recti.left() == -2);
		assert(recti.right() == 3);
		assert(recti.bottom() == 5);
		assert(recti.top() == 8);
		assert(recti.width() == 5);
		assert(recti.height() == 3);
		recti.sides()[size_t(RectangleSide::RIGHT)] = 0;
		assert(recti.right() == 0);
		assert(recti.width() == 2);
		assert(recti.height() == 3);
		recti.side(geometry::Rectangle<int>::Side::BOTTOM) = 4;
		assert(recti.bottom() == 4);
		assert(recti.width() == 2);
		assert(recti.height() == 4);

		Rectangle<float> rectf(-2.0,5.0,3.0,8.0);
		assert(rectf.sides()[size_t(Rectangle<float>::Side::TOP)] == 8.0);

		// test RefRectangle
		int x = -1, y = 7;
		RefRectangle<int,false,true,false,false,true> refrecti(x,5,3,y);
		assert(refrecti.left() == -1);
		assert(refrecti.right() == 3);
		assert(refrecti.bottom() == 5);
		assert(refrecti.top() == 7);
		assert(refrecti.width() == 4);
		assert(refrecti.height() == 2);
		refrecti.side(RectangleSide::RIGHT) = 0;
		assert(refrecti.right() == 0);
		assert(x == -1);
		assert(y == 7);
		assert(refrecti.width() == 1);
		assert(refrecti.height() == 2);
		refrecti.side(geometry::RefRectangle<int,false,true,false,false,true>::Side::BOTTOM) = 4;
		assert(refrecti.bottom() == 4);
		assert(x == -1);
		assert(y == 7);
		assert(refrecti.width() == 1);
		assert(refrecti.height() == 3);
		refrecti.left() = 0;
		assert(refrecti.left() == 0);
		assert(x == 0);
		x = -3;
		assert(refrecti.left() == -3);
		refrecti.top() = 9;
		assert(refrecti.top() == 9);
		assert(y == 9);
		y = 10;
		assert(refrecti.top() == 10);

		RefRectangle<float> refrectf(-2.0,5.0,3.0,8.0);
		assert(refrectf.side(Rectangle<float>::Side::TOP) == 8.0);

	} // end function runTestSuite

} // end namespace geometry

int main()
{
	geometry::runTestSuite();

	return 0;
}
