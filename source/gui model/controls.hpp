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

#ifndef CONTROLS_H
#define CONTROLS_H

#include <ratio>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "graphene.hpp"

namespace GUIModel
{
	namespace Controls
	{
		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using Button = graphene::DSEL::FrameStack<
			RectangleType,
			graphene::DSEL::Frame<graphene::Frames::Stateful::UniformlyBordered, typename RectangleType::coordinate_type>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Pressable>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Highlightable>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::BoxedAdaptableSizeText, Margin>,
			graphene::DSEL::Condition<graphene::FunctionObjects::Pressed,
				graphene::DSEL::Sequence<
					graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
					graphene::DSEL::FrameStack<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>>,
				graphene::DSEL::Condition<graphene::FunctionObjects::Highlighted,
					graphene::DSEL::Sequence<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>>,
					graphene::DSEL::Sequence<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>>
				>
			>
		>;

		template<typename CoordinateType>
		using IShapePart = graphene::DSEL::FrameStack<
			graphene::Frames::Interface::Empty,
			graphene::DSEL::Frame<graphene::Frames::Interface::Destructible>,
			graphene::DSEL::Frame<graphene::Frames::Interface::Movable, CoordinateType>,
			graphene::DSEL::Frame<graphene::Frames::Interface::Rectangular>, // TODO: move rectangular back to concrete control when the screen special case is handled
			graphene::DSEL::Frame<graphene::Frames::Interface::Containing>,
			graphene::DSEL::Frame<graphene::Frames::Interface::Renderable>
		>;

		/** Const instances should be constant and non-const instances should be non-constant
		 */
		template<typename CoordinateType, typename horizontallyMovable, typename verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		using ControlPart = graphene::DSEL::FrameStack<
			IShapePart<CoordinateType>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::Movable>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::HVMovable, horizontallyMovable, verticallyMovable>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Stippled>,
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Rectangular, geometry::RefRectangle<CoordinateType, constant, leftRef, bottomRef, rightRef, topRef>>
		>;

		template<typename CoordinateType, typename Const, typename CharType>
		using ICaret = graphene::DSEL::FrameStack<
			graphene::Frames::Interface::Empty,
			graphene::DSEL::Frame<graphene::Frames::Interface::Destructible>,
			graphene::DSEL::Frame<graphene::Frames::Interface::CaretLike, Const, CharType>,
			graphene::DSEL::Frame<graphene::Frames::Interface::KeyboardAndMouse, CoordinateType>, // TODO: handle in parent
			graphene::DSEL::Frame<graphene::Frames::Interface::Renderable>
		>;

