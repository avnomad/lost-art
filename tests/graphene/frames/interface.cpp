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

#include "graphene/frames/interface.hpp"
using namespace graphene;
using namespace Frames;
using namespace Frames::Interface;

#define BOOST_TEST_MODULE Interface Frames
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Interface_Frames)
{
	// TODO: find ways to test interface frames in isolation.
	// e.g. could use type traits to test that certain optional members exist only on mutable interfaces.
} // end test case
