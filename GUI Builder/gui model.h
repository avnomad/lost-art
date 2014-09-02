#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

#include <cassert>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

//#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/include/phoenix.hpp>

#include <Eigen/Dense>

#include "geometry.h"
#include "graphene.h"
#include "symbol table.h"
#include "linear system solving.h"

namespace GUIModel
{
	void runTestSuite();

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
		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string> class Button;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType> 
		class ButtonBase : public graphene::DSEL::FrameStack<
			RectangleType,
			graphene::Frames::UniformlyBordered<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
			graphene::Frames::Pressable<graphene::DSEL::Omit,Button<RectangleType,BorderSize,Margin,TextType>>,
			graphene::Frames::Highlightable<graphene::DSEL::Omit,Button<RectangleType,BorderSize,Margin,TextType>>,
			graphene::Frames::Textual<graphene::DSEL::Omit,TextType>,
			graphene::Frames::SizedText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,typename RectangleType::coordinate_type>,
			graphene::Frames::BoxedAdaptableSizeText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,Margin,typename RectangleType::coordinate_type>
		>::type{}; // poor man's template alias

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		class Button : public graphene::Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
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
								graphene::FunctionObjects::Pressed>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef graphene::Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
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
						graphene::FunctionObjects::Pressed> base_type;
			typedef typename Button::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Button(){/* empty body */}

