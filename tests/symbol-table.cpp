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

#include "symbol table.hpp"

#include <string>
using std::string;

#define BOOST_TEST_MODULE Symbol Table
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_Symbol_Table_Operations)
{
	using Symbolic::Common::SymbolTable;

	// currently does not check whether exceptions are indeed thrown on errors.
	SymbolTable<string,long long int> st;

	BOOST_CHECK(st.empty());
	BOOST_CHECK_EQUAL(st.size(), 0);
	BOOST_CHECK(!st.declared("x"));
	BOOST_CHECK(!st.declared(0));
	BOOST_CHECK(!st.declared("y"));
	BOOST_CHECK(!st.declared(1));
	BOOST_CHECK(!st.declared("z"));
	BOOST_CHECK(!st.declared(2));

	BOOST_CHECK_EQUAL(st.declare("x"), 0);
	BOOST_CHECK(!st.empty());
	BOOST_CHECK_EQUAL(st.size(), 1);
	BOOST_CHECK(st.declared("x"));
	BOOST_CHECK(st.declared(0));
	BOOST_CHECK(!st.declared("y"));
	BOOST_CHECK(!st.declared(1));
	BOOST_CHECK(!st.declared("z"));
	BOOST_CHECK(!st.declared(2));
	BOOST_CHECK_EQUAL(st.id("x"), 0);
	BOOST_CHECK_EQUAL(st.name(0), "x");

	BOOST_CHECK_EQUAL(st.declare("y"), 1);
	BOOST_CHECK(!st.empty());
	BOOST_CHECK_EQUAL(st.size(), 2);
	BOOST_CHECK(st.declared("x"));
	BOOST_CHECK(st.declared(0));
	BOOST_CHECK(st.declared("y"));
	BOOST_CHECK(st.declared(1));
	BOOST_CHECK(!st.declared("z"));
	BOOST_CHECK(!st.declared(2));
	BOOST_CHECK_EQUAL(st.id("x"), 0);
	BOOST_CHECK_EQUAL(st.name(0), "x");
	BOOST_CHECK_EQUAL(st.id("y"), 1);
	BOOST_CHECK_EQUAL(st.name(1), "y");

	BOOST_CHECK_EQUAL(st.declare("z"), 2);
	BOOST_CHECK(!st.empty());
	BOOST_CHECK_EQUAL(st.size(), 3);
	BOOST_CHECK(st.declared("x"));
	BOOST_CHECK(st.declared(0));
	BOOST_CHECK(st.declared("y"));
	BOOST_CHECK(st.declared(1));
	BOOST_CHECK(st.declared("z"));
	BOOST_CHECK(st.declared(2));
	BOOST_CHECK_EQUAL(st.id("x"), 0);
	BOOST_CHECK_EQUAL(st.name(0), "x");
	BOOST_CHECK_EQUAL(st.id("y"), 1);
	BOOST_CHECK_EQUAL(st.name(1), "y");
	BOOST_CHECK_EQUAL(st.id("z"), 2);
	BOOST_CHECK_EQUAL(st.name(2), "z");

	SymbolTable<string,long long int> cp1 = st;
	BOOST_CHECK(!cp1.empty());
	BOOST_CHECK_EQUAL(cp1.declare("z"), 2);	// should have no effect
	BOOST_CHECK_EQUAL(cp1.size(), 3);
	BOOST_CHECK(cp1.declared("x"));
	BOOST_CHECK(cp1.declared(0));
	BOOST_CHECK(cp1.declared("y"));
	BOOST_CHECK(cp1.declared(1));
	BOOST_CHECK(cp1.declared("z"));
	BOOST_CHECK(cp1.declared(2));
	BOOST_CHECK_EQUAL(cp1.id("x"), 0);
	BOOST_CHECK_EQUAL(cp1.name(0), "x");
	BOOST_CHECK_EQUAL(cp1.id("y"), 1);
	BOOST_CHECK_EQUAL(cp1.name(1), "y");
	BOOST_CHECK_EQUAL(cp1.id("z"), 2);
	BOOST_CHECK_EQUAL(cp1.name(2), "z");
	BOOST_CHECK_EQUAL(cp1.undeclare("z"), 2);
	BOOST_CHECK_EQUAL(cp1.undeclare("y"), 1);

	SymbolTable<string,long long int> cp2;
	BOOST_CHECK(cp2.empty());
	cp2 = st;
	BOOST_CHECK(!cp2.empty());
	BOOST_CHECK_EQUAL(cp2.declare("z"), 2);	// assert no aliasing
	BOOST_CHECK_EQUAL(cp2.size(), 3);
	BOOST_CHECK(cp2.declared("x"));
	BOOST_CHECK(cp2.declared(0));
	BOOST_CHECK(cp2.declared("y"));
	BOOST_CHECK(cp2.declared(1));
	BOOST_CHECK(cp2.declared("z"));
	BOOST_CHECK(cp2.declared(2));
	BOOST_CHECK_EQUAL(cp2.id("x"), 0);
	BOOST_CHECK_EQUAL(cp2.name(0), "x");
	BOOST_CHECK_EQUAL(cp2.id("y"), 1);
	BOOST_CHECK_EQUAL(cp2.name(1), "y");
	BOOST_CHECK_EQUAL(cp2.id("z"), 2);
	BOOST_CHECK_EQUAL(cp2.name(2), "z");
	BOOST_CHECK_EQUAL(cp2.undeclare("z"), 2);
	BOOST_CHECK_EQUAL(cp2.undeclare("y"), 1);
	BOOST_CHECK_EQUAL(cp2.undeclare("x"), 0);

	cp2 = std::move(cp1);
	BOOST_CHECK(!cp2.empty());
	BOOST_CHECK_EQUAL(cp2.size(), 1);
	BOOST_CHECK(cp2.declared("x"));
	BOOST_CHECK(cp2.declared(0));
	BOOST_CHECK(!cp2.declared("y"));
	BOOST_CHECK(!cp2.declared(1));
	BOOST_CHECK(!cp2.declared("z"));
	BOOST_CHECK(!cp2.declared(2));
	BOOST_CHECK_EQUAL(cp2.id("x"), 0);
	BOOST_CHECK_EQUAL(cp2.name(0), "x");
	BOOST_CHECK_EQUAL(cp2.undeclare("x"), 0);

	BOOST_CHECK_EQUAL(st.undeclare("z"), 2);
	BOOST_CHECK(!st.empty());
	BOOST_CHECK_EQUAL(st.size(), 2);
	BOOST_CHECK(st.declared("x"));
	BOOST_CHECK(st.declared(0));
	BOOST_CHECK(st.declared("y"));
	BOOST_CHECK(st.declared(1));
	BOOST_CHECK(!st.declared("z"));
	BOOST_CHECK(!st.declared(2));
	BOOST_CHECK_EQUAL(st.id("x"), 0);
	BOOST_CHECK_EQUAL(st.name(0), "x");
	BOOST_CHECK_EQUAL(st.id("y"), 1);
	BOOST_CHECK_EQUAL(st.name(1), "y");

	st.clear();
	BOOST_CHECK(st.empty());
	BOOST_CHECK_EQUAL(st.size(), 0);
	BOOST_CHECK(!st.declared("x"));
	BOOST_CHECK(!st.declared(0));
	BOOST_CHECK(!st.declared("y"));
	BOOST_CHECK(!st.declared(1));
	BOOST_CHECK(!st.declared("z"));
	BOOST_CHECK(!st.declared(2));
	BOOST_CHECK(!st.declared(-1));
} // end test case
