#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "geometry.h"
#include "symbol table.h"

namespace gui
{
	void runTestSuite();

	template<typename CoordinateType>
	class Control : public geometry::Rectangle<CoordinateType>
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;

		// TODO: add color
		// TODO: add type

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized Control.
		 */
		Control(){/* emtpy body */}

		/** Construct a Control with the specified sides.
		 */
		Control(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
			:Rectangle(left,bottom,right,top)
		{
			// empty body
		} // end Control constructor

		Control(const property_tree_type &tree)
			:Rectangle(tree.get<CoordinateType>("sides.left"),tree.get<CoordinateType>("sides.bottom"),
						tree.get<CoordinateType>("sides.right"),tree.get<CoordinateType>("sides.top"))
		{
			// empty body
		} // end Control conversion constructor

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("sides.left",left());
			tree.put("sides.bottom",bottom());
			tree.put("sides.right",right());
			tree.put("sides.top",top());

			return tree;
		} // end method operator property_tree_type

	}; // end class Control


	struct ConstraintEndPoint
	{
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;
		typedef geometry::RectangleSide side_type;

		/***************
		*    Fields    *
		***************/

		size_t control; // ordinal number of the referred control
		side_type side; // enumerator of the referred side

		/*********************
		*    Constructors    *
		*********************/

		ConstraintEndPoint(){/* empty body */}

		ConstraintEndPoint(size_t control, side_type side)
			:control(control),side(side)
		{
			// empty body
		} // end ConstraintEndPoint constructor

		ConstraintEndPoint(const property_tree_type &tree)
			:control(tree.get<size_t>("control")),side(to<side_type>(tree.get<std::string>("side")))
		{
			// empty body
		} // end ConstraintEndPoint conversion constructor

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("control",control);
			tree.put("side",to<std::string>(side));

			return tree;
		} // end method operator property_tree_type

	}; // end struct ConstraintEndPoint


	template<typename TextType = std::string>
	class Constraint
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		template<typename IDType, typename RationalType>
		struct ParseResult
		{
			// Fields
			std::map<IDType,RationalType> varCoeff;
			RationalType pxDensityCoeff;
			RationalType rhsConstant;
		}; // end struct ParseResult

		typedef TextType text_type;
		typedef boost::property_tree::ptree property_tree_type;
		typedef ConstraintEndPoint EndPoint;
		typedef EndPoint::side_type side_type;

	private:
		/***************
		*    Fields    *
		***************/

		TextType iText;
		EndPoint iEndPoints[2];

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized Constraint.
		 */
		Constraint(){/* emtpy body */}

		/** Construct a Constraint with the specified sides.
		 */
		Constraint(TextType text, const EndPoint &endPoint1, const EndPoint &endPoint2)
			:iText(std::move(text))
		{
			iEndPoints[0] = endPoint1;
			iEndPoints[1] = endPoint2;
		} // end Control constructor

		Constraint(TextType text, size_t control1, side_type side1, size_t control2, side_type side2)
			:iText(std::move(text))
		{
			iEndPoints[0].control = control1;
			iEndPoints[0].side = side1;
			iEndPoints[1].control = control2;
			iEndPoints[1].side = side2;
		} // end Control constructor

		Constraint(const property_tree_type &tree)
			:iText(tree.get<TextType>("text"))
		{
			iEndPoints[0] = EndPoint(tree.get_child("first-end-point"));
			iEndPoints[1] = EndPoint(tree.get_child("second-end-point"));
		} // end Constraint conversion constructor


		/*************************
		*    Accessor Methods    *
		*************************/

		TextType &text()
		{
			return iText;
		} // end method text

		const TextType &text() const
		{
			return iText;
		} // end method text

		EndPoint (&endPoints())[2]
		{
			return iEndPoints;
		} // end method endPoints

		const EndPoint (&endPoints() const)[2]
		{
			return iEndPoints;
		} // end method endPoints

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("text",iText);
			tree.put_child("first-end-point",iEndPoints[0]);
			tree.put_child("second-end-point",iEndPoints[1]);

			return tree;
		} // end method operator property_tree_type

		template<typename RationalType, typename IDType, typename NameType>
		ParseResult<IDType,RationalType> parse(std::shared_ptr<Symbolic::Common::SymbolTable<NameType,IDType>> symbols)
		{
			namespace qi = boost::spirit::qi;
			namespace px = boost::phoenix;
			using qi::rule;
			using qi::_val;	using qi::_1; using qi::_2;
			using qi::alpha; using qi::alnum; using qi::digit; using qi::char_; using qi::eps; using qi::ulong_long;
			using boost::spirit::ascii::space;
			using boost::spirit::ascii::space_type;

			ParseResult<IDType,RationalType> result;

			struct Workaround
			{
				/**	s must be a string of digits [0-9].
				 */
				static RationalType decimalPoint(RationalType r, const std::vector<char> &s)
				{ // TODO: make overflow safe
					unsigned long long n = 0;
					unsigned long long d = 1;

					for(auto c : s)
					{
						n *= 10;
						n += c - '0';
						d *= 10;
					} // end foreach
					
					return r + RationalType(n,d);
				} // end function decimalPoint

				static void populate(const RationalType &coeff, const NameType &symbol, ParseResult<IDType,RationalType> &result, 
					std::shared_ptr<Symbolic::Common::SymbolTable<NameType,IDType>> symbols)
				{
					if(symbol == "cm")
						result.rhsConstant += coeff;
					else if(symbol == "px")
						result.pxDensityCoeff += coeff;
					else
						result.varCoeff[symbols->declare(symbol)] += coeff;
				} // end function populate
			}; // end local struct Workaround

			rule<TextType::iterator,NameType()> identifier = (alpha | char_('_')) > *(alnum | char_('_'));
			rule<TextType::iterator,RationalType()> coefficient = (char_('-')[_val = -1] | eps[_val = 1]) > 
					-(ulong_long[_val *= _1] > -('/' > ulong_long[_val /= _1] | '.' > (*digit)[_val = px::bind(Workaround::decimalPoint,_val,_1)]));
			rule<TextType::iterator,space_type> constraint = (coefficient > identifier)[px::bind(Workaround::populate,_1,_2,px::ref(result),px::ref(symbols))] % '+';

			auto begin = iText.begin();
			auto end = iText.end();

			bool success = phrase_parse(begin,end,constraint,space);
			if(!success || begin != end)
				throw std::runtime_error("Error parsing constraint!");

			return result;
		} // end method parse

	}; // end class Constraint


	template<typename CoordinateType, typename TextType = std::string>
	class Model
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;
		typedef CoordinateType coordinate_type;
		typedef TextType text_type;

	public: // TODO: make private and add methods to manipulate...
		/***************
		*    Fields    *
		***************/

		std::vector<Control<CoordinateType>> controls;
		std::vector<Constraint<TextType>> constraints;

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an empty Model.
		 */
		Model(){/* emtpy body */}

		/****************
		*    Methods    *
		****************/

		void clear()
		{
			controls.clear();
			constraints.clear();
		} // end method clear

		void load(const std::string &fileName)
		{
			clear();
			property_tree_type tree;
			
			boost::property_tree::read_xml(fileName,tree);

			for(const auto &control : tree.get_child("gui-model.controls"))
				controls.emplace_back(control.second);

			for(const auto &constraint : tree.get_child("gui-model.constraints"))
				constraints.emplace_back(constraint.second);
		} // end method load

		void save(const std::string &fileName)
		{
			property_tree_type tree;

			for(const auto &control : controls)
				tree.add_child("gui-model.controls.control",control);

			for(const auto &constraint : constraints)
				tree.add_child("gui-model.constraints.constraint",constraint);

			boost::property_tree::write_xml(fileName,tree);
		} // end method save

	}; // end class Model

} // end namespace gui

#endif // GUI_MODEL_H
