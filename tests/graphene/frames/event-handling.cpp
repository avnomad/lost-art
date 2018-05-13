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

#include "graphene/frames/event-handling.hpp"
using namespace graphene;
using namespace Frames;
using namespace Frames::EventHandling;

#include "graphene/frames/stateful.hpp"
using namespace Frames::Stateful;

#include "graphene/dsel.hpp"
using namespace DSEL;

#include "graphene/frames/interface.hpp"
using Interface::Empty;

#include "graphene/function objects.hpp"
using FunctionObjects::GlutStrokeFontEngine;

#include "geometry.hpp"

#define BOOST_TEST_MODULE Event-handling Frames
#include <boost/test/included/unit_test.hpp>

// TODO: test more event-handling frames in isolation.

BOOST_AUTO_TEST_CASE(Test_KeyboardAndMouseStub)
{
	using geometry::Rectangle;

	FrameStack<
		Empty,
		Frame<Interface::KeyboardAndMouse, float>,
		Frame<KeyboardAndMouseStub>
	> knm1;

	FrameStack<
		Empty,
		Frame<Interface::KeyboardAndMouse, int>,
		Frame<Rectangular, Rectangle<int>>,
		Frame<KeyboardAndMouseStub>
	> knm2(-1, -2, -3, -4);
	BOOST_CHECK_EQUAL(knm2.left(), -1);
	BOOST_CHECK_EQUAL(knm2.bottom(), -2);
	BOOST_CHECK_EQUAL(knm2.right(), -3);
	BOOST_CHECK_EQUAL(knm2.top(), -4);

	FrameStack<
		Rectangle<int>,
		Frame<Interface::KeyboardAndMouse>,
		Frame<KeyboardAndMouseStub>
	> knm3;

	decltype(knm3) knm4(-1, -2, -3, -4);
	BOOST_CHECK_EQUAL(knm4.left(), -1);
	BOOST_CHECK_EQUAL(knm4.bottom(), -2);
	BOOST_CHECK_EQUAL(knm4.right(), -3);
	BOOST_CHECK_EQUAL(knm4.top(), -4);
} // end test case

BOOST_AUTO_TEST_CASE(Test_CaretLike)
{
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
		Frame<IndirectCaretLike, FunctionObjects::Textual, GlutStrokeFontEngine, char>,
		Frame<CaretLike>
	> ehcl1(-2,2,&t1,1);
	BOOST_CHECK_EQUAL(ehcl1.fontEngine().font(), GLUT_STROKE_ROMAN);
	BOOST_CHECK_EQUAL(ehcl1.xOffset(), -2);
	BOOST_CHECK_EQUAL(ehcl1.yOffset(), 2);
	BOOST_CHECK_EQUAL(ehcl1.pointer(), &t1);
	BOOST_CHECK_EQUAL(ehcl1.index(), 1);
	BOOST_CHECK_EQUAL(ehcl1.pointer()->text(), "some string");

	ehcl1.keyboardNonAscii(Interface::NonAsciiKey::HOME, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl1.xOffset(), 0);
	BOOST_CHECK_EQUAL(ehcl1.yOffset(), 2); // unchanged
	BOOST_CHECK_EQUAL(ehcl1.index(), 0);

	ehcl1.keyboardAscii(0x7f, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl1.pointer()->text(), "ome string");

	// constant textual
	const decltype(t1) t2("42");

	FrameStack<
		Empty,
		Frame<Indexing, size_t>,
		Frame<Pointing, decltype(t2)*>,
		Frame<Offset, float>,
		Frame<IndirectCaretLike, FunctionObjects::Textual, GlutStrokeFontEngine, char>,
		Frame<CaretLike>
	> ehcl2(1.0f,0.0f,&t2,1);
	BOOST_CHECK_EQUAL(ehcl2.fontEngine().font(), GLUT_STROKE_ROMAN);
	BOOST_CHECK_EQUAL(ehcl2.xOffset(), 1.0f);
	BOOST_CHECK_EQUAL(ehcl2.yOffset(), 0.0f);
	BOOST_CHECK_EQUAL(ehcl2.pointer(), &t2);
	BOOST_CHECK_EQUAL(ehcl2.index(), 1);
	BOOST_CHECK_EQUAL(ehcl2.pointer()->text(), "42");

	ehcl2.keyboardNonAscii(Interface::NonAsciiKey::HOME, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl2.xOffset(), 0.0f);
	BOOST_CHECK_EQUAL(ehcl2.yOffset(), 0.0f); // unchanged
	BOOST_CHECK_EQUAL(ehcl2.index(), 0);

	ehcl2.keyboardAscii(0x7f, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl2.pointer()->text(), "42"); // unchanged
} // end test case
