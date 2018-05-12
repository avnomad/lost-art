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

#include "gui model/controls.hpp"
#include "geometry.hpp"

#include <ratio>

#define BOOST_TEST_MODULE Controls
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Instantiations)
{
	using namespace GUIModel::Controls;

	Button<geometry::Rectangle<int>, std::ratio<10>, std::ratio<11>, std::string> button(10,"button",1,1,2,5,7);
	Control<geometry::Rectangle<int>, std::ratio<10>, std::ratio<11>, std::ratio<5>> control(15,"control",2,4,3,2,1);
	TextBox<geometry::Rectangle<int>, std::ratio<10>, std::ratio<11>, std::ratio<5>> textbox(15,"textbox",2,4,3,2,1);
} // end test case

BOOST_AUTO_TEST_CASE(Test_Controls)
{
	// TODO: test functionality appended to frame stacks, if any.
} // end test case
