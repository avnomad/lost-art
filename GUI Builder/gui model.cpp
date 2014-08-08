#include "gui model.h"

#include <vector>
using std::vector;

#include <cassert>

#include <boost/rational.hpp>

namespace gui
{
	void runTestSuite()
	{
		using geometry::RectangleSide;
		typedef boost::rational<long long> Rational;

		Model<int> model;

		model.controls.emplace_back(0,0,100,100);
		model.controls.emplace_back(20,20,80,80);
		model.controls.emplace_back(0,0,50,50);

		model.constraints.emplace_back("2.3a + 2cm",0,RectangleSide::LEFT,1,RectangleSide::LEFT);
		model.constraints.emplace_back("2/3a + 3.55b + 2px + cm",0,RectangleSide::RIGHT,1,RectangleSide::RIGHT);
		model.constraints.emplace_back("20.1px",1,RectangleSide::LEFT,1,RectangleSide::RIGHT);

		model.constraints.emplace_back("0.03cm",0,RectangleSide::BOTTOM,1,RectangleSide::BOTTOM);
		model.constraints.emplace_back("1/2b",0,RectangleSide::TOP,1,RectangleSide::TOP);
		model.constraints.emplace_back("c",1,RectangleSide::BOTTOM,1,RectangleSide::TOP);

		//auto symbols = std::make_shared<Symbolic::Common::SymbolTable<>>();
		//vector<Constraint<>::ParseResult<int,Rational>> results;
		//for(auto constraint : model.constraints)
		//	results.push_back(constraint.parse<Rational>(symbols));

		//assert(symbols->size() == 3);
		//assert(symbols->declared("a"));
		//assert(symbols->declared("b"));
		//assert(symbols->declared("c"));
		//assert(!symbols->declared("d"));
		//assert(!symbols->declared("cm"));
		//assert(!symbols->declared("px"));
		//assert(symbols->id("a") == 0);
		//assert(symbols->id("b") == 1);
		//assert(symbols->id("c") == 2);

		//assert(results.size() == 6);
		//assert(results[0].varCoeff.size() == 1);
		//assert(results[0].varCoeff[0] == Rational(23,10));
		//assert(results[0].rhsConstant == -2);
		//assert(results[0].pxSizeCoeff == 0);

		//assert(results[1].varCoeff.size() == 2);
		//assert(results[1].varCoeff[0] == Rational(2,3));
		//assert(results[1].varCoeff[1] == Rational(355,100));
		//assert(results[1].rhsConstant == -1);
		//assert(results[1].pxSizeCoeff == 2);

		//assert(results[2].varCoeff.size() == 0);
		//assert(results[2].rhsConstant == 0);
		//assert(results[2].pxSizeCoeff == Rational(201,10));

		//assert(results[3].varCoeff.size() == 0);
		//assert(results[3].rhsConstant == -Rational(3,100));
		//assert(results[3].pxSizeCoeff == 0);

		//assert(results[4].varCoeff.size() == 1);
		//assert(results[4].varCoeff[1] == Rational(1,2));
		//assert(results[4].rhsConstant == 0);
		//assert(results[4].pxSizeCoeff == 0);

		//assert(results[5].varCoeff.size() == 1);
		//assert(results[5].varCoeff[2] == 1);
		//assert(results[5].rhsConstant == 0);
		//assert(results[5].pxSizeCoeff == 0);
	} // end function runTestSuite

} // end namespace gui
