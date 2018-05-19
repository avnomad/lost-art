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

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <map>
#include <ratio>
#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <type_traits>

#include <cassert>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "geometry.hpp"
#include "graphene.hpp"
#include "symbol table.hpp"
#include "gui model/controls.hpp"

namespace GUIModel
{
	namespace Controls
	{
		// TODO: rework design of ConstraintEndPoint
		template<typename ControlContainerType>
		struct ConstraintEndPoint
		{
			/*********************
			*    Member Types    *
			*********************/

			using control_container_type = ControlContainerType;
			using coordinate_type = typename control_container_type::value_type::coordinate_type;
			using property_tree_type = boost::property_tree::ptree;
			using side_type = geometry::RectangleSide;

			/***************
			*    Fields    *
			***************/
		private:
			ControlContainerType *iContainer;
		public:
			size_t control; // ordinal number of the referred control
			side_type side; // enumerator of the referred side

			/*********************
			*    Constructors    *
			*********************/

			ConstraintEndPoint() = default;

			ConstraintEndPoint(ControlContainerType *container, size_t control, side_type side)
				:iContainer(container),control(control),side(side)
			{
				// empty body
			} // end ConstraintEndPoint constructor

			ConstraintEndPoint(const property_tree_type &tree, ControlContainerType *container)
				:iContainer(container),control(tree.get<size_t>("control")),side(to<side_type>(tree.get<std::string>("side")))
			{
				// empty body
			} // end ConstraintEndPoint conversion constructor

			/*************************
			*    Accessor Methods    *
			*************************/

			ControlContainerType *&container()
			{
				return iContainer;
			} // end method container

			ControlContainerType *const &container() const
			{
				return iContainer;
			} // end method container

			/****************
			*    Methods    *
			****************/

			// iContainer is not serialized because there is only one container in application.
			// May need to generalize later.
			operator property_tree_type() const
			{
				property_tree_type tree;

				tree.put("control",control);
				tree.put("side",to<std::string>(side));

				return tree;
			} // end method operator property_tree_type

			decltype(auto) referredSide()
			{
				return (*iContainer)[control].side(side);
			} // end method referredSide

			decltype(auto) referredSide() const
			{
				return (*iContainer)[control].side(side);
			} // end method referredSide

			decltype(auto) oppositeSide()
			{
				return (*iContainer)[control].side(geometry::opposite(side));
			} // end method oppositeSide

			decltype(auto) oppositeSide() const
			{
				return (*iContainer)[control].side(geometry::opposite(side));
			} // end method oppositeSide

			decltype(auto) referredControl()
			{
				return (*iContainer)[control];
			} // end method referredControl

			decltype(auto) referredControl() const
			{
				return (*iContainer)[control];
			} // end method referredControl
		}; // end struct ConstraintEndPoint

		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType = std::string>	class Constraint;

		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		using ConstraintBase =
		graphene::DSEL::FrameStack<
			RectangleType,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Selectable, Constraint<RectangleType,ControlContainerType,BorderSize, LineWidth, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Highlightable>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Focusable>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::BoxedAdaptableSizeText, BorderSize>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::MultiCharBorderedRectangle, graphene::FunctionObjects::Textual,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
					  Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Textual, typename TextType::value_type,
								  Constraint<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>*, CaretWidth>,
				const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Textual, typename TextType::value_type,
							const Constraint<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>*, CaretWidth>,
				size_t>,
			graphene::DSEL::Sequence<
				graphene::DSEL::Condition<graphene::FunctionObjects::Selected,
					graphene::DSEL::Sequence<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
						graphene::DSEL::FrameStack<
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>,
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>>,
					graphene::DSEL::Sequence<
						graphene::DSEL::Condition<graphene::FunctionObjects::Highlighted,
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>
						>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>>
				>,
				graphene::DSEL::Condition<graphene::FunctionObjects::Focused,
					graphene::DSEL::FrameStack<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Stippled>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>,
					graphene::DSEL::Frame<graphene::Frames::Renderable::Null>
				>
			>
		>;

		// TODO: decompose Constraint to frames
		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		class Constraint : public ConstraintBase<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			template<typename IDType, typename RationalType>
			struct ParseResult
			{
				// Fields
				std::map<IDType,RationalType> varCoeff;
				RationalType pxSizeCoeff;
				RationalType rhsConstant;
			}; // end struct ParseResult