		template<typename CoordinateType, typename Const, typename TextConceptMap, typename CharType, typename PointerType, typename Width>
		using Caret = graphene::DSEL::FrameStack<
			ICaret<CoordinateType,Const,CharType>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Indexing, size_t>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Pointing, PointerType>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Offset>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::FontEngined, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::IndirectCaretLike, TextConceptMap>,
			graphene::DSEL::Frame<graphene::Frames::EventHandling::KeyboardAndMouseStub>, // TODO: remove...
			graphene::DSEL::Frame<graphene::Frames::EventHandling::CaretLike>, // ...and handle in parent
			graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::IndirectCaret, TextConceptMap, Width>
		>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType = std::string>	class Control;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		using ControlBase =
			graphene::DSEL::FrameStack<
				RectangleType,
				graphene::DSEL::Frame<graphene::Frames::Stateful::UniformlyBordered, typename RectangleType::coordinate_type>,
				graphene::DSEL::Frame<graphene::Frames::Stateful::Selectable, Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
				graphene::DSEL::Frame<graphene::Frames::Stateful::Highlightable>,
				graphene::DSEL::Frame<graphene::Frames::Stateful::Focusable>,
				graphene::DSEL::Frame<graphene::Frames::Behavioural::Movable>,
				graphene::DSEL::Frame<graphene::Frames::Stateful::Named, TextType>,
				graphene::DSEL::Frame<graphene::Frames::Stateful::SizedName, graphene::FunctionObjects::GlutStrokeFontEngine>,
				graphene::DSEL::Frame<graphene::Frames::Behavioural::BoxedAdaptableSizeName, Margin>,
				graphene::DSEL::Frame<graphene::Frames::Behavioural::MultiCharBorderedRectangle, graphene::FunctionObjects::Named,
					std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
					std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
						  Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Named, typename TextType::value_type,
									  Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
					const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Named, typename TextType::value_type,
								const Control<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
					size_t>,
				graphene::DSEL::Frame<
					graphene::Frames::Behavioural::MultiPartBorderedRectangle,
					graphene::Frames::Behavioural::ConcreteReturnTypesBuilder<ControlPart, typename RectangleType::coordinate_type>,
					std::unique_ptr<      IShapePart<typename RectangleType::coordinate_type>>,
					std::unique_ptr<const IShapePart<typename RectangleType::coordinate_type>>
				>,
				graphene::DSEL::Sequence<
					graphene::DSEL::Condition<graphene::FunctionObjects::Selected,
						graphene::DSEL::Sequence<
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
							graphene::DSEL::FrameStack<
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Named>,
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>>,
						graphene::DSEL::Condition<graphene::FunctionObjects::Highlighted,
							graphene::DSEL::Sequence<
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>,
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Named>>,
							graphene::DSEL::Sequence<
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
								graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Named>>
						>
					>,
					graphene::DSEL::Condition<graphene::FunctionObjects::Focused,
						graphene::DSEL::FrameStack<
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>,
							graphene::DSEL::Frame<graphene::Frames::Renderable::Stippled>,
							graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Null>
					>
				>
			>;


		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		class Control : public ControlBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ControlBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>;
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
			graphene::DSEL::Frame<graphene::Frames::Stateful::UniformlyBordered, typename RectangleType::coordinate_type>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Focusable, TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Highlightable>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::Movable>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::Textual, TextType>,
			graphene::DSEL::Frame<graphene::Frames::Stateful::SizedText, graphene::FunctionObjects::GlutStrokeFontEngine>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::BoxedAdaptableSizeText, Margin>,
			graphene::DSEL::Frame<graphene::Frames::Behavioural::MultiCharBorderedRectangle, graphene::FunctionObjects::Textual,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type, std::false_type, typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type, std::true_type,  typename TextType::value_type>>,
				      Caret<typename RectangleType::coordinate_type, std::false_type, graphene::FunctionObjects::Textual, typename TextType::value_type,
								  TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
				const Caret<typename RectangleType::coordinate_type, std::true_type,  graphene::FunctionObjects::Textual, typename TextType::value_type,
							const TextBox<RectangleType, BorderSize, Margin, CaretWidth, TextType>*, CaretWidth>,
				size_t>,
			graphene::DSEL::Condition<graphene::FunctionObjects::Focused,
				graphene::DSEL::Sequence<
					graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::FilledRectangle>,
					graphene::DSEL::FrameStack<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::InversedColor>>>,
				graphene::DSEL::Condition<graphene::FunctionObjects::Highlighted,
					graphene::DSEL::Sequence<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, std::ratio<0>>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>>,
					graphene::DSEL::Sequence<
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BorderedRectangle, BorderSize>,
						graphene::DSEL::Frame<graphene::Frames::Renderable::Colorblind::BoxedText, graphene::FunctionObjects::Textual>>
				>
			>
		>;


		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		struct TextBox : public TextBoxBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>
		{
			using base_type = TextBoxBase<RectangleType, BorderSize, Margin, CaretWidth, TextType>;
			TextBox() = default;
			using base_type::base_type;
		}; // end struct TextBox

	} // end namespace Controls

} // end namespace GUIModel

#endif // CONTROLS_H
