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
using namespace graphene;
using namespace Frames;
using namespace DSEL;
using namespace Renderable;

#define BOOST_TEST_MODULE Graphene
#include <boost/test/included/unit_test.hpp>

// Declare frame stacks to verify they compile

template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string>
using Button = FrameStack<
	RectangleType,
	Frame<UniformlyBordered, typename RectangleType::coordinate_type>,
	Frame<Pressable>,
	Frame<Highlightable>,
	Frame<Textual, TextType>,
	Frame<SizedText, FunctionObjects::GlutStrokeFontEngine>,
	Frame<BoxedAdaptableSizeText, Margin>,
	Frame<EventHandling::TwoStagePressable>,
	Condition<FunctionObjects::Pressed,
		Sequence<
			Frame<Colorblind::FilledRectangle>,
			FrameStack<
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>,
				Frame<Colorblind::InversedColor>>>,
		Condition<FunctionObjects::Highlighted,
			Sequence<
				Frame<Colorblind::BorderedRectangle, BorderSize>,
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>>,
			Sequence<
				Frame<Colorblind::BorderedRectangle, std::ratio<0>>,
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>>
		>
	>
>;

template<typename CoordinateType>
using IShapePart = FrameStack<
	Bases::Empty,
	Frame<Bases::Destructible>,
	Frame<Bases::Movable, CoordinateType>,
	Frame<Bases::Containing>,
	Frame<Bases::Renderable>
>;

/** Const instances should be constant and non-const instances should be non constant
 */
template<typename CoordinateType, typename horizontallyMovable, typename verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
using ControlPart = FrameStack<
	IShapePart<CoordinateType>,
	Frame<Bases::Rectangular>,
	Frame<Movable::Rectangular>,
	Frame<Movable::HVMovable, horizontallyMovable, verticallyMovable>,
	Frame<Colorblind::FilledRectangle>,
	Frame<Stippled>,
	Frame<Colorblind::InversedColor>,
	Frame<Adapting::Rectangular, geometry::RefRectangle<CoordinateType, constant, leftRef, bottomRef, rightRef, topRef>>
>;

template<typename CoordinateType, typename TextType = std::string>
using IControl = FrameStack<
	Bases::Empty,
	Frame<Bases::Destructible>,
	Frame<Bases::Rectangular, CoordinateType>,
	Frame<Bases::Containing>,
	Frame<Bases::Movable>,
	Frame<Bases::UniformlyBordered, CoordinateType>,
	Frame<Bases::Textual, TextType>,
	Frame<Bases::SizedText>,
	Frame<Bases::Selectable>,
	Frame<Bases::Highlightable>,
	Frame<Bases::MultiPart, std::unique_ptr<      IShapePart<CoordinateType>>,
							std::unique_ptr<const IShapePart<CoordinateType>>>,
	Frame<Bases::Renderable>
>;

template<typename RectangleType, typename Margin, typename TextType = std::string>
using ControlBase =	FrameStack<
	IControl<typename RectangleType::coordinate_type, TextType>,
	Frame<Adapting::Rectangular, RectangleType>,
	Frame<UniformlyBordered>,
	Frame<Selectable, IControl<typename RectangleType::coordinate_type, TextType>>,
	Frame<Highlightable>,
	Frame<Movable::Rectangular>,
	Frame<Textual, TextType>,
	Frame<SizedText, FunctionObjects::GlutStrokeFontEngine>,
	Frame<BoxedAdaptableSizeText, Margin>,
	Frame<MultiPartBorderedRectangle, ConcreteReturnTypesBuilder<ControlPart, typename RectangleType::coordinate_type>>
>;

template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string>
using Control = FrameStack<
	ControlBase<RectangleType, Margin, TextType>,
	Condition<
		FunctionObjects::Selected,
		Sequence<
			Frame<Colorblind::FilledRectangle>,
			FrameStack<
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>,
				Frame<Colorblind::InversedColor>>>,
		Condition<
			FunctionObjects::Highlighted,
			Sequence<
				Frame<Colorblind::BorderedRectangle, BorderSize>,
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>>,
			Sequence<
				Frame<Colorblind::BorderedRectangle, std::ratio<0>>,
				Frame<Colorblind::BoxedText, FunctionObjects::Textual>>
		>
	>
>;