			using base_type = ConstraintBase<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>;
			using rectangle_type = RectangleType;
			using control_container_type = ControlContainerType;
			using coordinate_type = typename RectangleType::coordinate_type;
			using text_type = TextType;
			using property_tree_type = boost::property_tree::ptree;
			using EndPoint = ConstraintEndPoint<ControlContainerType>;
			using side_type = typename EndPoint::side_type;
			using PartType = std::unique_ptr<IShapePart<coordinate_type>>;
			using ConstPartType = std::unique_ptr<const IShapePart<coordinate_type>>;
			using border_size = BorderSize;
			using line_width = LineWidth;
			using caret_width = CaretWidth;

			/***************
			*    Fields    *
			***************/
		private:
			EndPoint iEndPoints[2];
			coordinate_type iLocalSides[2];
			coordinate_type outerLeft, outerRight;
			coordinate_type leftControlBottom, leftControlTop;
			coordinate_type rightControlBottom, rightControlTop;

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an uninitialized Constraint.
			 */
			Constraint() = default;

			/** Construct a Constraint with the specified sides.
			 */
			Constraint(const EndPoint &endPoint1, const EndPoint &endPoint2, coordinate_type localSide1, coordinate_type localSide2,
				TextType text, coordinate_type textHeight, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0] = endPoint1;
				endPoints()[1] = endPoint2;
				localSides()[0] = std::move(localSide1);
				localSides()[1] = std::move(localSide2);
				this->text() = std::move(text);
				this->textHeight() = textHeight;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;

				updateSides();
			} // end Control constructor

			Constraint(ControlContainerType *container, size_t control1, side_type side1, size_t control2, side_type side2, coordinate_type localSide1, coordinate_type localSide2,
				TextType text, coordinate_type textHeight, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0].container() = container;
				endPoints()[0].control = control1;
				endPoints()[0].side = side1;
				endPoints()[1].container() = container;
				endPoints()[1].control = control2;
				endPoints()[1].side = side2;
				localSides()[0] = std::move(localSide1);
				localSides()[1] = std::move(localSide2);
				this->text() = std::move(text);
				this->textHeight() = textHeight;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;

