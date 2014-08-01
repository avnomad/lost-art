#include "gui model.h"

#include <cassert>

namespace gui
{
	void runTestSuite()
	{
		// test Rectangle
		geometry::Rectangle<int> recti(-2,5,3,8);
		assert(recti.left() == -2);
		assert(recti.right() == 3);
		assert(recti.bottom() == 5);
		assert(recti.top() == 8);
		recti.sides()[size_t(geometry::Rectangle<int>::Side::RIGHT)] = 0;
		assert(recti.right() == 0);
		recti.side(geometry::Rectangle<int>::Side::BOTTOM) = 4;
		assert(recti.bottom() == 4);

		geometry::Rectangle<float> rectf(-2.0,5.0,3.0,8.0);
		assert(rectf.sides()[size_t(geometry::Rectangle<float>::Side::TOP)] == 8.0);
	} // end function runTestSuite


} // end namespace gui
