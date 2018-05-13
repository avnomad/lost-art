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

#include "graphene/frames/behavioural.hpp"
using namespace graphene;
using namespace Frames;
using namespace Frames::Behavioural;

#include "graphene/frames/stateful.hpp"
using namespace Frames::Stateful;

#include "graphene/dsel.hpp"
using namespace DSEL;

#include "graphene/frames/interface.hpp"
using Interface::Empty;

#include "graphene/function objects.hpp"
using FunctionObjects::GlutStrokeFontEngine;

#include "geometry.hpp"

#define BOOST_TEST_MODULE Behavioural Frames
#include <boost/test/included/unit_test.hpp>

// TODO: test more behavioural frames in isolation.

BOOST_AUTO_TEST_CASE(Test_Movable)
{
	using geometry::Rectangle;

	FrameStack<
		Empty,
		Frame<Rectangular, Rectangle<float>>,
		Frame<Movable>
	> mr1;
	mr1.top() = 2;
	BOOST_CHECK_EQUAL(mr1.top(), 2);

	decltype(mr1) mr2(1,2,3,4);
	BOOST_CHECK_EQUAL(mr2.left(), 1);
	BOOST_CHECK_EQUAL(mr2.bottom(), 2);
	BOOST_CHECK_EQUAL(mr2.right(), 3);
	BOOST_CHECK_EQUAL(mr2.top(), 4);
} // end test case

BOOST_AUTO_TEST_CASE(Test_HVMovable)
{
	using geometry::Rectangle;

	FrameStack<
		Empty,
		Frame<Rectangular, Rectangle<float>>,
		Frame<Movable>,
		Frame<HVMovable>
	> hvm1;
	hvm1.top() = 2;
	BOOST_CHECK_EQUAL(hvm1.top(), 2);

	decltype(hvm1) hvm2(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm2.left(), 1);
	BOOST_CHECK_EQUAL(hvm2.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm2.right(), 3);
	BOOST_CHECK_EQUAL(hvm2.top(), 4);

	FrameStack<
		Rectangle<float>,
		Frame<Movable>,
		Frame<HVMovable>
	> hvm3(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm3.left(), 1);
	BOOST_CHECK_EQUAL(hvm3.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm3.right(), 3);
	BOOST_CHECK_EQUAL(hvm3.top(), 4);
} // end test case

