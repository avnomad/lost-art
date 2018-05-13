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

#include "graphene/frames/stateful.hpp"
using namespace graphene;
using namespace Frames;
using namespace Frames::Stateful;

#include "graphene/dsel.hpp"
using namespace DSEL;

#include "graphene/frames/interface.hpp"
using Interface::Empty;

#include "graphene/function objects.hpp"
using FunctionObjects::GlutStrokeFontEngine;

#include "geometry.hpp"

#define BOOST_TEST_MODULE Stateful Frames
#include <boost/test/included/unit_test.hpp>

// Mostly force classes and constructors to be instantiated to catch compile-time errors.

BOOST_AUTO_TEST_CASE(Test_Rectangular)
{
	using geometry::Rectangle;

	// Rectangular
	Rectangular<Empty, Rectangle<int>> r1;
	r1.top() = 2;
	BOOST_CHECK_EQUAL(r1.top(), 2);
	decltype(r1) r2(1,2,3,4);
	BOOST_CHECK_EQUAL(r2.left(), 1);
	BOOST_CHECK_EQUAL(r2.bottom(), 2);
	BOOST_CHECK_EQUAL(r2.right(), 3);
	BOOST_CHECK_EQUAL(r2.top(), 4);

	decltype(r2) r2Copy = r2;
	BOOST_CHECK_EQUAL(r2Copy.left(), 1);
	BOOST_CHECK_EQUAL(r2Copy.bottom(), 2);
	BOOST_CHECK_EQUAL(r2Copy.right(), 3);
	BOOST_CHECK_EQUAL(r2Copy.top(), 4);

	decltype(r2) r2Move = std::move(r2);
	BOOST_CHECK_EQUAL(r2Move.left(), 1);
	BOOST_CHECK_EQUAL(r2Move.bottom(), 2);
	BOOST_CHECK_EQUAL(r2Move.right(), 3);
	BOOST_CHECK_EQUAL(r2Move.top(), 4);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Focusable)
{
	using geometry::Rectangle;

	FrameStack<
		Rectangle<float>,
		Frame<Interface::Focusable>,
		Frame<Focusable>
	> f1;

	decltype(f1) f2(1.0f,2.0f,3.0f,4.0f);
	BOOST_CHECK_EQUAL(f2.left(), 1.0f);
	BOOST_CHECK_EQUAL(f2.bottom(), 2.0f);
	BOOST_CHECK_EQUAL(f2.right(), 3.0f);
	BOOST_CHECK_EQUAL(f2.top(), 4.0f);

	decltype(f1) f3(true, 1.5f,2.5f,3.5f,4.5f);
	BOOST_CHECK_EQUAL(f3.left(), 1.5f);
	BOOST_CHECK_EQUAL(f3.bottom(), 2.5f);
	BOOST_CHECK_EQUAL(f3.right(), 3.5f);
	BOOST_CHECK_EQUAL(f3.top(), 4.5f);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Textual)
{
	FrameStack<
		Empty,
		Frame<Textual, std::string>
	> t1("some string");
	BOOST_CHECK_EQUAL(t1.text(), "some string");

	const decltype(t1) t2("42");
	BOOST_CHECK_EQUAL(t2.text(), "42");
} // end test case