template<typename RectangleType, typename BorderSize, typename Margin, typename LineSpacing, typename TextType = std::string>
using Paragraph = FrameStack<
	ControlBase<RectangleType, Margin, TextType>,
	Condition<
		FunctionObjects::Selected,
		Sequence<
			Frame<Colorblind::FilledRectangle>,
			FrameStack<
				Frame<Colorblind::BoxedParagraph, FunctionObjects::Textual, LineSpacing>,
				Frame<Colorblind::InversedColor>>>,
		Condition<
			FunctionObjects::Highlighted,
			Sequence<
				Frame<Colorblind::BorderedRectangle, BorderSize>,
				Frame<Colorblind::BoxedParagraph, FunctionObjects::Textual, LineSpacing>>,
			Sequence<
				Frame<Colorblind::BorderedRectangle, std::ratio<0>>,
				Frame<Colorblind::BoxedParagraph, FunctionObjects::Textual, LineSpacing>>
		>
	>
>;

template<typename RectangleType, typename Margin, typename TextType = std::string>
using Label = FrameStack<
	RectangleType,
	Frame<Movable::Rectangular>,
	Frame<Textual, TextType>,
	Frame<SizedText, FunctionObjects::GlutStrokeFontEngine>,
	Frame<BoxedAdaptableSizeText, Margin>,
	Sequence<
		Frame<Colorblind::FilledRectangle>,
		FrameStack<
			Frame<Colorblind::BoxedText, FunctionObjects::Textual>,
			Frame<Colorblind::InversedColor>>>
>;

