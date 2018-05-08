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

#include "gui model.hpp"

#include <vector>
using std::vector;

#include <boost/rational.hpp>

#define BOOST_TEST_MODULE GUI Model
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Constraint_Parser)
{
	using geometry::RectangleSide;
	using Rational = boost::rational<long long>;
	using GUIModel::Controls::Model;

	Model<int> model;

	// TODO: revert to test only relative frame.
	model.controls.emplace_back(Model<int>::control_type(10,"screen",1,0,0,100,100));
	model.controls.emplace_back(Model<int>::control_type(10,"control1",1,20,20,80,80));
	model.controls.emplace_back(Model<int>::control_type(10,"control2",1,0,0,50,50));

	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,0,RectangleSide::LEFT,1,RectangleSide::LEFT,0,0,"2.3a + 2mm",10));
	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,0,RectangleSide::RIGHT,1,RectangleSide::RIGHT,0,0,"2/3a + 3.55b + 2px + mm",10));
	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,1,RectangleSide::LEFT,1,RectangleSide::RIGHT,0,0,"20.1px",10));

	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,0,RectangleSide::BOTTOM,1,RectangleSide::BOTTOM,0,0,"0.03mm",10));
	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,0,RectangleSide::TOP,1,RectangleSide::TOP,0,0,"1/2b",10));
	model.constraints.emplace_back(Model<int>::constraint_type(&model.controls,1,RectangleSide::BOTTOM,1,RectangleSide::TOP,0,0,"c",10));

	auto symbols = std::make_shared<Symbolic::Common::SymbolTable<>>();
	vector<decltype(model.constraints.front().parse<Rational>(symbols))> results;
	for(auto constraint : model.constraints)
		results.push_back(constraint.parse<Rational>(symbols));

	BOOST_CHECK_EQUAL(symbols->size(), 3);
	BOOST_CHECK(symbols->declared("a"));
	BOOST_CHECK(symbols->declared("b"));
	BOOST_CHECK(symbols->declared("c"));
	BOOST_CHECK(!symbols->declared("d"));
	BOOST_CHECK(!symbols->declared("mm"));
	BOOST_CHECK(!symbols->declared("px"));
	BOOST_CHECK_EQUAL(symbols->id("a"), 0);
	BOOST_CHECK_EQUAL(symbols->id("b"), 1);
	BOOST_CHECK_EQUAL(symbols->id("c"), 2);

	BOOST_CHECK_EQUAL(results.size(), 6);
	BOOST_CHECK_EQUAL(results[0].varCoeff.size(), 1);
	BOOST_CHECK_EQUAL(results[0].varCoeff[0], Rational(23,10));
	BOOST_CHECK_EQUAL(results[0].rhsConstant, -2);
	BOOST_CHECK_EQUAL(results[0].pxSizeCoeff, 0);

	BOOST_CHECK_EQUAL(results[1].varCoeff.size(), 2);
	BOOST_CHECK_EQUAL(results[1].varCoeff[0], Rational(2,3));
	BOOST_CHECK_EQUAL(results[1].varCoeff[1], Rational(355,100));
	BOOST_CHECK_EQUAL(results[1].rhsConstant, -1);
	BOOST_CHECK_EQUAL(results[1].pxSizeCoeff, 2);

	BOOST_CHECK_EQUAL(results[2].varCoeff.size(), 0);
	BOOST_CHECK_EQUAL(results[2].rhsConstant, 0);
	BOOST_CHECK_EQUAL(results[2].pxSizeCoeff, Rational(201,10));

	BOOST_CHECK_EQUAL(results[3].varCoeff.size(), 0);
	BOOST_CHECK_EQUAL(results[3].rhsConstant, -Rational(3,100));
	BOOST_CHECK_EQUAL(results[3].pxSizeCoeff, 0);

	BOOST_CHECK_EQUAL(results[4].varCoeff.size(), 1);
	BOOST_CHECK_EQUAL(results[4].varCoeff[1], Rational(1,2));
	BOOST_CHECK_EQUAL(results[4].rhsConstant, 0);
	BOOST_CHECK_EQUAL(results[4].pxSizeCoeff, 0);

	BOOST_CHECK_EQUAL(results[5].varCoeff.size(), 1);
	BOOST_CHECK_EQUAL(results[5].varCoeff[2], 1);
	BOOST_CHECK_EQUAL(results[5].rhsConstant, 0);
	BOOST_CHECK_EQUAL(results[5].pxSizeCoeff, 0);
} // end test case