			Button(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, 
					TextType text, coordinate_type textHeight, bool pressed = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->text() = text;
				this->textHeight() = textHeight;
				this->pressed() = pressed;
				this->highlighted() = highlighted;
			} // end Button constructor
		}; // end class Button

		template<typename CoordinateType>
		class IShapePart : public graphene::DSEL::FrameStack<
			graphene::Bases::Empty,
			graphene::Bases::Movable<graphene::DSEL::Omit,CoordinateType>,
			graphene::Bases::Rectangular<graphene::DSEL::Omit,CoordinateType>, // TODO: move rectangular back to concrete control when the screen special case is handled
			graphene::Bases::Containing<graphene::DSEL::Omit,CoordinateType>,
			graphene::Bases::Renderable<graphene::DSEL::Omit>			
		>::type{}; // poor man's template alias

		/** Const instances should be constant and non-const instances should be non constant
		 */
		template<typename CoordinateType, bool horizontallyMovable, bool verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		class ControlPart : public graphene::DSEL::FrameStack<
				IShapePart<CoordinateType>,
				graphene::Frames::Movable::Rectangular<graphene::DSEL::Omit,CoordinateType>,
				graphene::Frames::Movable::HVMovable<graphene::DSEL::Omit,horizontallyMovable,verticallyMovable,CoordinateType>,
				graphene::Frames::Renderable::Colorblind::FilledRectangle<graphene::DSEL::Omit>,
				graphene::Frames::Renderable::Stippled<graphene::DSEL::Omit>,
				graphene::Frames::Renderable::Colorblind::InversedColor<graphene::DSEL::Omit>,
				graphene::Frames::Adapting::Rectangular<graphene::DSEL::Omit,geometry::RefRectangle<CoordinateType,constant,leftRef,bottomRef,rightRef,topRef>>
			>::type
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef typename graphene::DSEL::FrameStack<
				IShapePart<CoordinateType>,
				graphene::Frames::Movable::Rectangular<graphene::DSEL::Omit,CoordinateType>,
				graphene::Frames::Movable::HVMovable<graphene::DSEL::Omit,horizontallyMovable,verticallyMovable,CoordinateType>,
				graphene::Frames::Renderable::Colorblind::FilledRectangle<graphene::DSEL::Omit>,
				graphene::Frames::Renderable::Stippled<graphene::DSEL::Omit>,
				graphene::Frames::Renderable::Colorblind::InversedColor<graphene::DSEL::Omit>,
				graphene::Frames::Adapting::Rectangular<graphene::DSEL::Omit,geometry::RefRectangle<CoordinateType,constant,leftRef,bottomRef,rightRef,topRef>>
			>::type base_type;
			typedef typename ControlPart::rectangle_type rectangle_type;
			
			/*********************
			*    Constructors    *
			*********************/
		public:
			ControlPart(){/* empty body */}

			/** Forwards arguments to the underlying RectangularType constructor
			 */
			template<typename LeftType, typename BottomType, typename RightType, typename TopType> // TODO: use variadic templates or inheriting constructors when available
			ControlPart(LeftType &&left, BottomType &&bottom, RightType &&right, TopType &&top)
				:base_type(std::forward<LeftType>(left),std::forward<BottomType>(bottom),std::forward<RightType>(right),std::forward<TopType>(top))
			{
				// empty body
			} // end ControlPart constructor
		}; // end class ControlPart

		template<typename CoordinateType, typename CharType>
		class ICaret : public graphene::DSEL::FrameStack<
			graphene::Bases::Empty,
			graphene::Bases::CaretLike<graphene::DSEL::Omit,CharType>,
			graphene::Bases::EventHandling::KeyboardAndMouse<graphene::DSEL::Omit,CoordinateType>, // TODO: handle in parent
			graphene::Bases::Renderable<graphene::DSEL::Omit>			
		>::type{}; // poor man's template alias

		template<typename CoordinateType, typename TextConceptMap, typename CharType, typename PointedToType, typename Width>
		class Caret : public graphene::DSEL::FrameStack<
				ICaret<CoordinateType,CharType>,
				graphene::Frames::Indexing<graphene::DSEL::Omit,size_t>,
				graphene::Frames::Offset<graphene::DSEL::Omit,CoordinateType>,
				graphene::Frames::Pointing<graphene::DSEL::Omit,PointedToType*>,
				graphene::Frames::IndirectCaretLike<graphene::DSEL::Omit,TextConceptMap,graphene::FunctionObjects::GlutStrokeFontEngine,CharType>,
				graphene::Frames::EventHandling::KeyboardAndMouseStub<graphene::DSEL::Omit,CoordinateType>, // TODO: remove...
				graphene::Frames::EventHandling::CaretLike<graphene::DSEL::Omit,CoordinateType>, // ...and handle in parent
				graphene::Frames::Renderable::Colorblind::IndirectCaret<graphene::DSEL::Omit,TextConceptMap,graphene::FunctionObjects::GlutStrokeFontEngine,Width>
			>::type
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef typename graphene::DSEL::FrameStack<
				ICaret<CoordinateType,CharType>,
				graphene::Frames::Indexing<graphene::DSEL::Omit,size_t>,
				graphene::Frames::Offset<graphene::DSEL::Omit,CoordinateType>,
				graphene::Frames::Pointing<graphene::DSEL::Omit,PointedToType*>,
				graphene::Frames::IndirectCaretLike<graphene::DSEL::Omit,TextConceptMap,graphene::FunctionObjects::GlutStrokeFontEngine,CharType>,
				graphene::Frames::EventHandling::KeyboardAndMouseStub<graphene::DSEL::Omit,CoordinateType>, // TODO: remove...
				graphene::Frames::EventHandling::CaretLike<graphene::DSEL::Omit,CoordinateType>, // ...and handle in parent
				graphene::Frames::Renderable::Colorblind::IndirectCaret<graphene::DSEL::Omit,TextConceptMap,graphene::FunctionObjects::GlutStrokeFontEngine,Width>
			>::type base_type;
			typedef PointedToType pointed_to_type;
			typedef Width width;
			
			/*********************
			*    Constructors    *
			*********************/
		public:
			Caret(){/* empty body */}

			Caret(PointedToType *pointer, size_t index, CoordinateType xOffset)
			{
				this->pointer() = pointer;
				this->index() = index;
				this->xOffset() = xOffset;
				this->yOffset() = 0;
			} // end Caret constructor
		}; // end class Caret

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType = std::string>	class Control;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType> 
		class ControlBase : public 
			graphene::Frames::MultiPartBorderedRectangle<typename graphene::DSEL::FrameStack<
				RectangleType,
				graphene::Frames::UniformlyBordered<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
				graphene::Frames::Selectable<graphene::DSEL::Omit,Control<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
				graphene::Frames::Highlightable<graphene::DSEL::Omit,Control<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
				graphene::Frames::Focusable<graphene::DSEL::Omit,Control<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
				graphene::Frames::Movable::Rectangular<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
				graphene::Frames::Named<graphene::DSEL::Omit,TextType>,
				graphene::Frames::SizedName<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,typename RectangleType::coordinate_type>,
				graphene::Frames::BoxedAdaptableSizeName<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,Margin,typename RectangleType::coordinate_type>,
				graphene::Frames::MultiCharBorderedRectangle<graphene::DSEL::Omit,graphene::FunctionObjects::Named,graphene::FunctionObjects::GlutStrokeFontEngine,
					std::unique_ptr<      ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
					std::unique_ptr<const ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
						  Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Named,typename TextType::value_type,      Control<RectangleType,BorderSize,Margin,CaretWidth,TextType>,CaretWidth>,
					const Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Named,typename TextType::value_type,const Control<RectangleType,BorderSize,Margin,CaretWidth,TextType>,CaretWidth>,
					typename RectangleType::coordinate_type>
			>::type,ControlPart,std::unique_ptr<IShapePart<typename RectangleType::coordinate_type>>,std::unique_ptr<const IShapePart<typename RectangleType::coordinate_type>>>{}; // poor man's template alias

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		class Control : public graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
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
										graphene::FunctionObjects::Focused>>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef graphene::Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
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
							graphene::FunctionObjects::Focused>> base_type;
			typedef typename Control::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;
			typedef boost::property_tree::ptree property_tree_type;

			// TODO: add color
			// TODO: add type

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an uninitialized Control.
			 */
			Control(){/* empty body */}

			/** Construct a Control with the specified properties.
			 */
			Control(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, 
					TextType name, coordinate_type nameHeight, bool selected = false, bool highlighted = false, bool focused = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->name() = name;
				this->nameHeight() = nameHeight;
				this->selected() = selected;
				this->highlighted() = highlighted;
				this->focused() = focused;
			} // end Control constructor

			// TODO: consider serializing selected and other user interaction states.
			Control(const property_tree_type &tree)
			{
				this->left() = tree.get<coordinate_type>("sides.left");
				this->bottom() = tree.get<coordinate_type>("sides.bottom");
				this->right() = tree.get<coordinate_type>("sides.right");
				this->top() = tree.get<coordinate_type>("sides.top");
				this->borderSize() = tree.get<coordinate_type>("borderSize");
				this->name() = tree.get<TextType>("name");
				this->nameHeight() = tree.get<coordinate_type>("nameHeight");
				this->selected() = false;
				this->highlighted() = false;
				this->focused() = false;
			} // end Control conversion constructor

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: consider deserializing selected and other user interaction states.
			operator property_tree_type() const
			{
				property_tree_type tree;

				tree.put("sides.left",left());
				tree.put("sides.bottom",bottom());
				tree.put("sides.right",right());
				tree.put("sides.top",top());
				tree.put("borderSize",borderSize());
				tree.put("name",name());
				tree.put("nameHeight",nameHeight());
				return tree;
			} // end method operator property_tree_type
		}; // end class Control

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType = std::string> class TextBox;

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType> 
		class TextBoxBase : public graphene::DSEL::FrameStack<
			RectangleType,
			graphene::Frames::UniformlyBordered<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
			graphene::Frames::Focusable<graphene::DSEL::Omit,TextBox<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
			graphene::Frames::Highlightable<graphene::DSEL::Omit,TextBox<RectangleType,BorderSize,Margin,CaretWidth,TextType>>,
			graphene::Frames::Movable::Rectangular<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
			graphene::Frames::Textual<graphene::DSEL::Omit,TextType>,
			graphene::Frames::SizedText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,typename RectangleType::coordinate_type>,
			graphene::Frames::BoxedAdaptableSizeText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,Margin,typename RectangleType::coordinate_type>,
			graphene::Frames::MultiCharBorderedRectangle<graphene::DSEL::Omit,graphene::FunctionObjects::Textual,graphene::FunctionObjects::GlutStrokeFontEngine,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
				      Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Textual,typename TextType::value_type,      TextBox<RectangleType,BorderSize,Margin,CaretWidth,TextType>,CaretWidth>,
				const Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Textual,typename TextType::value_type,const TextBox<RectangleType,BorderSize,Margin,CaretWidth,TextType>,CaretWidth>,
				typename RectangleType::coordinate_type>
		>::type{}; // poor man's template alias

		template<typename RectangleType, typename BorderSize, typename Margin, typename CaretWidth, typename TextType>
		class TextBox : public graphene::Frames::Renderable::Conditional<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
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
									graphene::FunctionObjects::Focused>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef graphene::Frames::Renderable::Conditional<TextBoxBase<RectangleType,BorderSize,Margin,CaretWidth,TextType>,
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
						graphene::FunctionObjects::Focused> base_type;
			typedef typename TextBox::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an uninitialized TextBox.
			 */
			TextBox(){/* empty body */}

			/** Construct a TextBox with the specified properties.
			 */
			TextBox(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, 
					TextType text, coordinate_type textHeight, bool focused = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->text() = text;
				this->textHeight() = textHeight;
				this->focused() = focused;
				this->highlighted() = highlighted;
			} // end TextBox constructor
		}; // end class TextBox

		// TODO: rework design of ConstraintEndPoint
		template<typename ControlContainerType>
		struct ConstraintEndPoint
		{
			/*********************
			*    Member Types    *
			*********************/

			typedef ControlContainerType control_container_type;
			typedef typename control_container_type::value_type::coordinate_type coordinate_type;
			typedef boost::property_tree::ptree property_tree_type;
			typedef geometry::RectangleSide side_type;

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

			ConstraintEndPoint(){/* empty body */}

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

			auto referredSide()->decltype((*iContainer)[control].side(side))
			{
				return (*iContainer)[control].side(side);
			} // end method referredSide

			auto referredSide() const->decltype((*iContainer)[control].side(side))
			{
				return (*iContainer)[control].side(side);
			} // end method referredSide

			auto referredControl()->decltype((*iContainer)[control])
			{
				return (*iContainer)[control];
			} // end method referredControl

			auto referredControl() const->decltype((*iContainer)[control])
			{
				return (*iContainer)[control];
			} // end method referredControl
		}; // end struct ConstraintEndPoint

		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType = std::string>	class Constraint;

		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		class ConstraintBase : public
		graphene::DSEL::FrameStack<
			RectangleType,
			graphene::Frames::UniformlyBordered<graphene::DSEL::Omit,typename RectangleType::coordinate_type>,
			graphene::Frames::Selectable<graphene::DSEL::Omit,Constraint<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>>,
			graphene::Frames::Highlightable<graphene::DSEL::Omit,Constraint<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>>,
			graphene::Frames::Focusable<graphene::DSEL::Omit,Constraint<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>>,
			graphene::Frames::Textual<graphene::DSEL::Omit,TextType>,
			graphene::Frames::SizedText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,typename RectangleType::coordinate_type>,
			graphene::Frames::BoxedAdaptableSizeText<graphene::DSEL::Omit,graphene::FunctionObjects::GlutStrokeFontEngine,BorderSize,typename RectangleType::coordinate_type>,
			graphene::Frames::MultiCharBorderedRectangle<graphene::DSEL::Omit,graphene::FunctionObjects::Textual,graphene::FunctionObjects::GlutStrokeFontEngine,
				std::unique_ptr<      ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
				std::unique_ptr<const ICaret<typename RectangleType::coordinate_type,typename TextType::value_type>>,
					  Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Textual,typename TextType::value_type,      Constraint<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,CaretWidth>,
				const Caret<typename RectangleType::coordinate_type,graphene::FunctionObjects::Textual,typename TextType::value_type,const Constraint<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,CaretWidth>,
				typename RectangleType::coordinate_type>
		>::type{}; // poor man's template alias

		// TODO: decompose Constraint to frames
		template<typename RectangleType, typename ControlContainerType, typename BorderSize, typename LineWidth, typename CaretWidth, typename TextType>
		class Constraint : public	graphene::Frames::Renderable::Sequential<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
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
										graphene::FunctionObjects::Focused>>
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

			typedef graphene::Frames::Renderable::Sequential<ConstraintBase<RectangleType,ControlContainerType,BorderSize,LineWidth,CaretWidth,TextType>,
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
						graphene::FunctionObjects::Focused>> base_type;
			typedef RectangleType rectangle_type;
			typedef ControlContainerType control_container_type;
			typedef typename RectangleType::coordinate_type coordinate_type;
			typedef TextType text_type;
			typedef boost::property_tree::ptree property_tree_type;
			typedef ConstraintEndPoint<ControlContainerType> EndPoint;
			typedef typename EndPoint::side_type side_type;
			typedef std::unique_ptr<IShapePart<coordinate_type>> PartType;
			typedef std::unique_ptr<const IShapePart<coordinate_type>> ConstPartType;
			typedef BorderSize border_size;
			typedef LineWidth line_width;
			typedef CaretWidth caret_width;

		private:
			/***************
			*    Fields    *
			***************/

			EndPoint iEndPoints[2];
			coordinate_type iLocalSides[2];

		public:
			/*********************
			*    Constructors    *
			*********************/

			/** Construct an uninitialized Constraint.
			 */
			Constraint(){/* emtpy body */}

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
			Constraint(const property_tree_type &tree, ControlContainerType *container, TextType text, coordinate_type textHeight,
				coordinate_type borderSize = 0, bool selected = false, bool highlighted = false, bool focused = false)
			{
				endPoints()[0] = EndPoint(tree.get_child("first-end-point"),container);
				endPoints()[1] = EndPoint(tree.get_child("second-end-point",container));
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
			
			// TODO: consider serializing selected and other user interaction states.
			operator property_tree_type() const
			{
				property_tree_type tree;

				tree.put("text",text());
				tree.put("text-height",testHeight());
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
						return PartType(new ControlPart<coordinate_type,false,true,false,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return PartType(new ControlPart<coordinate_type,true,false,false,true,false,true,false>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
				else
					return nullptr;
			} // end method partUnderPoint

			ConstPartType partUnderPoint(coordinate_type x, coordinate_type y) const
			{
				if(contains(x,y))
					if(isHorizontal())
						return ConstPartType(new const ControlPart<coordinate_type,false,true,true,true,true,true,true>(iEndPoints[0].referredSide(),iLocalSides[0],iEndPoints[1].referredSide(),iLocalSides[1]));
					else
						return ConstPartType(new const ControlPart<coordinate_type,true,false,true,true,true,true,true>(iLocalSides[0],iEndPoints[0].referredSide(),iLocalSides[1],iEndPoints[1].referredSide()));
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

						if(selected())
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
						std::tie(effectiveTextWidth,effectiveTextHeight) = effectiveTextSize();
						coordinate_type textLeft = this->left() + (width() - effectiveTextWidth) / 2;
						coordinate_type textBottom = this->bottom() + (height() - effectiveTextHeight) / 2;

						glPushMatrix();
							glTranslated(textLeft,textBottom,0); // center text in inner rectangle
							glScaled(effectiveTextWidth / fontEngine.stringWidth(text()) , effectiveTextHeight / fontEngine.fontHeight() , 1);
							glTranslated(0,fontEngine.fontBelowBaseLine(),0);
							fontEngine.render(text());
						glPopMatrix();

						// left horizontal
						glRect(innerLeft,((2*bottom + height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),textLeft,((2*bottom + height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

						// right horizontal
						auto textRight = this->right() - (width() - effectiveTextWidth) / 2;
						glRect(textRight,((2*bottom + height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),innerRight,((2*bottom + height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den));

						if(selected())
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

						if(selected())
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
						std::tie(effectiveTextWidth,effectiveTextHeight) = effectiveTextSize();
						coordinate_type textLeft = this->bottom() + (height() - effectiveTextHeight) / 2;
						coordinate_type textBottom = this->left() + (width() - effectiveTextWidth) / 2;

						glPushMatrix();
							glTranslated(textLeft,textBottom,0); // center text in inner rectangle
							glScaled(effectiveTextHeight / fontEngine.fontHeight() , effectiveTextWidth / fontEngine.stringWidth(text()) , 1);
							glRotated(90,0,0,1); // rotate by 90 degrees
							glTranslated(0,-fontEngine.fontAboveBaseLine(),0);
							fontEngine.render(text());
						glPopMatrix();

						// bottom vertical
						glRect(((2*left + height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),innerBottom,((2*left + height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den),textBottom);

						// top vertical
						auto textTop = this->right() - (width() - effectiveTextWidth) / 2;
						glRect(((2*left + height())*LineWidth::den - LineWidth::num)/(2*LineWidth::den),textTop,((2*left + height())*LineWidth::den + LineWidth::num)/(2*LineWidth::den),innerTop);

						if(selected())
							glPopAttrib();
					} // end else
				glPopAttrib();
			} // end method render

			//template<typename RationalType, typename IDType, typename NameType>
			//ParseResult<IDType,RationalType> parse(std::shared_ptr<Symbolic::Common::SymbolTable<NameType,IDType>> symbols) const
			//{
			//	namespace qi = boost::spirit::qi;
			//	namespace px = boost::phoenix;
			//	using qi::rule;
			//	using qi::_val;	using qi::_1; using qi::_2;
			//	using qi::alpha; using qi::alnum; using qi::digit; using qi::char_; using qi::eps; using qi::ulong_long;
			//	using boost::spirit::ascii::space;
			//	using boost::spirit::ascii::space_type;

			//	ParseResult<IDType,RationalType> result;

			//	struct Workaround
			//	{
			//		/**	s must be a string of digits [0-9].
			//		 */
			//		static RationalType decimalPoint(RationalType r, const std::vector<char> &s)
			//		{ // TODO: make overflow safe
			//			unsigned long long n = 0;
			//			unsigned long long d = 1;

			//			for(auto c : s)
			//			{
			//				n *= 10;
			//				n += c - '0';
			//				d *= 10;
			//			} // end foreach
			//			
			//			return r + RationalType(n,d);
			//		} // end function decimalPoint

			//		static void populate(const RationalType &coeff, const NameType &symbol, ParseResult<IDType,RationalType> &result, 
			//			std::shared_ptr<Symbolic::Common::SymbolTable<NameType,IDType>> symbols)
			//		{
			//			if(symbol == "cm")
			//				result.rhsConstant -= coeff;
			//			else if(symbol == "px")
			//				result.pxSizeCoeff += coeff;
			//			else
			//				result.varCoeff[symbols->declare(symbol)] += coeff;
			//		} // end function populate
			//	}; // end local struct Workaround

			//	rule<TextType::const_iterator,NameType()> identifier = (alpha | char_('_')) > *(alnum | char_('_'));
			//	rule<TextType::const_iterator,RationalType()> coefficient = (char_('-')[_val = -1] | eps[_val = 1]) > 
			//			-(ulong_long[_val *= _1] > -('/' > ulong_long[_val /= _1] | '.' > (*digit)[_val = px::bind(Workaround::decimalPoint,_val,_1)]));
			//	rule<TextType::const_iterator,space_type> constraint = (coefficient > identifier)[px::bind(Workaround::populate,_1,_2,px::ref(result),px::ref(symbols))] % '+';

			//	auto begin = text().begin();
			//	auto end = text().end();

			//	bool success = phrase_parse(begin,end,constraint,space);
			//	if(!success || begin != end)
			//		throw std::runtime_error("Error parsing constraint!");

			//	return result;
			//} // end method parse
		}; // end class Constraint

		// TODO: split button and control handling code and move them into separate classes. ButtonManager?
		// TODO: split Model class to frames.
		template<typename CoordinateType, typename TextType = std::string>
		class Model : public geometry::Rectangle<CoordinateType>
		{
		public:
			/*********************
			*    Member Types    *
			*********************/

			typedef boost::property_tree::ptree property_tree_type;
			typedef CoordinateType coordinate_type;
			typedef TextType text_type;
			typedef typename TextType::value_type char_type;

			typedef Control<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType> control_type;
			typedef Constraint<geometry::Rectangle<CoordinateType>,std::vector<control_type>,std::ratio<1,2>,std::ratio<1,2>,std::ratio<1>,TextType> constraint_type;
			typedef Button<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,TextType> button_type;
			typedef TextBox<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType> text_box_type;

			typedef typename constraint_type::EndPoint end_point_type;
			typedef typename end_point_type::side_type side_type;

		public: // TODO: make private and add methods to manipulate...
			/***************
			*    Fields    *
			***************/

			static const int margin = 10; // in millimeters
			static const int buttonHeight = 15;  // in millimeters
			static const int buttonWidth = static_cast<int>(buttonHeight*1.61803)/* golden ration */;  // in millimeters

			static const int borderSize = 2;
			static const int buttonTextHeight = 10;
			static const int textBoxTextHeight = buttonTextHeight;
			static const int controlTextHeight = buttonTextHeight;
			static const int constraintTextHeight = 7;
			static const int constraintThickness = 7;

			// TODO: change vector to list for faster operations, updating ConstraintEndPoints.
			std::vector<control_type> controls;
			std::vector<constraint_type> constraints;
			std::vector<std::pair<button_type,std::function<void()>>> buttons;
			text_box_type tbFileName;

			typename std::common_type<decltype(buttons)>::type::iterator highlightedButton; // workaround for decltype not working alone
			typename std::common_type<decltype(buttons)>::type::iterator pressedButton;

			typename std::common_type<decltype(controls)>::type::reverse_iterator highlightedControl;
			typename std::common_type<decltype(controls)>::type::reverse_iterator selectedControl;
			typename std::common_type<decltype(controls)>::type::reverse_iterator focusedControl;

			typename std::common_type<decltype(constraints)>::type::iterator highlightedConstraint;
			typename std::common_type<decltype(constraints)>::type::iterator selectedConstraint;
			typename std::common_type<decltype(constraints)>::type::iterator focusedConstraint;

			std::unique_ptr<IShapePart<CoordinateType>> selectedPart;

			std::unique_ptr<ICaret<CoordinateType,char_type>> caret;

			coordinate_type lastX; // TODO: consider making lastPressX (this will require saving initial movable control position as well)
			coordinate_type lastY;
			unsigned long long controlIndex;

			bool firstResize; // GLUT workaround (can't do first resize in constructor)
			bool createOnMove;
			bool inConstraintAddMode;

			std::unique_ptr<IShapePart<CoordinateType>> endPoint1;
			std::unique_ptr<IShapePart<CoordinateType>> endPoint2;
			side_type side1;
			side_type side2;

		public:
			/*********************
			*    Constructors    *
			*********************/

			/** Construct an empty Model.
			 */
			Model()
				:firstResize(true),createOnMove(false),inConstraintAddMode(false),controlIndex(0),tbFileName(0,0,0,0,borderSize,"last session.las",textBoxTextHeight)
			{
				// initialize buttons
				buttons.emplace_back(button_type(0,0,0,0,borderSize,"Load",buttonTextHeight),[](){std::cout << "Load" << std::endl;});
				buttons.emplace_back(button_type(0,0,0,0,borderSize,"Save",buttonTextHeight),[](){std::cout << "Save" << std::endl;});
				buttons.emplace_back(button_type(0,0,0,0,borderSize,"Compile",buttonTextHeight),[](){std::cout << "Compile" << std::endl;});
				buttons.emplace_back(button_type(0,0,0,0,borderSize,"Run",buttonTextHeight),[](){std::cout << "Run" << std::endl;});
				
				// initialize controls
				controls.push_back(control_type(0,0,0,0,borderSize,"Screen",controlTextHeight)); // emplace_back can't take 6+ arguments yet...

				// initialize pointers and iterators
				highlightedButton = buttons.end();
				pressedButton = buttons.end();

				highlightedControl = controls.rend();
				selectedControl = controls.rend();
				focusedControl = controls.rend();

				highlightedConstraint = constraints.end();
				selectedConstraint = constraints.end();
				focusedConstraint = constraints.end();

				selectedPart = nullptr;
				caret = nullptr;
			} // end Model constructor

			/****************
			*    Methods    *
			****************/

			void clear()
			{
				controls.clear();
				constraints.clear();

				highlightedControl = controls.rend();
				selectedControl = controls.rend();
				focusedControl = controls.rend();

				highlightedConstraint = constraints.end();
				selectedControl = constraints.end();
				focusedConstraint = constraints.end();

				selectedPart = nullptr;
				caret = nullptr; // TODO: change to only become null if not pointing to text box.
			} // end method clear

			auto eraseControl(typename std::common_type<decltype(controls)>::type::iterator control)->decltype(controls.erase(control))
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

			auto eraseConstraint(typename std::common_type<decltype(constraints)>::type::iterator constraint)->decltype(constraints.erase(constraint))
			{
				return constraints.erase(constraint);
			} // end method eraseConstraint

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


			void save(const std::string &fileName) const
			{
				property_tree_type tree;

				for(const auto &control : controls)
					tree.add_child("gui-model.controls.control",control);

				for(const auto &constraint : constraints)
					tree.add_child("gui-model.constraints.constraint",constraint);

				boost::property_tree::write_xml(fileName,tree);
			} // end method save


			//template<typename RationalType, typename IDType /* = int */, typename NameType /* = std::string */> // current compiler version does not support default arguments
			//Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> generateSystemMatrix() const
			//{
			//	auto symbols = std::make_shared<Symbolic::Common::SymbolTable<NameType,IDType>>();
			//	std::vector<Constraint<TextType>::template ParseResult<IDType,RationalType>> parseResults;

			//	for(const auto &constraint : constraints)
			//		parseResults.push_back(constraint.parse<RationalType,IDType,NameType>(symbols));
			//	// TODO: optimize to not include unknown constants that are not present.

			//	Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> result; 
			//	result.setZero(constraints.size(),4*controls.size() + symbols->size() + 1); // 4*(nControls-1) + nSymbols + 4 + 1
			//	// the layout is control sides, then named variables, then unknown constants then known constant.

			//	// Fill in the augmented system matrix
			//	size_t i = 0;
			//	for(const auto &constraint : constraints)
			//	{
			//		int coeffs[2] = {1,-1};
			//		if(controls[constraint.endPoints()[0].control].side(constraint.endPoints()[0].side) > controls[constraint.endPoints()[1].control].side(constraint.endPoints()[1].side))
			//			std::swap(coeffs[0],coeffs[1]);

			//		for(size_t ep = 0 ; ep < 2 ; ++ep)
			//		{
			//			if(constraint.endPoints()[ep].control != 0)
			//				result(i,4*(constraint.endPoints()[ep].control-1) + size_t(constraint.endPoints()[ep].side)) += coeffs[ep];
			//			else if(constraint.endPoints()[ep].side == geometry::RectangleSide::RIGHT || constraint.endPoints()[ep].side == geometry::RectangleSide::TOP)
			//				result(i,4*(controls.size()-1) + symbols->size() + size_t(constraint.endPoints()[ep].side) - 2) += coeffs[ep];
			//		} // end for

			//		for(const auto &var : parseResults[i].varCoeff)
			//			result(i,4*(controls.size()-1) + var.first) = var.second;

			//		size_t offset = constraint.endPoints()[0].side == geometry::RectangleSide::LEFT || constraint.endPoints()[0].side == geometry::RectangleSide::RIGHT ? 0 : 1;
			//		result(i,4*(controls.size()-1) + symbols->size() + 2 + offset) = parseResults[i].pxSizeCoeff;

			//		result(i,4*(controls.size()-1) + symbols->size() + 4) = parseResults[i].rhsConstant; // already moved to rhs during parsing

			//		++i;
			//	} // end foreach

			//	return result;
			//} // end method generateSystemMatrix


			/** Takes the solution of a linear system (in the form of a vector space base and point space offset 
			 *	generating the solutions) describing a GUI and generates C++ code for an application 
			 *	that implements that GUI. The output code is intended to be saved in a file and #included be a 
			 *	suitable application-template .cpp file. Generated code is not tied to a specific GUI toolkit.
			 */
			template<typename RationalType, typename AppCoordType>
			void outputCppApp(const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &base, const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &offset, std::ostream &output) const
			{
				output << "// NO INCLUDE GUARD!!!\n\n";
				output << "std::vector<geometry::Rectangle<" << typeid(AppCoordType).name() << ">> controls(" << controls.size()-1 << ");\n\n";
				output << "void updateCoordinates(" << typeid(AppCoordType).name() << " screenWidth, " << typeid(AppCoordType).name() << " screenHeight, " 
					   << typeid(AppCoordType).name() << " pixelWidth, " << typeid(AppCoordType).name() << " pixelHeight)\n";
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
									output << " + ";
								nonZeroBefore = true;

								if(base(4*i+j,uc).numerator() != 1)
									output << base(4*i+j,uc).numerator() << '*';
								output << unknownConstants[uc];
								if(base(4*i+j,uc).denominator() != 1)
									output << '/' << base(4*i+j,uc).denominator();
							} // end if
						if(offset(4*i+j) != 0 || !nonZeroBefore)
						{
							if(nonZeroBefore)
								output << " + ";
							nonZeroBefore = true;

							output << offset(4*i+j).numerator();
							if(offset(4*i+j).denominator() != 1)
								output << "/(float)" << offset(4*i+j).denominator();
						} // end if
						output << ";\n";
					} // end for
					if(i != controls.size()-2) output << "\n";
				} // end for

				output << "} // end function updateCoordinates\n";
			} // end method outputOpenGLCppApp

			//template<typename RationalType, typename AppCoordType, typename IDType /* = int */, typename NameType /* = std::string */> // current compiler version does not support default arguments
			//void compile(const std::string &headerName) const
			//{
			//	auto systemMatrix = generateSystemMatrix<RationalType,IDType,NameType>();

			//	LinearSystem::numericReducedRowEchelonFormNoPivot(systemMatrix);
			//	auto investigation = LinearSystem::semiSymbolicInvestigate(systemMatrix,4);
			//	auto solution = LinearSystem::semiSymbolicSolve(systemMatrix,4);
			//	auto base = std::get<0>(solution);
			//	auto offset = std::get<1>(solution);

			//	// assume unique solution for now...
			//	std::ofstream output(headerName);
			//	outputCppApp<RationalType,AppCoordType>(base,offset,output);
			//	output.close();

			//	std::system("devenv \"..\\GUI Builder.sln\" /Clean \"Debug|Win32\" /Project \"Generated Application\"");
			//	std::system("devenv \"..\\GUI Builder.sln\" /Build \"Debug|Win32\" /Project \"Generated Application\"");
			//} // end method compile

			void run(const std::string &executableName) const
			{
				std::system(executableName.c_str());
			} // end method run

			// UI
			void render() const
			{
				// render controls
				for(const auto &control : controls)
					control.render();

				// render constraints
				for(const auto &constraint : constraints)
					constraint.render();

				// render buttons (buttons should be in front)
				for(const auto &button : buttons)
					button.first.render();

				// render text boxes (text boxes as well)
				tbFileName.render();

				if(selectedPart)
					selectedPart->render();

				if(caret)
					caret->render();
			} // end method render

			void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
			{
				// TODO: move application exit somewhere outside Model class (or any UI widget for that matter)
				if(code == 27 && down) // escape key
				{
					if(caret)
					{
						caret = nullptr;
						if(focusedControl != controls.rend())
							focusedControl->unfocus();
						focusedControl = controls.rend();
						tbFileName.unfocus();
					}
					else
						std::exit(0);
				}
				else if(caret)
					caret->keyboardAscii(code,down,x,y);
				else if(down && code == 0x7f) // delete key
					if(selectedConstraint != constraints.end())
					{
						eraseConstraint(selectedConstraint);
						highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
						selectedPart = nullptr;
					}
					else if(selectedControl != controls.rend() && selectedControl != controls.rend()-1)
					{
						eraseControl(selectedControl.base()-1);
						highlightedControl = selectedControl = focusedControl = controls.rend();
						highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
						// assumes more invalidations than actually happen but will fix when I switch to std::list
						selectedPart = nullptr;						 
					} // end if
			} // end method keyboardAscii

			void keyboardNonAscii(graphene::Bases::EventHandling::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
			{
				if(caret)
					caret->keyboardNonAscii(key,down,x,y);
			} // end method keyboardNonAscii

			void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
			{
				if(button == 0)
					if(down)
					{
						assert(highlightedButton == buttons.end() || highlightedControl == controls.rend() || !tbFileName.highlighted());

						if(highlightedButton != buttons.end())
						{
							highlightedButton->first.press();
							pressedButton = highlightedButton;
						} // pressing a button should not deselect
						else if(tbFileName.highlighted())
						{
							if(focusedControl != controls.rend())
							{
								focusedControl->unfocus();
								focusedControl = controls.rend();
							} // end if
							if(focusedConstraint != constraints.end())
							{
								focusedConstraint->unfocus();
								focusedConstraint = constraints.end();
							} // end if
							tbFileName.focus();
							caret = tbFileName.charUnderPoint(x,y);
						} // clicking the text box should not deselect either
						else
						{
							// deselect all
							if(selectedControl != controls.rend())
							{
								selectedControl->deselect();
								selectedControl = controls.rend();
							} // end if
							if(selectedConstraint != constraints.end())
							{
								selectedConstraint->deselect();
								selectedConstraint = constraints.end();
							} // end if
						} // end else

						if(highlightedControl != controls.rend())
						{
							// TODO: bring to front
							highlightedControl->select();
							selectedControl = highlightedControl;
							selectedPart = highlightedControl->partUnderPoint(x,y);

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
								tbFileName.unfocus();
								if(focusedConstraint != constraints.end())
								{
									focusedConstraint->unfocus();
									focusedConstraint = constraints.end();
								} // end if
								if(focusedControl != controls.rend())
									focusedControl->unfocus();
								highlightedControl->focus();
								focusedControl = highlightedControl;
								caret = highlightedControl->charUnderPoint(x,y);
							} // end else
						} // end if

						if(highlightedConstraint != constraints.end())
						{
							highlightedConstraint->select();
							selectedConstraint = highlightedConstraint;
							selectedPart = highlightedConstraint->partUnderPoint(x,y);

							tbFileName.unfocus();
							if(focusedControl != controls.rend())
							{
								focusedControl->unfocus();
								focusedControl = controls.rend();
							} // end if
							if(focusedConstraint != constraints.end())
								focusedConstraint->unfocus();
							highlightedConstraint->focus();
							focusedConstraint = highlightedConstraint;
							caret = highlightedConstraint->charUnderPoint(x,y);
						} // end if

						if(pressedButton == buttons.end() && selectedControl == controls.rend() && !tbFileName.highlighted() && selectedConstraint == constraints.end())
							createOnMove = true;

						lastX = x;
						lastY = y;
					}
					else
					{
						if(pressedButton != buttons.end() && pressedButton->first.pressed())
						{
							pressedButton->second();
							pressedButton->first.depress();
						} // end if
						pressedButton = buttons.end();

						selectedPart = nullptr;

						createOnMove = false;
					} // end else
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
						tbFileName.unfocus();
						if(focusedControl != controls.rend())
							focusedControl->unfocus();
						if(highlightedControl != controls.rend())
							highlightedControl->dehighlight();
						controls.push_back(control_type(x,y,x,y,borderSize,"control"+std::to_string(controlIndex++),controlTextHeight)); // emplace_back can't take 6+ arguments yet.
						focusedControl = highlightedControl = selectedControl = controls.rbegin();
						highlightedControl->highlight();
						selectedControl->select();
						selectedPart = selectedControl->partUnderPoint(x,y); // TODO: guarantee this will be a corner
						focusedControl->focus();
						caret = focusedControl->charUnderPoint(x,y);

						// add automatic constraints (temporary code)
						constraints.push_back(constraint_type(&controls,controls.size()-1,side_type::LEFT,controls.size()-1,side_type::RIGHT,y,y+constraintThickness,"0mm",constraintTextHeight));
						constraints.push_back(constraint_type(&controls,controls.size()-1,side_type::BOTTOM,controls.size()-1,side_type::TOP,x,x+constraintThickness,"0mm",constraintTextHeight));
						constraints.push_back(constraint_type(&controls,0,side_type::LEFT,controls.size()-1,side_type::LEFT,y,y+constraintThickness,"0mm",constraintTextHeight));
						constraints.push_back(constraint_type(&controls,0,side_type::BOTTOM,controls.size()-1,side_type::BOTTOM,x,x+constraintThickness,"0mm",constraintTextHeight));
						highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
					} // end if

					// dehighlight all
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

					tbFileName.dehighlight();

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
								button->first.highlight();
								highlightedButton = button;
								break;
							} // end if

						if(highlightedButton == buttons.end())
							tbFileName.highlighted() = tbFileName.contains(x,y);

						if(!tbFileName.highlighted())
							for(auto constraint = constraints.begin() ; constraint < constraints.end() ; ++constraint)
								if(constraint->contains(x,y))
								{
									constraint->highlight();
									highlightedConstraint = constraint;
									break;
								} // end if

						if(highlightedConstraint == constraints.end())
							for(auto control = controls.rbegin() ; control < controls.rend() ; ++control) // TODO: restore front to back iteration when screen-at-front issue fixed
								if(control->contains(x,y))
								{
									control->highlight();
									highlightedControl = control;
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

	} // end namespace Controls

} // end namespace GUIModel

#endif // GUI_MODEL_H
