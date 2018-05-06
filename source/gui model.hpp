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

#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <map>
#include <array>
#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include <cassert>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/rational.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <Eigen/Dense>

#include "geometry.hpp"
#include "graphene.hpp"
#include "symbol table.hpp"
#include "portable type names.hpp"
#include "linear system solving.hpp"

namespace GUIModel
{
	/** Will follow the Frames architecture from Graphene.
	 */
	namespace Bases
	{

	} // end namespace Bases

	namespace Frames
	{

	} // end namespace Frames

	namespace Controls
	{
		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string> struct Button;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using ButtonBase = graphene::DSEL::FrameStack<
			RectangleType,
			graphene::DSEL::Frame<graphene::Frames::UniformlyBordered, typename RectangleType::coordinate_type>,
			graphene::DSEL::Frame<graphene::Frames::Pressable, Button<RectangleType, BorderSize, Margin, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Highlightable, Button<RectangleType, BorderSize, Margin, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::BoxedAdaptableSizeText, Margin>
		>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using ButtonRenderableBase =
			graphene::Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
				graphene::Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
					graphene::Frames::Renderable::Colorblind::FilledRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>>,
					graphene::Frames::Renderable::Colorblind::InversedColor<graphene::Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,graphene::FunctionObjects::Textual,Margin>>>,
				graphene::Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
					graphene::Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>,std::ratio<0>>,
						graphene::Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,graphene::FunctionObjects::Textual,Margin>>,
					graphene::Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>,BorderSize>,
						graphene::Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,graphene::FunctionObjects::Textual,Margin>>,
					graphene::FunctionObjects::Highlighted>,
				graphene::FunctionObjects::Pressed>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		struct Button : public ButtonRenderableBase<RectangleType, BorderSize, Margin, TextType>
		{
			using base_type = ButtonRenderableBase<RectangleType, BorderSize, Margin, TextType>;
			Button() = default;
			using base_type::base_type;
		}; // end struct Button

		template<typename CoordinateType>
		using IShapePart = graphene::DSEL::FrameStack<
			graphene::Bases::Empty,
			graphene::DSEL::Frame<graphene::Bases::Destructible>,
			graphene::DSEL::Frame<graphene::Bases::Movable, CoordinateType>,
			graphene::DSEL::Frame<graphene::Bases::Rectangular>, // TODO: move rectangular back to concrete control when the screen special case is handled
			graphene::DSEL::Frame<graphene::Bases::Containing>,
			graphene::DSEL::Frame<graphene::Bases::Renderable>
		>;

		/** Const instances should be constant and non-const instances should be non-constant
		 */
		template<typename CoordinateType, typename horizontallyMovable, typename verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		using ControlPart = graphene::DSEL::FrameStack<
			IShapePart<CoordinateType>,
			graphene::DSEL::Frame<graphene::Frames::Movable::Rectangular>,
			graphene::DSEL::Frame<graphene::Frames::Movable::HVMovable, horizontallyMovable, verticallyMovable>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Stippled>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>,
			graphene::DSEL::Frame<graphene::Frames::Adapting::Rectangular, geometry::RefRectangle<CoordinateType, constant, leftRef, bottomRef, rightRef, topRef>>
		>;

		template<typename CoordinateType, typename Const, typename CharType>
		using ICaret = graphene::DSEL::FrameStack<
			graphene::Bases::Empty,
			graphene::DSEL::Frame<graphene::Bases::Destructible>,
			graphene::DSEL::Frame<graphene::Bases::CaretLike, Const, CharType>,
			graphene::DSEL::Frame<graphene::Bases::EventHandling::KeyboardAndMouse, CoordinateType>, // TODO: handle in parent
			graphene::DSEL::Frame<graphene::Bases::Renderable>
		>;