				updateSides();
			} // end Control constructor

			// TODO: consider deserializing selected and other user interaction states.
			Constraint(const property_tree_type &tree, ControlContainerType *container, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0] = EndPoint(tree.get_child("first-end-point"),container);
				endPoints()[1] = EndPoint(tree.get_child("second-end-point"),container);
				localSides()[0] = tree.get<coordinate_type>("first-local-side");
				localSides()[1] = tree.get<coordinate_type>("second-local-side");
				this->text() = tree.get<TextType>("text");
				this->textHeight() = tree.get<coordinate_type>("text-height");
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;

				updateSides();
			} // end Constraint conversion constructor

			/*************************
			*    Accessor Methods    *
			*************************/
		public:
			EndPoint (&endPoints())[2]
			{
				return iEndPoints;
			} // end method endPoints

			const EndPoint (&endPoints() const)[2]
			{
				return iEndPoints;
			} // end method endPoints

			coordinate_type (&localSides())[2]
			{
				return iLocalSides;
			} // end method localSides

			const coordinate_type (&localSides() const)[2]
			{
				return iLocalSides;
			} // end method localSides

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: consider serializing selected and other user interaction states.
			operator property_tree_type() const
			{
				property_tree_type tree;

				tree.put("text",this->text());
				tree.put("text-height",this->textHeight());
				tree.put_child("first-end-point",endPoints()[0]);
				tree.put_child("second-end-point",endPoints()[1]);
				tree.put("first-local-side",localSides()[0]);
				tree.put("second-local-side",localSides()[1]);

				return tree;
			} // end method operator property_tree_type

			// vertical constraints have horizontal end points and vice versa
			bool isVertical() const
			{
				assert(geometry::isHorizontal(iEndPoints[0].side) == geometry::isHorizontal(iEndPoints[1].side));
				return geometry::isHorizontal(iEndPoints[0].side);
			} // end method isVertical

			bool isHorizontal() const
			{
				assert(geometry::isVertical(iEndPoints[0].side) == geometry::isVertical(iEndPoints[1].side));
				return geometry::isVertical(iEndPoints[0].side);
			} // end method isHorizontal

			/** Returns whether the point (x,y) is inside the rectangle defined by
			 *	the two constraint end points and the two supplementary sides.
			 *	Supplementary sides should be adjacent to constraint text.
			 */
			bool contains(coordinate_type x, coordinate_type y) const
			{
				if(isHorizontal())
					return base_type::contains(x,y);
				else
					return base_type::contains(y,x);
			} // end method contains

			PartType partUnderPoint(coordinate_type x, coordinate_type y)
			{
				if(contains(x,y))
					if(isHorizontal())
						return PartType(new ControlPart<coordinate_type,std::false_type,std::true_type,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return PartType(new ControlPart<coordinate_type,std::true_type,std::false_type,true,false,true,false>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
				else
					return nullptr;
			} // end method partUnderPoint

			ConstPartType partUnderPoint(coordinate_type x, coordinate_type y) const
			{
				if(contains(x,y))
					if(isHorizontal())
						return ConstPartType(new const ControlPart<const coordinate_type,std::false_type,std::true_type,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return ConstPartType(new const ControlPart<const coordinate_type,std::true_type,std::false_type,true,true,true,true>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
				else
					return nullptr;
			} // end method partUnderPoint

			typename base_type::char_type charUnderPoint(coordinate_type x, coordinate_type y)
			{
				if(isHorizontal())
					return base_type::charUnderPoint(x,y);
				else
				{
					auto caret = base_type::charUnderPoint(y,x);
					caret.reset(new graphene::Frames::Renderable::Scaled<graphene::Frames::Renderable::Rotated<typename base_type::concrete_char_type,std::ratio<90>>,std::ratio<-1>,std::ratio<1>>(
						*dynamic_cast<typename base_type::concrete_char_type*>(caret.get()))); // TODO: avoid cast
					return std::move(caret);
				} // end else
			} // end method charUnderPoint

			typename base_type::const_char_type charUnderPoint(coordinate_type x, coordinate_type y) const
			{
				if(isHorizontal())
					return base_type::charUnderPoint(x,y);
				else
				{
					auto caret = base_type::charUnderPoint(y,x);
					caret.reset(new const graphene::Frames::Renderable::Scaled<graphene::Frames::Renderable::Rotated<typename base_type::const_concrete_char_type,std::ratio<90>>,std::ratio<-1>,std::ratio<1>>(
						*dynamic_cast<typename base_type::const_concrete_char_type*>(caret.get()))); // TODO: avoid cast
					return std::move(caret);
				} // end else
			} // end method charUnderPoint

			typename base_type::char_type charAtIndex(typename base_type::index_type i)
			{
				if(isHorizontal())
					return base_type::charAtIndex(i);
				else
				{
					auto caret = base_type::charAtIndex(i);
					caret.reset(new graphene::Frames::Renderable::Scaled<graphene::Frames::Renderable::Rotated<typename base_type::concrete_char_type,std::ratio<90>>,std::ratio<-1>,std::ratio<1>>(
						*dynamic_cast<typename base_type::concrete_char_type*>(caret.get()))); // TODO: avoid cast
					return std::move(caret);
				} // end else
			} // end method charAtIndex

			typename base_type::const_char_type charAtIndex(typename base_type::index_type i) const
			{
				if(isHorizontal())
					return base_type::charAtIndex(i);
				else
				{
					auto caret = base_type::charAtIndex(i);
					caret.reset(new const graphene::Frames::Renderable::Scaled<graphene::Frames::Renderable::Rotated<typename base_type::const_concrete_char_type,std::ratio<90>>,std::ratio<-1>,std::ratio<1>>(
						*dynamic_cast<typename base_type::const_concrete_char_type*>(caret.get()))); // TODO: avoid cast
					return std::move(caret);
				} // end else
			} // end method charAtIndex

			void updateSides()
			{
				// Code is almost identical for horizontal and vertical constraints, so I maintain a common code base.
				// Variable names have been chosen to make sense for horizontal constraints. Reasonable names for
				// vertical constraints can be obtained by substituting left<->bottom and right<->top.
				auto leftEndPoint  = endPoints()[0].referredSide() <  endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];
				auto rightEndPoint = endPoints()[0].referredSide() >= endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];

				outerLeft      = leftEndPoint.referredSide();
				this->bottom() = std::min(localSides()[0],localSides()[1]);
				outerRight     = rightEndPoint.referredSide();
				this->top()    = std::max(localSides()[0],localSides()[1]);

				if(isHorizontal())
				{
					std::tie( leftControlBottom, leftControlTop) = std::minmax( leftEndPoint.referredControl().bottom(), leftEndPoint.referredControl().top());
					std::tie(rightControlBottom,rightControlTop) = std::minmax(rightEndPoint.referredControl().bottom(),rightEndPoint.referredControl().top());
				}
				else
				{
					std::tie( leftControlBottom, leftControlTop) = std::minmax( leftEndPoint.referredControl().left(), leftEndPoint.referredControl().right());
					std::tie(rightControlBottom,rightControlTop) = std::minmax(rightEndPoint.referredControl().left(),rightEndPoint.referredControl().right());
				} // end else

				// render lines consistent with control borders (borders are rendered inside the controls so lines will too)
				this->left() = leftEndPoint.referredSide() <= leftEndPoint.oppositeSide() // innerLeft
							 ? (outerLeft *LineWidth::den + LineWidth::num) / LineWidth::den
							 : (outerLeft *LineWidth::den - LineWidth::num) / LineWidth::den;
				this->right() = rightEndPoint.referredSide() <= rightEndPoint.oppositeSide() // innerRight
							  ? (outerRight*LineWidth::den + LineWidth::num) / LineWidth::den
							  : (outerRight*LineWidth::den - LineWidth::num) / LineWidth::den;

				std::tie(outerLeft,this->left()) = std::pair<coordinate_type,coordinate_type>(std::minmax(outerLeft,this->left())); // won't work without creating a copy...
				std::tie(this->right(),outerRight) = std::pair<coordinate_type,coordinate_type>(std::minmax(this->right(),outerRight)); // one of the variables will get overriden before used.
			} // end method updateSides

			void render() const
			{
				// As with updateSides, code is common, names are chosen for the horizontal case and names for the
				// vertical case can be obtained by substituting left<->bottom and right<->top.
				if(isHorizontal())
					base_type::render();
				else
				{
					// TODO: simplify transformations.
					glPushMatrix();
						glTranslated(this->top(),0,0);
						glScaled(-1,1,1);
						glTranslated(-this->bottom(),0,0);
						glScaled(-1,1,1);
						glRotated(90,0,0,1); // 90 degrees
						base_type::render();
					glPopMatrix();
				} // end if

				glPushAttrib(GL_POLYGON_BIT|GL_TRANSFORM_BIT);
					glPolygonMode(GL_FRONT,GL_FILL);
					glMatrixMode(GL_MODELVIEW);

					if(isVertical())
					{
						glPushMatrix(); // (x,y) -> (y,x)
							glScaled(-1,1,1);
							glRotated(90,0,0,1); // 90 degrees
					} // end if

						// left vertical
						if(this->top() > leftControlTop)
							glRect(outerLeft,leftControlTop,this->left(),this->top());
						if(this->bottom() < leftControlBottom)
							glRect(outerLeft,this->bottom(),this->left(),leftControlBottom);

						// right vertical
						if(this->top() > rightControlTop)
							glRect(this->right(),rightControlTop,outerRight,this->top());
						if(this->bottom() < rightControlBottom)
							glRect(this->right(),this->bottom(),outerRight,rightControlBottom);

						if(this->selected())
						{
							float fgColor[4], bgColor[4];
							glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
								glGetFloatv(GL_CURRENT_COLOR,fgColor);
								glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);
								glClearColor(fgColor[0],fgColor[1],fgColor[2],bgColor[3]);
								glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
						} // end if

							const coordinate_type effectiveTextWidth = this->effectiveTextSize().first;
							const coordinate_type textLeft = this->left() + (this->width() - effectiveTextWidth) / 2;
							const coordinate_type textRight = this->right() - (this->width() - effectiveTextWidth) / 2;

							// left horizontal
							glRect(this->left(),((2*this->bottom() + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),
									textLeft,((2*this->bottom() + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

							// right horizontal
							glRect(textRight,((2*this->bottom() + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),
									this->right(),((2*this->bottom() + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

						if(this->selected())
							glPopAttrib();

					if(isVertical())
						glPopMatrix();
				glPopAttrib();
			} // end method render

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
						if(symbol == "mm")
							result.rhsConstant -= coeff;
						else if(symbol == "px")
							result.pxSizeCoeff += coeff;
						else
							result.varCoeff[symbols->declare(symbol)] += coeff;
					} // end function populate
				}; // end local struct Workaround

				rule<typename TextType::const_iterator,NameType()> identifier = (alpha | char_('_')) > *(alnum | char_('_'));
				rule<typename TextType::const_iterator,RationalType()> coefficient = (char_('-')[_val = -1] | eps[_val = 1]) >
						-(ulong_long[_val *= _1] > -('/' > ulong_long[_val /= _1] | '.' > (*digit)[_val = px::bind(Workaround::decimalPoint,_val,_1)]));
				rule<typename TextType::const_iterator,space_type> constraint = (coefficient > identifier)[px::bind(Workaround::populate,_1,_2,px::ref(result),px::ref(symbols))] % '+';

				auto begin = this->text().begin();
				auto end = this->text().end();

				bool success = phrase_parse(begin,end,constraint,space);
				if(!success || begin != end)
					throw std::runtime_error("Error parsing constraint!");

				return result;
			} // end method parse
		}; // end class Constraint

	} // end namespace Controls

} // end namespace GUIModel

#endif // CONSTRAINT_H
