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

#include <Eigen/Dense>

#include "geometry.h"
#include "symbol table.h"
#include "linear system solving.h"

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
			RationalType pxSizeCoeff;
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
		ParseResult<IDType,RationalType> parse(std::shared_ptr<Symbolic::Common::SymbolTable<NameType,IDType>> symbols) const
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
						result.rhsConstant -= coeff;
					else if(symbol == "px")
						result.pxSizeCoeff += coeff;
					else
						result.varCoeff[symbols->declare(symbol)] += coeff;
				} // end function populate
			}; // end local struct Workaround

			rule<TextType::const_iterator,NameType()> identifier = (alpha | char_('_')) > *(alnum | char_('_'));
			rule<TextType::const_iterator,RationalType()> coefficient = (char_('-')[_val = -1] | eps[_val = 1]) > 
					-(ulong_long[_val *= _1] > -('/' > ulong_long[_val /= _1] | '.' > (*digit)[_val = px::bind(Workaround::decimalPoint,_val,_1)]));
			rule<TextType::const_iterator,space_type> constraint = (coefficient > identifier)[px::bind(Workaround::populate,_1,_2,px::ref(result),px::ref(symbols))] % '+';

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

		// TODO: check that there is at least one control (the screen) and that all contraints refer to 
		// existent controls! Also that endpoints are consistent.
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

		template<typename RationalType, typename IDType /* = int */, typename NameType /* = std::string */> // current compiler version does not support default arguments
		void compile()
		{
			auto symbols = std::make_shared<Symbolic::Common::SymbolTable<NameType,IDType>>();
			std::vector<Constraint<TextType>::template ParseResult<IDType,RationalType>> parseResults;

			for(const auto &constraint : constraints)
				parseResults.push_back(constraint.parse<RationalType,IDType,NameType>(symbols));
			// TODO: optimize to not include unknown constants that are not present.

			Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> systemMatrix; 
			systemMatrix.setZero(constraints.size(),4*controls.size() + symbols->size() + 1); // 4*(nControls-1) + nSymbols + 4 + 1
			// the layout is control sides, then named variables, then unknown constants then known constant.

			// Fill in the augmented system matrix
			size_t i = 0;
			for(const auto &constraint : constraints)
			{
				int coeffs[2] = {1,-1};
				if(controls[constraint.endPoints()[0].control].side(constraint.endPoints()[0].side) > controls[constraint.endPoints()[1].control].side(constraint.endPoints()[1].side))
					std::swap(coeffs[0],coeffs[1]);

				for(size_t ep = 0 ; ep < 2 ; ++ep)
				{
					if(constraint.endPoints()[ep].control != 0)
						systemMatrix(i,4*(constraint.endPoints()[ep].control-1) + size_t(constraint.endPoints()[ep].side)) += coeffs[ep];
					else if(constraint.endPoints()[ep].side == geometry::RectangleSide::RIGHT || constraint.endPoints()[ep].side == geometry::RectangleSide::TOP)
						systemMatrix(i,4*(controls.size()-1) + symbols->size() + size_t(constraint.endPoints()[ep].side) - 2) += coeffs[ep];
				} // end for

				for(const auto &var : parseResults[i].varCoeff)
					systemMatrix(i,4*(controls.size()-1) + var.first) = var.second;

				size_t offset = constraint.endPoints()[0].side == geometry::RectangleSide::LEFT || constraint.endPoints()[0].side == geometry::RectangleSide::RIGHT ? 0 : 1;
				systemMatrix(i,4*(controls.size()-1) + symbols->size() + 2 + offset) = parseResults[i].pxSizeCoeff;

				systemMatrix(i,4*(controls.size()-1) + symbols->size() + 4) = parseResults[i].rhsConstant; // already moved to rhs during parsing

				++i;
			} // end foreach

			numericReducedRowEchelonFormNoPivot(systemMatrix);
			auto solution = semiSymbolicSolveLinearSystem(systemMatrix,4);

			// assume unique solution for now...
		} // end method compile

	}; // end class Model

} // end namespace gui

#endif // GUI_MODEL_H