		template<typename CoordinateType, typename Const, typename TextConceptMap, typename CharType, typename PointerType, typename Width>
		using Caret = graphene::DSEL::FrameStack<
			ICaret<CoordinateType,Const,CharType>,
			graphene::DSEL::Frame<graphene::Frames::Indexing, size_t>,
			graphene::DSEL::Frame<graphene::Frames::Offset>,
			graphene::DSEL::Frame<graphene::Frames::Pointing, PointerType>,
			graphene::DSEL::Frame<graphene::Frames::IndirectCaretLike, TextConceptMap, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::EventHandling::KeyboardAndMouseStub>, // TODO: remove...
			graphene::DSEL::Frame<graphene::Frames::EventHandling::CaretLike>, // ...and handle in parent
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::IndirectCaret, TextConceptMap, Width>
		>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType = std::string>	class Control;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		using ControlBase =
			graphene::Frames::MultiPartBorderedRectangle<graphene::DSEL::FrameStack<
				RectangleType,
				graphene::DSEL::Frame<graphene::Frames::UniformlyBordered, typename RectangleType::coordinate_type>,
				graphene::DSEL::Frame<graphene::Frames::Selectable, Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
				graphene::DSEL::Frame<graphene::Frames::Highlightable, Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
				graphene::DSEL::Frame<graphene::Frames::Focusable, Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
				graphene::DSEL::Frame<graphene::Frames::Movable::Rectangular>,
				graphene::DSEL::Frame<graphene::Frames::Named, TextType>,
				graphene::DSEL::Frame<graphene::Frames::SizedName, graphene::FunctionObjects::GlutStrokeFontEngine>,
				graphene::DSEL::Frame<graphene::Frames::BoxedAdaptableSizeName, Margin>,
				graphene::DSEL::Frame<graphene::Frames::MultiCharBorderedRectangle, graphene::FunctionObjects::Named,
					std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
					std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
						  Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Named, typename TextType::value_type,
									  Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
					const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Named, typename TextType::value_type,
								const Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
					size_t>
			>, ControlPart, std::unique_ptr<      IShapePart<typename RectangleType::coordinate_type>>,
							std::unique_ptr<const IShapePart<typename RectangleType::coordinate_type>>>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		using ControlRenderableBase =
			graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
				graphene::Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
					graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
						graphene::Frames::Renderable::Colorblind::FilledRectangle<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
						graphene::Frames::Renderable::Colorblind::InversedColor<
							graphene::Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Named,Margin>>>,
					graphene::Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
						graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
							graphene::Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,std::ratio<0>>,
							graphene::Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Named,Margin>>,
						graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
							graphene::Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,BorderSize>,
							graphene::Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Named,Margin>>,
						graphene::FunctionObjects::Highlighted>,
					graphene::FunctionObjects::Selected>,
				graphene::Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
					graphene::Frames::Renderable::Colorblind::InversedColor<graphene::Frames::Renderable::Stippled<
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,std::ratio<0>>>>,
					graphene::Frames::Renderable::Null<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
					graphene::FunctionObjects::Focused>>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		class Control : public ControlRenderableBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ControlRenderableBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>;
			using coordinate_type = typename Control::coordinate_type;
			using rectangle_type = RectangleType;
			using property_tree_type = boost::property_tree::ptree;

			// TODO: add color
			// TODO: add type

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an uninitialized Control.
			 */
			Control() = default;

			using base_type::base_type;

			// TODO: consider serializing selected and other user interaction states.
			Control(const property_tree_type &tree, bool selected = false, bool highlighted = false, bool focused = false)
			{
				this->left() = tree.get<coordinate_type>("sides.left");
				this->bottom() = tree.get<coordinate_type>("sides.bottom");
				this->right() = tree.get<coordinate_type>("sides.right");
				this->top() = tree.get<coordinate_type>("sides.top");
				this->borderSize() = tree.get<coordinate_type>("borderSize");
				this->name() = tree.get<TextType>("name");
				this->nameHeight() = tree.get<coordinate_type>("nameHeight");
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;
			} // end Control conversion constructor

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: consider deserializing selected and other user interaction states.
			operator property_tree_type() const
			{
				property_tree_type tree;

				tree.put("sides.left",this->left());
				tree.put("sides.bottom",this->bottom());
				tree.put("sides.right",this->right());
				tree.put("sides.top",this->top());
				tree.put("borderSize",this->borderSize());
				tree.put("name",this->name());
				tree.put("nameHeight",this->nameHeight());
				return tree;
			} // end method operator property_tree_type
		}; // end class Control

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType = std::string> struct TextBox;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		using TextBoxBase = graphene::DSEL::FrameStack<
			RectangleType,
			graphene::DSEL::Frame<graphene::Frames::UniformlyBordered, typename RectangleType::coordinate_type>,
			graphene::DSEL::Frame<graphene::Frames::Focusable, TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Highlightable, TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Movable::Rectangular>,
			graphene::DSEL::Frame<graphene::Frames::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::BoxedAdaptableSizeText, Margin>,
			graphene::DSEL::Frame<graphene::Frames::MultiCharBorderedRectangle, graphene::FunctionObjects::Textual,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
				      Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Textual, typename TextType::value_type,
								  TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
				const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Textual, typename TextType::value_type,
							const TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
				size_t>
		>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		using TextBoxRenderableBase =
			graphene::Frames::Renderable::Conditional<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
				graphene::Frames::Renderable::Sequential<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
					graphene::Frames::Renderable::Colorblind::FilledRectangle<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
					graphene::Frames::Renderable::Colorblind::InversedColor<
						graphene::Frames::Renderable::Colorblind::BoxedText<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Textual,Margin>>>,
				graphene::Frames::Renderable::Conditional<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
					graphene::Frames::Renderable::Sequential<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,std::ratio<0>>,
						graphene::Frames::Renderable::Colorblind::BoxedText<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Textual,Margin>>,
					graphene::Frames::Renderable::Sequential<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,BorderSize>,
						graphene::Frames::Renderable::Colorblind::BoxedText<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,graphene::FunctionObjects::Textual,Margin>>,
					graphene::FunctionObjects::Highlighted>,
				graphene::FunctionObjects::Focused>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		struct TextBox : public TextBoxRenderableBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>
		{
			using base_type = TextBoxRenderableBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>;
			TextBox() = default;
			using base_type::base_type;
		}; // end struct TextBox

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
			graphene::DSEL::Frame<graphene::Frames::Highlightable, Constraint<RectangleType,ControlContainerType,BorderSize, LineWidth, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Focusable, Constraint<RectangleType,ControlContainerType,BorderSize, LineWidth, CaretWidth, TextType>>,
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
				size_t>
		>;

		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		using ConstraintRenderableBase =
			graphene::Frames::Renderable::Sequential<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
				graphene::Frames::Renderable::Conditional<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
					graphene::Frames::Renderable::Colorblind::FilledRectangle<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>>,
					graphene::Frames::Renderable::Conditional<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,BorderSize>,
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,std::ratio<0>>,
						graphene::FunctionObjects::Highlighted>,
					graphene::FunctionObjects::Selected>,
				graphene::Frames::Renderable::Conditional<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
					graphene::Frames::Renderable::Colorblind::InversedColor<graphene::Frames::Renderable::Stippled<
						graphene::Frames::Renderable::Colorblind::BorderedRectangle<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,BorderSize>>>,
					graphene::Frames::Renderable::Null<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>>,
				graphene::FunctionObjects::Focused>>;

