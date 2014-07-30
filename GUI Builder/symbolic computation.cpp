#include "symbolic computation.h"

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <set>
using std::set;

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
		const size_t nTests = 390;
		auto st = std::make_shared<SymbolTable<string,int>>();
		auto Var = ExpressionBuilder<Primitive::Variable,Rational>(st);
		auto Con = ExpressionBuilder<Primitive::Constant,Rational>(st);
		Expression<Rational> x("x",st), y("y",st);

		struct Test
		{
			Expression<Rational> dsel;
			string fullParen1D;
			string minParen1D;
			string fullParen2D;
			string minParen2D;
		}; // end struct Test

		Test tests[nTests] = {
			// height == 0
			{Expression<Rational>(),    ""                             , ""                   , ""                                     , ""                         },
			// height == 1
			{x,                         "(x)"                          , "x"                  , "(x)\n"                                , "x\n"                      },
			{Con(32775),                "(32775)"                      , "32775"              , "(32775)\n"                            , "32775\n"                  },
			// height == 2
			{+y,                        "(+(y))"                       , "+y"                 , "(+(y))\n"                             , "+y\n"                     },
			{+Con(0),                   "(+(0))"                       , "+0"                 , "(+(0))\n"                             , "+0\n"                     },
			{-x,                        "(-(x))"                       , "-x"                 , "(-(x))\n"                             , "-x\n"                     },
			{-Con(2),                   "(-(2))"                       , "-2"                 , "(-(2))\n"                             , "-2\n"                     },
			{x + y,                     "((x)+(y))"                    , "x+y"                , "((x) + (y))\n"                        , "x + y\n"                  },
			{x + 3,                     "((x)+(3))"                    , "x+3"                , "((x) + (3))\n"                        , "x + 3\n"                  },
			{2 + y,                     "((2)+(y))"                    , "2+y"                , "((2) + (y))\n"                        , "2 + y\n"                  },
			{2 + Con(3),                "((2)+(3))"                    , "2+3"                , "((2) + (3))\n"                        , "2 + 3\n"                  },
			{x - x,                     "((x)-(x))"                    , "x-x"                , "((x) - (x))\n"                        , "x - x\n"                  },
			{x - 4,                     "((x)-(4))"                    , "x-4"                , "((x) - (4))\n"                        , "x - 4\n"                  },
			{2 - x,                     "((2)-(x))"                    , "2-x"                , "((2) - (x))\n"                        , "2 - x\n"                  },
			{2 - Con(4),                "((2)-(4))"                    , "2-4"                , "((2) - (4))\n"                        , "2 - 4\n"                  },
			{y * x,                     "((y)*(x))"                    , "y*x"                , "((y)*(x))\n"                          , "y*x\n"                    },
			{y * 5,                     "((y)*(5))"                    , "y*5"                , "((y)*(5))\n"                          , "y*5\n"                    },
			{2 * x,                     "((2)*(x))"                    , "2*x"                , "((2)*(x))\n"                          , "2*x\n"                    },
			{2 * Con(32775),            "((2)*(32775))"                , "2*32775"            , "((2)*(32775))\n"                      , "2*32775\n"                },
			{y / y,                     "((y)/(y))"                    , "y/y"                , " (y) \n"
			                                                                                    "(ΔΔΔ)\n"
																								" (y) \n"                              , "y\n"
																								                                         "Δ\n"
																										                                 "y\n"                      },
			{y / 5,                     "((y)/(5))"                    , "y/5"                , " (y) \n"
			                                                                                    "(ΔΔΔ)\n"
																								" (5) \n"                              , "y\n"
																								                                         "Δ\n"
																										                                 "5\n"                      },
			{2 / y,                     "((2)/(y))"                    , "2/y"                , " (2) \n"
			                                                                                    "(ΔΔΔ)\n"
																								" (y) \n"                              , "2\n"
																								                                         "Δ\n"
																										                                 "y\n"                      },
			{2 / Con(32775),            "((2)/(32775))"                , "2/32775"            , "   (2)   \n"
			                                                                                    "(ΔΔΔΔΔΔΔ)\n"
																								" (32775) \n"                          , "  2  \n"
																								                                         "ΔΔΔΔΔ\n"
																										                                 "32775\n"                  },
			{x ^ y,                     "((x)^(y))"                    , "x^y"                , "    (y) \n"
			                                                                                    "((x)   )\n"                           , " y\n"
			                                                                                                                             "x \n"                     },
			{x ^ 1,                     "((x)^(1))"                    , "x^1"                , "    (1) \n"
			                                                                                    "((x)   )\n"                           , " 1\n"
			                                                                                                                             "x \n"                     },
			{2 ^ y,                     "((2)^(y))"                    , "2^y"                , "    (y) \n"
			                                                                                    "((2)   )\n"                           , " y\n"
			                                                                                                                             "2 \n"                     },
			{2 ^ Con(1),                "((2)^(1))"                    , "2^1"                , "    (1) \n"
			                                                                                    "((2)   )\n"                           , " 1\n"
			                                                                                                                             "2 \n"                     },
			// height == 3
			{+ +x,                      "(+(+(x)))"                    , "++x"                , "(+(+(x)))\n"                          , "++x\n"                    },
			{+ +Con(32775),             "(+(+(32775)))"                , "++32775"            , "(+(+(32775)))\n"                      , "++32775\n"                },
			{+ -y,                      "(+(-(y)))"                    , "+-y"                , "(+(-(y)))\n"                          , "+-y\n"                    },
			{+ -Con(2),                 "(+(-(2)))"                    , "+-2"                , "(+(-(2)))\n"                          , "+-2\n"                    },
			{+(x + y),                  "(+((x)+(y)))"                 , "+(x+y)"             , "(+((x) + (y)))\n"                     , "+(x + y)\n"               },
			{+(x + 3),                  "(+((x)+(3)))"                 , "+(x+3)"             , "(+((x) + (3)))\n"                     , "+(x + 3)\n"               },
			{+(2 + y),                  "(+((2)+(y)))"                 , "+(2+y)"             , "(+((2) + (y)))\n"                     , "+(2 + y)\n"               },
			{+(2 + Con(3)),             "(+((2)+(3)))"                 , "+(2+3)"             , "(+((2) + (3)))\n"                     , "+(2 + 3)\n"               },
			{+(x - x),                  "(+((x)-(x)))"                 , "+(x-x)"             , "(+((x) - (x)))\n"                     , "+(x - x)\n"               },
			{+(x - 4),                  "(+((x)-(4)))"                 , "+(x-4)"             , "(+((x) - (4)))\n"                     , "+(x - 4)\n"               },
			{+(2 - x),                  "(+((2)-(x)))"                 , "+(2-x)"             , "(+((2) - (x)))\n"                     , "+(2 - x)\n"               },
			{+(2 - Con(4)),             "(+((2)-(4)))"                 , "+(2-4)"             , "(+((2) - (4)))\n"                     , "+(2 - 4)\n"               },
			{+(y * x),                  "(+((y)*(x)))"                 , "+(y*x)"             , "(+((y)*(x)))\n"                       , "+(y*x)\n"                 },
			{+(y * 5),                  "(+((y)*(5)))"                 , "+(y*5)"             , "(+((y)*(5)))\n"                       , "+(y*5)\n"                 },
			{+(2 * x),                  "(+((2)*(x)))"                 , "+(2*x)"             , "(+((2)*(x)))\n"                       , "+(2*x)\n"                 },
			{+(2 * Con(32775)),         "(+((2)*(32775)))"             , "+(2*32775)"         , "(+((2)*(32775)))\n"                   , "+(2*32775)\n"             },
			{+(y / y),                  "(+((y)/(y)))"                 , "+(y/y)"             , "   (y)  \n"
			                                                                                    "(+(ΔΔΔ))\n"
																								"   (y)  \n"                           , " y\n"
																								                                         "+Δ\n"
																										                                 " y\n"                     },
			{+(y / 5),                  "(+((y)/(5)))"                 , "+(y/5)"             , "   (y)  \n"
			                                                                                    "(+(ΔΔΔ))\n"
																								"   (5)  \n"                           , " y\n"
																								                                         "+Δ\n"
																										                                 " 5\n"                     },
			{+(2 / y),                  "(+((2)/(y)))"                 , "+(2/y)"             , "   (2)  \n"
			                                                                                    "(+(ΔΔΔ))\n"
																								"   (y)  \n"                           , " 2\n"
																								                                         "+Δ\n"
																										                                 " y\n"                     },
			{+(2 / Con(32775)),         "(+((2)/(32775)))"             , "+(2/32775)"         , "     (2)    \n"
			                                                                                    "(+(ΔΔΔΔΔΔΔ))\n"
																								"   (32775)  \n"                       , "   2  \n"
																								                                         "+ΔΔΔΔΔ\n"
																										                                 " 32775\n"                 },
			{+(x ^ y),                  "(+((x)^(y)))"                 , "+x^y"               , "      (y)  \n"
			                                                                                    "(+((x)   ))\n"                        , "  y\n"
			                                                                                                                             "+x \n"                    }, // ^
			{+(x ^ 1),                  "(+((x)^(1)))"                 , "+x^1"               , "      (1)  \n"
			                                                                                    "(+((x)   ))\n"                        , "  1\n"
			                                                                                                                             "+x \n"                    }, // ^
			{+(2 ^ y),                  "(+((2)^(y)))"                 , "+2^y"               , "      (y)  \n"
			                                                                                    "(+((2)   ))\n"                        , "  y\n"
			                                                                                                                             "+2 \n"                    }, // ^
			{+(2 ^ Con(1)),             "(+((2)^(1)))"                 , "+2^1"               , "      (1)  \n"
			                                                                                    "(+((2)   ))\n"                        , "  1\n"
			                                                                                                                             "+2 \n"                    }, // ^
			{- +x,                      "(-(+(x)))"                    , "-+x"                , "(-(+(x)))\n"                          , "-+x\n"                    },
			{- +Con(2),                 "(-(+(2)))"                    , "-+2"                , "(-(+(2)))\n"                          , "-+2\n"                    },
			{- -y,                      "(-(-(y)))"                    , "--y"                , "(-(-(y)))\n"                          , "--y\n"                    },
			{- -Con(2),                 "(-(-(2)))"                    , "--2"                , "(-(-(2)))\n"                          , "--2\n"                    },
			{-(x + y),                  "(-((x)+(y)))"                 , "-(x+y)"             , "(-((x) + (y)))\n"                     , "-(x + y)\n"               },
			{-(x + 3),                  "(-((x)+(3)))"                 , "-(x+3)"             , "(-((x) + (3)))\n"                     , "-(x + 3)\n"               },
			{-(2 + y),                  "(-((2)+(y)))"                 , "-(2+y)"             , "(-((2) + (y)))\n"                     , "-(2 + y)\n"               },
			{-(2 + Con(3)),             "(-((2)+(3)))"                 , "-(2+3)"             , "(-((2) + (3)))\n"                     , "-(2 + 3)\n"               },
			{-(x - x),                  "(-((x)-(x)))"                 , "-(x-x)"             , "(-((x) - (x)))\n"                     , "-(x - x)\n"               },
			{-(x - 4),                  "(-((x)-(4)))"                 , "-(x-4)"             , "(-((x) - (4)))\n"                     , "-(x - 4)\n"               },
			{-(2 - x),                  "(-((2)-(x)))"                 , "-(2-x)"             , "(-((2) - (x)))\n"                     , "-(2 - x)\n"               },
			{-(2 - Con(4)),             "(-((2)-(4)))"                 , "-(2-4)"             , "(-((2) - (4)))\n"                     , "-(2 - 4)\n"               },
			{-(y * x),                  "(-((y)*(x)))"                 , "-(y*x)"             , "(-((y)*(x)))\n"                       , "-(y*x)\n"                 },
			{-(y * 5),                  "(-((y)*(5)))"                 , "-(y*5)"             , "(-((y)*(5)))\n"                       , "-(y*5)\n"                 },
			{-(2 * x),                  "(-((2)*(x)))"                 , "-(2*x)"             , "(-((2)*(x)))\n"                       , "-(2*x)\n"                 },
			{-(2 * Con(32775)),         "(-((2)*(32775)))"             , "-(2*32775)"         , "(-((2)*(32775)))\n"                   , "-(2*32775)\n"             },
			{-(y / y),                  "(-((y)/(y)))"                 , "-(y/y)"             , "   (y)  \n"
			                                                                                    "(-(ΔΔΔ))\n"
			                                                                                    "   (y)  \n"                           , " y\n"
			                                                                                                                             "-Δ\n"
			                                                                                         	                                 " y\n"                     },
			{-(y / 5),                  "(-((y)/(5)))"                 , "-(y/5)"             , "   (y)  \n"
			                                                                                    "(-(ΔΔΔ))\n"
			                                                                                    "   (5)  \n"                           , " y\n"
			                                                                                                                             "-Δ\n"
			                                                                                    		                                 " 5\n"                     },
			{-(2 / y),                  "(-((2)/(y)))"                 , "-(2/y)"             , "   (2)  \n"
			                                                                                    "(-(ΔΔΔ))\n"
			                                                                                    "   (y)  \n"                           , " 2\n"
			                                                                                                                             "-Δ\n"
			                                                                                    		                                 " y\n"                     },
			{-(2 / Con(32775)),         "(-((2)/(32775)))"             , "-(2/32775)"         , "     (2)    \n"
			                                                                                    "(-(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "   (32775)  \n"                       , "   2  \n"
			                                                                                                                             "-ΔΔΔΔΔ\n"
			                                                                                    		                                 " 32775\n"                 },
			{-(x ^ y),                  "(-((x)^(y)))"                 , "-x^y"               , "      (y)  \n"
			                                                                                    "(-((x)   ))\n"                        , "  y\n"
			                                                                                                                             "-x \n"                    }, // ^
			{-(x ^ 1),                  "(-((x)^(1)))"                 , "-x^1"               , "      (1)  \n"
			                                                                                    "(-((x)   ))\n"                        , "  1\n"
			                                                                                                                             "-x \n"                    }, // ^
			{-(2 ^ y),                  "(-((2)^(y)))"                 , "-2^y"               , "      (y)  \n"
			                                                                                    "(-((2)   ))\n"                        , "  y\n"
			                                                                                                                             "-2 \n"                    }, // ^
			{-(2 ^ Con(1)),             "(-((2)^(1)))"                 , "-2^1"               , "      (1)  \n"
			                                                                                    "(-((2)   ))\n"                        , "  1\n"
			                                                                                                                             "-2 \n"                    }, // ^

			{x           + +y,          "((x)+(+(y)))"                 , "x++y"               , "((x) + (+(y)))\n"                     , "x + +y\n"                 },
			{x           + -y,          "((x)+(-(y)))"                 , "x+-y"               , "((x) + (-(y)))\n"                     , "x + -y\n"                 },
			{x           + (y + 25377), "((x)+((y)+(25377)))"          , "x+(y+25377)"        , "((x) + ((y) + (25377)))\n"            , "x + (y + 25377)\n"        },
			{x           + (y - 25377), "((x)+((y)-(25377)))"          , "x+(y-25377)"        , "((x) + ((y) - (25377)))\n"            , "x + (y - 25377)\n"        },
			{x           + (y * 25377), "((x)+((y)*(25377)))"          , "x+y*25377"          , "((x) + ((y)*(25377)))\n"              , "x + y*25377\n"            },
			{x           + (y / 25377), "((x)+((y)/(25377)))"          , "x+y/25377"          , "          (y)    \n"
			                                                                                    "((x) + (ΔΔΔΔΔΔΔ))\n"
                                                                                                "        (25377)  \n"                  , "      y  \n"
			                                                                                                                             "x + ΔΔΔΔΔ\n"
                                                                                                                                         "    25377\n"              },
			{x           + (y ^ 25377), "((x)+((y)^(25377)))"          , "x+y^25377"          , "           (25377)  \n"
			                                                                                    "((x) + ((y)       ))\n"               , "     25377\n"
																								                                         "x + y     \n"             },
			{+y          + x,           "((+(y))+(x))"                 , "+y+x"               , "((+(y)) + (x))\n"                     , "+y + x\n"                 },
			{-y          + x,           "((-(y))+(x))"                 , "-y+x"               , "((-(y)) + (x))\n"                     , "-y + x\n"                 },
			{(y + 25377) + x,           "(((y)+(25377))+(x))"          , "y+25377+x"          , "(((y) + (25377)) + (x))\n"            , "y + 25377 + x\n"          },
			{(y - 25377) + x,           "(((y)-(25377))+(x))"          , "y-25377+x"          , "(((y) - (25377)) + (x))\n"            , "y - 25377 + x\n"          },
			{(y * 25377) + x,           "(((y)*(25377))+(x))"          , "y*25377+x"          , "(((y)*(25377)) + (x))\n"              , "y*25377 + x\n"            },
			{(y / 25377) + x,           "(((y)/(25377))+(x))"          , "y/25377+x"          , "    (y)          \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + (x))\n"
                                                                                                "  (25377)        \n"                  , "  y      \n"
																															             "ΔΔΔΔΔ + x\n"
																															             "25377    \n"              },
			{(y ^ 25377) + x,           "(((y)^(25377))+(x))"          , "y^25377+x"          , "     (25377)        \n"
			                                                                                    "(((y)       ) + (x))\n"               , " 25377    \n"
																								                                         "y      + x\n"             },
			{x           - +y,          "((x)-(+(y)))"                 , "x-+y"               , "((x) - (+(y)))\n"                     , "x - +y\n"                 },
			{x           - -y,          "((x)-(-(y)))"                 , "x--y"               , "((x) - (-(y)))\n"                     , "x - -y\n"                 },
			{x           - (y + 25377), "((x)-((y)+(25377)))"          , "x-(y+25377)"        , "((x) - ((y) + (25377)))\n"            , "x - (y + 25377)\n"        },
			{x           - (y - 25377), "((x)-((y)-(25377)))"          , "x-(y-25377)"        , "((x) - ((y) - (25377)))\n"            , "x - (y - 25377)\n"        },
			{x           - (y * 25377), "((x)-((y)*(25377)))"          , "x-y*25377"          , "((x) - ((y)*(25377)))\n"              , "x - y*25377\n"            },
			{x           - (y / 25377), "((x)-((y)/(25377)))"          , "x-y/25377"          , "          (y)    \n"
			                                                                                    "((x) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "        (25377)  \n"                  , "      y  \n"
			                                                                                                                             "x - ΔΔΔΔΔ\n"
			                                                                                                                             "    25377\n"              },
			{x           - (y ^ 25377), "((x)-((y)^(25377)))"          , "x-y^25377"          , "           (25377)  \n"
			                                                                                    "((x) - ((y)       ))\n"               , "     25377\n"
			                                                                                                                             "x - y     \n"             },
			{+y          - x,           "((+(y))-(x))"                 , "+y-x"               , "((+(y)) - (x))\n"                     , "+y - x\n"                 },
			{-y          - x,           "((-(y))-(x))"                 , "-y-x"               , "((-(y)) - (x))\n"                     , "-y - x\n"                 },
			{(y + 25377) - x,           "(((y)+(25377))-(x))"          , "y+25377-x"          , "(((y) + (25377)) - (x))\n"            , "y + 25377 - x\n"          },
			{(y - 25377) - x,           "(((y)-(25377))-(x))"          , "y-25377-x"          , "(((y) - (25377)) - (x))\n"            , "y - 25377 - x\n"          },
			{(y * 25377) - x,           "(((y)*(25377))-(x))"          , "y*25377-x"          , "(((y)*(25377)) - (x))\n"              , "y*25377 - x\n"            },
			{(y / 25377) - x,           "(((y)/(25377))-(x))"          , "y/25377-x"          , "    (y)          \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - (x))\n"
			                                                                                    "  (25377)        \n"                  , "  y      \n"
			                                                                                    							             "ΔΔΔΔΔ - x\n"
			                                                                                    							             "25377    \n"              },
			{(y ^ 25377) - x,           "(((y)^(25377))-(x))"          , "y^25377-x"          , "     (25377)        \n"
			                                                                                    "(((y)       ) - (x))\n"               , " 25377    \n"
			                                                                                                                             "y      - x\n"             },
			{x           * +y,          "((x)*(+(y)))"                 , "x*+y"               , "((x)*(+(y)))\n"                       , "x*+y\n"                   },
			{x           * -y,          "((x)*(-(y)))"                 , "x*-y"               , "((x)*(-(y)))\n"                       , "x*-y\n"                   },
			{x           * (y + 25377), "((x)*((y)+(25377)))"          , "x*(y+25377)"        , "((x)*((y) + (25377)))\n"              , "x*(y + 25377)\n"          },
			{x           * (y - 25377), "((x)*((y)-(25377)))"          , "x*(y-25377)"        , "((x)*((y) - (25377)))\n"              , "x*(y - 25377)\n"          },
			{x           * (y * 25377), "((x)*((y)*(25377)))"          , "x*(y*25377)"        , "((x)*((y)*(25377)))\n"                , "x*(y*25377)\n"            },
			{x           * (y / 25377), "((x)*((y)/(25377)))"          , "x*(y/25377)"        , "        (y)    \n"
			                                                                                    "((x)*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "      (25377)  \n"                    , "    y  \n"
			                                                                                                                             "x*ΔΔΔΔΔ\n"
			                                                                                                                             "  25377\n"                },
			{x           * (y ^ 25377), "((x)*((y)^(25377)))"          , "x*y^25377"          , "         (25377)  \n"
			                                                                                    "((x)*((y)       ))\n"                 , "   25377\n"
			                                                                                                                             "x*y     \n"               },
			{+y          * x,           "((+(y))*(x))"                 , "+y*x"               , "((+(y))*(x))\n"                       , "+y*x\n"                   },
			{-y          * x,           "((-(y))*(x))"                 , "-y*x"               , "((-(y))*(x))\n"                       , "-y*x\n"                   },
			{(y + 25377) * x,           "(((y)+(25377))*(x))"          , "(y+25377)*x"        , "(((y) + (25377))*(x))\n"              , "(y + 25377)*x\n"          },
			{(y - 25377) * x,           "(((y)-(25377))*(x))"          , "(y-25377)*x"        , "(((y) - (25377))*(x))\n"              , "(y - 25377)*x\n"          },
			{(y * 25377) * x,           "(((y)*(25377))*(x))"          , "y*25377*x"          , "(((y)*(25377))*(x))\n"                , "y*25377*x\n"              },
			{(y / 25377) * x,           "(((y)/(25377))*(x))"          , "y/25377*x"          , "    (y)        \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*(x))\n"
			                                                                                    "  (25377)      \n"                    , "  y    \n"
			                                                                                    					                     "ΔΔΔΔΔ*x\n"
			                                                                                    						                 "25377  \n"                },
			{(y ^ 25377) * x,           "(((y)^(25377))*(x))"          , "y^25377*x"          , "     (25377)      \n"
			                                                                                    "(((y)       )*(x))\n"                 , " 25377  \n"
			                                                                                                                             "y     *x\n"               },
			{x           / +y,          "((x)/(+(y)))"                 , "x/+y"               , "   (x)  \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								" (+(y)) \n"                           , " x\n"
			                                                                                                                             "ΔΔ\n"
																					                               	                     "+y\n"                     },
			{x           / -y,          "((x)/(-(y)))"                 , "x/-y"               , "   (x)  \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								" (-(y)) \n"                           , " x\n"
			                                                                                                                             "ΔΔ\n"
																					                               	                     "-y\n"                     },
			{x           / (y + 25377), "((x)/((y)+(25377)))"          , "x/(y+25377)"        , "       (x)       \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) + (25377)) \n"                  , "    x    \n"
                                                                                                                                         "ΔΔΔΔΔΔΔΔΔ\n"
                                                                                                                                         "y + 25377\n"              },
			{x           / (y - 25377), "((x)/((y)-(25377)))"          , "x/(y-25377)"        , "       (x)       \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) - (25377)) \n"                  , "    x    \n"
                                                                                                                                         "ΔΔΔΔΔΔΔΔΔ\n"
                                                                                                                                         "y - 25377\n"              },
			{x           / (y * 25377), "((x)/((y)*(25377)))"          , "x/(y*25377)"        , "      (x)      \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y)*(25377)) \n"                    , "   x   \n"
                                                                                                                                         "ΔΔΔΔΔΔΔ\n"
                                                                                                                                         "y*25377\n"                },
			{x           / (y / 25377), "((x)/((y)/(25377)))"          , "x/(y/25377)"        , "    (x)    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
																								"    (y)    \n"
																								" (ΔΔΔΔΔΔΔ) \n"
																								"  (25377)  \n"                        , "   x   \n"
                                                                                                                                         "ΔΔΔΔΔΔΔ\n"
																														                 "   y   \n"
																														                 " ΔΔΔΔΔ \n"
																														                 " 25377 \n"                },
			{x           / (y ^ 25377), "((x)/((y)^(25377)))"          , "x/y^25377"          , "      (x)     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"     (25377)  \n"
																								" ((y)       ) \n"                     , "   x  \n"
                                                                                                                                         "ΔΔΔΔΔΔ\n"
																														                 " 25377\n"
																														                 "y     \n"                 },
			{+y          / x,           "((+(y))/(x))"                 , "+y/x"               , " (+(y)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								"   (x)  \n"                           , "+y\n"
			                                                                                                                             "ΔΔ\n"
																					                               	                     " x\n"                     },
			{-y          / x,           "((-(y))/(x))"                 , "-y/x"               , " (-(y)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								"   (x)  \n"                           , "-y\n"
			                                                                                                                             "ΔΔ\n"
																					                                                     " x\n"                     },
			{(y + 25377) / x,           "(((y)+(25377))/(x))"          , "(y+25377)/x"        , " ((y) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (x)       \n"                  , "y + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                                                             "    x    \n"              },
			{(y - 25377) / x,           "(((y)-(25377))/(x))"          , "(y-25377)/x"        , " ((y) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (x)       \n"                  , "y - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                                                             "    x    \n"              },
			{(y * 25377) / x,           "(((y)*(25377))/(x))"          , "y*25377/x"          , " ((y)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (x)      \n"                    , "y*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                                                             "   x   \n"                },
			{(y / 25377) / x,           "(((y)/(25377))/(x))"          , "y/25377/x"          , "    (y)    \n"
			                                                                                    " (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "  (25377)  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "    (x)    \n"                        , "   y   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "   x   \n"                },
			{(y ^ 25377) / x,           "(((y)^(25377))/(x))"          , "y^25377/x"          , "     (25377)  \n"
			                                                                                    " ((y)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"      (x)     \n"                     , " 25377\n"
			                                                                                    						                 "y     \n"
			                                                                                                                             "ΔΔΔΔΔΔ\n"
			                                                                                    						                 "   x  \n"                 },
			{x           ^ +y,          "((x)^(+(y)))"                 , "x^(+y)"             , "    (+(y)) \n"
			                                                                                    "((x)      )\n"                        , " +y\n"
																								                                         "x  \n"                    }, // ^
			{x           ^ -y,          "((x)^(-(y)))"                 , "x^(-y)"             , "    (-(y)) \n"
			                                                                                    "((x)      )\n"                        , " -y\n"
																								                                         "x  \n"                    }, // ^
			{x           ^ (y + 25377), "((x)^((y)+(25377)))"          , "x^(y+25377)"        , "    ((y) + (25377)) \n"
			                                                                                    "((x)               )\n"               , " y + 25377\n"
																								                                         "x         \n"             }, // ^
			{x           ^ (y - 25377), "((x)^((y)-(25377)))"          , "x^(y-25377)"        , "    ((y) - (25377)) \n"
			                                                                                    "((x)               )\n"               , " y - 25377\n"
																								                                         "x         \n"             }, // ^
			{x           ^ (y * 25377), "((x)^((y)*(25377)))"          , "x^(y*25377)"        , "    ((y)*(25377)) \n"
			                                                                                    "((x)             )\n"                 , " y*25377\n"
																								                                         "x       \n"               }, // ^
			{x           ^ (y / 25377), "((x)^((y)/(25377)))"          , "x^(y/25377)"        , "       (y)    \n"
			                                                                                    "    (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "     (25377)  \n"
			                                                                                    "((x)         )\n"                     , "   y  \n"
																								                                         " ΔΔΔΔΔ\n"
																								                                         " 25377\n"
																								                                         "x     \n"                 },
			{x           ^ (y ^ 25377), "((x)^((y)^(25377)))"          , "x^y^25377"          , "        (25377)  \n"
			                                                                                    "    ((y)       ) \n"
			                                                                                    "((x)            )\n"                  , "  25377\n"
																								                                         " y     \n"
																								                                         "x      \n"                },
			{+y          ^ x,           "((+(y))^(x))"                 , "(+y)^x"             , "       (x) \n"
			                                                                                    "((+(y))   )\n"                        , "    x\n"
			                                                                                                                             "(+y) \n"                  },
			{-y          ^ x,           "((-(y))^(x))"                 , "(-y)^x"             , "       (x) \n"
			                                                                                    "((-(y))   )\n"                        , "    x\n"
			                                                                                                                             "(-y) \n"                  },
			{(y + 25377) ^ x,           "(((y)+(25377))^(x))"          , "(y+25377)^x"        , "                (x) \n"
			                                                                                    "(((y) + (25377))   )\n"               , "           x\n"
			                                                                                                                             "(y + 25377) \n"           }, // ^
			{(y - 25377) ^ x,           "(((y)-(25377))^(x))"          , "(y-25377)^x"        , "                (x) \n"
			                                                                                    "(((y) - (25377))   )\n"               , "           x\n"
			                                                                                                                             "(y - 25377) \n"           }, // ^
			{(y * 25377) ^ x,           "(((y)*(25377))^(x))"          , "(y*25377)^x"        , "              (x) \n"
			                                                                                    "(((y)*(25377))   )\n"                 , "         x\n"
			                                                                                                                             "(y*25377) \n"             }, // ^
			{(y / 25377) ^ x,           "(((y)/(25377))^(x))"          , "(y/25377)^x"        , "          (x) \n"
			                                                                                    "    (y)       \n"
																								"((ΔΔΔΔΔΔΔ)   )\n"
																								"  (25377)     \n"                     , "     x\n"
			                                                                                                                             "  y   \n"
																								                                         "ΔΔΔΔΔ \n"
																								                                         "25377 \n"                 }, // /,^
			{(y ^ 25377) ^ x,           "(((y)^(25377))^(x))"          , "(y^25377)^x"        , "             (x) \n"
			                                                                                    "     (25377)     \n"
																								"(((y)       )   )\n"                  , "        x\n"
			                                                                                                                             "  25377  \n"
																							                             	             "(y     ) \n"              }, // ^

			{+x          + +y,          "((+(x))+(+(y)))"              , "+x++y"              , "((+(x)) + (+(y)))\n"                  , "+x + +y\n"                },
			{+x          + -y,          "((+(x))+(-(y)))"              , "+x+-y"              , "((+(x)) + (-(y)))\n"                  , "+x + -y\n"                },
			{+x          + (y + 25377), "((+(x))+((y)+(25377)))"       , "+x+(y+25377)"       , "((+(x)) + ((y) + (25377)))\n"         , "+x + (y + 25377)\n"       },
			{+x          + (y - 25377), "((+(x))+((y)-(25377)))"       , "+x+(y-25377)"       , "((+(x)) + ((y) - (25377)))\n"         , "+x + (y - 25377)\n"       },
			{+x          + (y * 25377), "((+(x))+((y)*(25377)))"       , "+x+y*25377"         , "((+(x)) + ((y)*(25377)))\n"           , "+x + y*25377\n"           },
			{+x          + (y / 25377), "((+(x))+((y)/(25377)))"       , "+x+y/25377"         , "             (y)    \n"
                                                                                                "((+(x)) + (ΔΔΔΔΔΔΔ))\n"
                                                                                                "           (25377)  \n"               , "       y  \n"
                                                                                                                                         "+x + ΔΔΔΔΔ\n"
                                                                                                                                         "     25377\n"             },
			{+x          + (y ^ 25377), "((+(x))+((y)^(25377)))"       , "+x+y^25377"         , "              (25377)  \n"
                                                                                                "((+(x)) + ((y)       ))\n"            , "      25377\n"
                                                                                                                                         "+x + y     \n"            },
			{-x          + +y,          "((-(x))+(+(y)))"              , "-x++y"              , "((-(x)) + (+(y)))\n"                  , "-x + +y\n"                },
			{-x          + -y,          "((-(x))+(-(y)))"              , "-x+-y"              , "((-(x)) + (-(y)))\n"                  , "-x + -y\n"                },
			{-x          + (y + 25377), "((-(x))+((y)+(25377)))"       , "-x+(y+25377)"       , "((-(x)) + ((y) + (25377)))\n"         , "-x + (y + 25377)\n"       },
			{-x          + (y - 25377), "((-(x))+((y)-(25377)))"       , "-x+(y-25377)"       , "((-(x)) + ((y) - (25377)))\n"         , "-x + (y - 25377)\n"       },
			{-x          + (y * 25377), "((-(x))+((y)*(25377)))"       , "-x+y*25377"         , "((-(x)) + ((y)*(25377)))\n"           , "-x + y*25377\n"           },
			{-x          + (y / 25377), "((-(x))+((y)/(25377)))"       , "-x+y/25377"         , "             (y)    \n"
			                                                                                    "((-(x)) + (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "           (25377)  \n"               , "       y  \n"
			                                                                                                                             "-x + ΔΔΔΔΔ\n"
			                                                                                                                             "     25377\n"             },
			{-x          + (y ^ 25377), "((-(x))+((y)^(25377)))"       , "-x+y^25377"         , "              (25377)  \n"
			                                                                                    "((-(x)) + ((y)       ))\n"            , "      25377\n"
			                                                                                                                             "-x + y     \n"            },
			{(x + 25377) + +y,          "(((x)+(25377))+(+(y)))"       , "x+25377++y"         , "(((x) + (25377)) + (+(y)))\n"         , "x + 25377 + +y\n"         },
			{(x + 25377) + -y,          "(((x)+(25377))+(-(y)))"       , "x+25377+-y"         , "(((x) + (25377)) + (-(y)))\n"         , "x + 25377 + -y\n"         },
			{(x + 25377) + (y + 25377), "(((x)+(25377))+((y)+(25377)))", "x+25377+(y+25377)"  , "(((x) + (25377)) + ((y) + (25377)))\n", "x + 25377 + (y + 25377)\n"},
			{(x + 25377) + (y - 25377), "(((x)+(25377))+((y)-(25377)))", "x+25377+(y-25377)"  , "(((x) + (25377)) + ((y) - (25377)))\n", "x + 25377 + (y - 25377)\n"},
			{(x + 25377) + (y * 25377), "(((x)+(25377))+((y)*(25377)))", "x+25377+y*25377"    , "(((x) + (25377)) + ((y)*(25377)))\n"  , "x + 25377 + y*25377\n"    },
			{(x + 25377) + (y / 25377), "(((x)+(25377))+((y)/(25377)))", "x+25377+y/25377"    , "                      (y)    \n"
			                                                                                    "(((x) + (25377)) + (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                    (25377)  \n"      , "              y  \n"
			                                                                                                                             "x + 25377 + ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x + 25377) + (y ^ 25377), "(((x)+(25377))+((y)^(25377)))", "x+25377+y^25377"    , "                       (25377)  \n"
			                                                                                    "(((x) + (25377)) + ((y)       ))\n"   , "             25377\n"
			                                                                                                                             "x + 25377 + y     \n"     },
			{(x - 25377) + +y,          "(((x)-(25377))+(+(y)))"       , "x-25377++y"         , "(((x) - (25377)) + (+(y)))\n"         , "x - 25377 + +y\n"         },
			{(x - 25377) + -y,          "(((x)-(25377))+(-(y)))"       , "x-25377+-y"         , "(((x) - (25377)) + (-(y)))\n"         , "x - 25377 + -y\n"         },
			{(x - 25377) + (y + 25377), "(((x)-(25377))+((y)+(25377)))", "x-25377+(y+25377)"  , "(((x) - (25377)) + ((y) + (25377)))\n", "x - 25377 + (y + 25377)\n"},
			{(x - 25377) + (y - 25377), "(((x)-(25377))+((y)-(25377)))", "x-25377+(y-25377)"  , "(((x) - (25377)) + ((y) - (25377)))\n", "x - 25377 + (y - 25377)\n"},
			{(x - 25377) + (y * 25377), "(((x)-(25377))+((y)*(25377)))", "x-25377+y*25377"    , "(((x) - (25377)) + ((y)*(25377)))\n"  , "x - 25377 + y*25377\n"    },
			{(x - 25377) + (y / 25377), "(((x)-(25377))+((y)/(25377)))", "x-25377+y/25377"    , "                      (y)    \n"
			                                                                                    "(((x) - (25377)) + (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                    (25377)  \n"      , "              y  \n"
			                                                                                                                             "x - 25377 + ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x - 25377) + (y ^ 25377), "(((x)-(25377))+((y)^(25377)))", "x-25377+y^25377"    , "                       (25377)  \n"
			                                                                                    "(((x) - (25377)) + ((y)       ))\n"   , "             25377\n"
			                                                                                                                             "x - 25377 + y     \n"     },
			{(x * 25377) + +y,          "(((x)*(25377))+(+(y)))"       , "x*25377++y"         , "(((x)*(25377)) + (+(y)))\n"           , "x*25377 + +y\n"           },
			{(x * 25377) + -y,          "(((x)*(25377))+(-(y)))"       , "x*25377+-y"         , "(((x)*(25377)) + (-(y)))\n"           , "x*25377 + -y\n"           },
			{(x * 25377) + (y + 25377), "(((x)*(25377))+((y)+(25377)))", "x*25377+(y+25377)"  , "(((x)*(25377)) + ((y) + (25377)))\n"  , "x*25377 + (y + 25377)\n"  },
			{(x * 25377) + (y - 25377), "(((x)*(25377))+((y)-(25377)))", "x*25377+(y-25377)"  , "(((x)*(25377)) + ((y) - (25377)))\n"  , "x*25377 + (y - 25377)\n"  },
			{(x * 25377) + (y * 25377), "(((x)*(25377))+((y)*(25377)))", "x*25377+y*25377"    , "(((x)*(25377)) + ((y)*(25377)))\n"    , "x*25377 + y*25377\n"      },
			{(x * 25377) + (y / 25377), "(((x)*(25377))+((y)/(25377)))", "x*25377+y/25377"    , "                    (y)    \n"
			                                                                                    "(((x)*(25377)) + (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                  (25377)  \n"        , "            y  \n"
			                                                                                                                             "x*25377 + ΔΔΔΔΔ\n"
			                                                                                                                             "          25377\n"        },
			{(x * 25377) + (y ^ 25377), "(((x)*(25377))+((y)^(25377)))", "x*25377+y^25377"    , "                     (25377)  \n"
			                                                                                    "(((x)*(25377)) + ((y)       ))\n"     , "           25377\n"
			                                                                                                                             "x*25377 + y     \n"       },
			{(x / 25377) + +y,          "(((x)/(25377))+(+(y)))"       , "x/25377++y"         , "    (x)             \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + (+(y)))\n"
			                                                                                    "  (25377)           \n"               , "  x       \n"
																								                                         "ΔΔΔΔΔ + +y\n"
                                                                                                                                         "25377     \n"             },
			{(x / 25377) + -y,          "(((x)/(25377))+(-(y)))"       , "x/25377+-y"         , "    (x)             \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + (-(y)))\n"
			                                                                                    "  (25377)           \n"               , "  x       \n"
																								                                         "ΔΔΔΔΔ + -y\n"
                                                                                                                                         "25377     \n"             },
			{(x / 25377) + (y + 25377), "(((x)/(25377))+((y)+(25377)))", "x/25377+(y+25377)"  , "    (x)                      \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + ((y) + (25377)))\n"
			                                                                                    "  (25377)                    \n"      , "  x                \n"
																								                                         "ΔΔΔΔΔ + (y + 25377)\n"
                                                                                                                                         "25377              \n"    },
			{(x / 25377) + (y - 25377), "(((x)/(25377))+((y)-(25377)))", "x/25377+(y-25377)"  , "    (x)                      \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + ((y) - (25377)))\n"
			                                                                                    "  (25377)                    \n"      , "  x                \n"
																								                                         "ΔΔΔΔΔ + (y - 25377)\n"
                                                                                                                                         "25377              \n"    },
			{(x / 25377) + (y * 25377), "(((x)/(25377))+((y)*(25377)))", "x/25377+y*25377"    , "    (x)                    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + ((y)*(25377)))\n"
			                                                                                    "  (25377)                  \n"        , "  x            \n"
																								                                         "ΔΔΔΔΔ + y*25377\n"
                                                                                                                                         "25377          \n"        },
			{(x / 25377) + (y / 25377), "(((x)/(25377))+((y)/(25377)))", "x/25377+y/25377"    , "    (x)         (y)    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "  (25377)     (25377)  \n"            , "  x       y  \n"
																								                                         "ΔΔΔΔΔ + ΔΔΔΔΔ\n"
                                                                                                                                         "25377   25377\n"          },
			{(x / 25377) + (y ^ 25377), "(((x)/(25377))+((y)^(25377)))", "x/25377+y^25377"    , "    (x)          (25377)  \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) + ((y)       ))\n"
			                                                                                    "  (25377)                 \n"         , "  x      25377\n"
																								                                         "ΔΔΔΔΔ + y     \n"
                                                                                                                                         "25377         \n"         },
			{(x ^ 25377) + +y,          "(((x)^(25377))+(+(y)))"       , "x^25377++y"         , "     (25377)           \n"
			                                                                                    "(((x)       ) + (+(y)))\n"            , " 25377     \n"
																								                                         "x      + +y\n"            },
			{(x ^ 25377) + -y,          "(((x)^(25377))+(-(y)))"       , "x^25377+-y"         , "     (25377)           \n"
			                                                                                    "(((x)       ) + (-(y)))\n"            , " 25377     \n"
																								                                         "x      + -y\n"            },
			{(x ^ 25377) + (y + 25377), "(((x)^(25377))+((y)+(25377)))", "x^25377+(y+25377)"  , "     (25377)                    \n"
			                                                                                    "(((x)       ) + ((y) + (25377)))\n"   , " 25377              \n"
																								                                         "x      + (y + 25377)\n"   },
			{(x ^ 25377) + (y - 25377), "(((x)^(25377))+((y)-(25377)))", "x^25377+(y-25377)"  , "     (25377)                    \n"
			                                                                                    "(((x)       ) + ((y) - (25377)))\n"   , " 25377              \n"
																								                                         "x      + (y - 25377)\n"   },
			{(x ^ 25377) + (y * 25377), "(((x)^(25377))+((y)*(25377)))", "x^25377+y*25377"    , "     (25377)                  \n"
			                                                                                    "(((x)       ) + ((y)*(25377)))\n"     , " 25377          \n"
																								                                         "x      + y*25377\n"       },
			{(x ^ 25377) + (y / 25377), "(((x)^(25377))+((y)/(25377)))", "x^25377+y/25377"    , "     (25377)       (y)    \n"
			                                                                                    "(((x)       ) + (ΔΔΔΔΔΔΔ))\n"
																								"                 (25377)  \n"         , " 25377     y  \n"
																								                                         "x      + ΔΔΔΔΔ\n"
                                                                                    													 "         25377\n"         },
			{(x ^ 25377) + (y ^ 25377), "(((x)^(25377))+((y)^(25377)))", "x^25377+y^25377"    , "     (25377)        (25377)  \n"
			                                                                                    "(((x)       ) + ((y)       ))\n"      , " 25377    25377\n"
																								                                         "x      + y     \n"        },

			{+x          - +y,          "((+(x))-(+(y)))"              , "+x-+y"              , "((+(x)) - (+(y)))\n"                  , "+x - +y\n"                },
			{+x          - -y,          "((+(x))-(-(y)))"              , "+x--y"              , "((+(x)) - (-(y)))\n"                  , "+x - -y\n"                },
			{+x          - (y + 25377), "((+(x))-((y)+(25377)))"       , "+x-(y+25377)"       , "((+(x)) - ((y) + (25377)))\n"         , "+x - (y + 25377)\n"       },
			{+x          - (y - 25377), "((+(x))-((y)-(25377)))"       , "+x-(y-25377)"       , "((+(x)) - ((y) - (25377)))\n"         , "+x - (y - 25377)\n"       },
			{+x          - (y * 25377), "((+(x))-((y)*(25377)))"       , "+x-y*25377"         , "((+(x)) - ((y)*(25377)))\n"           , "+x - y*25377\n"           },
			{+x          - (y / 25377), "((+(x))-((y)/(25377)))"       , "+x-y/25377"         , "             (y)    \n"
			                                                                                    "((+(x)) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "           (25377)  \n"               , "       y  \n"
			                                                                                                                             "+x - ΔΔΔΔΔ\n"
			                                                                                                                             "     25377\n"             },
			{+x          - (y ^ 25377), "((+(x))-((y)^(25377)))"       , "+x-y^25377"         , "              (25377)  \n"
			                                                                                    "((+(x)) - ((y)       ))\n"            , "      25377\n"
			                                                                                                                             "+x - y     \n"            },
			{-x          - +y,          "((-(x))-(+(y)))"              , "-x-+y"              , "((-(x)) - (+(y)))\n"                  , "-x - +y\n"                },
			{-x          - -y,          "((-(x))-(-(y)))"              , "-x--y"              , "((-(x)) - (-(y)))\n"                  , "-x - -y\n"                },
			{-x          - (y + 25377), "((-(x))-((y)+(25377)))"       , "-x-(y+25377)"       , "((-(x)) - ((y) + (25377)))\n"         , "-x - (y + 25377)\n"       },
			{-x          - (y - 25377), "((-(x))-((y)-(25377)))"       , "-x-(y-25377)"       , "((-(x)) - ((y) - (25377)))\n"         , "-x - (y - 25377)\n"       },
			{-x          - (y * 25377), "((-(x))-((y)*(25377)))"       , "-x-y*25377"         , "((-(x)) - ((y)*(25377)))\n"           , "-x - y*25377\n"           },
			{-x          - (y / 25377), "((-(x))-((y)/(25377)))"       , "-x-y/25377"         , "             (y)    \n"
			                                                                                    "((-(x)) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "           (25377)  \n"               , "       y  \n"
			                                                                                                                             "-x - ΔΔΔΔΔ\n"
			                                                                                                                             "     25377\n"             },
			{-x          - (y ^ 25377), "((-(x))-((y)^(25377)))"       , "-x-y^25377"         , "              (25377)  \n"
			                                                                                    "((-(x)) - ((y)       ))\n"            , "      25377\n"
			                                                                                                                             "-x - y     \n"            },
			{(x + 25377) - +y,          "(((x)+(25377))-(+(y)))"       , "x+25377-+y"         , "(((x) + (25377)) - (+(y)))\n"         , "x + 25377 - +y\n"         },
			{(x + 25377) - -y,          "(((x)+(25377))-(-(y)))"       , "x+25377--y"         , "(((x) + (25377)) - (-(y)))\n"         , "x + 25377 - -y\n"         },
			{(x + 25377) - (y + 25377), "(((x)+(25377))-((y)+(25377)))", "x+25377-(y+25377)"  , "(((x) + (25377)) - ((y) + (25377)))\n", "x + 25377 - (y + 25377)\n"},
			{(x + 25377) - (y - 25377), "(((x)+(25377))-((y)-(25377)))", "x+25377-(y-25377)"  , "(((x) + (25377)) - ((y) - (25377)))\n", "x + 25377 - (y - 25377)\n"},
			{(x + 25377) - (y * 25377), "(((x)+(25377))-((y)*(25377)))", "x+25377-y*25377"    , "(((x) + (25377)) - ((y)*(25377)))\n"  , "x + 25377 - y*25377\n"    },
			{(x + 25377) - (y / 25377), "(((x)+(25377))-((y)/(25377)))", "x+25377-y/25377"    , "                      (y)    \n"
			                                                                                    "(((x) + (25377)) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                    (25377)  \n"      , "              y  \n"
			                                                                                                                             "x + 25377 - ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x + 25377) - (y ^ 25377), "(((x)+(25377))-((y)^(25377)))", "x+25377-y^25377"    , "                       (25377)  \n"
			                                                                                    "(((x) + (25377)) - ((y)       ))\n"   , "             25377\n"
			                                                                                                                             "x + 25377 - y     \n"     },
			{(x - 25377) - +y,          "(((x)-(25377))-(+(y)))"       , "x-25377-+y"         , "(((x) - (25377)) - (+(y)))\n"         , "x - 25377 - +y\n"         },
			{(x - 25377) - -y,          "(((x)-(25377))-(-(y)))"       , "x-25377--y"         , "(((x) - (25377)) - (-(y)))\n"         , "x - 25377 - -y\n"         },
			{(x - 25377) - (y + 25377), "(((x)-(25377))-((y)+(25377)))", "x-25377-(y+25377)"  , "(((x) - (25377)) - ((y) + (25377)))\n", "x - 25377 - (y + 25377)\n"},
			{(x - 25377) - (y - 25377), "(((x)-(25377))-((y)-(25377)))", "x-25377-(y-25377)"  , "(((x) - (25377)) - ((y) - (25377)))\n", "x - 25377 - (y - 25377)\n"},
			{(x - 25377) - (y * 25377), "(((x)-(25377))-((y)*(25377)))", "x-25377-y*25377"    , "(((x) - (25377)) - ((y)*(25377)))\n"  , "x - 25377 - y*25377\n"    },
			{(x - 25377) - (y / 25377), "(((x)-(25377))-((y)/(25377)))", "x-25377-y/25377"    , "                      (y)    \n"
			                                                                                    "(((x) - (25377)) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                    (25377)  \n"      , "              y  \n"
			                                                                                                                             "x - 25377 - ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x - 25377) - (y ^ 25377), "(((x)-(25377))-((y)^(25377)))", "x-25377-y^25377"    , "                       (25377)  \n"
			                                                                                    "(((x) - (25377)) - ((y)       ))\n"   , "             25377\n"
			                                                                                                                             "x - 25377 - y     \n"     },
			{(x * 25377) - +y,          "(((x)*(25377))-(+(y)))"       , "x*25377-+y"         , "(((x)*(25377)) - (+(y)))\n"           , "x*25377 - +y\n"           },
			{(x * 25377) - -y,          "(((x)*(25377))-(-(y)))"       , "x*25377--y"         , "(((x)*(25377)) - (-(y)))\n"           , "x*25377 - -y\n"           },
			{(x * 25377) - (y + 25377), "(((x)*(25377))-((y)+(25377)))", "x*25377-(y+25377)"  , "(((x)*(25377)) - ((y) + (25377)))\n"  , "x*25377 - (y + 25377)\n"  },
			{(x * 25377) - (y - 25377), "(((x)*(25377))-((y)-(25377)))", "x*25377-(y-25377)"  , "(((x)*(25377)) - ((y) - (25377)))\n"  , "x*25377 - (y - 25377)\n"  },
			{(x * 25377) - (y * 25377), "(((x)*(25377))-((y)*(25377)))", "x*25377-y*25377"    , "(((x)*(25377)) - ((y)*(25377)))\n"    , "x*25377 - y*25377\n"      },
			{(x * 25377) - (y / 25377), "(((x)*(25377))-((y)/(25377)))", "x*25377-y/25377"    , "                    (y)    \n"
			                                                                                    "(((x)*(25377)) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                  (25377)  \n"        , "            y  \n"
			                                                                                                                             "x*25377 - ΔΔΔΔΔ\n"
			                                                                                                                             "          25377\n"        },
			{(x * 25377) - (y ^ 25377), "(((x)*(25377))-((y)^(25377)))", "x*25377-y^25377"    , "                     (25377)  \n"
			                                                                                    "(((x)*(25377)) - ((y)       ))\n"     , "           25377\n"
			                                                                                                                             "x*25377 - y     \n"       },
			{(x / 25377) - +y,          "(((x)/(25377))-(+(y)))"       , "x/25377-+y"         , "    (x)             \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - (+(y)))\n"
			                                                                                    "  (25377)           \n"               , "  x       \n"
																								                                         "ΔΔΔΔΔ - +y\n"
			                                                                                                                             "25377     \n"             },
			{(x / 25377) - -y,          "(((x)/(25377))-(-(y)))"       , "x/25377--y"         , "    (x)             \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - (-(y)))\n"
			                                                                                    "  (25377)           \n"               , "  x       \n"
																								                                         "ΔΔΔΔΔ - -y\n"
			                                                                                                                             "25377     \n"             },
			{(x / 25377) - (y + 25377), "(((x)/(25377))-((y)+(25377)))", "x/25377-(y+25377)"  , "    (x)                      \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - ((y) + (25377)))\n"
			                                                                                    "  (25377)                    \n"      , "  x                \n"
																								                                         "ΔΔΔΔΔ - (y + 25377)\n"
			                                                                                                                             "25377              \n"    },
			{(x / 25377) - (y - 25377), "(((x)/(25377))-((y)-(25377)))", "x/25377-(y-25377)"  , "    (x)                      \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - ((y) - (25377)))\n"
			                                                                                    "  (25377)                    \n"      , "  x                \n"
																								                                         "ΔΔΔΔΔ - (y - 25377)\n"
			                                                                                                                             "25377              \n"    },
			{(x / 25377) - (y * 25377), "(((x)/(25377))-((y)*(25377)))", "x/25377-y*25377"    , "    (x)                    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - ((y)*(25377)))\n"
			                                                                                    "  (25377)                  \n"        , "  x            \n"
																								                                         "ΔΔΔΔΔ - y*25377\n"
			                                                                                                                             "25377          \n"        },
			{(x / 25377) - (y / 25377), "(((x)/(25377))-((y)/(25377)))", "x/25377-y/25377"    , "    (x)         (y)    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - (ΔΔΔΔΔΔΔ))\n"
			                                                                                    "  (25377)     (25377)  \n"            , "  x       y  \n"
																								                                         "ΔΔΔΔΔ - ΔΔΔΔΔ\n"
			                                                                                                                             "25377   25377\n"          },
			{(x / 25377) - (y ^ 25377), "(((x)/(25377))-((y)^(25377)))", "x/25377-y^25377"    , "    (x)          (25377)  \n"
			                                                                                    "((ΔΔΔΔΔΔΔ) - ((y)       ))\n"
			                                                                                    "  (25377)                 \n"         , "  x      25377\n"
																								                                         "ΔΔΔΔΔ - y     \n"
			                                                                                                                             "25377         \n"         },
			{(x ^ 25377) - +y,          "(((x)^(25377))-(+(y)))"       , "x^25377-+y"         , "     (25377)           \n"
			                                                                                    "(((x)       ) - (+(y)))\n"            , " 25377     \n"
																								                                         "x      - +y\n"            },
			{(x ^ 25377) - -y,          "(((x)^(25377))-(-(y)))"       , "x^25377--y"         , "     (25377)           \n"
			                                                                                    "(((x)       ) - (-(y)))\n"            , " 25377     \n"
																								                                         "x      - -y\n"            },
			{(x ^ 25377) - (y + 25377), "(((x)^(25377))-((y)+(25377)))", "x^25377-(y+25377)"  , "     (25377)                    \n"
			                                                                                    "(((x)       ) - ((y) + (25377)))\n"   , " 25377              \n"
																								                                         "x      - (y + 25377)\n"   },
			{(x ^ 25377) - (y - 25377), "(((x)^(25377))-((y)-(25377)))", "x^25377-(y-25377)"  , "     (25377)                    \n"
			                                                                                    "(((x)       ) - ((y) - (25377)))\n"   , " 25377              \n"
																								                                         "x      - (y - 25377)\n"   },
			{(x ^ 25377) - (y * 25377), "(((x)^(25377))-((y)*(25377)))", "x^25377-y*25377"    , "     (25377)                  \n"
			                                                                                    "(((x)       ) - ((y)*(25377)))\n"     , " 25377          \n"
																								                                         "x      - y*25377\n"       },
			{(x ^ 25377) - (y / 25377), "(((x)^(25377))-((y)/(25377)))", "x^25377-y/25377"    , "     (25377)       (y)    \n"
			                                                                                    "(((x)       ) - (ΔΔΔΔΔΔΔ))\n"
																								"                 (25377)  \n"         , " 25377     y  \n"
																								                                         "x      - ΔΔΔΔΔ\n"
			                                                                        													 "         25377\n"         },
			{(x ^ 25377) - (y ^ 25377), "(((x)^(25377))-((y)^(25377)))", "x^25377-y^25377"    , "     (25377)        (25377)  \n"
			                                                                                    "(((x)       ) - ((y)       ))\n"      , " 25377    25377\n"
																								                                         "x      - y     \n"        },

			{+x          * +y,          "((+(x))*(+(y)))"              , "+x*+y"              , "((+(x))*(+(y)))\n"                    , "+x*+y\n"                  },
			{+x          * -y,          "((+(x))*(-(y)))"              , "+x*-y"              , "((+(x))*(-(y)))\n"                    , "+x*-y\n"                  },
			{+x          * (y + 25377), "((+(x))*((y)+(25377)))"       , "+x*(y+25377)"       , "((+(x))*((y) + (25377)))\n"           , "+x*(y + 25377)\n"         },
			{+x          * (y - 25377), "((+(x))*((y)-(25377)))"       , "+x*(y-25377)"       , "((+(x))*((y) - (25377)))\n"           , "+x*(y - 25377)\n"         },
			{+x          * (y * 25377), "((+(x))*((y)*(25377)))"       , "+x*(y*25377)"       , "((+(x))*((y)*(25377)))\n"             , "+x*(y*25377)\n"           },
			{+x          * (y / 25377), "((+(x))*((y)/(25377)))"       , "+x*(y/25377)"       , "           (y)    \n"
			                                                                                    "((+(x))*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "         (25377)  \n"                 , "     y  \n"
			                                                                                                                             "+x*ΔΔΔΔΔ\n"
			                                                                                                                             "   25377\n"               },
			{+x          * (y ^ 25377), "((+(x))*((y)^(25377)))"       , "+x*y^25377"         , "            (25377)  \n"
			                                                                                    "((+(x))*((y)       ))\n"              , "    25377\n"
			                                                                                                                             "+x*y     \n"              },
			{-x          * +y,          "((-(x))*(+(y)))"              , "-x*+y"              , "((-(x))*(+(y)))\n"                    , "-x*+y\n"                  },
			{-x          * -y,          "((-(x))*(-(y)))"              , "-x*-y"              , "((-(x))*(-(y)))\n"                    , "-x*-y\n"                  },
			{-x          * (y + 25377), "((-(x))*((y)+(25377)))"       , "-x*(y+25377)"       , "((-(x))*((y) + (25377)))\n"           , "-x*(y + 25377)\n"         },
			{-x          * (y - 25377), "((-(x))*((y)-(25377)))"       , "-x*(y-25377)"       , "((-(x))*((y) - (25377)))\n"           , "-x*(y - 25377)\n"         },
			{-x          * (y * 25377), "((-(x))*((y)*(25377)))"       , "-x*(y*25377)"       , "((-(x))*((y)*(25377)))\n"             , "-x*(y*25377)\n"           },
			{-x          * (y / 25377), "((-(x))*((y)/(25377)))"       , "-x*(y/25377)"       , "           (y)    \n"
			                                                                                    "((-(x))*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "         (25377)  \n"                 , "     y  \n"
			                                                                                                                             "-x*ΔΔΔΔΔ\n"
			                                                                                                                             "   25377\n"               },
			{-x          * (y ^ 25377), "((-(x))*((y)^(25377)))"       , "-x*y^25377"         , "            (25377)  \n"
			                                                                                    "((-(x))*((y)       ))\n"              , "    25377\n"
			                                                                                                                             "-x*y     \n"              },
			{(x + 25377) * +y,          "(((x)+(25377))*(+(y)))"       , "(x+25377)*+y"       , "(((x) + (25377))*(+(y)))\n"           , "(x + 25377)*+y\n"         },
			{(x + 25377) * -y,          "(((x)+(25377))*(-(y)))"       , "(x+25377)*-y"       , "(((x) + (25377))*(-(y)))\n"           , "(x + 25377)*-y\n"         },
			{(x + 25377) * (y + 25377), "(((x)+(25377))*((y)+(25377)))", "(x+25377)*(y+25377)", "(((x) + (25377))*((y) + (25377)))\n"  , "(x + 25377)*(y + 25377)\n"},
			{(x + 25377) * (y - 25377), "(((x)+(25377))*((y)-(25377)))", "(x+25377)*(y-25377)", "(((x) + (25377))*((y) - (25377)))\n"  , "(x + 25377)*(y - 25377)\n"},
			{(x + 25377) * (y * 25377), "(((x)+(25377))*((y)*(25377)))", "(x+25377)*(y*25377)", "(((x) + (25377))*((y)*(25377)))\n"    , "(x + 25377)*(y*25377)\n"  },
			{(x + 25377) * (y / 25377), "(((x)+(25377))*((y)/(25377)))", "(x+25377)*(y/25377)", "                    (y)    \n"
			                                                                                    "(((x) + (25377))*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                  (25377)  \n"        , "              y  \n"
			                                                                                                                             "(x + 25377)*ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x + 25377) * (y ^ 25377), "(((x)+(25377))*((y)^(25377)))", "(x+25377)*y^25377"  , "                     (25377)  \n"
			                                                                                    "(((x) + (25377))*((y)       ))\n"     , "             25377\n"
			                                                                                                                             "(x + 25377)*y     \n"     },
			{(x - 25377) * +y,          "(((x)-(25377))*(+(y)))"       , "(x-25377)*+y"       , "(((x) - (25377))*(+(y)))\n"           , "(x - 25377)*+y\n"         },
			{(x - 25377) * -y,          "(((x)-(25377))*(-(y)))"       , "(x-25377)*-y"       , "(((x) - (25377))*(-(y)))\n"           , "(x - 25377)*-y\n"         },
			{(x - 25377) * (y + 25377), "(((x)-(25377))*((y)+(25377)))", "(x-25377)*(y+25377)", "(((x) - (25377))*((y) + (25377)))\n"  , "(x - 25377)*(y + 25377)\n"},
			{(x - 25377) * (y - 25377), "(((x)-(25377))*((y)-(25377)))", "(x-25377)*(y-25377)", "(((x) - (25377))*((y) - (25377)))\n"  , "(x - 25377)*(y - 25377)\n"},
			{(x - 25377) * (y * 25377), "(((x)-(25377))*((y)*(25377)))", "(x-25377)*(y*25377)", "(((x) - (25377))*((y)*(25377)))\n"    , "(x - 25377)*(y*25377)\n"  },
			{(x - 25377) * (y / 25377), "(((x)-(25377))*((y)/(25377)))", "(x-25377)*(y/25377)", "                    (y)    \n"
			                                                                                    "(((x) - (25377))*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                  (25377)  \n"        , "              y  \n"
			                                                                                                                             "(x - 25377)*ΔΔΔΔΔ\n"
			                                                                                                                             "            25377\n"      },
			{(x - 25377) * (y ^ 25377), "(((x)-(25377))*((y)^(25377)))", "(x-25377)*y^25377"  , "                     (25377)  \n"
			                                                                                    "(((x) - (25377))*((y)       ))\n"     , "             25377\n"
			                                                                                                                             "(x - 25377)*y     \n"     },
			{(x * 25377) * +y,          "(((x)*(25377))*(+(y)))"       , "x*25377*+y"         , "(((x)*(25377))*(+(y)))\n"             , "x*25377*+y\n"             },
			{(x * 25377) * -y,          "(((x)*(25377))*(-(y)))"       , "x*25377*-y"         , "(((x)*(25377))*(-(y)))\n"             , "x*25377*-y\n"             },
			{(x * 25377) * (y + 25377), "(((x)*(25377))*((y)+(25377)))", "x*25377*(y+25377)"  , "(((x)*(25377))*((y) + (25377)))\n"    , "x*25377*(y + 25377)\n"    },
			{(x * 25377) * (y - 25377), "(((x)*(25377))*((y)-(25377)))", "x*25377*(y-25377)"  , "(((x)*(25377))*((y) - (25377)))\n"    , "x*25377*(y - 25377)\n"    },
			{(x * 25377) * (y * 25377), "(((x)*(25377))*((y)*(25377)))", "x*25377*(y*25377)"  , "(((x)*(25377))*((y)*(25377)))\n"      , "x*25377*(y*25377)\n"      },
			{(x * 25377) * (y / 25377), "(((x)*(25377))*((y)/(25377)))", "x*25377*(y/25377)"  , "                  (y)    \n"
			                                                                                    "(((x)*(25377))*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "                (25377)  \n"          , "          y  \n"
			                                                                                                                             "x*25377*ΔΔΔΔΔ\n"
			                                                                                                                             "        25377\n"          },
			{(x * 25377) * (y ^ 25377), "(((x)*(25377))*((y)^(25377)))", "x*25377*y^25377"    , "                   (25377)  \n"
			                                                                                    "(((x)*(25377))*((y)       ))\n"       , "         25377\n"
			                                                                                                                             "x*25377*y     \n"         },
			{(x / 25377) * +y,          "(((x)/(25377))*(+(y)))"       , "x/25377*+y"         , "    (x)           \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*(+(y)))\n"
			                                                                                    "  (25377)         \n"                 , "  x     \n"
			                                                                                                                             "ΔΔΔΔΔ*+y\n"
			                                                                                                                             "25377   \n"               },
			{(x / 25377) * -y,          "(((x)/(25377))*(-(y)))"       , "x/25377*-y"         , "    (x)           \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*(-(y)))\n"
			                                                                                    "  (25377)         \n"                 , "  x     \n"
			                                                                                                                             "ΔΔΔΔΔ*-y\n"
			                                                                                                                             "25377   \n"               },
			{(x / 25377) * (y + 25377), "(((x)/(25377))*((y)+(25377)))", "x/25377*(y+25377)"  , "    (x)                    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*((y) + (25377)))\n"
			                                                                                    "  (25377)                  \n"        , "  x              \n"
			                                                                                                                             "ΔΔΔΔΔ*(y + 25377)\n"
			                                                                                                                             "25377            \n"      },
			{(x / 25377) * (y - 25377), "(((x)/(25377))*((y)-(25377)))", "x/25377*(y-25377)"  , "    (x)                    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*((y) - (25377)))\n"
			                                                                                    "  (25377)                  \n"        , "  x              \n"
			                                                                                                                             "ΔΔΔΔΔ*(y - 25377)\n"
			                                                                                                                             "25377            \n"      },
			{(x / 25377) * (y * 25377), "(((x)/(25377))*((y)*(25377)))", "x/25377*(y*25377)"  , "    (x)                  \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*((y)*(25377)))\n"
			                                                                                    "  (25377)                \n"          , "  x            \n"
			                                                                                                                             "ΔΔΔΔΔ*(y*25377)\n"
			                                                                                                                             "25377          \n"        },
			{(x / 25377) * (y / 25377), "(((x)/(25377))*((y)/(25377)))", "x/25377*(y/25377)"  , "    (x)       (y)    \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*(ΔΔΔΔΔΔΔ))\n"
			                                                                                    "  (25377)   (25377)  \n"              , "  x     y  \n"
			                                                                                                                             "ΔΔΔΔΔ*ΔΔΔΔΔ\n"
			                                                                                                                             "25377 25377\n"            },
			{(x / 25377) * (y ^ 25377), "(((x)/(25377))*((y)^(25377)))", "x/25377*y^25377"    , "    (x)        (25377)  \n"
			                                                                                    "((ΔΔΔΔΔΔΔ)*((y)       ))\n"
			                                                                                    "  (25377)               \n"           , "  x    25377\n"
			                                                                                                                             "ΔΔΔΔΔ*y     \n"
			                                                                                                                             "25377       \n"           },
			{(x ^ 25377) * +y,          "(((x)^(25377))*(+(y)))"       , "x^25377*+y"         , "     (25377)         \n"
			                                                                                    "(((x)       )*(+(y)))\n"              , " 25377   \n"
			                                                                                                                             "x     *+y\n"              },
			{(x ^ 25377) * -y,          "(((x)^(25377))*(-(y)))"       , "x^25377*-y"         , "     (25377)         \n"
			                                                                                    "(((x)       )*(-(y)))\n"              , " 25377   \n"
			                                                                                                                             "x     *-y\n"              },
			{(x ^ 25377) * (y + 25377), "(((x)^(25377))*((y)+(25377)))", "x^25377*(y+25377)"  , "     (25377)                  \n"
			                                                                                    "(((x)       )*((y) + (25377)))\n"     , " 25377            \n"
			                                                                                                                             "x     *(y + 25377)\n"     },
			{(x ^ 25377) * (y - 25377), "(((x)^(25377))*((y)-(25377)))", "x^25377*(y-25377)"  , "     (25377)                  \n"
			                                                                                    "(((x)       )*((y) - (25377)))\n"     , " 25377            \n"
			                                                                                                                             "x     *(y - 25377)\n"     },
			{(x ^ 25377) * (y * 25377), "(((x)^(25377))*((y)*(25377)))", "x^25377*(y*25377)"  , "     (25377)                \n"
			                                                                                    "(((x)       )*((y)*(25377)))\n"       , " 25377          \n"
			                                                                                                                             "x     *(y*25377)\n"       },
			{(x ^ 25377) * (y / 25377), "(((x)^(25377))*((y)/(25377)))", "x^25377*(y/25377)"  , "     (25377)     (y)    \n"
			                                                                                    "(((x)       )*(ΔΔΔΔΔΔΔ))\n"
																							    "               (25377)  \n"           , " 25377   y  \n"
			                                                                                                                             "x     *ΔΔΔΔΔ\n"
			                                                                                  										     "       25377\n"           },
			{(x ^ 25377) * (y ^ 25377), "(((x)^(25377))*((y)^(25377)))", "x^25377*y^25377"    , "     (25377)      (25377)  \n"
			                                                                                    "(((x)       )*((y)       ))\n"        , " 25377  25377\n"
                                                                                                                                         "x     *y     \n"          },

			{+x          / +y,          "((+(x))/(+(y)))"              , "+x/+y"              , " (+(x)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								" (+(y)) \n"                           , "+x\n"
																								                                         "ΔΔ\n"
																																		 "+y\n"                     },
			{+x          / -y,          "((+(x))/(-(y)))"              , "+x/-y"              , " (+(x)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
																								" (-(y)) \n"                           , "+x\n"
																								                                         "ΔΔ\n"
																																		 "-y\n"                     },
			{+x          / (y + 25377), "((+(x))/((y)+(25377)))"       , "+x/(y+25377)"       , "      (+(x))     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) + (25377)) \n"                  , "    +x   \n"
																								                                         "ΔΔΔΔΔΔΔΔΔ\n"
																																		 "y + 25377\n"              },
			{+x          / (y - 25377), "((+(x))/((y)-(25377)))"       , "+x/(y-25377)"       , "      (+(x))     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) - (25377)) \n"                  , "    +x   \n"
																								                                         "ΔΔΔΔΔΔΔΔΔ\n"
																																		 "y - 25377\n"              },
			{+x          / (y * 25377), "((+(x))/((y)*(25377)))"       , "+x/(y*25377)"       , "     (+(x))    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y)*(25377)) \n"                    , "   +x  \n"
																								                                         "ΔΔΔΔΔΔΔ\n"
																																		 "y*25377\n"                },
			{+x          / (y / 25377), "((+(x))/((y)/(25377)))"       , "+x/(y/25377)"       , "   (+(x))  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
																								"    (y)    \n"
                                                                                                " (ΔΔΔΔΔΔΔ) \n"
                                                                                                "  (25377)  \n"	                       , "   +x  \n"
																								                                         "ΔΔΔΔΔΔΔ\n"
																																		 "   y   \n"
                                                                                                                                         " ΔΔΔΔΔ \n"
																																		 " 25377 \n"                },
			{+x          / (y ^ 25377), "((+(x))/((y)^(25377)))"       , "+x/y^25377"         , "    (+(x))    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"     (25377)  \n"
                                                                                                " ((y)       ) \n"                     , "  +x  \n"
																								                                         "ΔΔΔΔΔΔ\n"
																																		 " 25377\n"
                                                                                                                                         "y     \n"                 },
			{-x          / +y,          "((-(x))/(+(y)))"              , "-x/+y"              , " (-(x)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
			                                                                                    " (+(y)) \n"                           , "-x\n"
			                                                                                                                             "ΔΔ\n"
			                                                                                  									    	 "+y\n"                     },
			{-x          / -y,          "((-(x))/(-(y)))"              , "-x/-y"              , " (-(x)) \n"
			                                                                                    "(ΔΔΔΔΔΔ)\n"
			                                                                                    " (-(y)) \n"                           , "-x\n"
			                                                                                                                             "ΔΔ\n"
			                                                                                  									    	 "-y\n"                     },
			{-x          / (y + 25377), "((-(x))/((y)+(25377)))"       , "-x/(y+25377)"       , "      (-(x))     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) + (25377)) \n"                  , "    -x   \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y + 25377\n"              },
			{-x          / (y - 25377), "((-(x))/((y)-(25377)))"       , "-x/(y-25377)"       , "      (-(x))     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) - (25377)) \n"                  , "    -x   \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y - 25377\n"              },
			{-x          / (y * 25377), "((-(x))/((y)*(25377)))"       , "-x/(y*25377)"       , "     (-(x))    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y)*(25377)) \n"                    , "   -x  \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y*25377\n"                },
			{-x          / (y / 25377), "((-(x))/((y)/(25377)))"       , "-x/(y/25377)"       , "   (-(x))  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "    (y)    \n"
			                                                                                    " (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "  (25377)  \n"	                       , "   -x  \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                                                             "   y   \n"
			                                                                                                                             " ΔΔΔΔΔ \n"
			                                                                                  										     " 25377 \n"                },
			{-x          / (y ^ 25377), "((-(x))/((y)^(25377)))"       , "-x/y^25377"         , "    (-(x))    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "     (25377)  \n"
			                                                                                    " ((y)       ) \n"                     , "  -x  \n"
			                                                                                                                             "ΔΔΔΔΔΔ\n"
			                                                                                    										 " 25377\n"
			                                                                                                                             "y     \n"                 },
			{(x + 25377) / +y,          "(((x)+(25377))/(+(y)))"       , "(x+25377)/+y"       , " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (+(y))     \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "    +y   \n"              },
			{(x + 25377) / -y,          "(((x)+(25377))/(-(y)))"       , "(x+25377)/-y"       , " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (-(y))     \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "    -y   \n"              },
			{(x + 25377) / (y + 25377), "(((x)+(25377))/((y)+(25377)))", "(x+25377)/(y+25377)", " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) + (25377)) \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  										     "y + 25377\n"              },
			{(x + 25377) / (y - 25377), "(((x)+(25377))/((y)-(25377)))", "(x+25377)/(y-25377)", " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) - (25377)) \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y - 25377\n"              },
			{(x + 25377) / (y * 25377), "(((x)+(25377))/((y)*(25377)))", "(x+25377)/(y*25377)", " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "  ((y)*(25377))  \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 " y*25377 \n"              },
			{(x + 25377) / (y / 25377), "(((x)+(25377))/((y)/(25377)))", "(x+25377)/(y/25377)", " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (y)       \n"
			                                                                                    "    (ΔΔΔΔΔΔΔ)    \n"
			                                                                                    "     (25377)     \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  								    		 "    y    \n"
			                                                                                                                             "  ΔΔΔΔΔ  \n"
			                                                                                  								    		 "  25377  \n"              },
			{(x + 25377) / (y ^ 25377), "(((x)+(25377))/((y)^(25377)))", "(x+25377)/y^25377"  , " ((x) + (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (25377)   \n"
			                                                                                    "   ((y)       )  \n"                  , "x + 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  							    			 "   25377 \n"
			                                                                                                                             "  y      \n"              },
			{(x - 25377) / +y,          "(((x)-(25377))/(+(y)))"       , "(x-25377)/+y"       , " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (+(y))     \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "    +y   \n"              },
			{(x - 25377) / -y,          "(((x)-(25377))/(-(y)))"       , "(x-25377)/-y"       , " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (-(y))     \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "    -y   \n"              },
			{(x - 25377) / (y + 25377), "(((x)-(25377))/((y)+(25377)))", "(x-25377)/(y+25377)", " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) + (25377)) \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  										     "y + 25377\n"              },
			{(x - 25377) / (y - 25377), "(((x)-(25377))/((y)-(25377)))", "(x-25377)/(y-25377)", " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) - (25377)) \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y - 25377\n"              },
			{(x - 25377) / (y * 25377), "(((x)-(25377))/((y)*(25377)))", "(x-25377)/(y*25377)", " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "  ((y)*(25377))  \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 " y*25377 \n"              },
			{(x - 25377) / (y / 25377), "(((x)-(25377))/((y)/(25377)))", "(x-25377)/(y/25377)", " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (y)       \n"
			                                                                                    "    (ΔΔΔΔΔΔΔ)    \n"
			                                                                                    "     (25377)     \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  								    		 "    y    \n"
			                                                                                                                             "  ΔΔΔΔΔ  \n"
			                                                                                  								    		 "  25377  \n"              },
			{(x - 25377) / (y ^ 25377), "(((x)-(25377))/((y)^(25377)))", "(x-25377)/y^25377"  , " ((x) - (25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "       (25377)   \n"
			                                                                                    "   ((y)       )  \n"                  , "x - 25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  							    			 "   25377 \n"
			                                                                                                                             "  y      \n"              },
			{(x * 25377) / +y,          "(((x)*(25377))/(+(y)))"       , "x*25377/+y"         , " ((x)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "     (+(y))    \n"                    , "x*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "   +y  \n"                },
			{(x * 25377) / -y,          "(((x)*(25377))/(-(y)))"       , "x*25377/-y"         , " ((x)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "     (-(y))    \n"                    , "x*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "   -y  \n"                },
			{(x * 25377) / (y + 25377), "(((x)*(25377))/((y)+(25377)))", "x*25377/(y+25377)"  , "  ((x)*(25377))  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) + (25377)) \n"                  , " x*25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  										     "y + 25377\n"              },
			{(x * 25377) / (y - 25377), "(((x)*(25377))/((y)-(25377)))", "x*25377/(y-25377)"  , "  ((x)*(25377))  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) - (25377)) \n"                  , " x*25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y - 25377\n"              },
			{(x * 25377) / (y * 25377), "(((x)*(25377))/((y)*(25377)))", "x*25377/(y*25377)"  , " ((x)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y)*(25377)) \n"                    , "x*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  									    	 "y*25377\n"                },
			{(x * 25377) / (y / 25377), "(((x)*(25377))/((y)/(25377)))", "x*25377/(y/25377)"  , " ((x)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (y)      \n"
			                                                                                    "   (ΔΔΔΔΔΔΔ)   \n"
			                                                                                    "    (25377)    \n"                    , "x*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  								    		 "   y   \n"
			                                                                                                                             " ΔΔΔΔΔ \n"
			                                                                                  								    		 " 25377 \n"                },
			{(x * 25377) / (y ^ 25377), "(((x)*(25377))/((y)^(25377)))", "x*25377/y^25377"    , " ((x)*(25377)) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "      (25377)  \n"
			                                                                                    "  ((y)       ) \n"                    , "x*25377\n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                  							    			 "  25377\n"
			                                                                                                                             " y     \n"                },
			{(x / 25377) / +y,          "(((x)/(25377))/(+(y)))"       , "x/25377/+y"         , "    (x)    \n"
			                                                                                    " (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "  (25377)  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "   (+(y))  \n"                        , "   x   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "   +y  \n"                },
			{(x / 25377) / -y,          "(((x)/(25377))/(-(y)))"       , "x/25377/-y"         , "    (x)    \n"
			                                                                                    " (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "  (25377)  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "   (-(y))  \n"                        , "   x   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "   -y  \n"                },
			{(x / 25377) / (y + 25377), "(((x)/(25377))/((y)+(25377)))", "x/25377/(y+25377)"  , "       (x)       \n"
			                                                                                    "    (ΔΔΔΔΔΔΔ)    \n"
			                                                                                    "     (25377)     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) + (25377)) \n"                  , "    x    \n"
			                                                                                    						                 "  ΔΔΔΔΔ  \n"
			                                                                                    						                 "  25377  \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y + 25377\n"              },
			{(x / 25377) / (y - 25377), "(((x)/(25377))/((y)-(25377)))", "x/25377/(y-25377)"  , "       (x)       \n"
			                                                                                    "    (ΔΔΔΔΔΔΔ)    \n"
			                                                                                    "     (25377)     \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y) - (25377)) \n"                  , "    x    \n"
			                                                                                    						                 "  ΔΔΔΔΔ  \n"
			                                                                                    						                 "  25377  \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y - 25377\n"              },
			{(x / 25377) / (y * 25377), "(((x)/(25377))/((y)*(25377)))", "x/25377/(y*25377)"  , "      (x)      \n"
			                                                                                    "   (ΔΔΔΔΔΔΔ)   \n"
			                                                                                    "    (25377)    \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    " ((y)*(25377)) \n"                    , "   x   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y*25377\n"                },
			{(x / 25377) / (y / 25377), "(((x)/(25377))/((y)/(25377)))", "x/25377/(y/25377)"  , "    (x)    \n"
			                                                                                    " (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "  (25377)  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "    (y)    \n"
																								" (ΔΔΔΔΔΔΔ) \n"
																								"  (25377)  \n"                        , "   x   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
																																		 "   y   \n"
																																		 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"                },
			{(x / 25377) / (y ^ 25377), "(((x)/(25377))/((y)^(25377)))", "x/25377/y^25377"    , "      (x)     \n"
			                                                                                    "   (ΔΔΔΔΔΔΔ)  \n"
			                                                                                    "    (25377)   \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
			                                                                                    "     (25377)  \n"
																								" ((y)       ) \n"                     , "   x   \n"
			                                                                                    						                 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
																																		 "  25377\n"
			                                                                                    						                 " y     \n"                },
			{(x ^ 25377) / +y,          "(((x)^(25377))/(+(y)))"       , "x^25377/+y"         , "     (25377)  \n"
			                                                                                    " ((x)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"    (+(y))    \n"                     , " 25377\n"
			                                                                                    						                 "x     \n"
			                                                                                                                             "ΔΔΔΔΔΔ\n"
			                                                                                    						                 "  +y  \n"                 },
			{(x ^ 25377) / -y,          "(((x)^(25377))/(-(y)))"       , "x^25377/-y"         , "     (25377)  \n"
			                                                                                    " ((x)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"    (-(y))    \n"                     , " 25377\n"
			                                                                                    						                 "x     \n"
			                                                                                                                             "ΔΔΔΔΔΔ\n"
			                                                                                    						                 "  -y  \n"                 },
			{(x ^ 25377) / (y + 25377), "(((x)^(25377))/((y)+(25377)))", "x^25377/(y+25377)"  , "       (25377)   \n"
			                                                                                    "   ((x)       )  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) + (25377)) \n"                  , "   25377 \n"
			                                                                                    						                 "  x      \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y + 25377\n"              },
			{(x ^ 25377) / (y - 25377), "(((x)^(25377))/((y)-(25377)))", "x^25377/(y-25377)"  , "       (25377)   \n"
			                                                                                    "   ((x)       )  \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y) - (25377)) \n"                  , "   25377 \n"
			                                                                                    						                 "  x      \n"
			                                                                                                                             "ΔΔΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y - 25377\n"              },
			{(x ^ 25377) / (y * 25377), "(((x)^(25377))/((y)*(25377)))", "x^25377/(y*25377)"  , "      (25377)  \n"
			                                                                                    "  ((x)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								" ((y)*(25377)) \n"                    , "  25377\n"
			                                                                                    						                 " x     \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
			                                                                                    						                 "y*25377\n"                },
			{(x ^ 25377) / (y / 25377), "(((x)^(25377))/((y)/(25377)))", "x^25377/(y/25377)"  , "     (25377)  \n"
			                                                                                    " ((x)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"      (y)     \n"
																								"   (ΔΔΔΔΔΔΔ)  \n"
																								"    (25377)   \n"                     , "  25377\n"
			                                                                                    						                 " x     \n"
			                                                                                                                             "ΔΔΔΔΔΔΔ\n"
																																		 "   y   \n"
																																		 " ΔΔΔΔΔ \n"
			                                                                                    						                 " 25377 \n"                },
			{(x ^ 25377) / (y ^ 25377), "(((x)^(25377))/((y)^(25377)))", "x^25377/y^25377"    , "     (25377)  \n"
			                                                                                    " ((x)       ) \n"
			                                                                                    "(ΔΔΔΔΔΔΔΔΔΔΔΔ)\n"
																								"     (25377)  \n"
																								" ((y)       ) \n"                     , " 25377\n"
			                                                                                    						                 "x     \n"
			                                                                                                                             "ΔΔΔΔΔΔ\n"
																																		 " 25377\n"
			                                                                                    						                 "y     \n"                 },

			{+x          ^ +y,          "((+(x))^(+(y)))"              , "(+x)^(+y)"          , "       (+(y)) \n"
			                                                                                    "((+(x))      )\n"                     , "    +y\n"
																								                                         "(+x)  \n"                 },
			{+x          ^ -y,          "((+(x))^(-(y)))"              , "(+x)^(-y)"          , "       (-(y)) \n"
			                                                                                    "((+(x))      )\n"                     , "    -y\n"
																								                                         "(+x)  \n"                 },
			{+x          ^ (y + 25377), "((+(x))^((y)+(25377)))"       , "(+x)^(y+25377)"     , "       ((y) + (25377)) \n"
			                                                                                    "((+(x))               )\n"            , "    y + 25377\n"
																								                                         "(+x)         \n"          },
			{+x          ^ (y - 25377), "((+(x))^((y)-(25377)))"       , "(+x)^(y-25377)"     , "       ((y) - (25377)) \n"
			                                                                                    "((+(x))               )\n"            , "    y - 25377\n"
																								                                         "(+x)         \n"          },
			{+x          ^ (y * 25377), "((+(x))^((y)*(25377)))"       , "(+x)^(y*25377)"     , "       ((y)*(25377)) \n"
			                                                                                    "((+(x))             )\n"              , "    y*25377\n"
																								                                         "(+x)       \n"            },
			{+x          ^ (y / 25377), "((+(x))^((y)/(25377)))"       , "(+x)^(y/25377)"     , "          (y)    \n"
			                                                                                    "       (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "        (25377)  \n"
			                                                                                    "((+(x))         )\n"                  , "      y  \n"
																								                                         "    ΔΔΔΔΔ\n"
																								                                         "    25377\n"
																								                                         "(+x)     \n"              },
			{+x          ^ (y ^ 25377), "((+(x))^((y)^(25377)))"       , "(+x)^y^25377"       , "           (25377)  \n"
			                                                                                    "       ((y)       ) \n"
			                                                                                    "((+(x))            )\n"               , "     25377\n"
																								                                         "    y     \n"
																								                                         "(+x)      \n"             }, // ^
			{-x          ^ +y,          "((-(x))^(+(y)))"              , "(-x)^(+y)"          , "       (+(y)) \n"
			                                                                                    "((-(x))      )\n"                     , "    +y\n"
			                                                                                                                             "(-x)  \n"                 },
			{-x          ^ -y,          "((-(x))^(-(y)))"              , "(-x)^(-y)"          , "       (-(y)) \n"
			                                                                                    "((-(x))      )\n"                     , "    -y\n"
			                                                                                                                             "(-x)  \n"                 },
			{-x          ^ (y + 25377), "((-(x))^((y)+(25377)))"       , "(-x)^(y+25377)"     , "       ((y) + (25377)) \n"
			                                                                                    "((-(x))               )\n"            , "    y + 25377\n"
			                                                                                                                             "(-x)         \n"          },
			{-x          ^ (y - 25377), "((-(x))^((y)-(25377)))"       , "(-x)^(y-25377)"     , "       ((y) - (25377)) \n"
			                                                                                    "((-(x))               )\n"            , "    y - 25377\n"
			                                                                                                                             "(-x)         \n"          },
			{-x          ^ (y * 25377), "((-(x))^((y)*(25377)))"       , "(-x)^(y*25377)"     , "       ((y)*(25377)) \n"
			                                                                                    "((-(x))             )\n"              , "    y*25377\n"
			                                                                                                                             "(-x)       \n"            },
			{-x          ^ (y / 25377), "((-(x))^((y)/(25377)))"       , "(-x)^(y/25377)"     , "          (y)    \n"
			                                                                                    "       (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "        (25377)  \n"
			                                                                                    "((-(x))         )\n"                  , "      y  \n"
			                                                                                                                             "    ΔΔΔΔΔ\n"
			                                                                                                                             "    25377\n"
			                                                                                                                             "(-x)     \n"              },
			{-x          ^ (y ^ 25377), "((-(x))^((y)^(25377)))"       , "(-x)^y^25377"       , "           (25377)  \n"
			                                                                                    "       ((y)       ) \n"
			                                                                                    "((-(x))            )\n"               , "     25377\n"
			                                                                                                                             "    y     \n"
			                                                                                                                             "(-x)      \n"             }, // ^
			{(x + 25377) ^ +y,          "(((x)+(25377))^(+(y)))"       , "(x+25377)^(+y)"     , "                (+(y)) \n"
			                                                                                    "(((x) + (25377))      )\n"            , "           +y\n"
			                                                                                                                             "(x + 25377)  \n"          },
			{(x + 25377) ^ -y,          "(((x)+(25377))^(-(y)))"       , "(x+25377)^(-y)"     , "                (-(y)) \n"
			                                                                                    "(((x) + (25377))      )\n"            , "           -y\n"
			                                                                                                                             "(x + 25377)  \n"          },
			{(x + 25377) ^ (y + 25377), "(((x)+(25377))^((y)+(25377)))", "(x+25377)^(y+25377)", "                ((y) + (25377)) \n"
			                                                                                    "(((x) + (25377))               )\n"   , "           y + 25377\n"
			                                                                                                                             "(x + 25377)         \n"   },
			{(x + 25377) ^ (y - 25377), "(((x)+(25377))^((y)-(25377)))", "(x+25377)^(y-25377)", "                ((y) - (25377)) \n"
			                                                                                    "(((x) + (25377))               )\n"   , "           y - 25377\n"
			                                                                                                                             "(x + 25377)         \n"   },
			{(x + 25377) ^ (y * 25377), "(((x)+(25377))^((y)*(25377)))", "(x+25377)^(y*25377)", "                ((y)*(25377)) \n"
			                                                                                    "(((x) + (25377))             )\n"     , "           y*25377\n"
			                                                                                                                             "(x + 25377)       \n"     },
			{(x + 25377) ^ (y / 25377), "(((x)+(25377))^((y)/(25377)))", "(x+25377)^(y/25377)", "                   (y)    \n"
			                                                                                    "                (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "                 (25377)  \n"
			                                                                                    "(((x) + (25377))         )\n"         , "             y  \n"
			                                                                                                                             "           ΔΔΔΔΔ\n"
			                                                                                                                             "           25377\n"
			                                                                                                                             "(x + 25377)     \n"       },
			{(x + 25377) ^ (y ^ 25377), "(((x)+(25377))^((y)^(25377)))", "(x+25377)^y^25377"  , "                    (25377)  \n"
			                                                                                    "                ((y)       ) \n"
			                                                                                    "(((x) + (25377))            )\n"      , "            25377\n"
			                                                                                                                             "           y     \n"
			                                                                                                                             "(x + 25377)      \n"      }, // ^
			{(x - 25377) ^ +y,          "(((x)-(25377))^(+(y)))"       , "(x-25377)^(+y)"     , "                (+(y)) \n"
			                                                                                    "(((x) - (25377))      )\n"            , "           +y\n"
			                                                                                                                             "(x - 25377)  \n"          },
			{(x - 25377) ^ -y,          "(((x)-(25377))^(-(y)))"       , "(x-25377)^(-y)"     , "                (-(y)) \n"
			                                                                                    "(((x) - (25377))      )\n"            , "           -y\n"
			                                                                                                                             "(x - 25377)  \n"          },
			{(x - 25377) ^ (y + 25377), "(((x)-(25377))^((y)+(25377)))", "(x-25377)^(y+25377)", "                ((y) + (25377)) \n"
			                                                                                    "(((x) - (25377))               )\n"   , "           y + 25377\n"
			                                                                                                                             "(x - 25377)         \n"   },
			{(x - 25377) ^ (y - 25377), "(((x)-(25377))^((y)-(25377)))", "(x-25377)^(y-25377)", "                ((y) - (25377)) \n"
			                                                                                    "(((x) - (25377))               )\n"   , "           y - 25377\n"
			                                                                                                                             "(x - 25377)         \n"   },
			{(x - 25377) ^ (y * 25377), "(((x)-(25377))^((y)*(25377)))", "(x-25377)^(y*25377)", "                ((y)*(25377)) \n"
			                                                                                    "(((x) - (25377))             )\n"     , "           y*25377\n"
			                                                                                                                             "(x - 25377)       \n"     },
			{(x - 25377) ^ (y / 25377), "(((x)-(25377))^((y)/(25377)))", "(x-25377)^(y/25377)", "                   (y)    \n"
			                                                                                    "                (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "                 (25377)  \n"
			                                                                                    "(((x) - (25377))         )\n"         , "             y  \n"
			                                                                                                                             "           ΔΔΔΔΔ\n"
			                                                                                                                             "           25377\n"
			                                                                                                                             "(x - 25377)     \n"       },
			{(x - 25377) ^ (y ^ 25377), "(((x)-(25377))^((y)^(25377)))", "(x-25377)^y^25377"  , "                    (25377)  \n"
			                                                                                    "                ((y)       ) \n"
			                                                                                    "(((x) - (25377))            )\n"      , "            25377\n"
			                                                                                                                             "           y     \n"
			                                                                                                                             "(x - 25377)      \n"      }, // ^
			{(x * 25377) ^ +y,          "(((x)*(25377))^(+(y)))"       , "(x*25377)^(+y)"     , "              (+(y)) \n"
			                                                                                    "(((x)*(25377))      )\n"              , "         +y\n"
			                                                                                                                             "(x*25377)  \n"            },
			{(x * 25377) ^ -y,          "(((x)*(25377))^(-(y)))"       , "(x*25377)^(-y)"     , "              (-(y)) \n"
			                                                                                    "(((x)*(25377))      )\n"              , "         -y\n"
			                                                                                                                             "(x*25377)  \n"            },
			{(x * 25377) ^ (y + 25377), "(((x)*(25377))^((y)+(25377)))", "(x*25377)^(y+25377)", "              ((y) + (25377)) \n"
			                                                                                    "(((x)*(25377))               )\n"     , "         y + 25377\n"
			                                                                                                                             "(x*25377)         \n"     },
			{(x * 25377) ^ (y - 25377), "(((x)*(25377))^((y)-(25377)))", "(x*25377)^(y-25377)", "              ((y) - (25377)) \n"
			                                                                                    "(((x)*(25377))               )\n"     , "         y - 25377\n"
			                                                                                                                             "(x*25377)         \n"     },
			{(x * 25377) ^ (y * 25377), "(((x)*(25377))^((y)*(25377)))", "(x*25377)^(y*25377)", "              ((y)*(25377)) \n"
			                                                                                    "(((x)*(25377))             )\n"       , "         y*25377\n"
			                                                                                                                             "(x*25377)       \n"       },
			{(x * 25377) ^ (y / 25377), "(((x)*(25377))^((y)/(25377)))", "(x*25377)^(y/25377)", "                 (y)    \n"
			                                                                                    "              (ΔΔΔΔΔΔΔ) \n"
			                                                                                    "               (25377)  \n"
			                                                                                    "(((x)*(25377))         )\n"           , "           y  \n"
			                                                                                                                             "         ΔΔΔΔΔ\n"
			                                                                                                                             "         25377\n"
			                                                                                                                             "(x*25377)     \n"         },
			{(x * 25377) ^ (y ^ 25377), "(((x)*(25377))^((y)^(25377)))", "(x*25377)^y^25377"  , "                  (25377)  \n"
			                                                                                    "              ((y)       ) \n"
			                                                                                    "(((x)*(25377))            )\n"        , "          25377\n"
			                                                                                                                             "         y     \n"
			                                                                                                                             "(x*25377)      \n"        }, // ^
			{(x / 25377) ^ +y,          "(((x)/(25377))^(+(y)))"       , "(x/25377)^(+y)"     , "          (+(y)) \n"
			                                                                                    "    (x)          \n"
																								"((ΔΔΔΔΔΔΔ)      )\n"
																								"  (25377)        \n"                  , "     +y\n"
			                                                                                                                             "  x    \n"
																																		 "ΔΔΔΔΔ  \n"
																																		 "25377  \n"                }, // /
			{(x / 25377) ^ -y,          "(((x)/(25377))^(-(y)))"       , "(x/25377)^(-y)"     , "          (-(y)) \n"
			                                                                                    "    (x)          \n"
																								"((ΔΔΔΔΔΔΔ)      )\n"
																								"  (25377)        \n"                  , "     -y\n"
			                                                                                                                             "  x    \n"
																																		 "ΔΔΔΔΔ  \n"
																																		 "25377  \n"                }, // /
			{(x / 25377) ^ (y + 25377), "(((x)/(25377))^((y)+(25377)))", "(x/25377)^(y+25377)", "          ((y) + (25377)) \n"
			                                                                                    "    (x)                   \n"
																								"((ΔΔΔΔΔΔΔ)               )\n"
																								"  (25377)                 \n"         , "     y + 25377\n"
			                                                                                                                             "  x           \n"
																																		 "ΔΔΔΔΔ         \n"
																																		 "25377         \n"         }, // /
			{(x / 25377) ^ (y - 25377), "(((x)/(25377))^((y)-(25377)))", "(x/25377)^(y-25377)", "          ((y) - (25377)) \n"
			                                                                                    "    (x)                   \n"
																								"((ΔΔΔΔΔΔΔ)               )\n"
																								"  (25377)                 \n"         , "     y - 25377\n"
			                                                                                                                             "  x           \n"
																																		 "ΔΔΔΔΔ         \n"
																																		 "25377         \n"         }, // /
			{(x / 25377) ^ (y * 25377), "(((x)/(25377))^((y)*(25377)))", "(x/25377)^(y*25377)", "          ((y)*(25377)) \n"
			                                                                                    "    (x)                 \n"
																								"((ΔΔΔΔΔΔΔ)             )\n"
																								"  (25377)               \n"           , "     y*25377\n"
			                                                                                                                             "  x         \n"
																																		 "ΔΔΔΔΔ       \n"
																																		 "25377       \n"           }, // /
			{(x / 25377) ^ (y / 25377), "(((x)/(25377))^((y)/(25377)))", "(x/25377)^(y/25377)", "             (y)    \n"
			                                                                                    "          (ΔΔΔΔΔΔΔ) \n"
																								"           (25377)  \n"
			                                                                                    "    (x)             \n"
																								"((ΔΔΔΔΔΔΔ)         )\n"
																								"  (25377)           \n"               , "       y  \n"
																								                                         "     ΔΔΔΔΔ\n"
																																		 "     25377\n"
			                                                                                                                             "  x       \n"
																																		 "ΔΔΔΔΔ     \n"
																																		 "25377     \n"             }, // /
			{(x / 25377) ^ (y ^ 25377), "(((x)/(25377))^((y)^(25377)))", "(x/25377)^y^25377"  , "              (25377)  \n"
																								"          ((y)       ) \n"
			                                                                                    "    (x)                \n"
																								"((ΔΔΔΔΔΔΔ)            )\n"
																								"  (25377)              \n"            , "      25377\n"
																																		 "     y     \n"
			                                                                                                                             "  x        \n"
																																		 "ΔΔΔΔΔ      \n"
																																		 "25377      \n"            }, // /,^
			{(x ^ 25377) ^ +y,          "(((x)^(25377))^(+(y)))"       , "(x^25377)^(+y)"     , "             (+(y)) \n"
			                                                                                    "     (25377)        \n"
			                                                                                    "(((x)       )      )\n"               , "        +y\n"
			                                                                                                                             "  25377   \n"
			                                                                                                                             "(x     )  \n"             }, // ^
			{(x ^ 25377) ^ -y,          "(((x)^(25377))^(-(y)))"       , "(x^25377)^(-y)"     , "             (-(y)) \n"
			                                                                                    "     (25377)        \n"
			                                                                                    "(((x)       )      )\n"               , "        -y\n"
			                                                                                                                             "  25377   \n"
			                                                                                                                             "(x     )  \n"             }, // ^
			{(x ^ 25377) ^ (y + 25377), "(((x)^(25377))^((y)+(25377)))", "(x^25377)^(y+25377)", "             ((y) + (25377)) \n"
			                                                                                    "     (25377)                 \n"
			                                                                                    "(((x)       )               )\n"      , "        y + 25377\n"
			                                                                                                                             "  25377          \n"
			                                                                                                                             "(x     )         \n"      }, // ^
			{(x ^ 25377) ^ (y - 25377), "(((x)^(25377))^((y)-(25377)))", "(x^25377)^(y-25377)", "             ((y) - (25377)) \n"
			                                                                                    "     (25377)                 \n"
			                                                                                    "(((x)       )               )\n"      , "        y - 25377\n"
			                                                                                                                             "  25377          \n"
			                                                                                                                             "(x     )         \n"      }, // ^
			{(x ^ 25377) ^ (y * 25377), "(((x)^(25377))^((y)*(25377)))", "(x^25377)^(y*25377)", "             ((y)*(25377)) \n"
			                                                                                    "     (25377)               \n"
			                                                                                    "(((x)       )             )\n"        , "        y*25377\n"
			                                                                                                                             "  25377        \n"
			                                                                                                                             "(x     )       \n"        }, // ^
			{(x ^ 25377) ^ (y / 25377), "(((x)^(25377))^((y)/(25377)))", "(x^25377)^(y/25377)", "                (y)    \n"
			                                                                                    "             (ΔΔΔΔΔΔΔ) \n"
																								"              (25377)  \n"
			                                                                                    "     (25377)           \n"
			                                                                                    "(((x)       )         )\n"            , "          y  \n"
																								                                         "        ΔΔΔΔΔ\n"
																																		 "        25377\n"
			                                                                                                                             "  25377      \n"
			                                                                                                                             "(x     )     \n"        }, // ^
			{(x ^ 25377) ^ (y ^ 25377), "(((x)^(25377))^((y)^(25377)))", "(x^25377)^y^25377"  , "                 (25377)  \n"
																								"             ((y)       ) \n"
			                                                                                    "     (25377)              \n"
			                                                                                    "(((x)       )            )\n"         , "         25377\n"
																																		 "        y     \n"
			                                                                                                                             "  25377       \n"
			                                                                                                                             "(x     )      \n"        }, // ^
		}; // end tests initializer

		// assert no two different test cases have the same result 
		// (otherwise there would be an ambiguity which is a specification error)
		set<string> prints[4];
		for(size_t i = 0 ; i < nTests ; ++i)
		{
			assert(prints[0].insert(tests[i].fullParen1D).second);
			assert(prints[1].insert(tests[i].minParen1D).second);
			assert(prints[2].insert(tests[i].fullParen2D).second);
			assert(prints[3].insert(tests[i].minParen2D).second);
		} // end for

		// run tests
		for(size_t i = 0 ; i < nTests ; ++i)
		{
			ostringstream strout;
			tests[i].dsel.print1D(strout,true);
			assert(strout.str() == tests[i].fullParen1D);
			strout.str("");
			tests[i].dsel.print1D(strout,false);
			assert(strout.str() == tests[i].minParen1D);
			strout.str("");
			tests[i].dsel.print2D(strout,true);
			assert(strout.str() == tests[i].fullParen2D);
			strout.str("");
			tests[i].dsel.print2D(strout,false);
			assert(strout.str() == tests[i].minParen2D);
		} // end for
	} // end function testExpression

} // end namespace Symbolic
