#include "graphene.h"

#include <cassert>

namespace graphene
{
	void runTestSuite()
	{
		Controls::Button<geometry::Rectangle<int>> button(1,2,5,7,1);

		assert(button.left() == 1);
		assert(button.bottom() == 2);
		assert(button.right() == 5);
		assert(button.top() == 7);
		assert(button.borderSize() == 1);
		assert(button.pressed() == false);
		button.depress().press().depress().press().borderSize() = 2;
		assert(button.pressed() == true);
		assert(button.borderSize() == 2);
	} // end function runTestSuite

} // end namespace graphene