		// TODO: decompose Constraint to frames
		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		class Constraint : public ConstraintRenderableBase<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>
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

			using base_type = ConstraintRenderableBase<RectangleType, ControlContainerType, BorderSize, LineWidth, CaretWidth, TextType>;
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


		/**
		 * Invokes the supplied function on the supplied arguments and prints any exceptions to the standard error stream.
		 *
		 * The return value and type are those of the invoked function if no exceptions are thrown.
		 */
		template<typename Function, typename... Arguments>
		decltype(auto) reportExceptions(Function &&function, Arguments &&...arguments)
		{
			try {
				return std::forward<Function>(function)(std::forward<Arguments>(arguments)...);
			} catch(const std::exception &exception) {
				std::cerr << "Error: " << exception.what() << std::endl;
			} // end catch
		} // end method reportExceptions

		// TODO: split button and control handling code and move them into separate classes. ButtonManager?
		// TODO: split Model class into frames.
		template<typename CoordinateType, typename TextType = std::string>
		class Model : public geometry::Rectangle<CoordinateType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			// parameters
			using coordinate_type = CoordinateType;
			using text_type = TextType;

			// components
			using control_type = Control<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType>;
			using constraint_type = Constraint<geometry::Rectangle<CoordinateType>,std::vector<control_type>,std::ratio<1,2>,std::ratio<1,2>,std::ratio<1>,TextType>;
			using button_type = Button<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,TextType>;
			using text_box_type = TextBox<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType>;

			// containers
			using control_container_type = std::vector<control_type>;
			using constraint_container_type = std::vector<constraint_type>;
			using button_container_type = std::vector<std::pair<button_type,std::function<void()>>>;

			// iterators
			using control_iterator = typename control_container_type::iterator;
			using constraint_iterator = typename constraint_container_type::iterator;
			using button_iterator = typename button_container_type::iterator;

			// shorthands
			using property_tree_type = boost::property_tree::ptree;
			using char_type = typename TextType::value_type;
			using end_point_type = typename constraint_type::EndPoint;
			using side_type = typename end_point_type::side_type;

			/***************
			*    Fields    *
			***************/
		public: // TODO: make private and add methods to manipulate...

			static constexpr int margin = 10; // in millimeters
			static constexpr int buttonHeight = 15;  // in millimeters
			static constexpr int buttonWidth = static_cast<int>(buttonHeight*1.61803)/* golden ratio */;  // in millimeters

			static constexpr int borderSize = 2;
			static constexpr int buttonTextHeight = 10;
			static constexpr int textBoxTextHeight = buttonTextHeight;
			static constexpr int controlTextHeight = buttonTextHeight;
			static constexpr int constraintTextHeight = 7;
			static constexpr int constraintThickness = 7;

			// Colors
			std::array<float,4> buttonColour = {{0.94f, 0.9f, 0.55f, 1.0f}}; // khaki
			std::array<float,4> controlColour = {{1.0f, 0.75f, 0.0f, 1.0f}};	// gold
			std::array<float,4> constraintColour = {{0.0f, 0.5f, 0.0f, 1.0f}}; // dark green

