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

#include <cassert>

#include <boost/rational.hpp>

namespace GUIModel
{
	void runTestSuite()
	{
		using geometry::RectangleSide;
		typedef boost::rational<long long> Rational;

		Controls::Model<int> model;

		// TODO: revert to test only relative frame.
		model.controls.emplace_back(Controls::Model<int>::control_type(0,0,100,100,1,"screen",10));
		model.controls.emplace_back(Controls::Model<int>::control_type(20,20,80,80,1,"control1",10));
		model.controls.emplace_back(Controls::Model<int>::control_type(0,0,50,50,1,"control2",10));

		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,0,RectangleSide::LEFT,1,RectangleSide::LEFT,0,0,"2.3a + 2mm",10));
		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,0,RectangleSide::RIGHT,1,RectangleSide::RIGHT,0,0,"2/3a + 3.55b + 2px + mm",10));
		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,1,RectangleSide::LEFT,1,RectangleSide::RIGHT,0,0,"20.1px",10));

		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,0,RectangleSide::BOTTOM,1,RectangleSide::BOTTOM,0,0,"0.03mm",10));
		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,0,RectangleSide::TOP,1,RectangleSide::TOP,0,0,"1/2b",10));
		model.constraints.emplace_back(Controls::Model<int>::constraint_type(&model.controls,1,RectangleSide::BOTTOM,1,RectangleSide::TOP,0,0,"c",10));

		auto symbols = std::make_shared<Symbolic::Common::SymbolTable<>>();
		vector<Controls::Model<int>::constraint_type::template ParseResult<int,Rational>> results;
		for(auto constraint : model.constraints)
			results.push_back(constraint.parse<Rational>(symbols));

		assert(symbols->size() == 3);
		assert(symbols->declared("a"));
		assert(symbols->declared("b"));
		assert(symbols->declared("c"));
		assert(!symbols->declared("d"));
		assert(!symbols->declared("mm"));
		assert(!symbols->declared("px"));
		assert(symbols->id("a") == 0);
		assert(symbols->id("b") == 1);
		assert(symbols->id("c") == 2);

		assert(results.size() == 6);
		assert(results[0].varCoeff.size() == 1);
		assert(results[0].varCoeff[0] == Rational(23,10));
		assert(results[0].rhsConstant == -2);
		assert(results[0].pxSizeCoeff == 0);

		assert(results[1].varCoeff.size() == 2);
		assert(results[1].varCoeff[0] == Rational(2,3));
		assert(results[1].varCoeff[1] == Rational(355,100));
		assert(results[1].rhsConstant == -1);
		assert(results[1].pxSizeCoeff == 2);

		assert(results[2].varCoeff.size() == 0);
		assert(results[2].rhsConstant == 0);
		assert(results[2].pxSizeCoeff == Rational(201,10));

		assert(results[3].varCoeff.size() == 0);
		assert(results[3].rhsConstant == -Rational(3,100));
		assert(results[3].pxSizeCoeff == 0);

		assert(results[4].varCoeff.size() == 1);
		assert(results[4].varCoeff[1] == Rational(1,2));
		assert(results[4].rhsConstant == 0);
		assert(results[4].pxSizeCoeff == 0);

		assert(results[5].varCoeff.size() == 1);
		assert(results[5].varCoeff[2] == 1);
		assert(results[5].rhsConstant == 0);
		assert(results[5].pxSizeCoeff == 0);
	} // end function runTestSuite

} // end namespace GUIModel
