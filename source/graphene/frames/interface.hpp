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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <utility>
#include <type_traits>

#include "geometry.hpp"

namespace graphene
{
	namespace Frames
	{
		namespace Interface
		{
			struct Empty {};

			template<typename BaseType>
			struct Destructible : BaseType
			{
				// Member Types
				using base_type = BaseType;

				// Constructors
				Destructible() = default;
				using BaseType::BaseType;

				// Methods
				virtual ~Destructible() = default;
			}; // end struct Destructible

			template<typename BaseType>
			struct Renderable : BaseType
			{
				// Member Types
				using base_type = BaseType;

				// Constructors
				Renderable() = default;
				using BaseType::BaseType;

				// Methods
				virtual void render() const = 0;
			}; // end struct Renderable

			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			struct Movable : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
				Movable() = default;
				using BaseType::BaseType;

				// Methods
				virtual void move(CoordinateType xOffset, CoordinateType yOffset) = 0;
			}; // end struct Movable

			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			struct Rectangular : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
				Rectangular() = default;
				using BaseType::BaseType;

				// Methods
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
			}; // end struct Rectangular

			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			struct Containing : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
				Containing() = default;
				using BaseType::BaseType;

				// Methods
				virtual bool contains(CoordinateType x, CoordinateType y) const = 0;
			}; // end struct Containing

			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			struct Offset : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
				Offset() = default;
				using BaseType::BaseType;

				// Methods
				virtual CoordinateType &xOffset() = 0;
				virtual const CoordinateType &xOffset() const = 0;
				virtual CoordinateType &yOffset() = 0;
				virtual const CoordinateType &yOffset() const = 0;
			}; // end struct Offset

#define ableMacro(ClassName, setVerb, unsetVerb, testVerb) \
			template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>\
			struct ClassName : BaseType\
			{\
				/* Member Types */\
				using base_type = BaseType;\
				using frame_stack_type = FrameStackType;\
\
				/* Constructors */\
				ClassName() = default;\
				using BaseType::BaseType;\
\
				/* Destructor */\
				virtual ~ClassName()\
				{\
					/* workaround because is_base_of can't work with incomplete types */\
					static_assert(std::is_base_of<ClassName, FrameStackType>()\
							   || std::is_base_of<FrameStackType, ClassName>(),\
								  "This struct and FrameStackType must be connected by inheritance!");\
				} /* end destructor */\
\
				/* Methods */\
				virtual FrameStackType &setVerb() = 0;\
				virtual FrameStackType &unsetVerb() = 0;\
				virtual bool &testVerb() = 0;\
				virtual const bool &testVerb() const = 0;\
			}; /* end struct ClassName */

			ableMacro(Selectable, select, deselect, selected)
			ableMacro(Pressable, press, depress, pressed)
			ableMacro(Highlightable, highlight, dehighlight, highlighted)
			ableMacro(Focusable, focus, unfocus, focused)
#undef ableMacro

#define singleFieldByReference(ClassName, methodName, type_alias_prefix, TypeNamePrefix) \
			template<typename BaseType, typename TypeNamePrefix##Type = typename BaseType::type_alias_prefix##_type>\
			struct ClassName : BaseType\
			{\
				/* Member Types */\
				using base_type = BaseType;\
				using type_alias_prefix##_type = TypeNamePrefix##Type;\
\
				/* Constructors */\
				ClassName() = default;\
				using BaseType::BaseType;\
\
				/* Methods */\
				virtual TypeNamePrefix##Type &methodName() = 0;\
				virtual const TypeNamePrefix##Type &methodName() const = 0;\
			}; /* end struct ClassName */

			singleFieldByReference(Textual, text, text, Text)
			singleFieldByReference(Named, name, name, Name)
			singleFieldByReference(Pointing, pointer, pointer, Pointer)
			singleFieldByReference(Indexing, index, index, Index)
			singleFieldByReference(UniformlyBordered, borderSize, border_size, BorderSize)
#undef singleFieldByReference

#define sizedMacro(ClassSuffix, methodPrefix) \
			/** SizedText instances have a fixed 'natural' aspect ratio, so changing the height,\
			 *  actually changes the width as well. Some frame stacks, may interpret  text sizes, as\
			 *  preferred instead of mandatory.\
			 */\
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>\
			struct Sized##ClassSuffix : BaseType\
			{\
				/* Member Types */\
				using base_type = BaseType;\
				using coordinate_type = CoordinateType;\
\
				/* Constructors */\
				Sized##ClassSuffix() = default;\
				using BaseType::BaseType;\
\
				/* Methods */\
				virtual CoordinateType &methodPrefix##Height() = 0;\
				virtual const CoordinateType &methodPrefix##Height() const = 0;\
				/* TODO: add support for setting the methodPrefix width using properties, and perhaps the ability\
				 * to set/get the aspect ratio.\
				 */\
				virtual CoordinateType methodPrefix##Width() const = 0;\
				virtual CoordinateType methodPrefix##CharWidth(size_t index) const = 0;\
				virtual void set##ClassSuffix##Width(const CoordinateType &value) = 0; /* deprecated */\
				/* TODO: allow arbitrary writable height & width combinations, add a preferred aspect ratio\
				 * and add preferred width, height read-only properties.\
				 */\
			}; /* end struct Sized##ClassSuffix */

			sizedMacro(Text, text)
			sizedMacro(Name, name)