			// TODO: change vector to list for faster operations, updating ConstraintEndPoints.
			control_container_type controls;
			constraint_container_type constraints;
			button_container_type buttons;
			text_box_type tbFileName;

			button_iterator highlightedButton;
			button_iterator pressedButton;

			typename control_container_type::reverse_iterator highlightedControl;
			typename control_container_type::reverse_iterator selectedControl;
			typename control_container_type::reverse_iterator focusedControl;

			constraint_iterator highlightedConstraint;
			constraint_iterator selectedConstraint;
			constraint_iterator focusedConstraint;

			std::unique_ptr<IShapePart<CoordinateType>> selectedPart;

			std::unique_ptr<ICaret<CoordinateType, std::false_type, char_type>> caret;

			coordinate_type lastX; // TODO: consider making lastPressX (this will require saving initial movable control position as well)
			coordinate_type lastY;
			unsigned long long controlIndex;

			bool firstResize; // GLUT workaround (can't do first resize in constructor)
			bool createOnMove;
			bool inConstraintAddMode;

			std::unique_ptr<IShapePart<CoordinateType>> endPoint1;
			std::unique_ptr<IShapePart<CoordinateType>> endPoint2;
			size_t control1;
			size_t control2;
			side_type side1;
			side_type side2;

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an empty Model.
			 */
			Model()
				:tbFileName(textBoxTextHeight,"last session.las",borderSize,0,0,0,0),
				controlIndex(0),firstResize(true),createOnMove(false),inConstraintAddMode(false)
			{
				// initialize buttons
				buttons.emplace_back(button_type(buttonTextHeight,"Load",borderSize,0,0,0,0),[this](){load(tbFileName.text());});
				buttons.emplace_back(button_type(buttonTextHeight,"Save",borderSize,0,0,0,0),[this](){save(tbFileName.text());});
				// TODO: allow customizable header and executable names
				buttons.emplace_back(button_type(buttonTextHeight,"Compile",borderSize,0,0,0,0),
					[this](){compile<boost::rational<long long>,float,int,TextType>("application customization.hpp", "build");});
				buttons.emplace_back(button_type(buttonTextHeight,"Run",borderSize,0,0,0,0),
					[this](){run("build");});

				// initialize controls
				controls.emplace_back(controlTextHeight,"Screen",borderSize,0,0,0,0);

				// initialize pointers and iterators
				highlightedButton = buttons.end();
				pressedButton = buttons.end();

				clearControlIterators();
				clearConstraintIterators();

				endPoint1 = nullptr;
				endPoint2 = nullptr;
				selectedPart = nullptr;
				caret = nullptr;
			} // end Model constructor

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: remove when converted to use std::list
			void clearControlIterators()
			{
				highlightedControl = selectedControl = focusedControl = controls.rend();
			} // end method clearControlIterators

			void clearConstraintIterators()
			{
				highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
			} // end method clearConstraintIterators

			void clear()
			{
				controls.clear();
				constraints.clear();

				clearControlIterators();
				clearConstraintIterators();

				selectedPart = nullptr;
				caret = nullptr; // TODO: change to only become null if not pointing to text box.
			} // end method clear

			auto eraseControl(control_iterator control)
			{
				size_t index = control - controls.begin();
				auto result = controls.erase(control);
				// TODO: replace with STL algorithms
				for(auto constraint = constraints.begin() ; constraint < constraints.end() ; )
					if(constraint->endPoints()[0].control == index || constraint->endPoints()[1].control == index)
						constraint = constraints.erase(constraint);
					else
					{
						if(constraint->endPoints()[0].control > index)
							--constraint->endPoints()[0].control;
						if(constraint->endPoints()[1].control > index)
							--constraint->endPoints()[1].control;
						++constraint;
					} // end else
				return result;
			} // end method eraseControl

			auto eraseConstraint(constraint_iterator constraint)
			{
				return constraints.erase(constraint);
			} // end method eraseConstraint

			void dehighlightAll()
			{
				if(highlightedButton != buttons.end())
				{
					highlightedButton->first.dehighlight();
					highlightedButton = buttons.end();
				} // end if
				if(highlightedControl != controls.rend())
				{
					highlightedControl->dehighlight();
					highlightedControl = controls.rend();
				} // end if
				if(highlightedConstraint != constraints.end())
				{
					highlightedConstraint->dehighlight();
					highlightedConstraint = constraints.end();
				} // end if
			} // end method dehighlightAll

			void deselectAll()
			{
				if(selectedConstraint != constraints.end())
				{
					selectedConstraint->deselect();
					selectedConstraint = constraints.end();
				} // end if
				if(selectedControl != controls.rend())
				{
					selectedControl->deselect();
					selectedControl = controls.rend();
				} // end if
				selectedPart = nullptr;
			} // end method deselectAll

