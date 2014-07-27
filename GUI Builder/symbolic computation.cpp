#include "symbolic computation.h"

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <utility>

#include <cassert>

#include <boost/rational.hpp>
using boost::rational;


namespace Symbolic
{
	void testSymbolTable();
	void testExpression();

	void runTestSuite()
	{
		testSymbolTable();
		testExpression();
	} // end function runTestSuite

	void testSymbolTable()
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
	} // end function testSymbolTable

	void testExpression()
	{
		using Symbolic::Common::SymbolTable;
		using Symbolic::FreeForms::Expression;
		using namespace Symbolic::DSEL;
		typedef rational<long long int> Rational;

		// test DSEL, print methods and parser.
		const size_t nTests = 157;
		auto st = std::make_shared<SymbolTable<string,int>>();
		auto Var = ExpressionBuilder<Primitive::Variable,Rational>(st);
		auto Con = ExpressionBuilder<Primitive::Constant,Rational>(st);
		Expression<Rational> x("x",st), y("y",st);

		Expression<Rational> dselExpressions[nTests] = {
			// height == 0
			Expression<Rational>(),
			// height == 1
			x,
			Con(5),
			Con(Rational(3,5)),
			// height == 2
			+y,
			+Con(0),
			+Con(Rational(7,2)),
			-x,
			-Con(-2),
			-Con(Rational(-32,3)),
			x + y,
			x + -3,
			x + Rational(-3,2),
			-2 + y,
			-2 + Con(-3),
			-2 + Con(Rational(-3,2)),
			Rational(-2,7) + y,
			Rational(-2,7) + Con(-3),
			Rational(-2,7) + Con(Rational(-3,2)),
			x - x,
			x - -4,
			x - Rational(2,4),
			-2 - x,
			-2 - Con(-4),
			-2 - Con(Rational(2,4)),
			Rational(-2,7) - x,
			Rational(-2,7) - Con(-4),
			Rational(-2,7) - Con(Rational(2,4)),
			y * x,
			y * -5,
			y * Rational(2,3),
			-2 * x,
			-2 * Con(-5),
			-2 * Con(Rational(2,3)),
			Rational(-2,7) * x,
			Rational(-2,7) * Con(-5),
			Rational(-2,7) * Con(Rational(2,3)),
			y / y,
			y / 5,
			y / Rational(-3,2),
			-2 / y,
			-2 / Con(5),
			-2 / Con(Rational(-3,2)),
			Rational(-2,7) / y,
			Rational(-2,7) / Con(5),
			Rational(-2,7) / Con(Rational(-3,2)),
			x ^ y,
			x ^ -1,
			x ^ Rational(-1/200),
			-2 ^ y,
			-2 ^ Con(-1),
			-2 ^ Con(Rational(-1/200)),
			Rational(-2,7) ^ y,
			Rational(-2,7) ^ Con(-1),
			Rational(-2,7) ^ Con(Rational(-1/200)),
			// height == 3
			+ +x,
			+ +Con(-5),
			+ +Con(Rational(3,-5)),
			+ -y,
			+ -Con(-2),
			+ -Con(Rational(-3,-5)),
			+(x + y),
			+(x + -3),
			+(x + Rational(-3,2)),
			+(-2 + y),
			+(-2 + Con(-3)),
			+(-2 + Con(Rational(-3,2))),
			+(Rational(-2,7) + y),
			+(Rational(-2,7) + Con(-3)),
			+(Rational(-2,7) + Con(Rational(-3,2))),
			+(x - x),
			+(x - -4),
			+(x - Rational(2,4)),
			+(-2 - x),
			+(-2 - Con(-4)),
			+(-2 - Con(Rational(2,4))),
			+(Rational(-2,7) - x),
			+(Rational(-2,7) - Con(-4)),
			+(Rational(-2,7) - Con(Rational(2,4))),
			+(y * x),
			+(y * -5),
			+(y * Rational(2,3)),
			+(-2 * x),
			+(-2 * Con(-5)),
			+(-2 * Con(Rational(2,3))),
			+(Rational(-2,7) * x),
			+(Rational(-2,7) * Con(-5)),
			+(Rational(-2,7) * Con(Rational(2,3))),
			+(y / y),
			+(y / 5),
			+(y / Rational(-3,2)),
			+(-2 / y),
			+(-2 / Con(5)),
			+(-2 / Con(Rational(-3,2))),
			+(Rational(-2,7) / y),
			+(Rational(-2,7) / Con(5)),
			+(Rational(-2,7) / Con(Rational(-3,2))),
			+(x ^ y),
			+(x ^ -1),
			+(x ^ Rational(-1/200)),
			+(-2 ^ y),
			+(-2 ^ Con(-1)),
			+(-2 ^ Con(Rational(-1/200))),
			+(Rational(-2,7) ^ y),
			+(Rational(-2,7) ^ Con(-1)),
			+(Rational(-2,7) ^ Con(Rational(-1/200))),
			- +x,
			- +Con(2),
			- +Con(Rational(3,5)),
			- -y,
			- -Con(2),
			- -Con(Rational(3,5)),
			-(x + y),
			-(x + -3),
			-(x + Rational(-3,2)),
			-(-2 + y),
			-(-2 + Con(-3)),
			-(-2 + Con(Rational(-3,2))),
			-(Rational(-2,7) + y),
			-(Rational(-2,7) + Con(-3)),
			-(Rational(-2,7) + Con(Rational(-3,2))),
			-(x - x),
			-(x - -4),
			-(x - Rational(2,4)),
			-(-2 - x),
			-(-2 - Con(-4)),
			-(-2 - Con(Rational(2,4))),
			-(Rational(-2,7) - x),
			-(Rational(-2,7) - Con(-4)),
			-(Rational(-2,7) - Con(Rational(2,4))),
			-(y * x),
			-(y * -5),
			-(y * Rational(2,3)),
			-(-2 * x),
			-(-2 * Con(-5)),
			-(-2 * Con(Rational(2,3))),
			-(Rational(-2,7) * x),
			-(Rational(-2,7) * Con(-5)),
			-(Rational(-2,7) * Con(Rational(2,3))),
			-(y / y),
			-(y / 5),
			-(y / Rational(-3,2)),
			-(-2 / y),
			-(-2 / Con(5)),
			-(-2 / Con(Rational(-3,2))),
			-(Rational(-2,7) / y),
			-(Rational(-2,7) / Con(5)),
			-(Rational(-2,7) / Con(Rational(-3,2))),
			-(x ^ y),
			-(x ^ -1),
			-(x ^ Rational(-1/200)),
			-(-2 ^ y),
			-(-2 ^ Con(-1)),
			-(-2 ^ Con(Rational(-1/200))),
			-(Rational(-2,7) ^ y),
			-(Rational(-2,7) ^ Con(-1)),
			-(Rational(-2,7) ^ Con(Rational(-1/200))),
		}; // end dselExpressions initializer

		// -3/5^2, -3^2


		Expression<Rational> c(Rational(3,5));
		Expression<Rational> a("a");
		Expression<Rational> b("b");
		Expression<Rational> e;

		auto s1 = a+c;
		auto s2 = b+c;
		auto s3 = Var("x") + Var("y") / Con(2);
		auto s4 = Var("x") + Var("y") / Rational(2);
		auto s5 = Var("x") + Var("y") / 2;
		auto s6 = Var("x") + Con(2) / Var("y");
		auto s7 = Var("x") + Rational(2) / Var("y");
		auto s8 = Var("x") + 2 / Var("y");
		Expression<Rational> z("z",st);
		auto s9 = Con(1) - 2 + 3 - 4 + 5;
		auto s10 = Con(1) - 2 + 3 - 4 /+ Con(5);
		auto s11 = (3*x+1)/(1 + x/(2 + x/(3 + x/(4 + x/(5 + x)))));
		auto s12 = (((x+1)^2)/(y-Rational(223,100)))/((z+1)/(y+2));
		auto s13 = Con(Rational(5,3)) / Con(Rational(7,2));

		// x+y; // should throw exception!

		s1.print1D(cout,true);
		cout << endl;
		s2.print1D(cout,true);
		cout << endl;
		s3.print1D(cout,true);
		cout << endl;
		s4.print1D(cout,true);
		cout << endl;
		s5.print1D(cout,true);
		cout << endl;
		s6.print1D(cout,true);
		cout << endl;
		s7.print1D(cout,true);
		cout << endl;
		s8.print1D(cout,true);
		cout << endl;
		s9.print1D(cout,true);
		cout << endl;
		s10.print1D(cout,true);
		cout << endl;
		s11.print1D(cout,true);
		cout << endl;
		s12.print1D(cout,true);
		cout << endl;
		s13.print1D(cout,true);
		cout << endl;

		s1.print1D(cout);
		cout << endl;
		s2.print1D(cout);
		cout << endl;
		s3.print1D(cout);
		cout << endl;
		s4.print1D(cout);
		cout << endl;
		s5.print1D(cout);
		cout << endl;
		s6.print1D(cout);
		cout << endl;
		s7.print1D(cout);
		cout << endl;
		s8.print1D(cout);
		cout << endl;
		s9.print1D(cout);
		cout << endl;
		s10.print1D(cout);
		cout << endl;
		s11.print1D(cout);
		cout << endl;
		s12.print1D(cout);
		cout << endl;
		s13.print1D(cout);
		cout << endl;

		s1.print2D(cout,true);
		cout << endl;
		s2.print2D(cout,true);
		cout << endl;
		s3.print2D(cout,true);
		cout << endl;
		s4.print2D(cout,true);
		cout << endl;
		s5.print2D(cout,true);
		cout << endl;
		s6.print2D(cout,true);
		cout << endl;
		s7.print2D(cout,true);
		cout << endl;
		s8.print2D(cout,true);
		cout << endl;
		s9.print2D(cout,true);
		cout << endl;
		s10.print2D(cout,true);
		cout << endl;
		s11.print2D(cout,true);
		cout << endl;
		s12.print2D(cout,true);
		cout << endl;
		s13.print2D(cout,true);
		cout << endl;

		s1.print2D(cout);
		cout << endl;
		s2.print2D(cout);
		cout << endl;
		s3.print2D(cout);
		cout << endl;
		s4.print2D(cout);
		cout << endl;
		s5.print2D(cout);
		cout << endl;
		s6.print2D(cout);
		cout << endl;
		s7.print2D(cout);
		cout << endl;
		s8.print2D(cout);
		cout << endl;
		s9.print2D(cout);
		cout << endl;
		s10.print2D(cout);
		cout << endl;
		s11.print2D(cout);
		cout << endl;
		s12.print2D(cout);
		cout << endl;
		s13.print2D(cout);
		cout << endl;

		//string s = "1 - 2 + 3 - 4 + 5";
		//Expression<Rational> expr1(s.begin(),s.end());
		//s = "1 - 2 + 3 - 4 /+ 5";
		//Expression<Rational> expr2(s.begin(),s.end());
		//try	{
		//	s = "1 - 2 + 3 - 4 +/ 5";
		//	Expression<Rational> expr3(s.begin(),s.end());
		//}catch(std::exception &e){
		//	cout << e.what() << endl;
		//} // end catch
		//s = "(3*x+1)/(1 + x/(2 + x/(3 + x/(4 + x/(5 + x)))))";
		//Expression<Rational> expr4(s.begin(),s.end());
		//s = "((x+1)/(x-2))/((x+1)/(x+2))";
		//Expression<Rational> expr5(s.begin(),s.end());
		//s = "((x+1)^2/(y-2.23))/((z+1)/(y+2))";
		//Expression<Rational> expr6(s.begin(),s.end());
		//try	{
		//	s = "((x+1)^2/(y-@2.23))/((z+1)/(y+2))";
		//	Expression<Rational> expr7(s.begin(),s.end());
		//}catch(std::exception &e){
		//	cout << e.what() << endl;
		//} // end catch
	} // end function testExpression

} // end namespace Symbolic