BOOST_AUTO_TEST_CASE(Test_Button)
{
	Button<geometry::Rectangle<int>,std::ratio<10>,std::ratio<11>> button(10,"button",1,1,2,5,7);
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

BOOST_AUTO_TEST_CASE(Test_Label)
{
	Label<geometry::Rectangle<int>,std::ratio<10>> label(10,"label",2,3,4,5);
	BOOST_CHECK_EQUAL(label.left(), 2);
	BOOST_CHECK_EQUAL(label.bottom(), 3);
	BOOST_CHECK_EQUAL(label.right(), 4);
	BOOST_CHECK_EQUAL(label.top(), 5);
	BOOST_CHECK_EQUAL(label.text(), "label");
	BOOST_CHECK_EQUAL(label.textHeight(), 10);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Paragraph)
{
	Paragraph<geometry::Rectangle<int>,std::ratio<10>,std::ratio<11>,std::ratio<5>> paragraph(11,"paragraph",1,3,4,5,6);
	BOOST_CHECK_EQUAL(paragraph.left(), 3);
	BOOST_CHECK_EQUAL(paragraph.bottom(), 4);
	BOOST_CHECK_EQUAL(paragraph.right(), 5);
	BOOST_CHECK_EQUAL(paragraph.top(), 6);
	BOOST_CHECK_EQUAL(paragraph.borderSize(), 1);
	BOOST_CHECK_EQUAL(paragraph.text(), "paragraph");
	BOOST_CHECK_EQUAL(paragraph.textHeight(), 11);
	BOOST_CHECK_EQUAL(paragraph.selected(), false);
	BOOST_CHECK_EQUAL(paragraph.highlighted(), false);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Control)
{
	Control<geometry::Rectangle<int>,std::ratio<10>,std::ratio<11>> control(15,"control",2,4,3,2,1);
	BOOST_CHECK_EQUAL(control.left(), 4);
	BOOST_CHECK_EQUAL(control.bottom(), 3);
	BOOST_CHECK_EQUAL(control.right(), 2);
	BOOST_CHECK_EQUAL(control.top(), 1);
	BOOST_CHECK_EQUAL(control.borderSize(), 2);
	BOOST_CHECK_EQUAL(control.text(), "control");
	BOOST_CHECK_EQUAL(control.textHeight(), 15);
	BOOST_CHECK_EQUAL(control.selected(), false);
	BOOST_CHECK_EQUAL(control.highlighted(), false);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Constructors)
{
	using geometry::Rectangle;

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
	FrameStack<
		Rectangle<float>,
		Frame<Movable::Rectangular>,
		Frame<Movable::HVMovable>
	> hvm3(1,2,3,4);
	BOOST_CHECK_EQUAL(hvm3.left(), 1);
	BOOST_CHECK_EQUAL(hvm3.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm3.right(), 3);
	BOOST_CHECK_EQUAL(hvm3.top(), 4);
	FrameStack<
		Bases::Empty,
		Frame<Adapting::Rectangular, Rectangle<float>>,
		Frame<Movable::Rectangular>,
		Frame<Movable::HVMovable>,
		Frame<Textual, std::string>
	> hvm4("ok",1,2,3,4), hvm5(1,2,3,4), hvm6("ok");
	BOOST_CHECK_EQUAL(hvm4.left(), 1);
	BOOST_CHECK_EQUAL(hvm4.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm4.right(), 3);
	BOOST_CHECK_EQUAL(hvm4.top(), 4);
	BOOST_CHECK_EQUAL(hvm4.text(), "ok");
	FrameStack<
		Rectangle<float>,
		Frame<Movable::Rectangular>,
		Frame<Movable::HVMovable>,
		Frame<Textual, std::string>
	> hvm7("ok",1,2,3,4), hvm8(1,2,3,4), hvm9("ok");
	BOOST_CHECK_EQUAL(hvm7.left(), 1);
	BOOST_CHECK_EQUAL(hvm7.bottom(), 2);
	BOOST_CHECK_EQUAL(hvm7.right(), 3);
	BOOST_CHECK_EQUAL(hvm7.top(), 4);
	BOOST_CHECK_EQUAL(hvm7.text(), "ok");

	// Focusable
	FrameStack<
		Rectangle<float>,
		Frame<Bases::Focusable>,
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

	// Textual
	FrameStack<
		Bases::Empty,
		Frame<Textual, std::string>
	> t1("some string");
	BOOST_CHECK_EQUAL(t1.text(), "some string");

	const decltype(t1) t2("42");
	BOOST_CHECK_EQUAL(t2.text(), "42");

	// IndirectCaretLike
	int argc = 0;
	char *argv[] = {nullptr};
	glutInit(&argc, argv); // GlutStrokeFontEngine requires GLUT to be initialized first:

	FrameStack<
		Bases::Empty,
		Frame<Indexing, size_t>,
		Frame<Pointing, decltype(t1)*>,
		Frame<Offset, int>,
		Frame<IndirectCaretLike, FunctionObjects::Textual, FunctionObjects::GlutStrokeFontEngine, char>
	> icl1(-2,2,&t1,1); // TODO: IndirectCaretLike constructor shouldn't allow arbitrary offsets, but instead set them based on index.
	BOOST_CHECK_EQUAL(icl1.xOffset(), -2);
	BOOST_CHECK_EQUAL(icl1.yOffset(), 2);
	BOOST_CHECK_EQUAL(icl1.pointer(), &t1);
	BOOST_CHECK_EQUAL(icl1.index(), 1);
	BOOST_CHECK_EQUAL(icl1.pointer()->text(), "some string");

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

	FrameStack<
		Bases::Empty,
		Frame<Indexing, size_t>,
		Frame<Pointing, decltype(t2)*>,
		Frame<Offset, float>,
		Frame<IndirectCaretLike, FunctionObjects::Textual, FunctionObjects::GlutStrokeFontEngine, char>
	> icl2(1.0f,0.0f,&t2,1);
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

	// EventHandling::CaretLike
	EventHandling::CaretLike<decltype(icl1)> ehcl1(-2,2,&t1,1);
	BOOST_CHECK_EQUAL(ehcl1.xOffset(), -2);
	BOOST_CHECK_EQUAL(ehcl1.yOffset(), 2);
	BOOST_CHECK_EQUAL(ehcl1.pointer(), &t1);
	BOOST_CHECK_EQUAL(ehcl1.index(), 1);
	BOOST_CHECK_EQUAL(ehcl1.pointer()->text(), "ome strin-");

	ehcl1.keyboardNonAscii(Bases::EventHandling::NonAsciiKey::HOME, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl1.xOffset(), 0);
	BOOST_CHECK_EQUAL(ehcl1.yOffset(), 2); // unchanged
	BOOST_CHECK_EQUAL(ehcl1.index(), 0);

	ehcl1.keyboardAscii(0x7f, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl1.pointer()->text(), "me strin-");

	EventHandling::CaretLike<decltype(icl2)> ehcl2(1.0f,0.0f,&t2,1);
	BOOST_CHECK_EQUAL(ehcl2.xOffset(), 1.0f);
	BOOST_CHECK_EQUAL(ehcl2.yOffset(), 0.0f);
	BOOST_CHECK_EQUAL(ehcl2.pointer(), &t2);
	BOOST_CHECK_EQUAL(ehcl2.index(), 1);
	BOOST_CHECK_EQUAL(ehcl2.pointer()->text(), "42");

	ehcl2.keyboardNonAscii(Bases::EventHandling::NonAsciiKey::HOME, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl2.xOffset(), 0.0f);
	BOOST_CHECK_EQUAL(ehcl2.yOffset(), 0.0f); // unchanged
	BOOST_CHECK_EQUAL(ehcl2.index(), 0);

	ehcl2.keyboardAscii(0x7f, true, 0, 0);
	BOOST_CHECK_EQUAL(ehcl2.pointer()->text(), "42"); // unchanged
} // end test case
