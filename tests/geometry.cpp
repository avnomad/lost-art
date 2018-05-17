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
using geometry::RefRectangle;
using geometry::RectangleSide;

#define BOOST_TEST_MODULE Geometry
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_RectangleSize)
{
	// test RectangleSide
	BOOST_CHECK(!isHorizontal(RectangleSide::LEFT));
	BOOST_CHECK(isHorizontal(RectangleSide::BOTTOM));
	BOOST_CHECK(!isHorizontal(RectangleSide::RIGHT));
	BOOST_CHECK(isHorizontal(RectangleSide::TOP));
	BOOST_CHECK(isVertical(RectangleSide::LEFT));
	BOOST_CHECK(!isVertical(RectangleSide::BOTTOM));
	BOOST_CHECK(isVertical(RectangleSide::RIGHT));
	BOOST_CHECK(!isVertical(RectangleSide::TOP));
	BOOST_CHECK_EQUAL(opposite(RectangleSide::LEFT), RectangleSide::RIGHT);
	BOOST_CHECK_EQUAL(opposite(RectangleSide::RIGHT), RectangleSide::LEFT);
	BOOST_CHECK_EQUAL(opposite(RectangleSide::TOP), RectangleSide::BOTTOM);
	BOOST_CHECK_EQUAL(opposite(RectangleSide::BOTTOM), RectangleSide::TOP);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Rectangle)
{
	geometry::Rectangle<int> recti(-2,5,3,8);
	BOOST_CHECK_EQUAL(recti.left(), -2);
	BOOST_CHECK_EQUAL(recti.right(), 3);
	BOOST_CHECK_EQUAL(recti.bottom(), 5);
	BOOST_CHECK_EQUAL(recti.top(), 8);
	BOOST_CHECK_EQUAL(recti.width(), 5);
	BOOST_CHECK_EQUAL(recti.height(), 3);
	recti.sides()[size_t(RectangleSide::RIGHT)] = 0;
	BOOST_CHECK_EQUAL(recti.right(), 0);
	BOOST_CHECK_EQUAL(recti.width(), 2);
	BOOST_CHECK_EQUAL(recti.height(), 3);
	recti.side(geometry::Rectangle<int>::Side::BOTTOM) = 4;
	BOOST_CHECK_EQUAL(recti.bottom(), 4);
	BOOST_CHECK_EQUAL(recti.width(), 2);
	BOOST_CHECK_EQUAL(recti.height(), 4);

	geometry::Rectangle<float> rectf(-2.0,5.0,3.0,8.0);
	BOOST_CHECK_EQUAL(rectf.sides()[size_t(geometry::Rectangle<float>::Side::TOP)], 8.0);
} // end test case

BOOST_AUTO_TEST_CASE(Test_RefRectangle)
{
	int x = -1, y = 7;
	RefRectangle<int,false,true,false,false,true> refrecti(x,5,3,y);
	BOOST_CHECK_EQUAL(refrecti.left(), -1);
	BOOST_CHECK_EQUAL(refrecti.right(), 3);
	BOOST_CHECK_EQUAL(refrecti.bottom(), 5);
	BOOST_CHECK_EQUAL(refrecti.top(), 7);
	BOOST_CHECK_EQUAL(refrecti.width(), 4);
	BOOST_CHECK_EQUAL(refrecti.height(), 2);
	refrecti.side(RectangleSide::RIGHT) = 0;
	BOOST_CHECK_EQUAL(refrecti.right(), 0);
	BOOST_CHECK_EQUAL(x, -1);
	BOOST_CHECK_EQUAL(y, 7);
	BOOST_CHECK_EQUAL(refrecti.width(), 1);
	BOOST_CHECK_EQUAL(refrecti.height(), 2);
	refrecti.side(RefRectangle<int,false,true,false,false,true>::Side::BOTTOM) = 4;
	BOOST_CHECK_EQUAL(refrecti.bottom(), 4);
	BOOST_CHECK_EQUAL(x, -1);
	BOOST_CHECK_EQUAL(y, 7);
	BOOST_CHECK_EQUAL(refrecti.width(), 1);
	BOOST_CHECK_EQUAL(refrecti.height(), 3);
	refrecti.left() = 0;
	BOOST_CHECK_EQUAL(refrecti.left(), 0);
	BOOST_CHECK_EQUAL(x, 0);
	x = -3;
	BOOST_CHECK_EQUAL(refrecti.left(), -3);
	refrecti.top() = 9;
	BOOST_CHECK_EQUAL(refrecti.top(), 9);
	BOOST_CHECK_EQUAL(y, 9);
	y = 10;
	BOOST_CHECK_EQUAL(refrecti.top(), 10);

	RefRectangle<float> refrectf(-2.0,5.0,3.0,8.0);
	BOOST_CHECK_EQUAL(refrectf.side(geometry::Rectangle<float>::Side::TOP), 8.0);
} // end test case
