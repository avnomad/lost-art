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
			graphene::DSEL::Frame<graphene::Frames::UniformlyBordered, typename RectangleType::coordinate_type>,
			graphene::DSEL::Frame<graphene::Frames::Selectable, Constraint<RectangleType,ControlContainerType,BorderSize, LineWidth, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Highlightable>,
			graphene::DSEL::Frame<graphene::Frames::Focusable>,
			graphene::DSEL::Frame<graphene::Frames::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::BoxedAdaptableSizeText, BorderSize>,
			graphene::DSEL::Frame<graphene::Frames::MultiCharBorderedRectangle, graphene::FunctionObjects::Textual,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
					  Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Textual, typename TextType::value_type,
								  Constraint<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>*, CaretWidth>,
				const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Textual, typename TextType::value_type,
							const Constraint<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>*, CaretWidth>,
				size_t>,
			graphene::DSEL::Sequence<
				graphene::DSEL::Condition<graphene::FunctionObjects::Selected,
					graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
					graphene::DSEL::Condition<graphene::FunctionObjects::Highlighted,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>
					>
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
				TextType text, coordinate_type textHeight, coordinate_type borderSize = 0, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0] = endPoint1;
				endPoints()[1] = endPoint2;
				localSides()[0] = std::move(localSide1);
				localSides()[1] = std::move(localSide2);
				this->text() = std::move(text);
				this->textHeight() = textHeight;
				this->borderSize() = borderSize;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;
			} // end Control constructor

			Constraint(ControlContainerType *container, size_t control1, side_type side1, size_t control2, side_type side2, coordinate_type localSide1, coordinate_type localSide2,
				TextType text, coordinate_type textHeight, coordinate_type borderSize = 0, bool selected = false, bool highlighted = false, bool focused = false)
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
				this->borderSize() = borderSize;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;
			} // end Control constructor

			// TODO: consider deserializing selected and other user interaction states.
			Constraint(const property_tree_type &tree, ControlContainerType *container,	coordinate_type borderSize = 0, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0] = EndPoint(tree.get_child("first-end-point"),container);
				endPoints()[1] = EndPoint(tree.get_child("second-end-point"),container);
				localSides()[0] = tree.get<coordinate_type>("first-local-side");
				localSides()[1] = tree.get<coordinate_type>("second-local-side");
				this->text() = tree.get<TextType>("text");
				this->textHeight() = tree.get<coordinate_type>("text-height");
				this->borderSize() = borderSize;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;
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
				auto left   = isHorizontal() ? std::min(iEndPoints[0].referredSide(),iEndPoints[1].referredSide()) : std::min(iLocalSides[0],iLocalSides[1]);
				auto bottom = isVertical()   ? std::min(iEndPoints[0].referredSide(),iEndPoints[1].referredSide()) : std::min(iLocalSides[0],iLocalSides[1]);
				auto right  = isHorizontal() ? std::max(iEndPoints[0].referredSide(),iEndPoints[1].referredSide()) : std::max(iLocalSides[0],iLocalSides[1]);
				auto top    = isVertical()   ? std::max(iEndPoints[0].referredSide(),iEndPoints[1].referredSide()) : std::max(iLocalSides[0],iLocalSides[1]);
				return left <= x && x <= right && bottom <= y && y <= top;
			} // end method contains

			PartType partUnderPoint(coordinate_type x, coordinate_type y)
			{
				if(contains(x,y))
					if(isHorizontal())
						return PartType(new ControlPart<coordinate_type,std::false_type,std::true_type,false,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return PartType(new ControlPart<coordinate_type,std::true_type,std::false_type,false,true,false,true,false>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
				else
					return nullptr;
			} // end method partUnderPoint

			ConstPartType partUnderPoint(coordinate_type x, coordinate_type y) const
			{
				if(contains(x,y))
					if(isHorizontal())
						return ConstPartType(new const ControlPart<coordinate_type,std::false_type,std::true_type,true,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return ConstPartType(new const ControlPart<coordinate_type,std::true_type,std::false_type,true,true,true,true,true>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
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

			void render() const
			{
				// TODO: remove text rendering code and use available frames for same effect.
				graphene::FunctionObjects::GlutStrokeFontEngine fontEngine;

				if(isHorizontal())
					base_type::render();
				else
				{
					glPushMatrix();
						glScaled(-1,1,1);
						glRotated(90,0,0,1); // 90 degrees
						base_type::render();
					glPopMatrix();
				} // end if

				glPushAttrib(GL_POLYGON_BIT|GL_TRANSFORM_BIT);
					glPolygonMode(GL_FRONT,GL_FILL);
					glMatrixMode(GL_MODELVIEW);
					if(isHorizontal())
					{
						coordinate_type left   = std::min(endPoints()[0].referredSide(),endPoints()[1].referredSide());
						coordinate_type bottom = std::min(localSides()[0],localSides()[1]);
						coordinate_type right  = std::max(endPoints()[0].referredSide(),endPoints()[1].referredSide());
						coordinate_type top    = std::max(localSides()[0],localSides()[1]);

						auto leftEndPoint  = endPoints()[0].referredSide() <  endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];
						auto rightEndPoint = endPoints()[0].referredSide() >= endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];

						coordinate_type leftControlLeft, leftControlBottom, leftControlRight, leftControlTop, rightControlLeft, rightControlBottom, rightControlRight, rightControlTop;
						std::tie(leftControlLeft,leftControlRight) = std::minmax(leftEndPoint.referredControl().left(),leftEndPoint.referredControl().right());
						std::tie(leftControlBottom,leftControlTop) = std::minmax(leftEndPoint.referredControl().bottom(),leftEndPoint.referredControl().top());
						std::tie(rightControlLeft,rightControlRight) = std::minmax(rightEndPoint.referredControl().left(),rightEndPoint.referredControl().right());
						std::tie(rightControlBottom,rightControlTop) = std::minmax(rightEndPoint.referredControl().bottom(),rightEndPoint.referredControl().top());

						// render lines consistent with control borders (borders are rendered inside the controls)
						coordinate_type innerLeft  = leftControlLeft == leftEndPoint.referredSide() ?
														(left *LineWidth::den + LineWidth::num) / LineWidth::den : (left *LineWidth::den - LineWidth::num) / LineWidth::den;
						coordinate_type innerRight = rightControlLeft == rightEndPoint.referredSide() ?
														(right*LineWidth::den + LineWidth::num) / LineWidth::den : (right*LineWidth::den - LineWidth::num) / LineWidth::den;
						std::tie(left,innerLeft) = std::pair<coordinate_type,coordinate_type>(std::minmax(left,innerLeft)); // won't work without creating a copy...
						std::tie(innerRight,right) = std::pair<coordinate_type,coordinate_type>(std::minmax(innerRight,right)); // one of the variables will get overriden before used.

						// left vertical
						if(top > leftControlTop)
							glRect(left,leftControlTop,innerLeft,top);
						if(bottom < leftControlBottom)
							glRect(left,bottom,innerLeft,leftControlBottom);

						// right vertical
						if(top > rightControlTop)
							glRect(innerRight,rightControlTop,right,top);
						if(bottom < rightControlBottom)
							glRect(innerRight,bottom,right,rightControlBottom);

						if(this->selected())
						{
							float fgColor[4], bgColor[4];
							glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
								glGetFloatv(GL_CURRENT_COLOR,fgColor);
								glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);
								glClearColor(fgColor[0],fgColor[1],fgColor[2],bgColor[3]);
								glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
						} // end if

						// text
						// TODO: remove this monstrosity!
						const_cast<coordinate_type&>(this->left())   = innerLeft;
						const_cast<coordinate_type&>(this->bottom()) = bottom;
						const_cast<coordinate_type&>(this->right())  = innerRight;
						const_cast<coordinate_type&>(this->top())    = top;
						coordinate_type effectiveTextWidth,effectiveTextHeight;
						std::tie(effectiveTextWidth,effectiveTextHeight) = this->effectiveTextSize();
						coordinate_type textLeft = this->left() + (this->width() - effectiveTextWidth) / 2;
						coordinate_type textBottom = this->bottom() + (this->height() - effectiveTextHeight) / 2;

						glPushMatrix();
							glTranslated(textLeft,textBottom,0); // center text in inner rectangle
							glScaled(effectiveTextWidth / fontEngine.stringWidth(this->text()) , effectiveTextHeight / fontEngine.fontHeight() , 1);
							glTranslated(0,fontEngine.fontBelowBaseLine(),0);
							fontEngine.render(this->text());
						glPopMatrix();

						// left horizontal
						glRect(innerLeft,((2*bottom + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),textLeft,((2*bottom + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

						// right horizontal
						auto textRight = this->right() - (this->width() - effectiveTextWidth) / 2;
						glRect(textRight,((2*bottom + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),innerRight,((2*bottom + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

						if(this->selected())
							glPopAttrib();
					}
					else
					{
						coordinate_type left   = std::min(localSides()[0],localSides()[1]);
						coordinate_type bottom = std::min(endPoints()[0].referredSide(),endPoints()[1].referredSide());
						coordinate_type right  = std::max(localSides()[0],localSides()[1]);
						coordinate_type top    = std::max(endPoints()[0].referredSide(),endPoints()[1].referredSide());

						auto bottomEndPoint  = endPoints()[0].referredSide() <  endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];
						auto topEndPoint     = endPoints()[0].referredSide() >= endPoints()[1].referredSide() ? endPoints()[0] : endPoints()[1];

						coordinate_type bottomControlLeft, bottomControlBottom, bottomControlRight, bottomControlTop, topControlLeft, topControlBottom, topControlRight, topControlTop;
						std::tie(bottomControlLeft,bottomControlRight) = std::minmax(bottomEndPoint.referredControl().left(),bottomEndPoint.referredControl().right());
						std::tie(bottomControlBottom,bottomControlTop) = std::minmax(bottomEndPoint.referredControl().bottom(),bottomEndPoint.referredControl().top());
						std::tie(topControlLeft,topControlRight) = std::minmax(topEndPoint.referredControl().left(),topEndPoint.referredControl().right());
						std::tie(topControlBottom,topControlTop) = std::minmax(topEndPoint.referredControl().bottom(),topEndPoint.referredControl().top());

						// render lines consistent with control borders (borders are rendered inside the controls)
						coordinate_type innerBottom  = bottomControlBottom == bottomEndPoint.referredSide() ?
														(bottom *LineWidth::den + LineWidth::num) / LineWidth::den : (bottom *LineWidth::den - LineWidth::num) / LineWidth::den;
						coordinate_type innerTop = topControlBottom == topEndPoint.referredSide() ?
														(top*LineWidth::den + LineWidth::num) / LineWidth::den : (top*LineWidth::den - LineWidth::num) / LineWidth::den;
						std::tie(bottom,innerBottom) = std::pair<coordinate_type,coordinate_type>(std::minmax(bottom,innerBottom)); // won't work without creating a copy...
						std::tie(innerTop,top) = std::pair<coordinate_type,coordinate_type>(std::minmax(innerTop,top)); // one of the variables will get overriden before used.

						// bottom horizontal
						if(right > bottomControlRight)
							glRect(bottomControlRight,bottom,right,innerBottom);
						if(left < bottomControlLeft)
							glRect(left,bottom,bottomControlLeft,innerBottom);

						// top horizontal
						if(right > topControlRight)
							glRect(topControlRight,innerTop,right,top);
						if(left < topControlLeft)
							glRect(left,innerTop,topControlLeft,top);

						if(this->selected())
						{
							float fgColor[4], bgColor[4];
							glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
								glGetFloatv(GL_CURRENT_COLOR,fgColor);
								glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);
								glClearColor(fgColor[0],fgColor[1],fgColor[2],bgColor[3]);
								glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
						} // end if

						// text
						// TODO: remove this monstrosity!
						const_cast<coordinate_type&>(this->left())   = innerBottom;
						const_cast<coordinate_type&>(this->bottom()) = left;
						const_cast<coordinate_type&>(this->right())  = innerTop;
						const_cast<coordinate_type&>(this->top())    = right;
						coordinate_type effectiveTextWidth,effectiveTextHeight;
						std::tie(effectiveTextWidth,effectiveTextHeight) = this->effectiveTextSize();
						coordinate_type textLeft = this->bottom() + (this->height() - effectiveTextHeight) / 2;
						coordinate_type textBottom = this->left() + (this->width() - effectiveTextWidth) / 2;

						glPushMatrix();
							glTranslated(textLeft,textBottom,0); // center text in inner rectangle
							glScaled(effectiveTextHeight / fontEngine.fontHeight() , effectiveTextWidth / fontEngine.stringWidth(this->text()) , 1);
							glRotated(90,0,0,1); // rotate by 90 degrees
							glTranslated(0,-fontEngine.fontAboveBaseLine(),0);
							fontEngine.render(this->text());
						glPopMatrix();

						// bottom vertical
						glRect(((2*left + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),innerBottom,((2*left + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den),textBottom);

						// top vertical
						auto textTop = this->right() - (this->width() - effectiveTextWidth) / 2;
						glRect(((2*left + this->height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),textTop,((2*left + this->height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den),innerTop);

						if(this->selected())
							glPopAttrib();
					} // end else
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
