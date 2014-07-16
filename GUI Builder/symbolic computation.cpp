#include "symbolic computation.h"

#include <string>
using std::string;

#include <utility>

#include <cassert>


namespace Symbolic
{
	void runTestSuite()
	{
		// currently does not check whether exceptions are indeed thrown on errors.
		Common::SymbolTable<string,long long int> st;

		assert(st.empty());
		assert(st.size() == 0);
		assert(!st.declared("x"));
		assert(!st.declared(0));
		assert(!st.declared("y"));
		assert(!st.declared(1));
		assert(!st.declared("z"));
		assert(!st.declared(2));

		assert(st.declare("x") == 0);
		assert(!st.empty());
		assert(st.size() == 1);
		assert(st.declared("x"));
		assert(st.declared(0));
		assert(!st.declared("y"));
		assert(!st.declared(1));
		assert(!st.declared("z"));
		assert(!st.declared(2));
		assert(st.id("x") == 0);
		assert(st.name(0) == "x");

		assert(st.declare("y") == 1);
		assert(!st.empty());
		assert(st.size() == 2);
		assert(st.declared("x"));
		assert(st.declared(0));
		assert(st.declared("y"));
		assert(st.declared(1));
		assert(!st.declared("z"));
		assert(!st.declared(2));
		assert(st.id("x") == 0);
		assert(st.name(0) == "x");
		assert(st.id("y") == 1);
		assert(st.name(1) == "y");

		assert(st.declare("z") == 2);
		assert(!st.empty());
		assert(st.size() == 3);
		assert(st.declared("x"));
		assert(st.declared(0));
		assert(st.declared("y"));
		assert(st.declared(1));
		assert(st.declared("z"));
		assert(st.declared(2));
		assert(st.id("x") == 0);
		assert(st.name(0) == "x");
		assert(st.id("y") == 1);
		assert(st.name(1) == "y");
		assert(st.id("z") == 2);
		assert(st.name(2) == "z");

		Common::SymbolTable<string,long long int> cp1 = st;
		assert(!cp1.empty());
		assert(cp1.declare("z") == 2);	// should have no effect
		assert(cp1.size() == 3);
		assert(cp1.declared("x"));
		assert(cp1.declared(0));
		assert(cp1.declared("y"));
		assert(cp1.declared(1));
		assert(cp1.declared("z"));
		assert(cp1.declared(2));
		assert(cp1.id("x") == 0);
		assert(cp1.name(0) == "x");
		assert(cp1.id("y") == 1);
		assert(cp1.name(1) == "y");
		assert(cp1.id("z") == 2);
		assert(cp1.name(2) == "z");
		assert(cp1.undeclare("z") == 2);
		assert(cp1.undeclare("y") == 1);

		Common::SymbolTable<string,long long int> cp2;
		assert(cp2.empty());
		cp2 = st;
		assert(!cp2.empty());
		assert(cp2.declare("z") == 2);	// assert no aliasing
		assert(cp2.size() == 3);
		assert(cp2.declared("x"));
		assert(cp2.declared(0));
		assert(cp2.declared("y"));
		assert(cp2.declared(1));
		assert(cp2.declared("z"));
		assert(cp2.declared(2));
		assert(cp2.id("x") == 0);
		assert(cp2.name(0) == "x");
		assert(cp2.id("y") == 1);
		assert(cp2.name(1) == "y");
		assert(cp2.id("z") == 2);
		assert(cp2.name(2) == "z");
		assert(cp2.undeclare("z") == 2);
		assert(cp2.undeclare("y") == 1);
		assert(cp2.undeclare("x") == 0);

		cp2 = std::move(cp1);
		assert(!cp2.empty());
		assert(cp2.size() == 1);
		assert(cp2.declared("x"));
		assert(cp2.declared(0));
		assert(!cp2.declared("y"));
		assert(!cp2.declared(1));
		assert(!cp2.declared("z"));
		assert(!cp2.declared(2));
		assert(cp2.id("x") == 0);
		assert(cp2.name(0) == "x");
		assert(cp2.undeclare("x") == 0);

		assert(st.undeclare("z") == 2);
		assert(!st.empty());
		assert(st.size() == 2);
		assert(st.declared("x"));
		assert(st.declared(0));
		assert(st.declared("y"));
		assert(st.declared(1));
		assert(!st.declared("z"));
		assert(!st.declared(2));
		assert(st.id("x") == 0);
		assert(st.name(0) == "x");
		assert(st.id("y") == 1);
		assert(st.name(1) == "y");

		st.clear();
		assert(st.empty());
		assert(st.size() == 0);
		assert(!st.declared("x"));
		assert(!st.declared(0));
		assert(!st.declared("y"));
		assert(!st.declared(1));
		assert(!st.declared("z"));
		assert(!st.declared(2));
		assert(!st.declared(-1));
	} // runTestSuite

} // end namespace Symbolic
