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

		Expression<Rational> c(Rational(3,5));
		Expression<Rational> a("a");
		Expression<Rational> b("b");
		Expression<Rational> e;

		std::shared_ptr<SymbolTable<string,int>> st(new SymbolTable<string,int>());
		auto Var = ExpressionBuilder<Primitive::Variable,Rational>(st);
		auto Con = ExpressionBuilder<Primitive::Constant,Rational>(st);
		auto s1 = a+c;
		auto s2 = b+c;
		auto s3 = Var("x") + Var("y") / Con(2);
		auto s4 = Var("x") + Var("y") / Rational(2);
		auto s5 = Var("x") + Var("y") / 2;
		auto s6 = Var("x") + Con(2) / Var("y");
		auto s7 = Var("x") + Rational(2) / Var("y");
		auto s8 = Var("x") + 2 / Var("y");
		Expression<Rational> x("x",st);
		Expression<Rational> y("y",st);
		Expression<Rational> z("z",st);
		auto s9 = Con(1) - 2 + 3 - 4 + 5;
		auto s10 = Con(1) - 2 + 3 - 4 /+ Con(5);
		auto s11 = (3*x+1)/(1 + x/(2 + x/(3 + x/(4 + x/(5 + x)))));
		auto s12 = (((x+1)^2)/(y-Rational(223,100)))/((z+1)/(y+2));

		// x+y; // should throw exception!

		s1.print1DFullParen(cout);
		cout << endl;
		s2.print1DFullParen(cout);
		cout << endl;
		s3.print1DFullParen(cout);
		cout << endl;
		s4.print1DFullParen(cout);
		cout << endl;
		s5.print1DFullParen(cout);
		cout << endl;
		s6.print1DFullParen(cout);
		cout << endl;
		s7.print1DFullParen(cout);
		cout << endl;
		s8.print1DFullParen(cout);
		cout << endl;
		s9.print1DFullParen(cout);
		cout << endl;
		s10.print1DFullParen(cout);
		cout << endl;
		s11.print1DFullParen(cout);
		cout << endl;
		s12.print1DFullParen(cout);
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