BOOST_AUTO_TEST_CASE(Test_IndirectCaretLike)
{
	int argc = 0;
	char *argv[] = {nullptr};
	glutInit(&argc, argv); // GlutStrokeFontEngine requires GLUT to be initialized first:

	// mutable textual
	FrameStack<
		Empty,
		Frame<Textual, std::string>
	> t1("some string");

	FrameStack<
		Empty,
		Frame<Indexing, size_t>,
		Frame<Pointing, decltype(t1)*>,
		Frame<Offset, int>,
		Frame<IndirectCaretLike, FunctionObjects::Textual, GlutStrokeFontEngine, char>
	> icl1(GlutStrokeFontEngine(GLUT_STROKE_MONO_ROMAN),-2,2,&t1,1);
	// TODO: IndirectCaretLike constructor shouldn't allow arbitrary offsets, but instead set them based on index.
	BOOST_CHECK_EQUAL(icl1.fontEngine().font(), GLUT_STROKE_MONO_ROMAN);
	BOOST_CHECK_EQUAL(icl1.xOffset(), -2);
	BOOST_CHECK_EQUAL(icl1.yOffset(), 2);
	BOOST_CHECK_EQUAL(icl1.pointer(), &t1);
	BOOST_CHECK_EQUAL(icl1.index(), 1);
	BOOST_CHECK_EQUAL(icl1.pointer()->text(), "some string");

	decltype(icl1) icl1Copy = icl1;
	BOOST_CHECK_EQUAL(icl1Copy.fontEngine().font(), GLUT_STROKE_MONO_ROMAN);
	BOOST_CHECK_EQUAL(icl1Copy.xOffset(), -2);
	BOOST_CHECK_EQUAL(icl1Copy.yOffset(), 2);
	BOOST_CHECK_EQUAL(icl1Copy.pointer(), &t1);
	BOOST_CHECK_EQUAL(icl1Copy.index(), 1);
	BOOST_CHECK_EQUAL(icl1Copy.pointer()->text(), "some string");

	decltype(icl1) icl1Move = std::move(icl1);
	BOOST_CHECK_EQUAL(icl1Move.fontEngine().font(), GLUT_STROKE_MONO_ROMAN);
	BOOST_CHECK_EQUAL(icl1Move.xOffset(), -2);
	BOOST_CHECK_EQUAL(icl1Move.yOffset(), 2);
	BOOST_CHECK_EQUAL(icl1Move.pointer(), &t1);
	BOOST_CHECK_EQUAL(icl1Move.index(), 1);
	BOOST_CHECK_EQUAL(icl1Move.pointer()->text(), "some string");

	icl1.nextPosition();
	icl1.prevPosition();
	BOOST_CHECK_EQUAL(icl1.xOffset(), -2);
	BOOST_CHECK_EQUAL(icl1.yOffset(), 2);
	BOOST_CHECK_EQUAL(icl1.index(), 1);

	icl1.firstPosition();
	BOOST_CHECK_EQUAL(icl1.xOffset(), 0);
	BOOST_CHECK_EQUAL(icl1.yOffset(), 2); // unchanged
	BOOST_CHECK_EQUAL(icl1.index(), 0);
	icl1.eraseNext();
	BOOST_CHECK_EQUAL(icl1.pointer()->text(), "ome string");
	icl1.lastPosition();
	icl1.erasePrev();
	BOOST_CHECK_EQUAL(icl1.pointer()->text(), "ome strin");

	icl1.insert('-');
	BOOST_CHECK_EQUAL(icl1.pointer()->text(), "ome strin-");

	// constant textual
	const decltype(t1) t2("42");

	FrameStack<
		Empty,
		Frame<Indexing, size_t>,
		Frame<Pointing, decltype(t2)*>,
		Frame<Offset, float>,
		Frame<IndirectCaretLike, FunctionObjects::Textual, GlutStrokeFontEngine, char>
	> icl2(1.0f,0.0f,&t2,1);
	BOOST_CHECK_EQUAL(icl2.fontEngine().font(), GLUT_STROKE_ROMAN);
	BOOST_CHECK_EQUAL(icl2.xOffset(), 1.0f);
	BOOST_CHECK_EQUAL(icl2.yOffset(), 0.0f);
	BOOST_CHECK_EQUAL(icl2.pointer(), &t2);
	BOOST_CHECK_EQUAL(icl2.index(), 1);
	BOOST_CHECK_EQUAL(icl2.pointer()->text(), "42");

	icl2.firstPosition();
	icl2.nextPosition();
	icl2.prevPosition();
	BOOST_CHECK_EQUAL(icl2.xOffset(), 0.0f);
	BOOST_CHECK_EQUAL(icl2.yOffset(), 0.0f);
	BOOST_CHECK_EQUAL(icl2.index(), 0);
} // end test case