#undef sizedMacro

#define adaptableSizeMacro(ClassAffix) \
			/** This frame is intended for use in frame stacks that use textHeight/textWidth as a\
			 *  "preferred" size and need another method to return the actual text height/width.\
			 *  The first pair member is the width and the second the height.\
			 */\
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>\
			struct AdaptableSize##ClassAffix : BaseType\
			{\
				/* Member Types */\
				using base_type = BaseType;\
				using coordinate_type = CoordinateType;\
\
				/* Constructors */\
				AdaptableSize##ClassAffix() = default;\
				using BaseType::BaseType;\
\
				/* Methods */\
				virtual std::pair<CoordinateType,CoordinateType> effective##ClassAffix##Size() const = 0;\
				virtual std::pair<CoordinateType,CoordinateType> effective##ClassAffix##CharSize(size_t index) const = 0;\
			}; /* end struct AdaptableSize##ClassAffix */

			adaptableSizeMacro(Text)
			adaptableSizeMacro(Name)
#undef adaptableSizeMacro

			/** If a MultiPart is also Containing then partUnderPoint(x,y) should return a value designating "no part"
			 *   if and only if contains(x,y) returns false.
			 */
			template<typename BaseType, typename PartType = typename BaseType::part_type, typename ConstPartType = typename BaseType::const_part_type, typename CoordinateType = typename BaseType::coordinate_type>
			struct MultiPart : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using part_type = PartType;
				using const_part_type = ConstPartType;
				using coordinate_type = CoordinateType;

				// Constructors
				MultiPart() = default;
				using BaseType::BaseType;

				// Methods
				virtual PartType partUnderPoint(CoordinateType x, CoordinateType y) = 0;
				virtual ConstPartType partUnderPoint(CoordinateType x, CoordinateType y) const = 0;
			}; // end struct MultiPart

			/** If a MultiChar is also Containing then charUnderPoint(x,y) should return a value designating "no part"
			 *  if and only if contains(x,y) returns false.
			 */
			// TODO: perhaps find a better name to disambiguate with char?
			template<typename BaseType, typename CharType = typename BaseType::char_type, typename ConstCharType = typename BaseType::const_char_type,
				typename IndexType = typename BaseType::index_type, typename CoordinateType = typename BaseType::coordinate_type>
			struct MultiChar : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using char_type = CharType;
				using const_char_type = ConstCharType;
				using index_type = IndexType;
				using coordinate_type = CoordinateType;

				// Constructors
				MultiChar() = default;
				using BaseType::BaseType;

				// Methods
				virtual CharType charUnderPoint(CoordinateType x, CoordinateType y) = 0;
				virtual ConstCharType charUnderPoint(CoordinateType x, CoordinateType y) const = 0;
				virtual CharType charAtIndex(IndexType i) = 0;
				virtual ConstCharType charAtIndex(IndexType i) const = 0;
			}; // end struct MultiChar

			template<typename BaseType, typename Const = std::false_type, typename CharType = typename BaseType::char_type>
			struct CaretLike;

			template<typename BaseType, typename CharType>
			struct CaretLike<BaseType, std::true_type, CharType> : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using char_type = CharType;

				// Constructors
				CaretLike() = default;
				using BaseType::BaseType;

				// Methods
				// TODO: consider more methods like moveToLeft/Right/Up/Down (will differ in e.g. right-to-left scripts)
				// TODO: consider arguments like wraparound:bool
				virtual void nextPosition() = 0;
				virtual void prevPosition() = 0;
				virtual void firstPosition() = 0;
				virtual void lastPosition() = 0;
			}; // end struct CaretLike

			template<typename BaseType, typename CharType>
			struct CaretLike<BaseType, std::false_type, CharType> : CaretLike<BaseType, std::true_type, CharType>
			{
				// Member Types
				using base = CaretLike<BaseType, std::true_type, CharType>;

				// Constructors
				CaretLike() = default;
				using base::base;

				// Methods
				virtual void eraseNext() = 0;
				virtual void erasePrev() = 0;
				virtual void insert(CharType character) = 0;
			}; // end struct CaretLike

			enum struct NonAsciiKey: unsigned{F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PAGE_UP,PAGE_DOWN,
				HOME,END,LEFT,RIGHT,UP,DOWN,INSERT,L_SHIFT,R_SHIFT,L_ALT,R_ALT,L_CTRL,R_CTRL,NUM_LOCK};

			/** Controls implementing this interface should document whether they expect to receive all
			 *  mouse events or only the ones that happen above them.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			struct KeyboardAndMouse : BaseType
			{
				// Member Types
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				// Constructors
				KeyboardAndMouse() = default;
				using BaseType::BaseType;

				// Methods
				virtual void keyboardAscii(unsigned char charCode, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void keyboardNonAscii(NonAsciiKey key, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y) = 0;
				virtual void mouseEnter(CoordinateType x, CoordinateType y) = 0;
				virtual void mouseMove(CoordinateType x, CoordinateType y) = 0;
				virtual void mouseExit(CoordinateType x, CoordinateType y) = 0;
			}; // end struct KeyboardAndMouse

		} // end namespace Interface

	} // end namespace Frames

} // end namespace graphene

#endif // INTERFACE_H