			void unfocusAll()
			{
				if(focusedConstraint != constraints.end())
				{
					focusedConstraint->unfocus();
					focusedConstraint = constraints.end();
				} // end if
				if(focusedControl != controls.rend())
				{
					focusedControl->unfocus();
					focusedControl = controls.rend();
				} // end if
				tbFileName.unfocus();
				caret = nullptr;
			} // end method unfocusAll

			// TODO: check that there is at least one control (the screen) and that all constraints refer to
			// existent controls! Also that endpoints are consistent.
			void load(const std::string &fileName)
			{
				clear();
				property_tree_type tree, emptyTree;

				boost::property_tree::read_xml(fileName,tree);

				for(const auto &control : tree.get_child("gui-model.controls"))
					controls.emplace_back(control.second);

				for(const auto &constraint : tree.get_child("gui-model.constraints",emptyTree))
					constraints.emplace_back(constraint.second,&controls);

				clearControlIterators();
				clearConstraintIterators();
			} // end method load

			void save(const std::string &fileName) const
			{
				property_tree_type tree;

				for(const auto &control : controls)
					tree.add_child("gui-model.controls.control",control);

				for(const auto &constraint : constraints)
					tree.add_child("gui-model.constraints.constraint",constraint);

				using namespace boost::property_tree;
				write_xml(fileName, tree, std::locale(), xml_writer_make_settings<std::string>('\t', 1));
			} // end method save


			template<typename RationalType, typename IDType /* = int */, typename NameType /* = std::string */> // current compiler version does not support default arguments
			Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> generateSystemMatrix() const
			{
				auto symbols = std::make_shared<Symbolic::Common::SymbolTable<NameType,IDType>>();
				std::vector<typename constraint_type::template ParseResult<IDType,RationalType>> parseResults;

				for(const auto &constraint : constraints)
					parseResults.push_back(constraint.template parse<RationalType,IDType,NameType>(symbols));
				// TODO: optimize to not include unknown constants that are not present.

				Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> result;
				result.setZero(constraints.size(),4*controls.size() + symbols->size() + 1); // 4*(nControls-1) + nSymbols + 4 + 1
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
							result(i,4*(constraint.endPoints()[ep].control-1) + size_t(constraint.endPoints()[ep].side)) += coeffs[ep];
						else if(constraint.endPoints()[ep].side == geometry::RectangleSide::RIGHT || constraint.endPoints()[ep].side == geometry::RectangleSide::TOP)
							result(i,4*(controls.size()-1) + symbols->size() + size_t(constraint.endPoints()[ep].side) - 2) += coeffs[ep];
					} // end for

					for(const auto &var : parseResults[i].varCoeff)
						result(i,4*(controls.size()-1) + var.first) = var.second;

					size_t offset = constraint.endPoints()[0].side == geometry::RectangleSide::LEFT || constraint.endPoints()[0].side == geometry::RectangleSide::RIGHT ? 0 : 1;
					result(i,4*(controls.size()-1) + symbols->size() + 2 + offset) = parseResults[i].pxSizeCoeff;

					result(i,4*(controls.size()-1) + symbols->size() + 4) = parseResults[i].rhsConstant; // already moved to rhs during parsing

