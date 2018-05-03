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

#include "graphene.hpp"

#define BOOST_TEST_MODULE Graphene
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Button)
{
	graphene::Controls::Button<geometry::Rectangle<int>,std::ratio<10>,std::ratio<11>> button(1,2,5,7,1,"button",10);

	BOOST_CHECK_EQUAL(button.left(), 1);
	BOOST_CHECK_EQUAL(button.bottom(), 2);
	BOOST_CHECK_EQUAL(button.right(), 5);
	BOOST_CHECK_EQUAL(button.top(), 7);
	BOOST_CHECK_EQUAL(button.borderSize(), 1);
	BOOST_CHECK_EQUAL(button.text(), "button");
	BOOST_CHECK_EQUAL(button.textHeight(), 10);
	BOOST_CHECK_EQUAL(button.pressed(), false);
	BOOST_CHECK_EQUAL(button.highlighted(), false);
	button.depress().press().depress().press().borderSize() = 2;
	BOOST_CHECK_EQUAL(button.pressed(), true);
	BOOST_CHECK_EQUAL(button.borderSize(), 2);
	button.dehighlight().highlight().dehighlight().highlight().borderSize() = 3;
	BOOST_CHECK_EQUAL(button.highlighted(), true);
	BOOST_CHECK_EQUAL(button.borderSize(), 3);
	button.text() = "a";
	BOOST_CHECK_EQUAL(button.text(), "a");
	button.textHeight() = 12;
	BOOST_CHECK_EQUAL(button.textHeight(), 12);
	// TODO: check textWidth. What is the correct result?
} // end test case

BOOST_AUTO_TEST_CASE(Test_Constructors)
{
	using geometry::Rectangle;
	using namespace graphene;
	using namespace graphene::Frames;
	using namespace graphene::DSEL;

	// Mostly force classes and constructors to be instantiated to catch compile-time errors.

	// Adapting::Rectangular
	Adapting::Rectangular<Bases::Empty, Rectangle<int>> r1;
	r1.top() = 2;
	BOOST_CHECK_EQUAL(r1.top(), 2);
	Adapting::Rectangular<Bases::Empty, Rectangle<int>> r2(1,2,3,4);
	BOOST_CHECK_EQUAL(r2.left(), 1);
	BOOST_CHECK_EQUAL(r2.bottom(), 2);
	BOOST_CHECK_EQUAL(r2.right(), 3);
	BOOST_CHECK_EQUAL(r2.top(), 4);

	// KeyboardAndMouseStub
	FrameStack<
		Bases::Empty,
		Frame<Bases::EventHandling::KeyboardAndMouse, float>,
		Frame<EventHandling::KeyboardAndMouseStub>
	> knm1;
	FrameStack<
		Bases::Empty,
		Frame<Bases::EventHandling::KeyboardAndMouse, int>,
		Frame<Adapting::Rectangular, Rectangle<int>>,
		Frame<EventHandling::KeyboardAndMouseStub>
	> knm2(-1, -2, -3, -4);
	BOOST_CHECK_EQUAL(knm2.left(), -1);
	BOOST_CHECK_EQUAL(knm2.bottom(), -2);
	BOOST_CHECK_EQUAL(knm2.right(), -3);
	BOOST_CHECK_EQUAL(knm2.top(), -4);
	FrameStack<
		Rectangle<int>,
		Frame<Bases::EventHandling::KeyboardAndMouse>,
		Frame<EventHandling::KeyboardAndMouseStub>
	> knm3(-1, -2, -3, -4);
	BOOST_CHECK_EQUAL(knm3.left(), -1);
	BOOST_CHECK_EQUAL(knm3.bottom(), -2);
	BOOST_CHECK_EQUAL(knm3.right(), -3);
	BOOST_CHECK_EQUAL(knm3.top(), -4);
	FrameStack<
		Rectangle<int>,
		Frame<Bases::EventHandling::KeyboardAndMouse>,
		Frame<EventHandling::KeyboardAndMouseStub>
	> knm4(4,3,2,1);

	// Movable::Rectangular
	FrameStack<
		Bases::Empty,
		Frame<Adapting::Rectangular, Rectangle<float>>,
		Frame<Movable::Rectangular>
	> mr1;
	mr1.top() = 2;
	BOOST_CHECK_EQUAL(mr1.top(), 2);
	FrameStack<
		Bases::Empty,
		Frame<Adapting::Rectangular, Rectangle<float>>,
		Frame<Movable::Rectangular>
	> mr2(1,2,3,4);
	BOOST_CHECK_EQUAL(mr2.left(), 1);
	BOOST_CHECK_EQUAL(mr2.bottom(), 2);
	BOOST_CHECK_EQUAL(mr2.right(), 3);
	BOOST_CHECK_EQUAL(mr2.top(), 4);

	// HVMovable
	FrameStack<
		Bases::Empty,
		Frame<Adapting::Rectangular, Rectangle<float>>,
		Frame<Movable::Rectangular>,
		Frame<Movable::HVMovable>
	> hvm1;
	hvm1.top() = 2;
	BOOST_CHECK_EQUAL(hvm1.top(), 2);
	FrameStack<
		Bases::Empty,
		Frame<Adapting::Rectangular, Rectangle<float>>,
		Frame<Movable::Rectangular>,
		Frame<Movable::HVMovable>
	> hvm2(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm2.left(), 1);
	BOOST_CHECK_EQUAL(hvm2.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm2.right(), 3);
	BOOST_CHECK_EQUAL(hvm2.top(), 4);

	// Focusable
	Focusable<CRFrameStack<Bases::Focusable, Rectangle<float>>> f1;
	Focusable<CRFrameStack<Bases::Focusable, Rectangle<float>>> f2(1.0f,2.0f,3.0f,4.0f);
	BOOST_CHECK_EQUAL(f2.left(), 1.0f);
	BOOST_CHECK_EQUAL(f2.bottom(), 2.0f);
	BOOST_CHECK_EQUAL(f2.right(), 3.0f);
	BOOST_CHECK_EQUAL(f2.top(), 4.0f);
	Focusable<CRFrameStack<Bases::Focusable, Rectangle<float>>> f3(true, 1.5f,2.5f,3.5f,4.5f);
	BOOST_CHECK_EQUAL(f3.left(), 1.5f);
	BOOST_CHECK_EQUAL(f3.bottom(), 2.5f);
	BOOST_CHECK_EQUAL(f3.right(), 3.5f);
	BOOST_CHECK_EQUAL(f3.top(), 4.5f);
} // end test case