BOOST_AUTO_TEST_CASE(Test_HVMovable_with_Textual)
{
	using geometry::Rectangle;

	FrameStack<
		Empty,
		Frame<Rectangular, Rectangle<float>>,
		Frame<Movable>,
		Frame<HVMovable>,
		Frame<Textual, std::string>
	> hvm4("ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm4.text(), "ok");
	BOOST_CHECK_EQUAL(hvm4.left(), 1);
	BOOST_CHECK_EQUAL(hvm4.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm4.right(), 3);
	BOOST_CHECK_EQUAL(hvm4.top(), 4);

	decltype(hvm4) hvm5(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm5.left(), 1);
	BOOST_CHECK_EQUAL(hvm5.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm5.right(), 3);
	BOOST_CHECK_EQUAL(hvm5.top(), 4);
	BOOST_CHECK_EQUAL(hvm5.text(), "");

	decltype(hvm4) hvm6("ok");
	BOOST_CHECK_EQUAL(hvm6.text(), "ok");

	FrameStack<
		Rectangle<float>,
		Frame<Movable>,
		Frame<HVMovable>,
		Frame<Textual, std::string>
	> hvm7("ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm7.left(), 1);
	BOOST_CHECK_EQUAL(hvm7.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm7.right(), 3);
	BOOST_CHECK_EQUAL(hvm7.top(), 4);
	BOOST_CHECK_EQUAL(hvm7.text(), "ok");

	decltype(hvm7) hvm8(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm8.left(), 1);
	BOOST_CHECK_EQUAL(hvm8.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm8.right(), 3);
	BOOST_CHECK_EQUAL(hvm8.top(), 4);
	BOOST_CHECK_EQUAL(hvm8.text(), "");

	decltype(hvm7) hvm9("ok");
	BOOST_CHECK_EQUAL(hvm9.text(), "ok");
} // end test case

BOOST_AUTO_TEST_CASE(Test_HVMovable_with_Textual_and_SizedText)
{
	using geometry::Rectangle;

	FrameStack<
		Empty,
		Frame<Rectangular, Rectangle<float>>,
		Frame<Movable>,
		Frame<HVMovable>,
		Frame<Textual, std::string>,
		Frame<SizedText, GlutStrokeFontEngine>
	> hvm10(12,"ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm10.textHeight(), 12);
	BOOST_CHECK_EQUAL(hvm10.text(), "ok");
	BOOST_CHECK_EQUAL(hvm10.left(), 1);
	BOOST_CHECK_EQUAL(hvm10.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm10.right(), 3);
	BOOST_CHECK_EQUAL(hvm10.top(), 4);

//	decltype(hvm10) hvm11(1,2,3,4); // currently compile-time error
	decltype(hvm10) hvm12("ok");
	BOOST_CHECK_EQUAL(hvm12.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm12.text(), "ok");

	decltype(hvm10) hvm13(12);
	BOOST_CHECK_EQUAL(hvm13.textHeight(), 12);
	BOOST_CHECK_EQUAL(hvm13.text(), "");

	decltype(hvm10) hvm14(12,"ok");
	BOOST_CHECK_EQUAL(hvm14.textHeight(), 12);
	BOOST_CHECK_EQUAL(hvm14.text(), "ok");

	decltype(hvm10) hvm15(12,1,2,3,4);
	BOOST_CHECK_EQUAL(hvm15.textHeight(), 12);
	BOOST_CHECK_EQUAL(hvm15.text(), "");
	BOOST_CHECK_EQUAL(hvm15.left(), 1);
	BOOST_CHECK_EQUAL(hvm15.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm15.right(), 3);
	BOOST_CHECK_EQUAL(hvm15.top(), 4);

	decltype(hvm10) hvm16("ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm16.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm16.text(), "ok");
	BOOST_CHECK_EQUAL(hvm16.left(), 1);
	BOOST_CHECK_EQUAL(hvm16.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm16.right(), 3);
	BOOST_CHECK_EQUAL(hvm16.top(), 4);

	decltype(hvm10) hvm17;
	BOOST_CHECK_EQUAL(hvm17.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm17.text(), "");

	FrameStack<
		Rectangle<float>,
		Frame<Movable>,
		Frame<HVMovable>,
		Frame<Textual, std::string>,
		Frame<SizedText, GlutStrokeFontEngine>
	> hvm18(12,"ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm18.left(), 1);
	BOOST_CHECK_EQUAL(hvm18.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm18.right(), 3);
	BOOST_CHECK_EQUAL(hvm18.top(), 4);
	BOOST_CHECK_EQUAL(hvm18.text(), "ok");
	BOOST_CHECK_EQUAL(hvm18.textHeight(), 12);

//	decltype(hvm18) hvm19(1,2,3,4); // currently compile-time error
	decltype(hvm18) hvm20("ok");
	BOOST_CHECK_EQUAL(hvm20.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm20.text(), "ok");

	decltype(hvm18) hvm21(12);
	BOOST_CHECK_EQUAL(hvm21.text(), "");
	BOOST_CHECK_EQUAL(hvm21.textHeight(), 12);

	decltype(hvm18) hvm22(12,"ok");
	BOOST_CHECK_EQUAL(hvm22.text(), "ok");
	BOOST_CHECK_EQUAL(hvm22.textHeight(), 12);

	decltype(hvm18) hvm23(12,1,2,3,4);
	BOOST_CHECK_EQUAL(hvm23.left(), 1);
	BOOST_CHECK_EQUAL(hvm23.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm23.right(), 3);
	BOOST_CHECK_EQUAL(hvm23.top(), 4);
	BOOST_CHECK_EQUAL(hvm23.text(), "");
	BOOST_CHECK_EQUAL(hvm23.textHeight(), 12);

	decltype(hvm18) hvm24("ok",1,2,3,4);
	BOOST_CHECK_EQUAL(hvm24.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm24.text(), "ok");
	BOOST_CHECK_EQUAL(hvm24.left(), 1);
	BOOST_CHECK_EQUAL(hvm24.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm24.right(), 3);
	BOOST_CHECK_EQUAL(hvm24.top(), 4);

	decltype(hvm18) hvm25;
	BOOST_CHECK_EQUAL(hvm25.textHeight(), 0);
	BOOST_CHECK_EQUAL(hvm25.text(), "");
} // end test case