					++i;
				} // end foreach

				return result;
			} // end method generateSystemMatrix


			/** Takes the solution of a linear system (in the form of a vector space base and point space offset
			 *	generating the solutions) describing a GUI and generates C++ code for an application
			 *	that implements that GUI. The output code is intended to be saved in a file and #included be a
			 *	suitable application-template .cpp file. Generated code is not tied to a specific GUI toolkit.
			 */
			template<typename RationalType, typename AppCoordType>
			void outputCppApp(const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &base, const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &offset, std::ostream &output) const
			{
				output << "// NO INCLUDE GUARD!!!\n\n";
				output << "std::vector<geometry::Rectangle<" << name<AppCoordType>() << ">> controls(" << controls.size()-1 << ");\n\n";
				output << "void updateCoordinates(" << name<AppCoordType>() << " screenWidth, " << name<AppCoordType>() << " screenHeight, "
					   << name<AppCoordType>() << " pixelWidth, " << name<AppCoordType>() << " pixelHeight)\n";
				output << "{\n";

				std::string unknownConstants[4] = {"screenWidth","screenHeight","pixelWidth","pixelHeight"};
				for(size_t i = 0 ; i < controls.size()-1 ; ++i)
				{
					for(size_t j = 0 ; j < 4 ; ++j)
					{ // TODO: optimize for constant functions (assign at application initialization)
						output << "\tcontrols[" << i << "].sides()[" << j << "] = ";
						bool nonZeroBefore = false;
						for(size_t uc = 0 ; uc < 4 ; ++uc)
							if(base(4*i+j,uc) != 0)
							{
								if(nonZeroBefore)
									output << (base(4*i+j,uc).numerator() > 0 ? " + " : " - ");
								else if(base(4*i+j,uc).numerator() < 0)
									output << '-';
								nonZeroBefore = true;

								if(std::abs(base(4*i+j,uc).numerator()) != 1)
									output << std::abs(base(4*i+j,uc).numerator()) << '*';
								output << unknownConstants[uc];
								if(base(4*i+j,uc).denominator() != 1)
									output << '/' << base(4*i+j,uc).denominator();
							} // end if
						if(offset(4*i+j) != 0 || !nonZeroBefore)
						{
							if(nonZeroBefore)
								output << (offset(4*i+j).numerator() > 0 ? " + " : " - ");
							else if(offset(4*i+j).numerator() < 0)
								output << '-';
							nonZeroBefore = true;

							output << std::abs(offset(4*i+j).numerator());
							if(offset(4*i+j).denominator() != 1)
								output << "/(" << name<AppCoordType>() << ")" << offset(4*i+j).denominator();
						} // end if
						output << ";\n";
					} // end for
					if(i != controls.size()-2) output << "\n";
				} // end for

				output << "} // end function updateCoordinates\n";
			} // end method outputOpenGLCppApp

			template<typename RationalType, typename AppCoordType, typename IDType = int, typename NameType = std::string>
			void compile(const std::string &headerName, const std::string &buildDirectoryName) const
			{
				auto systemMatrix = generateSystemMatrix<RationalType,IDType,NameType>();

				LinearSystem::numericReducedRowEchelonFormNoPivot(systemMatrix);
				auto investigation = LinearSystem::semiSymbolicInvestigate(systemMatrix,4);
				auto solution = LinearSystem::semiSymbolicSolve(systemMatrix,4);
				auto base = std::get<0>(solution);
				auto offset = std::get<1>(solution);

				// temporary code for outputting errors and warnings.
				if(investigation.nIndependentEquations < investigation.nEquations)
					std::cout << "Warning: " << investigation.nEquations - investigation.nIndependentEquations << " constraint(s) are redundant!" << std::endl;
				if(investigation.isImpossible)
					std::cout << "Error: The specified constraint system is impossible!" << std::endl;
				if(!investigation.boundUnknownConstants.empty())
					std::cout << "Error: " << investigation.boundUnknownConstants.size() << " unknown constant(s) have been bound!" << std::endl;
				if(!investigation.freeVariables.empty())
					std::cout << "Error: " << investigation.freeVariables.size() << " variable(s) have unspecified value! Consider adding more constraints." << std::endl;
				std::cout << std::endl;

				if(investigation.hasUniqueSolution && investigation.boundUnknownConstants.empty())
				{
					std::ofstream output(headerName);
					outputCppApp<RationalType,AppCoordType>(base,offset,output);
					output.close();

					std::thread([buildDirectoryName](){
						std::system(("cmake -E make_directory \""+buildDirectoryName+"\"").c_str());
						std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake .. ").c_str());
						std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake --build .").c_str());
					}).detach();
				} // end if
			} // end method compile

			void run(const std::string &buildDirectoryName) const
			{
				std::thread([buildDirectoryName](){
					std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake --build . --target run").c_str());
				}).detach();
			} // end method run

			// UI
			void render() const
			{
				// render controls
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(controlColour.data());
					for(const auto &control : controls)
						control.render();
				glPopAttrib();

				// render constraints
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(constraintColour.data());
					for(const auto &constraint : constraints)
						constraint.render();
				glPopAttrib();

				// render buttons (buttons should be in front)
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(buttonColour.data());
					for(const auto &button : buttons)
						button.first.render();

					// render text boxes (text boxes as well)
					tbFileName.render();

					if(selectedPart)
						selectedPart->render();

					if(caret)
						caret->render();

					if(endPoint1)
						endPoint1->render();

					if(endPoint2)
						endPoint2->render();
				glPopAttrib();
			} // end method render

			void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
			{
				// TODO: move application exit somewhere outside Model class (or any UI widget for that matter)
				if(code == 27 && down) // escape key
					if(caret)
						unfocusAll();
					else
						std::exit(0);
				else if(caret)
					caret->keyboardAscii(code,down,x,y);
				else if(down && code == 0x7f) // delete key
				{
					if(selectedConstraint != constraints.end())
					{
						eraseConstraint(selectedConstraint);
						clearConstraintIterators();
						selectedPart = nullptr;
					}
					else if(selectedControl != controls.rend() && selectedControl != controls.rend()-1)
					{
						eraseControl(selectedControl.base()-1);
						clearControlIterators();
						clearConstraintIterators();
						// assumes more invalidations than actually happen but will fix when I switch to std::list
						selectedPart = nullptr;
					} // end if
				} // end else
			} // end method keyboardAscii

			void keyboardNonAscii(graphene::Bases::EventHandling::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
			{
				if(key == graphene::Bases::EventHandling::NonAsciiKey::L_CTRL)
				{
					if(!(inConstraintAddMode = down))
					{
						endPoint1 = nullptr;
						endPoint2 = nullptr;
					} // end else
				}
				else if(caret)
					caret->keyboardNonAscii(key,down,x,y);
			} // end method keyboardNonAscii

			void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
			{
				if(button == 0)
				{
					if(down)
					{
						if(inConstraintAddMode)
						{
							if(highlightedControl != controls.rend())
							{
								endPoint1 = highlightedControl->partUnderPoint(x,y);
								control1 = highlightedControl.base()-1 - controls.begin();
								// Currently partUnderPoint returns a 'left' object if (x,y) is really left (where left < right),
								// not on the LEFT side of the object.
								// TODO: investigate if this is what we want.
								if(dynamic_cast<typename control_type::concrete_return_type::left*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::LEFT) == endPoint1->side(side_type::LEFT) ? side_type::LEFT : side_type::RIGHT;
								else if(dynamic_cast<typename control_type::concrete_return_type::right*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::RIGHT) == endPoint1->side(side_type::RIGHT) ? side_type::RIGHT : side_type::LEFT;
								else if(dynamic_cast<typename control_type::concrete_return_type::bottom*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::BOTTOM) == endPoint1->side(side_type::BOTTOM) ? side_type::BOTTOM : side_type::TOP;
								else if(dynamic_cast<typename control_type::concrete_return_type::top*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::TOP) == endPoint1->side(side_type::TOP) ? side_type::TOP : side_type::BOTTOM;
								else
									endPoint1 = nullptr;

								if(endPoint1)
								{
									if(endPoint2 && geometry::isHorizontal(side1) == geometry::isHorizontal(side2))
									{
										// create new constraint and focus it
										unfocusAll();
										deselectAll();
										auto avg = geometry::isHorizontal(side1) ? (endPoint1->left() + endPoint1->right() + endPoint2->left() + endPoint2->right())/4.0
																				 : (endPoint1->bottom() + endPoint1->top() + endPoint2->bottom() + endPoint2->top())/4.0;
										constraints.emplace_back(&controls,control1,side1,control2,side2,avg-0.5*constraintThickness,avg+0.5*constraintThickness,"",constraintTextHeight);
										highlightedConstraint = constraints.end();
										(selectedConstraint = focusedConstraint = constraints.end()-1)->select().focus(); // no constraint was highlighted
										caret = focusedConstraint->charAtIndex(focusedConstraint->text().size());

										endPoint1 = nullptr;
										endPoint2 = nullptr;
									}
									else
									{
										endPoint2 = std::move(endPoint1); // endPoint1 should become empty
										control2 = control1;
										side2 = side1;
									} // end else
								} // end else
							} // end if
						}
						else
						{
							assert(highlightedButton == buttons.end() || highlightedControl == controls.rend() || !tbFileName.highlighted());

							if(highlightedButton != buttons.end())
							{
								(pressedButton = highlightedButton)->first.press();
							} // pressing a button should not deselect
							else if(tbFileName.highlighted())
							{
								unfocusAll();
								tbFileName.focus();
								caret = tbFileName.charUnderPoint(x,y);
							} // clicking the text box should not deselect either
							else
							{
								deselectAll();
							} // end else

							if(highlightedControl != controls.rend())
							{
								// TODO: bring to front
								selectedPart = (selectedControl = highlightedControl)->select().partUnderPoint(x,y);

								// TODO: consider representing screen with a different control type and encapsulate special case in part selection code.
								if(selectedPart && selectedPart->left() == selectedControl->left() && selectedPart->bottom() == selectedControl->bottom()
									&& selectedPart->right() == selectedControl->right() && selectedPart->top() == selectedControl->top() && selectedControl == controls.rend()-1)
								{ // deselect screen central area (effectively make it transparent to mouse clicks)
									selectedPart = nullptr;
									selectedControl->deselect();
									selectedControl = controls.rend();
								}
								else
								{
									unfocusAll();
									caret = (focusedControl = highlightedControl)->focus().charUnderPoint(x,y);
								} // end else
							} // end if

							if(highlightedConstraint != constraints.end())
							{
								selectedPart = (selectedConstraint = highlightedConstraint)->select().partUnderPoint(x,y);
								unfocusAll();
								caret = (focusedConstraint = highlightedConstraint)->focus().charUnderPoint(x,y);
							} // end if

							if(pressedButton == buttons.end() && selectedControl == controls.rend() && !tbFileName.highlighted() && selectedConstraint == constraints.end())
								createOnMove = true;

							lastX = x;
							lastY = y;
						} // end else
					}
					else
					{
						if(pressedButton != buttons.end() && pressedButton->first.pressed())
						{
							reportExceptions(pressedButton->second);
							pressedButton->first.depress();
						} // end if
						pressedButton = buttons.end();

						selectedPart = nullptr;

						createOnMove = false;
					} // end else
				} // end if
			} // end method mouseButton

			void mouseEnter(CoordinateType x, CoordinateType y)
			{
				// do nothing
			} // end method mouseEnter

			void mouseMove(CoordinateType x, CoordinateType y)
			{
				if(pressedButton != buttons.end())
					pressedButton->first.pressed() = pressedButton->first.contains(x,y);
				else
				{
					if(createOnMove)
					{
						createOnMove = false;
						// create new control
						// TODO: push at front
						unfocusAll();
						if(highlightedControl != controls.rend())
							highlightedControl->dehighlight();
						controls.emplace_back(controlTextHeight,"control"+std::to_string(controlIndex++),borderSize,x,y,x,y);
						(focusedControl = highlightedControl = selectedControl = controls.rbegin())->highlight().select().focus();
						selectedPart = selectedControl->partUnderPoint(x,y); // TODO: guarantee this will be a corner
						caret = focusedControl->charUnderPoint(x,y);

						// add automatic constraints (temporary code)
						//constraints.emplace_back(&controls,controls.size()-1,side_type::LEFT,controls.size()-1,side_type::RIGHT,y,y+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,controls.size()-1,side_type::BOTTOM,controls.size()-1,side_type::TOP,x,x+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,0,side_type::LEFT,controls.size()-1,side_type::LEFT,y,y+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,0,side_type::BOTTOM,controls.size()-1,side_type::BOTTOM,x,x+constraintThickness,"0mm",constraintTextHeight);
						//highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
					} // end if

					dehighlightAll();

					if(selectedPart)
					{
						selectedPart->move(x-lastX,y-lastY);
						lastX = x;
						lastY = y;
					}
					else // if dropping an object on another makes sence, then highlighting should be done regardless of selectedPart
					{
						// highlight only the front-most
						for(auto button = buttons.begin() ; button < buttons.end() ; ++button)
							if(button->first.contains(x,y))
							{
								(highlightedButton = button)->first.highlight();
								break;
							} // end if

						if(highlightedButton == buttons.end())
							tbFileName.highlighted() = tbFileName.contains(x,y);

						if(!tbFileName.highlighted())
							for(auto constraint = constraints.begin() ; constraint < constraints.end() ; ++constraint)
								if(constraint->contains(x,y))
								{
									(highlightedConstraint = constraint)->highlight();
									break;
								} // end if

						if(highlightedConstraint == constraints.end())
							for(auto control = controls.rbegin() ; control < controls.rend() ; ++control) // TODO: restore front to back iteration when screen-at-front issue fixed
								if(control->contains(x,y))
								{
									(highlightedControl = control)->highlight();
									break;
								} // end if
					} // end else
				} // end else
			} // end method mouseMove

			void mouseExit(CoordinateType x, CoordinateType y)
			{
				// do nothing
			} // end method mouseExit

			// TODO: perhaps replace resize with use of properties for left,right,bottom,top?
			void resize(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;

				// load button
				buttons[0].first.left() = left+margin;
				buttons[0].first.bottom() = top-margin-buttonHeight;
				buttons[0].first.right() = left+margin+buttonWidth;
				buttons[0].first.top() = top-margin;
				// save button
				buttons[1].first.left() = right-3*margin-3*buttonWidth;
				buttons[1].first.bottom() = top-margin-buttonHeight;
				buttons[1].first.right() = right-3*margin-2*buttonWidth;
				buttons[1].first.top() = top-margin;
				// compile button
				buttons[2].first.left() = right-2*margin-2*buttonWidth;
				buttons[2].first.bottom() = top-margin-buttonHeight;
				buttons[2].first.right() = right-2*margin-buttonWidth;
				buttons[2].first.top() = top-margin;
				// run button
				buttons[3].first.left() = right-margin-buttonWidth;
				buttons[3].first.bottom() = top-margin-buttonHeight;
				buttons[3].first.right() = right-margin;
				buttons[3].first.top() = top-margin;

				// file name text box
				tbFileName.left() = left+2*margin+buttonWidth;
				tbFileName.bottom() = top-margin-buttonHeight;
				tbFileName.right() = right-4*margin-3*buttonWidth;
				tbFileName.top() = top-margin;

				// screen control
				if(firstResize)
				{
					firstResize = false;
					controls.front().left() = left+margin;
					controls.front().bottom() = bottom+margin;
					controls.front().right() = right-margin;
					controls.front().top() = top-2*margin-buttonHeight;
				} // end if
			} // end method resize
		}; // end class Model

		// out-of-class initializations
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::borderSize;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::controlTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::buttonTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::textBoxTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::constraintTextHeight;

	} // end namespace Controls

} // end namespace GUIModel

#endif // GUI_MODEL_H

