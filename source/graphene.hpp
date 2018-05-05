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

#ifndef GRAPHENE_H
#define GRAPHENE_H

#include <ratio>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>
#include <type_traits>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "opengl overloads.hpp"

#include "geometry.hpp"

namespace graphene
{
	// TODO: add overloads taking suitable vector types

	namespace Bases
	{
		class Empty {};

		template<typename BaseType>
		class Destructible : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;

			// Constructors
		public:
			Destructible() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual ~Destructible() = default;
		}; // end class Destructible

		template<typename BaseType>
		class Renderable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;

			// Constructors
		public:
			Renderable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual void render() const = 0;
		}; // end class Renderable

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Movable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			Movable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual void move(CoordinateType xOffset, CoordinateType yOffset) = 0;
		}; // end class Movable

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Rectangular : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			Rectangular() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual CoordinateType &left() = 0;
			virtual const CoordinateType &left() const = 0;
			virtual CoordinateType &bottom() = 0;
			virtual const CoordinateType &bottom() const = 0;
			virtual CoordinateType &right() = 0;
			virtual const CoordinateType &right() const = 0;
			virtual CoordinateType &top() = 0;
			virtual const CoordinateType &top() const = 0;

			virtual CoordinateType &side(geometry::RectangleSide sideName) = 0;
			virtual const CoordinateType &side(geometry::RectangleSide sideName) const = 0;

			virtual CoordinateType width() const = 0;
			virtual CoordinateType height() const = 0;
		}; // end class Rectangular

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Containing : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			Containing() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual bool contains(CoordinateType x, CoordinateType y) const = 0;
		}; // end class Containing

		template<typename BaseType, typename PointerType = typename BaseType::pointer_type>
		class Pointing : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using pointer_type = PointerType;

			// Constructors
		public:
			Pointing() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual PointerType &pointer() = 0;
			virtual const PointerType &pointer() const = 0;
		}; // end class Pointing

		template<typename BaseType, typename IndexType = typename BaseType::index_type>
		class Indexing : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using index_type = IndexType;

			// Constructors
		public:
			Indexing() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual IndexType &index() = 0;
			virtual const IndexType &index() const = 0;
		}; // end class Indexing

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Offset : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			Offset() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual CoordinateType &xOffset() = 0;
			virtual const CoordinateType &xOffset() const = 0;
			virtual CoordinateType &yOffset() = 0;
			virtual const CoordinateType &yOffset() const = 0;
		}; // end class Offset

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Selectable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			// Constructors
		public:
			Selectable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual FrameStackType &select() = 0;
			virtual FrameStackType &deselect() = 0;
			virtual bool &selected() = 0;
			virtual const bool &selected() const = 0;
		}; // end class Selectable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Pressable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			// Constructors
		public:
			Pressable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual FrameStackType &press() = 0;
			virtual FrameStackType &depress() = 0;
			virtual bool &pressed() = 0;
			virtual const bool &pressed() const = 0;
		}; // end class Pressable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Highlightable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			// Constructors
		public:
			Highlightable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual FrameStackType &highlight() = 0;
			virtual FrameStackType &dehighlight() = 0;
			virtual bool &highlighted() = 0;
			virtual const bool &highlighted() const = 0;
		}; // end class Highlightable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Focusable : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			// Constructors
		public:
			Focusable() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual FrameStackType &focus() = 0;
			virtual FrameStackType &unfocus() = 0;
			virtual bool &focused() = 0;
			virtual const bool &focused() const = 0;
		}; // end class Focusable

		template<typename BaseType, typename BorderSizeType = typename BaseType::border_size_type>
		class UniformlyBordered : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using border_size_type = BorderSizeType;

			// Constructors
		public:
			UniformlyBordered() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual BorderSizeType &borderSize() = 0;
			virtual const BorderSizeType &borderSize() const = 0;
		}; // end class UniformlyBordered

		template<typename BaseType, typename TextType = typename BaseType::text_type>
		class Textual : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using text_type = TextType;

			// Constructors
		public:
			Textual() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual TextType &text() = 0;
			virtual const TextType &text() const = 0;
		}; // end class Textual

		/** SizedText instances have a fixed 'natural' aspect ratio, so changing the height,
		 *	actually changes the width as well. Some frame stacks, may interpret  text sizes, as
		 *	preferred instead of mandatory.
		 */
		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedText : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			SizedText() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual CoordinateType &textHeight() = 0;
			virtual const CoordinateType &textHeight() const = 0;
			// TODO: add support for setting the text width using properties, and perhaps the ability
			// to set/get the aspect ratio.
			virtual CoordinateType textWidth() const = 0;
			virtual CoordinateType textCharWidth(size_t index) const = 0;
			virtual void setTextWidth(const CoordinateType &value) = 0; // deprecated
			// TODO: allow arbitrary writable height & width combinations, add a preferred aspect ratio
			// and add preferred width, height read-only properties.
		}; // end class SizedText

		/** This frame is intended for use in frame stacks that use textHeight/textWidth as a
		 *	"preferred" size and need another method to return the actual text height/width.
		 *	The first pair member is the width and the second the height.
		 */
		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class AdaptableSizeText : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			AdaptableSizeText() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual std::pair<CoordinateType,CoordinateType> effectiveTextSize() const = 0;
			virtual std::pair<CoordinateType,CoordinateType> effectiveTextCharSize(size_t index) const = 0;
		}; // end class AdaptableSizeText

		template<typename BaseType, typename NameType = typename BaseType::name_type>
		class Named : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using name_type = NameType;

			// Constructors
		public:
			Named() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual NameType &name() = 0;
			virtual const NameType &name() const = 0;
		}; // end class Named

		/** SizedName instances have a fixed 'natural' aspect ratio, so changing the height,
		 *	actually changes the width as well. Some frame stacks, may interpret  name sizes, as
		 *	preferred instead of mandatory.
		 */
		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedName : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			SizedName() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual CoordinateType &nameHeight() = 0;
			virtual const CoordinateType &nameHeight() const = 0;
			// TODO: add support for setting the name width using properties, and perhaps the ability
			// to set/get the aspect ratio.
			virtual CoordinateType nameWidth() const = 0;
			virtual CoordinateType nameCharWidth(size_t index) const = 0;
			virtual void setNameWidth(const CoordinateType &value) = 0; // deprecated
			// TODO: allow arbitrary writable height & width combinations, add a preferred aspect ratio
			// and add preferred width, height read-only properties.
		}; // end class SizedName

		/** This frame is intended for use in frame stacks that use nameHeight/nameWidth as a
		 *	"preferred" size and need another method to return the actual name height/width.
		 *	The first pair member is the width and the second the height.
		 */
		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class AdaptableSizeName : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			AdaptableSizeName() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual std::pair<CoordinateType,CoordinateType> effectiveNameSize() const = 0;
			virtual std::pair<CoordinateType,CoordinateType> effectiveNameCharSize(size_t index) const = 0;
		}; // end class AdaptableSizeName

		/** If a MultiPart is also Containing then partUnderPoint(x,y) should return a value designating "no part"
		 *	if and only if contains(x,y) returns false.
		 */
		template<typename BaseType, typename PartType = typename BaseType::part_type, typename ConstPartType = typename BaseType::const_part_type, typename CoordinateType = typename BaseType::coordinate_type>
		class MultiPart : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using part_type = PartType;
			using const_part_type = ConstPartType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			MultiPart() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual PartType partUnderPoint(CoordinateType x, CoordinateType y) = 0;
			virtual ConstPartType partUnderPoint(CoordinateType x, CoordinateType y) const = 0;
		}; // end class MultiPart

		/** If a MultiChar is also Containing then charUnderPoint(x,y) should return a value designating "no part"
		 *	if and only if contains(x,y) returns false.
		 */
		// TODO: perhaps find a better name to disambiguate with char?
		template<typename BaseType, typename CharType = typename BaseType::char_type, typename ConstCharType = typename BaseType::const_char_type,
			typename IndexType = typename BaseType::index_type, typename CoordinateType = typename BaseType::coordinate_type>
		class MultiChar : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using char_type = CharType;
			using const_char_type = ConstCharType;
			using index_type = IndexType;
			using coordinate_type = CoordinateType;

			// Constructors
		public:
			MultiChar() = default;
			using BaseType::BaseType;

			// Methods
		public:
			virtual CharType charUnderPoint(CoordinateType x, CoordinateType y) = 0;
			virtual ConstCharType charUnderPoint(CoordinateType x, CoordinateType y) const = 0;
			virtual CharType charWithIndex(IndexType i) = 0;
			virtual ConstCharType charWithIndex(IndexType i) const = 0;
		}; // end class MultiChar

		template<typename BaseType, typename Const = std::false_type, typename CharType = typename BaseType::char_type>
		class CaretLike;

		template<typename BaseType, typename CharType>
		class CaretLike<BaseType, std::true_type, CharType> : public BaseType
		{
			// Member Types
		public:
			using base_type = BaseType;
			using char_type = CharType;

			// Constructors
		public:
			CaretLike() = default;
			using BaseType::BaseType;

			// Methods
		public:
			// TODO: consider more methods like moveToLeft/Right/Up/Down (will differ in e.g. right-to-left scripts)
			// TODO: consider arguments like wraparound:bool
			virtual void nextPosition() = 0;
			virtual void prevPosition() = 0;
			virtual void firstPosition() = 0;
			virtual void lastPosition() = 0;
		}; // end class CaretLike

		template<typename BaseType, typename CharType>
		class CaretLike<BaseType, std::false_type, CharType> : public CaretLike<BaseType, std::true_type, CharType>
		{
			// Member Types
			using base = CaretLike<BaseType, std::true_type, CharType>;

			// Constructors
		public:
			CaretLike() = default;
			using base::base;

			// Methods
		public:
			virtual void eraseNext() = 0;
			virtual void erasePrev() = 0;
			virtual void insert(CharType character) = 0;
		}; // end class CaretLike

		namespace EventHandling
		{
			enum class NonAsciiKey: unsigned{F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PAGE_UP,PAGE_DOWN,
				HOME,END,LEFT,RIGHT,UP,DOWN,INSERT,L_SHIFT,R_SHIFT,L_ALT,R_ALT,L_CTRL,R_CTRL,NUM_LOCK};

			/** Controls implementing this interface should document whether they expect to receive all
			 *	mouse events or only the ones that happen above them.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class KeyboardAndMouse : public BaseType
			{
				// Member Types
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
			public:
				KeyboardAndMouse() = default;
				using BaseType::BaseType;

				// Methods
			public:
				virtual void keyboardAscii(unsigned char charCode, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void keyboardNonAscii(NonAsciiKey key, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseEnter(CoordinateType x, CoordinateType y) = 0;
				virtual void mouseMove(CoordinateType x, CoordinateType y) = 0;
				virtual void mouseExit(CoordinateType x, CoordinateType y) = 0;
			}; // end class KeyboardAndMouse

		} // end namespace EventHandling

	} // end namespace Bases


	namespace Frames
	{
		namespace Movable
		{
			/** The base type should be Rectangular
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class Rectangular : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Rectangular() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void move(CoordinateType xOffset, CoordinateType yOffset)
				{
					this->left() += xOffset;
					this->bottom() += yOffset;
					this->right() += xOffset;
					this->top() += yOffset;
				} // end method move
			}; // end class Rectangular

			/** The base type should be Movable
			 */
			template<typename BaseType, typename allowHorizontal = std::true_type, typename allowVertical = std::true_type, typename CoordinateType = typename BaseType::coordinate_type>
			class HVMovable : public BaseType
			{
				/**********************
				*    Concept Check    *
				**********************/

				static_assert(std::is_same<allowHorizontal, std::true_type>() || std::is_same<allowHorizontal, std::false_type>(),
					"allowHorizontal must be either std::true_type or std::false_type!");
				static_assert(std::is_same<allowVertical, std::true_type>() || std::is_same<allowVertical, std::false_type>(),
					"allowVertical must be either std::true_type or std::false_type!");

				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				HVMovable() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void move(CoordinateType xOffset, CoordinateType yOffset)
				{
					BaseType::move(allowHorizontal()?xOffset:0 , allowVertical()?yOffset:0);
				} // end method move
			}; // end class HVMovable

		} // end namespace Movable

		template<typename BaseType, typename PointerType = typename BaseType::pointer_type>
		class Pointing : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using pointer_type = PointerType;

			/***************
			*    Fields    *
			***************/
		private:
			PointerType iPointer = nullptr;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Pointing() = default;

			template<typename... ArgTypes>
			Pointing(PointerType &&pointer, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iPointer(std::forward<PointerType>(pointer))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			PointerType &pointer()
			{
				return iPointer;
			} // end method pointer

			const PointerType &pointer() const
			{
				return iPointer;
			} // end method pointer
		}; // end class Pointing

		template<typename BaseType, typename IndexType = typename BaseType::index_type>
		class Indexing : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using index_type = IndexType;

			/***************
			*    Fields    *
			***************/
		private:
			IndexType iIndex;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Indexing() = default;

			template<typename... ArgTypes>
			Indexing(IndexType &&index, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iIndex(std::forward<IndexType>(index))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			IndexType &index()
			{
				return iIndex;
			} // end method index

			const IndexType &index() const
			{
				return iIndex;
			} // end method index
		}; // end class Indexing

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Offset : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;

			/***************
			*    Fields    *
			***************/
		private:
			CoordinateType iXoffset = 0;
			CoordinateType iYoffset = 0;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Offset() = default;

			template<typename... ArgTypes>
			Offset(CoordinateType &&xOffset, CoordinateType &&yOffset, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...),
				 iXoffset(std::forward<CoordinateType>(xOffset)),
				 iYoffset(std::forward<CoordinateType>(yOffset))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			CoordinateType &xOffset()
			{
				return iXoffset;
			} // end method xOffset

			const CoordinateType &xOffset() const
			{
				return iXoffset;
			} // end method xOffset

			CoordinateType &yOffset()
			{
				return iYoffset;
			} // end method yOffset

			const CoordinateType &yOffset() const
			{
				return iYoffset;
			} // end method yOffset
		}; // end class Offset

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Selectable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Selectable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			/***************
			*    Fields    *
			***************/
		private:
			bool iSelected = false;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Selectable() = default;

			template<typename... ArgTypes>
			Selectable(bool selected, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iSelected(selected)
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &select()
			{
				iSelected = true;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method select

			FrameStackType &deselect()
			{
				iSelected = false;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method deselect

			bool &selected()
			{
				return iSelected;
			} // end method selected

			const bool &selected() const
			{
				return iSelected;
			} // end method selected
		}; // end class Selectable


		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Pressable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Pressable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			/***************
			*    Fields    *
			***************/
		private:
			bool iPressed = false;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Pressable() = default;

			template<typename... ArgTypes>
			Pressable(bool pressed, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iPressed(pressed)
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &press()
			{
				iPressed = true;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method press

			FrameStackType &depress()
			{
				iPressed = false;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method depress

			bool &pressed()
			{
				return iPressed;
			} // end method pressed

			const bool &pressed() const
			{
				return iPressed;
			} // end method pressed
		}; // end class Pressable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Highlightable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Highlightable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			/***************
			*    Fields    *
			***************/
		private:
			bool iHighlighted = false;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Highlightable() = default;

			template<typename... ArgTypes>
			Highlightable(bool highlighted, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iHighlighted(highlighted)
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &highlight()
			{
				iHighlighted = true;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method highlight

			FrameStackType &dehighlight()
			{
				iHighlighted = false;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method dehighlight

			bool &highlighted()
			{
				return iHighlighted;
			} // end method highlighted

			const bool &highlighted() const
			{
				return iHighlighted;
			} // end method highlighted
		}; // end class Highlightable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Focusable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Focusable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/***************
			*    Fields    *
			***************/
		private:
			bool iFocused = false;

			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using frame_stack_type = FrameStackType;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Focusable() = default;

			template<typename... ArgTypes>
			Focusable(bool focused, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iFocused(focused)
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &focus()
			{
				iFocused = true;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method focus

			FrameStackType &unfocus()
			{
				iFocused = false;
				return *static_cast<FrameStackType *>(this); // can be dangerous without check...
			} // end method unfocus

			bool &focused()
			{
				return iFocused;
			} // end method focused

			const bool &focused() const
			{
				return iFocused;
			} // end method focused
		}; // end class Focusable

		template<typename BaseType, typename BorderSizeType = typename BaseType::border_size_type>
		class UniformlyBordered : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using border_size_type = BorderSizeType;

			/***************
			*    Fields    *
			***************/
		private:
			BorderSizeType iBorderSize = 0;

			/*********************
			*    Constructors    *
			*********************/
		public:
			UniformlyBordered() = default;

			template<typename... ArgTypes>
			UniformlyBordered(border_size_type &&borderSize, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iBorderSize(std::forward<border_size_type>(borderSize))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			BorderSizeType &borderSize()
			{
				return iBorderSize;
			} // end method borderSize

			const BorderSizeType &borderSize() const
			{
				return iBorderSize;
			} // end method borderSize
		}; // end class UniformlyBordered

		template<typename BaseType, typename TextType = typename BaseType::text_type>
		class Textual : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using text_type = TextType;

			/***************
			*    Fields    *
			***************/
		private:
			TextType iText;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Textual() = default;

			template<typename... ArgTypes>
			Textual(text_type &&text, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iText(std::forward<text_type>(text))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			TextType &text()
			{
				return iText;
			} // end method text

			const TextType &text() const
			{
				return iText;
			} // end method text
		}; // end class Textual

		/** BaseType should be Textual, TextEngineType should be default constructible
		 */
		template<typename BaseType, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedText : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;
			using font_engine_type = FontEngineType;

			/***************
			*    Fields    *
			***************/
		private:
			CoordinateType iTextHeight;

			/*********************
			*    Constructors    *
			*********************/
		public:
			SizedText() = default;

			template<typename... ArgTypes>
			SizedText(CoordinateType &&textHeight, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iTextHeight(std::forward<CoordinateType>(textHeight))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			CoordinateType &textHeight()
			{
				return iTextHeight;
			} // end method textHeight

			const CoordinateType &textHeight() const
			{
				return iTextHeight;
			} // end method textHeight

			// TODO: add support for setting the text width using properties, and perhaps the ability
			// to set/get the aspect ratio.
			CoordinateType textWidth() const
			{
				FontEngineType fontEngine;
				return fontEngine.stringWidth(this->text()) * textHeight() / fontEngine.fontHeight();
			} // end method textWidth

			CoordinateType textCharWidth(size_t index) const
			{
				FontEngineType fontEngine;
				return fontEngine.charWidth(this->text().at(index)) * textHeight() / fontEngine.fontHeight();
			} // end method textCharWidth

			void setTextWidth(const CoordinateType &value) // deprecated
			{
				FontEngineType fontEngine;
				textHeight() = fontEngine.fontHeight() * value / fontEngine.stringWidth(this->text());
			} // end method setTextWidth
		}; // end class SizedText

		/** BaseType should be Rectangular and SizedText, TextEngineType should be default constructible
		 */
		template<typename BaseType, typename Margin = typename BaseType::margin, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>
		class BoxedAdaptableSizeText : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;
			using font_engine_type = FontEngineType;
			using margin = Margin;

			/*********************
			*    Constructors    *
			*********************/
		public:
			BoxedAdaptableSizeText() = default;

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			std::pair<CoordinateType,CoordinateType> effectiveTextSize() const
			{
				font_engine_type fontEngine;

				// scale text down to fit in rectangle:
				CoordinateType effectiveTextHeight = std::min(this->textHeight(),(this->height()*margin::den - 2*margin::num) / margin::den);
				CoordinateType effectiveTextWidth = std::min(effectiveTextHeight * fontEngine.stringWidth(this->text()) / fontEngine.fontHeight(),(this->width()*margin::den - 2*margin::num) / margin::den);
				if(this->text().empty())
					return std::make_pair(static_cast<CoordinateType>(0),effectiveTextHeight); // avoid division by zero
				effectiveTextHeight = std::min(effectiveTextHeight, effectiveTextWidth * fontEngine.fontHeight() / fontEngine.stringWidth(this->text()));

				return std::make_pair(effectiveTextWidth,effectiveTextHeight);
			} // end method effectiveTextSize

			// TODO: This is too inefficient. Consider caching.
			std::pair<CoordinateType,CoordinateType> effectiveTextCharSize(size_t index) const
			{
				font_engine_type fontEngine;
				auto result = effectiveTextSize();
				result.first = fontEngine.charWidth(this->text().at(index)) * result.second / fontEngine.fontHeight();

				return  result;
			} // end method effectiveTextCharSize
		}; // end class BoxedAdaptableSizeText

		template<typename BaseType, typename NameType = typename BaseType::name_type>
		class Named : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using name_type = NameType;

			/***************
			*    Fields    *
			***************/
		private:
			NameType iName;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Named() = default;

			template<typename... ArgTypes>
			Named(NameType &&name, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iName(std::forward<NameType>(name))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			NameType &name()
			{
				return iName;
			} // end method name

			const NameType &name() const
			{
				return iName;
			} // end method name
		}; // end class Named

		/** BaseType should be Named, TextEngineType should be default constructible
		 */
		template<typename BaseType, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedName : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;
			using font_engine_type = FontEngineType;

			/***************
			*    Fields    *
			***************/
		private:
			CoordinateType iNameHeight = 0;

			/*********************
			*    Constructors    *
			*********************/
		public:
			SizedName() = default;

			template<typename... ArgTypes>
			SizedName(CoordinateType &&nameHeight, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...), iNameHeight(std::forward<CoordinateType>(nameHeight))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			CoordinateType &nameHeight()
			{
				return iNameHeight;
			} // end method nameHeight

			const CoordinateType &nameHeight() const
			{
				return iNameHeight;
			} // end method nameHeight

			// TODO: add support for setting the name width using properties, and perhaps the ability
			// to set/get the aspect ratio.
			CoordinateType nameWidth() const
			{
				FontEngineType fontEngine;
				return fontEngine.stringWidth(this->name()) * nameHeight() / fontEngine.fontHeight();
			} // end method nameWidth

			CoordinateType nameCharWidth(size_t index) const
			{
				FontEngineType fontEngine;
				return fontEngine.charWidth(this->name().at(index)) * nameHeight() / fontEngine.fontHeight();
			} // end method nameCharWidth

			void setNameWidth(const CoordinateType &value) // deprecated
			{
				FontEngineType fontEngine;
				nameHeight() = fontEngine.fontHeight() * value / fontEngine.stringWidth(this->name());
			} // end method setNameWidth
		}; // end class SizedName

		/** BaseType should be Rectangular and SizedName, TextEngineType should be default constructible
		 */
		template<typename BaseType, typename Margin = typename BaseType::margin, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>
		class BoxedAdaptableSizeName : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using coordinate_type = CoordinateType;
			using font_engine_type = FontEngineType;
			using margin = Margin;

			/*********************
			*    Constructors    *
			*********************/
		public:
			BoxedAdaptableSizeName() = default;

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			std::pair<CoordinateType,CoordinateType> effectiveNameSize() const
			{
				font_engine_type fontEngine;

				// scale name down to fit in rectangle:
				CoordinateType effectiveNameHeight = std::min(this->nameHeight(),(this->height()*margin::den - 2*margin::num) / margin::den);
				CoordinateType effectiveNameWidth = std::min(effectiveNameHeight * fontEngine.stringWidth(this->name()) / fontEngine.fontHeight(),(this->width()*margin::den - 2*margin::num) / margin::den);
				if(this->name().empty())
					return std::make_pair(static_cast<CoordinateType>(0),effectiveNameHeight); // avoid division by zero
				effectiveNameHeight = std::min(effectiveNameHeight, effectiveNameWidth * fontEngine.fontHeight() / fontEngine.stringWidth(this->name()));

				return std::make_pair(effectiveNameWidth,effectiveNameHeight);
			} // end method effectiveNameSize

			// TODO: This is too inefficient. Consider caching.
			std::pair<CoordinateType,CoordinateType> effectiveNameCharSize(size_t index) const
			{
				font_engine_type fontEngine;
				auto result = effectiveNameSize();
				result.first = fontEngine.charWidth(this->name().at(index)) * result.second / fontEngine.fontHeight();

				return  result;
			} // end method nameCharWidth
		}; // end class BoxedAdaptableSizeName

		/**	BaseType should be Rectangular, UniformlyBordered and Containing, and PartType should be a (preferably smart) pointer type.
		 *	ConcretePartTemplate instantiations should be constructible from 4 rectangle sides.
		 */
		template<typename BaseType, template<typename CoordinateType, typename horizontal, typename vertical, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef> class ConcretePartTemplate,
			typename PartType = typename BaseType::part_type, typename ConstPartType = typename BaseType::const_part_type, typename CoordinateType = typename BaseType::coordinate_type>
		class MultiPartBorderedRectangle : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using part_type = PartType;
			using const_part_type = ConstPartType;
			using coordinate_type = CoordinateType;
			// TODO: add template alias for ConcretePartTemplate when implemented

			struct concrete_return_type
			{
				using       bottom_left  =       ConcretePartTemplate<CoordinateType, std::true_type, std::true_type,false, true, true,false,false>;
				using const_bottom_left  = const ConcretePartTemplate<CoordinateType, std::true_type, std::true_type, true, true, true,false,false>;
				using       left         =       ConcretePartTemplate<CoordinateType, std::true_type,std::false_type,false, true,false,false,false>;
				using const_left         = const ConcretePartTemplate<CoordinateType, std::true_type,std::false_type, true, true,false,false,false>;
				using       top_left     =       ConcretePartTemplate<CoordinateType, std::true_type, std::true_type,false, true,false,false, true>;
				using const_top_left     = const ConcretePartTemplate<CoordinateType, std::true_type, std::true_type, true, true,false,false, true>;
				using       bottom       =       ConcretePartTemplate<CoordinateType,std::false_type, std::true_type,false,false, true,false,false>;
				using const_bottom       = const ConcretePartTemplate<CoordinateType,std::false_type, std::true_type, true,false, true,false,false>;
				using       center       =       ConcretePartTemplate<CoordinateType, std::true_type, std::true_type,false, true, true, true, true>;
				using const_center       = const ConcretePartTemplate<CoordinateType, std::true_type, std::true_type, true, true, true, true, true>;
				using       top          =       ConcretePartTemplate<CoordinateType,std::false_type, std::true_type,false,false,false,false, true>;
				using const_top          = const ConcretePartTemplate<CoordinateType,std::false_type, std::true_type, true,false,false,false, true>;
				using       bottom_right =       ConcretePartTemplate<CoordinateType, std::true_type, std::true_type,false,false, true, true,false>;
				using const_bottom_right = const ConcretePartTemplate<CoordinateType, std::true_type, std::true_type, true,false, true, true,false>;
				using       right        =       ConcretePartTemplate<CoordinateType, std::true_type,std::false_type,false,false,false, true,false>;
				using const_right        = const ConcretePartTemplate<CoordinateType, std::true_type,std::false_type, true,false,false, true,false>;
				using       top_right    =       ConcretePartTemplate<CoordinateType, std::true_type, std::true_type,false,false,false, true, true>;
				using const_top_right    = const ConcretePartTemplate<CoordinateType, std::true_type, std::true_type, true,false,false, true, true>;
			}; // end struct concrete_return_type

			/*********************
			*    Constructors    *
			*********************/
		public:
			MultiPartBorderedRectangle() = default;

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: does it work in every case where left() > right() || bottom() > top()?
#define partUnderPointMacro(PartType,Const,const_) \
			PartType partUnderPoint(CoordinateType x, CoordinateType y) Const\
			{\
				auto &left = this->left() < this->right() ? this->left() : this->right();\
				auto &bottom = this->bottom() < this->top() ? this->bottom() : this->top();\
				auto &right = this->left() >= this->right() ? this->left() : this->right();\
				auto &top = this->bottom() >= this->top() ? this->bottom() : this->top();\
				auto borderSize = this->borderSize();\
				\
				if(left <= x && x <= right && bottom <= y && y <= top)\
				{\
					if(x <= left+borderSize)\
					{\
						if(y <= bottom+borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##bottom_left(left,bottom,left+borderSize,bottom+borderSize));\
						}\
						else if(y < top-borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##left(left,bottom+borderSize,left+borderSize,top-borderSize));\
						}\
						else /* top-borderSize <= y */\
						{\
							return PartType(new typename concrete_return_type::const_##top_left(left,top-borderSize,left+borderSize,top));\
						} /* end else */\
					}\
					else if(x < right-borderSize)\
					{\
						if(y <= bottom+borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##bottom(left+borderSize,bottom,right-borderSize,bottom+borderSize));\
						}\
						else if(y < top-borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##center(left,bottom,right,top));\
						}\
						else /* top-borderSize <= y */\
						{\
							return PartType(new typename concrete_return_type::const_##top(left+borderSize,top-borderSize,right-borderSize,top));\
						} /* end else */\
					}\
					else /* right-borderSize <= x */\
					{\
						if(y <= bottom+borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##bottom_right(right-borderSize,bottom,right,bottom+borderSize));\
						}\
						else if(y < top-borderSize)\
						{\
							return PartType(new typename concrete_return_type::const_##right(right-borderSize,bottom+borderSize,right,top-borderSize));\
						}\
						else /* top-borderSize <= y */\
						{\
							return PartType(new typename concrete_return_type::const_##top_right(right-borderSize,top-borderSize,right,top));\
						} /* end else */\
					} /* end else */\
				}\
				else\
					return nullptr;\
			} // end method partUnderPoint

			partUnderPointMacro(PartType,/* omit */,/* omit */)
			partUnderPointMacro(ConstPartType,const,const_)
#undef partUnderPointMacro
		}; // end class MultiPartBorderedRectangle

		/**	BaseType should be Rectangular, UniformlyBordered, Containing, have a font_engine_type member type and
		 *  expose text traits via the supplied TextConceptMap. CharType should be a (preferably smart) pointer type.
		 *	ConcreteCharTemplate instantiations should be constructible from a reference to *this, an index and an instance of CoordinateType.
		 */
		template<typename BaseType, typename TextConceptMap,
			typename CharType = typename BaseType::char_type,
			typename ConstCharType = typename BaseType::const_char_type,
			typename ConcreteCharType = typename BaseType::concrete_char_type,
			typename ConstConcreteCharType = typename BaseType::const_concrete_char_type,
			typename IndexType = typename BaseType::index_type,
			typename FontEngineType = typename BaseType::font_engine_type,
			typename CoordinateType = typename BaseType::coordinate_type>
		class MultiCharBorderedRectangle : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using font_engine_type = FontEngineType;
			using char_type = CharType;
			using const_char_type = ConstCharType;
			using concrete_char_type = ConcreteCharType;
			using const_concrete_char_type = ConstConcreteCharType;
			using index_type = IndexType;
			using coordinate_type = CoordinateType;
			// TODO: add template alias for ConcreteCharTemplate when implemented

			/*********************
			*    Constructors    *
			*********************/
		public:
			MultiCharBorderedRectangle() = default;

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: does it work in every case where left() > right() || bottom() > top()?
#define charUnderPointMacro(CharType,ConcreteCharType,Const) \
			CharType charUnderPoint(CoordinateType x, CoordinateType y) Const\
			{\
				TextConceptMap textConceptMap;\
				FontEngineType fontEngine;\
				auto left   = std::min(this->left(),this->right());\
				auto bottom = std::min(this->bottom(),this->top());\
				auto right  = std::max(this->left(),this->right());\
				auto top    = std::max(this->bottom(),this->top());\
				\
				if(this->contains(x,y))\
				{\
					size_t i = 0;\
					coordinate_type sceneTextLeft = left + (this->width() - textConceptMap.effectiveTextSize(*this).first)/2;\
					coordinate_type fontX = (x-sceneTextLeft)*fontEngine.stringWidth(textConceptMap.text(*this)) / textConceptMap.effectiveTextSize(*this).first;\
					coordinate_type fontCharLeft = 0; /* fontX calculation can cause a division by zero, but then the loop won't run and return value will be correct. */\
					for( ; i < textConceptMap.text(*this).size() ; fontCharLeft += fontEngine.charWidth(textConceptMap.text(*this)[i]), ++i)\
						if(fontX <= fontCharLeft + fontEngine.charWidth(textConceptMap.text(*this)[i])/2)\
							break;\
					return CharType(new ConcreteCharType(static_cast<typename ConcreteCharType::pointed_to_type*>(this),i,fontCharLeft));\
															/* TODO: check that this indeed points to that type */\
				}\
				else\
					return nullptr;\
			} // end method charUnderPoint

			charUnderPointMacro(     CharType,     ConcreteCharType,/* omit */)
			charUnderPointMacro(ConstCharType,ConstConcreteCharType,const     )
#undef charUnderPointMacro

			/** IndexType is expected to be numeric and i should take values in [0,text().size()]
			 */
#define charWithIndexMacro(CharType,ConcreteCharType,Const) \
			CharType charWithIndex(IndexType i) Const\
			{\
				TextConceptMap textConceptMap;\
				FontEngineType fontEngine;\
				\
				if(i > textConceptMap.text(*this).size())\
					throw std::out_of_range("charWithIndex called with i outside the valid range of [0,text().size()].");\
				\
				coordinate_type fontCharLeft = 0;\
				for(IndexType c = 0 ; c < i ; ++c)\
					fontCharLeft += fontEngine.charWidth(textConceptMap.text(*this)[c]);\
				return CharType(new ConcreteCharType(static_cast<typename ConcreteCharType::pointed_to_type*>(this),i,fontCharLeft));\
			} // end method charWithIndex

			charWithIndexMacro(     CharType,     ConcreteCharType,/* omit */)
			charWithIndexMacro(ConstCharType,ConstConcreteCharType,const     )
#undef charWithIndexMacro
		}; // end class MultiCharBorderedRectangle

		template<typename BaseType, typename TextConceptMap,
			typename FontEngineType = typename BaseType::font_engine_type,
			typename CharType = typename BaseType::char_type>
		class MutIndirectCaretLike;

		template<typename BaseType, typename TextConceptMap,
			typename FontEngineType = typename BaseType::font_engine_type,
			typename CharType = typename BaseType::char_type>
		class ConstIndirectCaretLike : public BaseType
		{
			/****************
			*    Friends    *
			****************/

			friend class MutIndirectCaretLike<BaseType, TextConceptMap, FontEngineType, CharType>;

			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = BaseType;
			using char_type = CharType;
			using font_engine_type = FontEngineType;

			/***************
			*    Fields    *
			***************/
		private:
			TextConceptMap iTextConceptMap;
			FontEngineType iFontEngine;

			/*********************
			*    Constructors    *
			*********************/
		public:
			ConstIndirectCaretLike() = default;

			template<typename... ArgTypes>
			ConstIndirectCaretLike(TextConceptMap &&textConceptMap, FontEngineType &&fontEngine, ArgTypes&&... args)
				:base_type(std::forward<ArgTypes>(args)...),
				 iTextConceptMap(std::forward<TextConceptMap>(textConceptMap)),
				 iFontEngine(std::forward<FontEngineType>(fontEngine))
			{
				// empty body
			}

			using BaseType::BaseType;

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: consider more methods like moveToLeft/Right/Up/Down (will differ in e.g. right-to-left scripts)
			// TODO: consider arguments like wraparound:bool
			void nextPosition()
			{
				if(this->index() < iTextConceptMap.text(*this->pointer()).size())
				{
					this->xOffset() += iFontEngine.charWidth(iTextConceptMap.text(*this->pointer())[this->index()]);
					++this->index();
				} // end if
			} // end method nextPosition

			void prevPosition()
			{
				if(this->index() > 0)
				{
					--this->index();
					this->xOffset() -= iFontEngine.charWidth(iTextConceptMap.text(*this->pointer())[this->index()]);
				} // end if
			} // end method prevPosition

			void firstPosition()
			{
				this->index() = 0;
				this->xOffset() = 0;
			} // end method firstPosition

			void lastPosition()
			{
				this->index() = iTextConceptMap.text(*this->pointer()).size();
				this->xOffset() = iFontEngine.stringWidth(iTextConceptMap.text(*this->pointer()));
			} // end method lastPosition

		}; // end class ConstIndirectCaretLike

		template<typename BaseType, typename TextConceptMap, typename FontEngineType, typename CharType>
		class MutIndirectCaretLike : public ConstIndirectCaretLike<BaseType, TextConceptMap, FontEngineType, CharType>
		{
			/*********************
			*    Member Types    *
			*********************/
		private:
			using base = ConstIndirectCaretLike<BaseType, TextConceptMap, FontEngineType, CharType>;

			/*********************
			*    Constructors    *
			*********************/
		public:
			MutIndirectCaretLike() = default;
			using base::base;

			/****************
			*    Methods    *
			****************/
		public:
			void eraseNext()
			{
				this->iTextConceptMap.text(*this->pointer()).erase(this->index(),1);
			} // end method eraseNext

			void erasePrev()
			{
				if(this->index() > 0)
				{
					this->prevPosition();
					this->eraseNext();
				} // end if
			} // end method erasePrev

			void insert(CharType character)
			{
				this->iTextConceptMap.text(*this->pointer()).insert(this->index(),1,character);
				this->nextPosition();
			} // end method insert

		}; // end class MutIndirectCaretLike

		template<typename BaseType, typename TextConceptMap,
			typename FontEngineType = typename BaseType::font_engine_type,
			typename CharType = typename BaseType::char_type>
		using IndirectCaretLike = typename std::conditional<
			std::is_const<typename std::pointer_traits<typename BaseType::pointer_type>::element_type>::value,
			ConstIndirectCaretLike<BaseType, TextConceptMap, FontEngineType, CharType>,
			MutIndirectCaretLike<BaseType, TextConceptMap, FontEngineType, CharType>
		>::type;

		/** These are intended primarily to adapt geometric types that are unaware of frame architecture,
		 *	in order to present them as frames.
		 */
		namespace Adapting
		{
			template<typename BaseType, typename RectangleType>
			class Rectangular : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using rectangle_type = RectangleType;
				using coordinate_type = typename rectangle_type::coordinate_type;
				using Side = typename rectangle_type::Side;

				/***************
				*    Fields    *
				***************/
			private:
				RectangleType iRectangle;

				/*********************
				*    Constructors    *
				*********************/
			public:
				/**
				 * Default constructs base_type and the underlying RectangularType
				 */
				Rectangular() = default;

				/**
				 * Initializes underlying RectangularType using a supplied rectangle
				 * and forwards remaining arguments to base class constructor.
				 */
				template<typename... ArgTypes>
				Rectangular(RectangleType &&rectangle, ArgTypes&&... args)
					:base_type(std::forward<ArgTypes>(args)...), iRectangle(std::forward<RectangleType>(rectangle))
				{
					// empty body
				}

				/**
				 * Default construct the underlying RectangularType forwarding arguments to base class constructors.
				 */
				using BaseType::BaseType;

				/**
				 * Forwards arguments to the underlying RectangularType constructor
				 */
				template<typename... ArgTypes>
				Rectangular(ArgTypes&&... args)
					:iRectangle(std::forward<ArgTypes>(args)...)
				{
					// empty body
				} // end Rectangular constructor

				/****************
				*    Methods    *
				****************/
			public:

#define accessor(name) \
				coordinate_type &name()\
				{\
					return iRectangle.name();\
				} /* end method name */\
				\
				const coordinate_type &name() const\
				{\
					return iRectangle.name();\
				} /* end method name */

				accessor(left)
				accessor(bottom)
				accessor(right)
				accessor(top)
#undef accessor

				coordinate_type &side(Side sideName)
				{
					return iRectangle.side(sideName);
				} // end method side

				const coordinate_type &side(Side sideName) const
				{
					return iRectangle.side(sideName);
				} // end method side

				coordinate_type width() const
				{
					return iRectangle.width();
				} // end method width

				coordinate_type height() const
				{
					return iRectangle.height();
				} // end method height

				bool contains(coordinate_type x, coordinate_type y) const
				{
					return iRectangle.contains(x,y);
				} // end method contains
			}; // end class Rectangular

		} // end namespace Adapting

		namespace Renderable
		{
			namespace Colorblind
			{
				template<typename BaseType>
				class FilledRectangle : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;

					/*********************
					*    Constructors    *
					*********************/
				public:
					FilledRectangle() = default;

					template<typename OtherType>
					explicit FilledRectangle(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end FilledRectangle forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						glPushAttrib(GL_POLYGON_BIT);
							glPolygonMode(GL_FRONT,GL_FILL);
							glRect(std::min(this->left(),this->right()),std::min(this->bottom(),this->top()),std::max(this->left(),this->right()),std::max(this->bottom(),this->top()));
						glPopAttrib();
					} // end method render
				}; // end class FilledRectangle

				template<typename BaseType, typename CTRational = typename BaseType::compile_time_rational_type>
				class BorderedRectangle : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;
					using compile_time_rational_type = CTRational;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BorderedRectangle() = default;

					template<typename OtherType>
					explicit BorderedRectangle(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end BorderedRectangle forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						glPushAttrib(GL_POLYGON_BIT);
							glPolygonMode(GL_FRONT,GL_FILL);
							auto bSize = (CTRational::den * this->borderSize() + CTRational::num) / CTRational::den;
							auto minX = std::min(this->left(),this->right());
							auto maxX = std::max(this->left(),this->right());
							auto minY = std::min(this->bottom(),this->top());
							auto maxY = std::max(this->bottom(),this->top());
							glRect(minX,minY,minX+bSize,maxY-bSize);
							glRect(minX,maxY-bSize,maxX-bSize,maxY);
							glRect(maxX-bSize,minY+bSize,maxX,maxY);
							glRect(minX+bSize,minY,maxX,minY+bSize);
						glPopAttrib();
					} // end method render
				}; // end class BorderedRectangle

				/** BaseType should be Rectangular and expose text traits via the supplied TextConceptMap.
				 *	Margin should be a compile-time rational type representing the margin reserved between the rectangle sides and the text.
				 *	If the text cannot fit in the rectangle, it is silently scaled down for rendering. This does not affect
				 *	the size returned by the object methods.
				 */
				// TODO: Is silent scaling the best option?
				template<typename BaseType, typename TextConceptMap, typename Margin = typename BaseType::margin>
				class BoxedText : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;
					using margin = Margin;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BoxedText() = default;

					template<typename OtherType>
					explicit BoxedText(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end BoxedText forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						typename BaseType::font_engine_type fontEngine;
						auto effectiveTextSize = TextConceptMap().effectiveTextSize(*this);
						auto effectiveTextWidth = effectiveTextSize.first;
						auto effectiveTextHeight = effectiveTextSize.second;

						glPushAttrib(GL_TRANSFORM_BIT);
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
								glTranslated((((this->width() - effectiveTextWidth)*Margin::den - 2*Margin::num) / Margin::den) / 2 + (std::min(this->left(),this->right())*Margin::den + Margin::num) / Margin::den,
											 (((this->height() - effectiveTextHeight)*Margin::den - 2*Margin::num) / Margin::den) / 2 + (std::min(this->bottom(),this->top())*Margin::den + Margin::num) / Margin::den,
											 0); // center text in inner rectangle
								glScaled(effectiveTextWidth / fontEngine.stringWidth(TextConceptMap().text(*this)) , effectiveTextHeight / fontEngine.fontHeight() , 1);
								glTranslated(0,fontEngine.fontBelowBaseLine(),0);
								fontEngine.render(TextConceptMap().text(*this));
							glPopMatrix();
						glPopAttrib();
					} // end method render
				}; // end class BoxedText

				/** BaseType should be Rectangular, Textual, SizedText. Margin and LineSpacing should be compile-time rational types
				 *	representing the margin reserved between the rectangle sides and the paragraph text and the empty space left
				 *	between lines respectively.
				 *	Newline characters in the text are respected, but new ones may be added as text is wrapped to fit
				 *	in the rectangle.
				 *	If the text cannot fit in the rectangle, it is silently scaled down for rendering. This does not affect
				 *	the size returned by the object methods.
				 */
				// TODO: Is silent scaling the best option?
				// TODO: Add support for dynamic line spacing using SFINAE
				template<typename BaseType, typename TextConceptMap, typename Margin = typename BaseType::margin, typename LineSpacing = typename BaseType::line_spacing>
				class BoxedParagraph : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;
					using margin = Margin;
					using line_spacing = LineSpacing;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BoxedParagraph() = default;

					template<typename OtherType>
					explicit BoxedParagraph(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end BoxedParagraph forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						// TODO: add scaling support. nlgn algorithm based on binary search?
						typename BaseType::font_engine_type fontEngine;
						auto innerLeft   = (std::min(this->left(),this->right())*Margin::den + Margin::num)/Margin::den;
						auto innerBottom = (std::min(this->bottom(),this->top())*Margin::den + Margin::num)/Margin::den;
						auto innerRight  = (std::max(this->left(),this->right())*Margin::den - Margin::num)/Margin::den;
						auto innerTop    = (std::max(this->bottom(),this->top())*Margin::den - Margin::num)/Margin::den;
						typename BaseType::text_type currentLine;
						auto lineHeight = TextConceptMap().textHeight(*this); // change later...

						// TODO: add line wrapping.
						glPushAttrib(GL_TRANSFORM_BIT);
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
								glTranslated(innerLeft,innerTop - fontEngine.fontAboveBaseLine()*lineHeight/fontEngine.fontHeight(),0); // set caret to initial position
								for(auto character : TextConceptMap().text(*this))
								{
									if(character == '\n') // TODO: use character traits for comparison
									{
										glPushMatrix();
											glScaled(lineHeight/fontEngine.fontHeight(),lineHeight/fontEngine.fontHeight(),1);
											fontEngine.render(currentLine);
										glPopMatrix();
										currentLine.clear();
										glTranslated(0,-(lineHeight*LineSpacing::den + LineSpacing::num)/LineSpacing::den,0); // move to next line
									}
									else
									{
										currentLine.push_back(character);
									} // end else
								} // end foreach
								glScaled(lineHeight/fontEngine.fontHeight(),lineHeight/fontEngine.fontHeight(),1);
								fontEngine.render(currentLine);
							glPopMatrix();
						glPopAttrib();
					} // end method render
				}; // end class BoxedParagraph

				/** Renders what its BaseType would render, but with the foreground and background
				 *	colors swapped. Transparencies aren't swapped!
				 */
				template<typename BaseType>
				class InversedColor : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;

					/*********************
					*    Constructors    *
					*********************/
				public:
					InversedColor() = default;

					template<typename OtherType>
					explicit InversedColor(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end InversedColor forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						float fgColor[4], bgColor[4];
						glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
							glGetFloatv(GL_CURRENT_COLOR,fgColor);
							glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);
							glClearColor(fgColor[0],fgColor[1],fgColor[2],bgColor[3]);
							glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
							BaseType::render();
						glPopAttrib();
					} // end method render
				}; // end class InversedColor

				/** Renders a caret as a vertical line. Deprecated
				 */
				// TODO: remove and compose from smaller frames when able to present IndirectCaret as rectangle
				template<typename BaseType, typename TextConceptMap, typename Width = typename BaseType::width, typename FontEngineType = typename BaseType::font_engine_type>
				class IndirectCaret : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					using base_type = BaseType;
					using font_engine_type = FontEngineType;
					using width = Width;

					/*********************
					*    Constructors    *
					*********************/
				public:
					IndirectCaret() = default;

					template<typename OtherType>
					explicit IndirectCaret(OtherType &&other)
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end IndirectCaret forwarding constructor (may move/copy/convert)

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						auto left   = std::min(this->pointer()->left(),this->pointer()->right());
						auto bottom = std::min(this->pointer()->bottom(),this->pointer()->top());
						auto right  = std::max(this->pointer()->left(),this->pointer()->right());
						auto top    = std::max(this->pointer()->bottom(),this->pointer()->top());

						auto textLeft = left + (this->pointer()->width() - TextConceptMap().effectiveTextSize(*this->pointer()).first)/2;
						auto caretMiddle = textLeft + (TextConceptMap().text(*this->pointer()).empty() ? 0 :
							this->xOffset()*TextConceptMap().effectiveTextSize(*this->pointer()).first / FontEngineType().stringWidth(TextConceptMap().text(*this->pointer())));

						float fgColor[4], bgColor[4];
						glPushAttrib(GL_CURRENT_BIT);
							glGetFloatv(GL_CURRENT_COLOR,fgColor);
							glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);

							glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
							glRect((caretMiddle*2*width::den - width::num)/(2*width::den),bottom+this->pointer()->borderSize(),
									(caretMiddle*2*width::den + width::num)/(2*width::den),top-this->pointer()->borderSize());
							glColor4fv(fgColor);
							glRect((caretMiddle*6*width::den - width::num)/(6*width::den),((bottom+this->pointer()->borderSize())*3*width::den + width::num)/(3*width::den),
									(caretMiddle*6*width::den + width::num)/(6*width::den),((top-this->pointer()->borderSize())*3*width::den - width::num)/(3*width::den));
						glPopAttrib();
					} // end method render
				}; // end class IndirectCaret

			} // end namespace Colorblind

			namespace Colored
			{

			} // end namespace Colored

			template<typename BaseType>
			class Stippled : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Stippled() = default;

				template<typename OtherType>
				explicit Stippled(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Stippled forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					const GLubyte pattern[128] = {
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
						0x88,0x88,0x88,0x88,
						0x55,0x55,0x55,0x55,
						0x22,0x22,0x22,0x22,
						0x55,0x55,0x55,0x55,
					}; // end pattern initializer
					glPushAttrib(GL_POLYGON_BIT|GL_POLYGON_STIPPLE_BIT);
						glEnable(GL_POLYGON_STIPPLE);
						glPolygonStipple(pattern);
						BaseType::render();
					glPopAttrib();
				} // end method render
			}; // end class Stippled

			template<typename BaseType, typename TrueWrapper, typename FalseWrapper, typename UnaryPredicate>
			class Conditional : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using unary_predicate_type = UnaryPredicate;
				using true_wrapper_type = TrueWrapper;
				using false_wrapper_type = FalseWrapper;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Conditional() = default;

				template<typename OtherType>
				explicit Conditional(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Conditional forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					if(UnaryPredicate()(*this))
						TrueWrapper(*this).render();
					else
						FalseWrapper(*this).render();
				} // end method render
			}; // end class Conditional

			template<typename BaseType, typename FirstWrapper, typename SecondWrapper> // TODO: use variadic templates when available
			class Sequential : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using first_wrapper_type = FirstWrapper;
				using second_wrapper_type = SecondWrapper;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Sequential() = default;

				template<typename OtherType>
				explicit Sequential(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Sequential forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					FirstWrapper(*this).render();
					SecondWrapper(*this).render();
				} // end method render
			}; // end class Sequential

			template<typename BaseType, typename Angle>
			class Rotated : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using angle = Angle;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Rotated() = default;

				template<typename OtherType>
				explicit Rotated(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Rotated forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					glPushAttrib(GL_TRANSFORM_BIT);
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
							glRotated(static_cast<GLdouble>(angle::num)/angle::den,0,0,1);
							base_type::render();
						glPopMatrix();
					glPopAttrib();
				} // end method render
			}; // end class Rotated

			template<typename BaseType, typename XCoeff, typename YCoeff>
			class Scaled : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using x_coeff = XCoeff;
				using y_coeff = YCoeff;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Scaled() = default;

				template<typename OtherType>
				explicit Scaled(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Scaled forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					glPushAttrib(GL_TRANSFORM_BIT);
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
							glScaled(static_cast<GLdouble>(x_coeff::num)/x_coeff::den,static_cast<GLdouble>(y_coeff::num)/y_coeff::den,1);
							base_type::render();
						glPopMatrix();
					glPopAttrib();
				} // end method render
			}; // end class Scaled

			template<typename BaseType, typename XOffset, typename YOffset>
			class Translated : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using x_offset = XOffset;
				using y_offset = YOffset;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Translated() = default;

				template<typename OtherType>
				explicit Translated(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Translated forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					glPushAttrib(GL_TRANSFORM_BIT);
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
							glTranslated(static_cast<GLdouble>(x_offset::num)/x_offset::den,static_cast<GLdouble>(y_offset::num)/y_offset::den,0);
							base_type::render();
						glPopMatrix();
					glPopAttrib();
				} // end method render
			}; // end class Translated

			/** A rendering frame that does not actually render anything.
			 *	Useful when you only want to render something in one clause of
			 *	the conditional rendering frame.
			 */
			template<typename BaseType>
			class Null : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Null() = default;

				template<typename OtherType>
				explicit Null(OtherType &&other)
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Null forwarding constructor (may move/copy/convert)

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					// do nothing
				} // end method render
			}; // end class Null

		} // end namespace Renderable

		namespace EventHandling
		{
			/** Frame providing stub (empty) implementations for the methods of the KeyboardAndMouse interface.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class KeyboardAndMouseStub : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				KeyboardAndMouseStub() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y){}
				void keyboardNonAscii(Bases::EventHandling::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y){}
				void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y){}
				void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y){}
				void mouseEnter(CoordinateType x, CoordinateType y){}
				void mouseMove(CoordinateType x, CoordinateType y){}
				void mouseExit(CoordinateType x, CoordinateType y){}
			}; // end class KeyboardAndMouseStub

			// TODO: should use atomic operations for thread safety...
			template<typename FrameType>
			struct Focus
			{
				static Focus *target;
			}; // end class Focus

			template<typename FrameType>
			Focus<FrameType> *Focus<FrameType>::target = nullptr;

			/** This frame is intended for the implementation of buttons. It requires the BaseType to be
			 *	pressable and highlightable and expects to receive only mouse events than happen over it,
			 *	except when it has the focus, in which case it expects all.
			 *	It ignores keyboard and wheel events for now.
			 */
			// TODO: formally define behaviour using a state machine.
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class TwoStagePressable : public BaseType, public Focus<TwoStagePressable<Bases::Empty,void>>
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;
				using focus_type = Focus<TwoStagePressable<Bases::Empty,void>>;

				/*********************
				*    Constructors    *
				*********************/
			public:
				TwoStagePressable() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					// ignore keyboard events
				} // end method keyboardAscii

				void keyboardNonAscii(Bases::EventHandling::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
				{
					// ignore keyboard events... for now.
				} // end method keyboardNonAscii

				void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
				{
					if(button == 0)
					{
						this->pressed() = down;
						if(down)
							target = this;
						else
						{
							target = nullptr;
							this->highlighted() = contains(x,y);
						} // end else
					} // end if
				} // end method mouseButton

				void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y)
				{
					// ignore mouse wheel events
				} // end method mouseWheel

				void mouseEnter(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						this->highlight();
					else if(target == this)
						this->press();
				} // end method mouseEnter

				void mouseMove(CoordinateType x, CoordinateType y)
				{
					// nothing to do
				} // end method mouseMove

				void mouseExit(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						this->dehighlight();
					else if(target == this)
						this->depress();
				} // end method mouseExit
			}; // end class TwoStagePressable

			/** BaseType must conform to the CaretLike interface.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class CaretLike : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				CaretLike() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					if(down) // ignore up events
					{
						switch(code)
						{
						case 0x7f: // delete key
							this->eraseNext();
							return;
						case '\b': // backspace key
							this->erasePrev();
							return;
						default:
							this->insert(code);
							return;
						} // end switch
					} // end if
				} // end method keyboardAscii

				void keyboardNonAscii(Bases::EventHandling::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
				{
					using Bases::EventHandling::NonAsciiKey;

					if(down) // ignore up events
					{
						switch(key)
						{
						case NonAsciiKey::LEFT:
							this->prevPosition();
							return;
						case NonAsciiKey::RIGHT:
							this->nextPosition();
							return;
						case NonAsciiKey::HOME:
							this->firstPosition();
							return;
						case NonAsciiKey::END:
							this->lastPosition();
							return;
						default:
							// do nothing
							return;
						} // end switch
					} // end if
				} // end method keyboardNonAscii
			}; // end class CaretLike

		} // end namespace EventHandling

	} // end namespace Frames

	namespace FunctionObjects
	{
		struct Pressed
		{
			template<typename PressableType>
			bool operator()(const PressableType &pressable) const
			{
				return pressable.pressed();
			} // end method operator()
		}; // end struct Pressed

		struct Selected
		{
			template<typename SelectableType>
			bool operator()(const SelectableType &selectable) const
			{
				return selectable.selected();
			} // end method operator()
		}; // end struct Selected

		struct Highlighted
		{
			template<typename HighlightableType>
			bool operator()(const HighlightableType &highlightable) const
			{
				return highlightable.highlighted();
			} // end method operator()
		}; // end struct Highlighted

		struct Focused
		{
			template<typename FocusableType>
			bool operator()(const FocusableType &focusable) const
			{
				return focusable.focused();
			} // end method operator()
		}; // end struct Focused

		// TODO: use concept maps instead when available
		struct Textual
		{
			template<typename TextualType>
			decltype(auto) text(TextualType &&textual) const
			{
				return textual.text();
			} // end method text

			template<typename TextualType>
			decltype(auto) textHeight(TextualType &&textual) const
			{
				return textual.textHeight();
			} // end method textHeight

			template<typename TextualType>
			decltype(auto) textWidth(TextualType &&textual) const
			{
				return textual.textWidth();
			} // end method textWidth

			template<typename TextualType>
			decltype(auto) textCharWidth(TextualType &&textual, size_t index) const
			{
				return textual.textCharWidth(index);
			} // end method textCharWidth

			template<typename TextualType>
			void setTextWidth(TextualType &&textual, const typename TextualType::coordinate_type &value) const
			{
				textual.setTextWidth(value);
			} // end method setTextWidth

			// TODO: use SFINAE to fallback to (textWidth,textHeight) if effectiveTextSize is not available.
			template<typename TextualType>
			decltype(auto) effectiveTextSize(TextualType &&textual) const
			{
				return textual.effectiveTextSize();
			} // end method effectiveTextSize

			// TODO: use SFINAE to fallback to (textCharWidth,textHeight) if effectiveTextCharSize is not available.
			template<typename TextualType>
			decltype(auto) effectiveTextCharSize(TextualType &&textual, size_t index) const
			{
				return textual.effectiveTextCharSize(index);
			} // end method effectiveTextCharSize
		}; // end struct Textual

		// TODO: use concept maps instead when available
		struct Named
		{
			template<typename NamedType>
			decltype(auto) text(NamedType &&named) const
			{
				return named.name();
			} // end method text

			template<typename NamedType>
			decltype(auto) textHeight(NamedType &&named) const
			{
				return named.nameHeight();
			} // end method textHeight

			template<typename NamedType>
			decltype(auto) textWidth(NamedType &&named) const
			{
				return named.nameWidth();
			} // end method textWidth

			template<typename NamedType>
			decltype(auto) textCharWidth(NamedType &&named, size_t index) const
			{
				return named.nameCharWidth(index);
			} // end method textCharWidth

			template<typename NamedType>
			void setTextWidth(NamedType &&named, const typename NamedType::coordinate_type &value) const
			{
				named.setNameWidth(value);
			} // end method setTextWidth

			// TODO: use SFINAE to fallback to (textWidth,textHeight) if effectiveNameSize is not available.
			template<typename NamedType>
			decltype(auto) effectiveTextSize(NamedType &&named) const
			{
				return named.effectiveNameSize();
			} // end method effectiveTextSize

			// TODO: use SFINAE to fallback to (nameWidth,nameHeight) if effectiveNameCharSize is not available.
			template<typename NamedType>
			decltype(auto) effectiveTextCharSize(NamedType &&named, size_t index) const
			{
				return named.effectiveNameCharSize(index);
			} // end method effectiveTextSize
		}; // end struct Named

		/** Font engines encapsulate low level font metric and rendering functions, to present
		 *	them in a uniform manner. They should be constructible from a font and default
		 *	constructible (that should give font a default value).
		 *	The actual method signatures depend on what the underlying toolkit supports.
		 */
		// TODO: rethink the font engine and font concepts.
		struct GlutStrokeFontEngine
		{
			/***************
			*    Fields    *
			***************/
		private:
			void *iFont;

			/*********************
			*    Constructors    *
			*********************/
		public:
			explicit GlutStrokeFontEngine(void *font = GLUT_STROKE_ROMAN)
				:iFont(font)
			{
				// empty body
			} // end GlutStrokeFontEngine constructor

			/****************
			*    Methods    *
			****************/
		public:
			void *&font()
			{
				return iFont;
			} // end method font

			void *const &font() const
			{
				return iFont;
			} // end method font

			GLfloat fontHeight() const
			{
				return glutStrokeHeight(iFont);
			} // end method fontHeight

			GLfloat fontAboveBaseLine() const
			{
				return 119.05f; // magic value from GLUT documentation
			} // end method fontAboveBaseLine

			GLfloat fontBelowBaseLine() const
			{
				return 33.33f; // magic value from GLUT documentation
			} // end method fontBelowBaseLine

			int charWidth(int character) const
			{
				return glutStrokeWidth(iFont,character);
			} // end method charWidth

			template<typename CharSequenceType>
			int stringWidth(const CharSequenceType &string) const // for some reason glutStrokeLength takes unsigned char * instead of the char * documented!
			{
				return glutStrokeLength(iFont,std::basic_string<unsigned char>(std::begin(string),std::end(string)).c_str());
			} // end method stringWidth

			/** The string will be rendered in some sort of 'current position' having some sort of 'current size'.
			 */
			template<typename CharSequenceType>
			void render(const CharSequenceType &string) const // for some reason glutStrokeString takes unsigned char * instead of the char * documented!
			{
				glutStrokeString(iFont,std::basic_string<unsigned char>(std::begin(string),std::end(string)).c_str());
			} // end method render
		}; // end struct GlutStrokeFontEngine

	} // end namespace FuntionObjects

	/** This is a DSEL for composing frames to create frame stacks.
	 *	The syntax is far from ideal due to limitations of the C++ language and current implementations,
	 *	but should be a noticeable improvement compared to composing them by hand.
	 */
	namespace DSEL
	{
		// TODO: Create a real DSEL using overloaded operators and/or macros. Include conditionals and sequences.

		/**
		 * Wraps a single frame with its parameters.
		 */
		template<template<typename...> class Template, typename... Arguments>
		struct Frame {};

		/**
		 * Takes care of nesting frames and filling-in default arguments.
		 */
		template<typename... Frames>
		struct ProcessFrames;

		// recursion base
		template<typename BaseType>
		struct ProcessFrames<BaseType>
		{using type = BaseType;};

		// recursion step
		template<typename BaseType, template<typename...> class Template, typename... Arguments, typename... Frames>
		struct ProcessFrames<BaseType, DSEL::Frame<Template, Arguments...>, Frames...>
		{using type = typename ProcessFrames<Template<BaseType, Arguments...>, Frames...>::type;};

		/**
		 * Represents a complete frame stack.
		 */
		template<typename... Frames>
		struct FrameStack : public ProcessFrames<Frames...>::type
		{
			// Member types
			using base_type = typename ProcessFrames<Frames...>::type;

			// Constructors
			FrameStack() = default;
			using base_type::base_type;
		}; // end struct FrameStack

		/**
		 * Represents a complete frame stack using the curiously-recurring template pattern.
		 */
		template<template<typename...> class Template, typename... Arguments>
		struct CRFrameStack : public Template<Arguments..., CRFrameStack<Template, Arguments...>>
		{
			// Member types
			using base_type = Template<Arguments..., CRFrameStack<Template, Arguments...>>;

			// Constructors
			CRFrameStack() = default;
			using base_type::base_type;
		}; // end struct CRFrameStack
	} // end namespace DSEL

	/** The intention is to let the client easily combine frames to create controls as needed.
	 *	But my compiler does not support inheriting constructors, so the created controls would only
	 *	be default constructible. To workaround that in the common case, I specialize common control
	 *	types and add constructors to them.
	 */
	namespace Controls
	{
		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string> class Button;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using ButtonBase = DSEL::FrameStack<
			RectangleType,
			DSEL::Frame<Frames::UniformlyBordered, typename RectangleType::coordinate_type>,
			DSEL::Frame<Frames::Pressable, Button<RectangleType, BorderSize, Margin, TextType>>,
			DSEL::Frame<Frames::Highlightable, Button<RectangleType, BorderSize, Margin, TextType>>,
			DSEL::Frame<Frames::Textual, TextType>,
			DSEL::Frame<Frames::SizedText, FunctionObjects::GlutStrokeFontEngine>,
			DSEL::Frame<Frames::BoxedAdaptableSizeText,Margin>,
			DSEL::Frame<Frames::EventHandling::TwoStagePressable>
		>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using ButtonRenderableBase =
			Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
				Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Colorblind::FilledRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>>,
					Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>>,
				Frames::Renderable::Conditional<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>,BorderSize>,
						Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>,
					Frames::Renderable::Sequential<ButtonBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,BorderSize,Margin,TextType>,std::ratio<0>>,
						Frames::Renderable::Colorblind::BoxedText<ButtonBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>,
					FunctionObjects::Highlighted>,
				FunctionObjects::Pressed>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		class Button : public ButtonRenderableBase<RectangleType, BorderSize, Margin, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ButtonRenderableBase<RectangleType, BorderSize, Margin, TextType>;
			using coordinate_type = typename Button::coordinate_type;
			using rectangle_type = RectangleType;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Button() = default;

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
		using IShapePart = DSEL::FrameStack<
			Bases::Empty,
			DSEL::Frame<Bases::Destructible>,
			DSEL::Frame<Bases::Movable, CoordinateType>,
			DSEL::Frame<Bases::Containing>,
			DSEL::Frame<Bases::Renderable>
		>;

		template<typename CoordinateType, typename horizontallyMovable, typename verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		using ControlPartBase = DSEL::FrameStack<
			IShapePart<CoordinateType>,
			DSEL::Frame<Bases::Rectangular>,
			DSEL::Frame<Frames::Movable::Rectangular>,
			DSEL::Frame<Frames::Movable::HVMovable, horizontallyMovable, verticallyMovable>,
			DSEL::Frame<Frames::Renderable::Colorblind::FilledRectangle>,
			DSEL::Frame<Frames::Renderable::Stippled>,
			DSEL::Frame<Frames::Renderable::Colorblind::InversedColor>,
			DSEL::Frame<Frames::Adapting::Rectangular, geometry::RefRectangle<CoordinateType, constant, leftRef, bottomRef, rightRef, topRef>>
		>;

		/** Const instances should be constant and non-const instances should be non constant
		 */
		template<typename CoordinateType, typename horizontallyMovable, typename verticallyMovable, bool constant, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		class ControlPart : public ControlPartBase<CoordinateType, horizontallyMovable, verticallyMovable, constant, leftRef, bottomRef, rightRef, topRef>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ControlPartBase<CoordinateType, horizontallyMovable, verticallyMovable, constant, leftRef, bottomRef, rightRef, topRef>;
			using rectangle_type = typename ControlPart::rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			ControlPart() = default;

			/** Forwards arguments to the underlying RectangularType constructor
			 */
			template<typename LeftType, typename BottomType, typename RightType, typename TopType> // TODO: use variadic templates or inheriting constructors when available
			ControlPart(LeftType &&left, BottomType &&bottom, RightType &&right, TopType &&top)
				:base_type(std::forward<LeftType>(left),std::forward<BottomType>(bottom),std::forward<RightType>(right),std::forward<TopType>(top))
			{
				// empty body
			} // end ControlPart constructor

		}; // end class ControlPart

		template<typename CoordinateType, typename TextType = std::string>
		class IControl : public DSEL::FrameStack<
			Bases::Empty,
			DSEL::Frame<Bases::Destructible>,
			DSEL::Frame<Bases::Rectangular, CoordinateType>,
			DSEL::Frame<Bases::Containing>,
			DSEL::Frame<Bases::Movable>,
			DSEL::Frame<Bases::UniformlyBordered>,
			DSEL::Frame<Bases::Textual, TextType>,
			DSEL::Frame<Bases::SizedText>,
			DSEL::Frame<Bases::Selectable, IControl<CoordinateType,TextType>>,
			DSEL::Frame<Bases::Highlightable, IControl<CoordinateType,TextType>>,
			DSEL::Frame<Bases::MultiPart, std::unique_ptr<      IShapePart<CoordinateType>>,
										  std::unique_ptr<const IShapePart<CoordinateType>>>,
			DSEL::Frame<Bases::Renderable>
		>{}; // poor man's template alias

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType>
		using ControlBase =
			Frames::MultiPartBorderedRectangle<DSEL::FrameStack<
				IControl<typename RectangleType::coordinate_type, TextType>,
				DSEL::Frame<Frames::Adapting::Rectangular, RectangleType>,
				DSEL::Frame<Frames::UniformlyBordered>,
				DSEL::Frame<Frames::Selectable, IControl<typename RectangleType::coordinate_type, TextType>/*Control<RectangleType,BorderSize,Margin,TextType>*/>,
				DSEL::Frame<Frames::Highlightable, IControl<typename RectangleType::coordinate_type, TextType>>,
				DSEL::Frame<Frames::Movable::Rectangular>,
				DSEL::Frame<Frames::Textual, TextType>,
				DSEL::Frame<Frames::SizedText, FunctionObjects::GlutStrokeFontEngine>,
				DSEL::Frame<Frames::BoxedAdaptableSizeText, Margin>
			>,ControlPart>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string>
		using ControlRenderableBase =
			Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,TextType>,
				Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Colorblind::FilledRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>>,
					Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>>,
				Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>,BorderSize>,
						Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>,
					Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>,std::ratio<0>>,
						Frames::Renderable::Colorblind::BoxedText<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin>>,
					FunctionObjects::Highlighted>,
				FunctionObjects::Selected>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename TextType = std::string>
		class Control : public ControlRenderableBase<RectangleType, BorderSize, Margin, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ControlRenderableBase<RectangleType, BorderSize, Margin, TextType>;
			using coordinate_type = typename Control::coordinate_type;
			using rectangle_type = RectangleType;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Control() = default;

			Control(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize,
					TextType text, coordinate_type textHeight, bool selected = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->text() = text;
				this->textHeight() = textHeight;
				this->selected() = selected;
				this->highlighted() = highlighted;
			} // end Control constructor
		}; // end class Control

		template<typename RectangleType, typename BorderSize, typename Margin, typename LineSpacing, typename TextType = std::string>
		using ParagraphBase =
			Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,TextType>,
				Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Colorblind::FilledRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>>,
					Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedParagraph<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin,LineSpacing>>>,
				Frames::Renderable::Conditional<ControlBase<RectangleType,BorderSize,Margin,TextType>,
					Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>,BorderSize>,
						Frames::Renderable::Colorblind::BoxedParagraph<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin,LineSpacing>>,
					Frames::Renderable::Sequential<ControlBase<RectangleType,BorderSize,Margin,TextType>,
						Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,BorderSize,Margin,TextType>,std::ratio<0>>,
						Frames::Renderable::Colorblind::BoxedParagraph<ControlBase<RectangleType,BorderSize,Margin,TextType>,FunctionObjects::Textual,Margin,LineSpacing>>,
					FunctionObjects::Highlighted>,
				FunctionObjects::Selected>;

		template<typename RectangleType, typename BorderSize, typename Margin, typename LineSpacing, typename TextType = std::string>
		class Paragraph : public ParagraphBase<RectangleType, BorderSize, Margin, LineSpacing, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = ParagraphBase<RectangleType, BorderSize, Margin, LineSpacing, TextType>;
			using coordinate_type = typename Paragraph::coordinate_type;
			using rectangle_type = RectangleType;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Paragraph() = default;

			Paragraph(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize,
					TextType text, coordinate_type textHeight, bool selected = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->text() = text;
				this->textHeight() = textHeight;
				this->selected() = selected;
				this->highlighted() = highlighted;
			} // end Paragraph constructor
		}; // end class Paragraph

		template<typename RectangleType, typename Margin, typename TextType>
		using LabelBase = DSEL::FrameStack<
			RectangleType,
			DSEL::Frame<Frames::Movable::Rectangular>,
			DSEL::Frame<Frames::Textual, TextType>,
			DSEL::Frame<Frames::SizedText, FunctionObjects::GlutStrokeFontEngine>,
			DSEL::Frame<Frames::BoxedAdaptableSizeText, Margin>
		>;

		template<typename RectangleType, typename Margin, typename TextType = std::string>
		using LabelRenderableBase =
			Frames::Renderable::Sequential<
				LabelBase<RectangleType,Margin,TextType>,
				Frames::Renderable::Colorblind::FilledRectangle<LabelBase<RectangleType,Margin,TextType>>,
				Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedText<LabelBase<RectangleType,Margin,TextType>,FunctionObjects::Textual,Margin>>>;

		template<typename RectangleType, typename Margin, typename TextType = std::string>
		class Label : public LabelRenderableBase<RectangleType, Margin, TextType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using base_type = LabelRenderableBase<RectangleType, Margin, TextType>;
			using coordinate_type = typename Label::coordinate_type;
			using rectangle_type = RectangleType;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Label() = default;

			Label(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, TextType text, coordinate_type textHeight)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->text() = text;
				this->textHeight() = textHeight;
			} // end Label constructor
		}; // end class Label

	} // end namespace Controls

	namespace EventAdaptors
	{
		// TODO: rethink about who should be responsible to define the coordinate system.
		template<typename RootControlType>
		class GLUT
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			using root_control_type = RootControlType;
			using coordinate_type = typename root_control_type::coordinate_type;

			/***************
			*    Fields    *
			***************/
		private:
			static RootControlType rootControl;
			static coordinate_type pixelWidth, pixelHeight; // in millimetres
			static bool wasInside;

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: add support for initializing rootControl
			/** GLUT must be initialized before calling this method
			 */
			static void initialize()
			{
				pixelWidth = (coordinate_type)glutGet(GLUT_SCREEN_WIDTH_MM) / glutGet(GLUT_SCREEN_WIDTH);
				pixelHeight = (coordinate_type)glutGet(GLUT_SCREEN_HEIGHT_MM) / glutGet(GLUT_SCREEN_HEIGHT);
				wasInside = false;
			} // end method initialize

			// TODO: add static methods implementing the rest of GLUT event handling functions
			// TODO: use SFINAE to revert to default behaviour if root control does not implement
			// all methods
			static void idle()
			{
				glutPostRedisplay();
			} // end function idle

			static void display() // TODO: what if I have to clear more buffers?
			{
				glClear(GL_COLOR_BUFFER_BIT);

				rootControl.render();

				glutSwapBuffers();
			} // end function display

			static void keyboard(unsigned char key, int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				rootControl.keyboardAscii(key,true,sceneX,sceneY);
			} // end function keyboard

			static void keyboardUp(unsigned char key, int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				rootControl.keyboardAscii(key,false,sceneX,sceneY);
			} // end function keyboard

			static Bases::EventHandling::NonAsciiKey grapheneKey(int glutKey)
			{
				using Bases::EventHandling::NonAsciiKey;

				// TODO: add more keys if they appear in GLUT's documentation
				switch(glutKey)
				{
				case GLUT_KEY_F1: return NonAsciiKey::F1;
				case GLUT_KEY_F2: return NonAsciiKey::F2;
				case GLUT_KEY_F3: return NonAsciiKey::F3;
				case GLUT_KEY_F4: return NonAsciiKey::F4;
				case GLUT_KEY_F5: return NonAsciiKey::F5;
				case GLUT_KEY_F6: return NonAsciiKey::F6;
				case GLUT_KEY_F7: return NonAsciiKey::F7;
				case GLUT_KEY_F8: return NonAsciiKey::F8;
				case GLUT_KEY_F9: return NonAsciiKey::F9;
				case GLUT_KEY_F10: return NonAsciiKey::F10;
				case GLUT_KEY_F11: return NonAsciiKey::F11;
				case GLUT_KEY_F12: return NonAsciiKey::F12;
				case GLUT_KEY_PAGE_DOWN: return NonAsciiKey::PAGE_DOWN;
				case GLUT_KEY_PAGE_UP: return NonAsciiKey::PAGE_UP;
				case GLUT_KEY_HOME: return NonAsciiKey::HOME;
				case GLUT_KEY_END: return NonAsciiKey::END;
				case GLUT_KEY_LEFT: return NonAsciiKey::LEFT;
				case GLUT_KEY_RIGHT: return NonAsciiKey::RIGHT;
				case GLUT_KEY_UP: return NonAsciiKey::UP;
				case GLUT_KEY_DOWN: return NonAsciiKey::DOWN;
				case GLUT_KEY_INSERT: return NonAsciiKey::INSERT;
					// FreeGLUT-specific
				case GLUT_KEY_NUM_LOCK: return NonAsciiKey::NUM_LOCK;
				case GLUT_KEY_SHIFT_L: return NonAsciiKey::L_SHIFT;
				case GLUT_KEY_SHIFT_R: return NonAsciiKey::R_SHIFT;
				case GLUT_KEY_CTRL_L: return NonAsciiKey::L_CTRL;
				case GLUT_KEY_CTRL_R: return NonAsciiKey::R_CTRL;
				case GLUT_KEY_ALT_L: return NonAsciiKey::L_ALT;
				case GLUT_KEY_ALT_R: return NonAsciiKey::R_ALT;
				default:
					throw std::runtime_error("Unknown GLUT key code!");
				} // end switch
			} // end function grapheneKey

			static void special(int glutKey, int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				rootControl.keyboardNonAscii(grapheneKey(glutKey),true,sceneX,sceneY);
			} // end function special

			static void specialUp(int glutKey, int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				rootControl.keyboardNonAscii(grapheneKey(glutKey),false,sceneX,sceneY);
			} // end function specialUp

			static void mouse(int button, int state, int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				rootControl.mouseButton(button == GLUT_LEFT_BUTTON ? 0 : (button == GLUT_RIGHT_BUTTON ? 1 : 2),state == GLUT_DOWN,sceneX,sceneY);
			} // end function motion

			static void motion(int glutX, int glutY)
			{
				passiveMotion(glutX,glutY);
			} // end function motion

			static void passiveMotion(int glutX, int glutY)
			{
				coordinate_type sceneX = glutX*pixelWidth;
				coordinate_type sceneY = (glutGet(GLUT_WINDOW_HEIGHT)-1 - glutY)*pixelHeight;

				if(rootControl.contains(sceneX,sceneY))
					if(wasInside)
						rootControl.mouseMove(sceneX,sceneY);
					else
					{
						rootControl.mouseEnter(sceneX,sceneY);
						wasInside = true;
					} // end else
				else
					if(wasInside)
					{
						rootControl.mouseExit(sceneX,sceneY);
						wasInside = false;
					}
					else
					{
						// do nothing
					} // end else
			} // end function motion

			static void reshape(int windowWidth, int windowHeight)
			{
				// TODO: perhaps replace resize with use of properties for left,right,bottom,top?
				rootControl.resize(0,0,pixelWidth*windowWidth,pixelHeight*windowHeight);

				glViewport(0, 0, windowWidth, windowHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluOrtho2D(rootControl.left(),rootControl.right(),rootControl.bottom(),rootControl.top());
			} // end function reshape
		}; // end class GLUT

		// Out-of-class static field definitions
		template<typename RootControlType>
		RootControlType GLUT<RootControlType>::rootControl;
		template<typename RootControlType>
		typename GLUT<RootControlType>::coordinate_type GLUT<RootControlType>::pixelWidth;  // in millimetres
		template<typename RootControlType>
		typename GLUT<RootControlType>::coordinate_type GLUT<RootControlType>::pixelHeight; // in millimetres
		template<typename RootControlType>
		bool GLUT<RootControlType>::wasInside;

	} // end namespace EventAdaptors

} // end namespace graphene

#endif // GRAPHENE_H
