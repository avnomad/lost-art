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

#ifndef STATEFUL_H
#define STATEFUL_H

#include <memory>
#include <utility>
#include <type_traits>

namespace graphene
{
	namespace Frames
	{
		namespace Stateful
		{
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
				CoordinateType iXoffset = CoordinateType();
				CoordinateType iYoffset = CoordinateType();

				/*********************
				*    Constructors    *
				*********************/
			public:
				Offset() = default;

				template<typename XCoordinateArgType, typename YCoordinateArgType, typename... ArgTypes,
					typename = typename std::enable_if<std::is_constructible<CoordinateType, XCoordinateArgType &&>::value, void>::type,
					typename = typename std::enable_if<std::is_constructible<CoordinateType, YCoordinateArgType &&>::value, void>::type>
				Offset(XCoordinateArgType &&xOffset, YCoordinateArgType &&yOffset, ArgTypes &&...args)
					:base_type(std::forward<ArgTypes>(args)...),
					 iXoffset(std::forward<XCoordinateArgType>(xOffset)),
					 iYoffset(std::forward<YCoordinateArgType>(yOffset))
				{
					// empty body
				} // end constructor

				template<typename ArgType, typename... ArgTypes,
					typename = typename std::enable_if<!std::is_base_of<Offset, typename std::decay<ArgType>::type>::value, void>::type>
				Offset(ArgType &&arg, ArgTypes &&...args)
					:base_type(std::forward<ArgType>(arg), std::forward<ArgTypes>(args)...)
				{
					// empty body
				} // end constructor

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

#define ableMacro(ClassName, fieldName, setVerb, unsetVerb, testVerb) \
			/**\
			 * Adds state and methods to make a stack frame '*able'.\
			 *\
			 * It is an error to instantiate a stack frame that contains Frame<ClassName, FrameStackType>,\
			 * but doesn't include a complete FrameStackType sub-stack at the top.\
			 */\
			template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>\
			class ClassName : public BaseType\
			{\
				/*********************\
				*    Member Types    *\
				*********************/\
			public:\
				using base_type = BaseType;\
				using frame_stack_type = FrameStackType;\
\
				/***************\
				*    Fields    *\
				***************/\
			private:\
				bool fieldName = false;\
\
				/*********************\
				*    Constructors    *\
				*********************/\
			public:\
				ClassName() = default;\
\
				template<typename... ArgTypes>\
				ClassName(bool testVerb, ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...), fieldName(testVerb)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename ArgType,\
					typename = typename std::enable_if<std::is_base_of<ClassName, typename std::decay<ArgType>::type>::value, void>::type>\
				ClassName(ArgType &&other)\
					:base_type(std::forward<ArgType>(other)), fieldName(std::forward<ArgType>(other).fieldName)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename... ArgTypes>\
				ClassName(ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				/*******************\
				*    Destructor    *\
				*******************/\
			public:\
				~ClassName()\
				{\
					/* workaround because is_base_of can't work with incomplete types */\
					static_assert(std::is_base_of<ClassName, FrameStackType>()\
							   || std::is_base_of<FrameStackType, ClassName>(),\
								  "This class and FrameStackType must be connected by inheritance!");\
				} /* end destructor */\
\
				/****************\
				*    Methods    *\
				****************/\
			public:\
				FrameStackType &setVerb()\
				{\
					fieldName = true;\
					return *static_cast<FrameStackType *>(this);\
				} /* end method setVerb */\
\
				FrameStackType &unsetVerb()\
				{\
					fieldName = false;\
					return *static_cast<FrameStackType *>(this);\
				} /* end method unsetVerb */\
\
				bool &testVerb()\
				{\
					return fieldName;\
				} /* end method testVerb */\
\
				const bool &testVerb() const\
				{\
					return fieldName;\
				} /* end method testVerb */\
			}; /* end class ClassName */

			ableMacro(Selectable, iSelected, select, deselect, selected)
			ableMacro(Pressable, iPressed, press, depress, pressed)
			ableMacro(Highlightable, iHighlighted, highlight, dehighlight, highlighted)
			ableMacro(Focusable, iFocused, focus, unfocus, focused)
#undef ableMacro

#define singleFieldByReference(ClassName, lowercaseAffix, underscore_affix, CapitalizedAffix) \
			template<typename BaseType, typename CapitalizedAffix##Type = typename BaseType::underscore_affix##_type>\
			class ClassName : public BaseType\
			{\
				/*********************\
				*    Member Types    *\
				*********************/\
			public:\
				using base_type = BaseType;\
				using underscore_affix##_type = CapitalizedAffix##Type;\
\
				/***************\
				*    Fields    *\
				***************/\
			private:\
				CapitalizedAffix##Type i##CapitalizedAffix = CapitalizedAffix##Type();\
\
				/*********************\
				*    Constructors    *\
				*********************/\
			public:\
				ClassName() = default;\
\
				template<typename CapitalizedAffix##ArgType, typename... ArgTypes,\
					typename = typename std::enable_if<std::is_constructible<CapitalizedAffix##Type, CapitalizedAffix##ArgType &&>::value, void>::type>\
				ClassName(CapitalizedAffix##ArgType &&lowercaseAffix, ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...), i##CapitalizedAffix(std::forward<CapitalizedAffix##ArgType>(lowercaseAffix))\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename ArgType,\
					typename = typename std::enable_if<std::is_base_of<ClassName, typename std::decay<ArgType>::type>::value, void>::type>\
				ClassName(ArgType &&other)\
					:base_type(std::forward<ArgType>(other)), i##CapitalizedAffix(std::forward<ArgType>(other).i##CapitalizedAffix)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename... ArgTypes>\
				ClassName(ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				/****************\
				*    Methods    *\
				****************/\
			public:\
				CapitalizedAffix##Type &lowercaseAffix()\
				{\
					return i##CapitalizedAffix;\
				} /* end method lowercaseAffix */\
\
				const CapitalizedAffix##Type &lowercaseAffix() const\
				{\
					return i##CapitalizedAffix;\
				} /* end method lowercaseAffix */\
			}; /* end class ClassName */

			singleFieldByReference(Textual, text, text, Text)
			singleFieldByReference(Named, name, name, Name)
			singleFieldByReference(Pointing, pointer, pointer, Pointer)
			singleFieldByReference(Indexing, index, index, Index)
			singleFieldByReference(UniformlyBordered, borderSize, border_size, BorderSize)
#undef singleFieldByReference

#define sizedMacro(ClassName, lowercaseAffix, CapitalizedAffix) \
			/**\
			 * BaseType should be ClassName, TextEngineType should be default constructible\
			 */\
			template<typename BaseType, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>\
			class Sized##CapitalizedAffix : public BaseType\
			{\
				/*********************\
				*    Member Types    *\
				*********************/\
			public:\
				using base_type = BaseType;\
				using coordinate_type = CoordinateType;\
				using font_engine_type = FontEngineType;\
\
				/***************\
				*    Fields    *\
				***************/\
			private:\
				CoordinateType i##CapitalizedAffix##Height = CoordinateType();\
\
				/*********************\
				*    Constructors    *\
				*********************/\
			public:\
				Sized##CapitalizedAffix() = default;\
\
				template<typename CapitalizedAffix##HeightArgType, typename... ArgTypes,\
					typename = typename std::enable_if<std::is_constructible<CoordinateType, CapitalizedAffix##HeightArgType &&>::value,void>::type>\
				Sized##CapitalizedAffix(CapitalizedAffix##HeightArgType &&lowercaseAffix##Height, ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...), i##CapitalizedAffix##Height(std::forward<CapitalizedAffix##HeightArgType>(lowercaseAffix##Height))\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename ArgType,\
					typename = typename std::enable_if<std::is_base_of<Sized##CapitalizedAffix, typename std::decay<ArgType>::type>::value, void>::type>\
				Sized##CapitalizedAffix(ArgType &&other)\
					:base_type(std::forward<ArgType>(other)), i##CapitalizedAffix##Height(std::forward<ArgType>(other).i##CapitalizedAffix##Height)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				template<typename... ArgTypes>\
				Sized##CapitalizedAffix(ArgTypes &&...args)\
					:base_type(std::forward<ArgTypes>(args)...)\
				{\
					/* empty body */\
				} /* end constructor */\
\
				/****************\
				*    Methods    *\
				****************/\
			public:\
				CoordinateType &lowercaseAffix##Height()\
				{\
					return i##CapitalizedAffix##Height;\
				} /* end method lowercaseAffix##Height */\
\
				const CoordinateType &lowercaseAffix##Height() const\
				{\
					return i##CapitalizedAffix##Height;\
				} /* end method lowercaseAffix##Height */\
\
				/* TODO: add support for setting the lowercaseAffix width using properties, and perhaps the ability\
				 * to set/get the aspect ratio.\
				 */\
				CoordinateType lowercaseAffix##Width() const\
				{\
					FontEngineType fontEngine;\
					return fontEngine.stringWidth(this->lowercaseAffix()) * lowercaseAffix##Height() / fontEngine.fontHeight();\
				} /* end method lowercaseAffix##Width */\
\
				CoordinateType lowercaseAffix##CharWidth(size_t index) const\
				{\
					FontEngineType fontEngine;\
					return fontEngine.charWidth(this->lowercaseAffix().at(index)) * lowercaseAffix##Height() / fontEngine.fontHeight();\
				} /* end method lowercaseAffix##CharWidth */\
\
				void set##CapitalizedAffix##Width(const CoordinateType &value) /* deprecated */\
				{\
					FontEngineType fontEngine;\
					lowercaseAffix##Height() = fontEngine.fontHeight() * value / fontEngine.stringWidth(this->lowercaseAffix());\
				} /* end method set##CapitalizedAffix##Width */\
			}; /* end class Sized##CapitalizedAffix */

			sizedMacro(Textual, text, Text)
			sizedMacro(Named, name, Name)
#undef sizedMacro

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
				using text_concept_map = TextConceptMap;
				using font_engine_type = FontEngineType;

				/***************
				*    Fields    *
				***************/
			private:
				FontEngineType iFontEngine;

				/*********************
				*    Constructors    *
				*********************/
			public:
				ConstIndirectCaretLike() = default;

				template<typename... ArgTypes>
				ConstIndirectCaretLike(FontEngineType fontEngine, ArgTypes&&... args)
					:base_type(std::forward<ArgTypes>(args)...),
					 iFontEngine(fontEngine)
				{
					// empty body
				} // end constructor

				template<typename ArgType,
					typename = typename std::enable_if<std::is_base_of<ConstIndirectCaretLike, typename std::decay<ArgType>::type>::value, void>::type>
				ConstIndirectCaretLike(ArgType &&other)
					:base_type(std::forward<ArgType>(other)), iFontEngine(std::forward<ArgType>(other).iFontEngine)
				{
					// empty body
				} // end constructor

				template<typename... ArgTypes>
				ConstIndirectCaretLike(ArgTypes &&...args)
					:base_type(std::forward<ArgTypes>(args)...)
				{
					// empty body
				} // end constructor

				/****************
				*    Methods    *
				****************/
			public:
				FontEngineType &fontEngine()
				{
					return iFontEngine;
				} // end method fontEngine

				const FontEngineType &fontEngine() const
				{
					return iFontEngine;
				} // end method fontEngine

				// TODO: consider more methods like moveToLeft/Right/Up/Down (will differ in e.g. right-to-left scripts)
				// TODO: consider arguments like wraparound:bool
				void nextPosition()
				{
					if(this->index() < TextConceptMap::text(*this->pointer()).size())
					{
						this->xOffset() += iFontEngine.charWidth(TextConceptMap::text(*this->pointer())[this->index()]);
						++this->index();
					} // end if
				} // end method nextPosition

				void prevPosition()
				{
					if(this->index() > 0)
					{
						--this->index();
						this->xOffset() -= iFontEngine.charWidth(TextConceptMap::text(*this->pointer())[this->index()]);
					} // end if
				} // end method prevPosition

				void firstPosition()
				{
					this->index() = 0;
					this->xOffset() = 0;
				} // end method firstPosition

				void lastPosition()
				{
					this->index() = TextConceptMap::text(*this->pointer()).size();
					this->xOffset() = iFontEngine.stringWidth(TextConceptMap::text(*this->pointer()));
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
					TextConceptMap::text(*this->pointer()).erase(this->index(),1);
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
					TextConceptMap::text(*this->pointer()).insert(this->index(),1,character);
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

			/**
			 * Adapts a rectangular type that is unaware of the frame architecture in order to present it as a frame.
			 *
			 * This is part of a family of "adapting" frames intended to adapt geometric types for use as frames.
			 */
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
				template<typename RectangleArgType, typename... ArgTypes,
					typename = typename std::enable_if<std::is_constructible<RectangleType, RectangleArgType &&>::value, void>::type>
				Rectangular(RectangleArgType &&rectangle, ArgTypes &&...args)
					:base_type(std::forward<ArgTypes>(args)...), iRectangle(std::forward<RectangleArgType>(rectangle))
				{
					// empty body
				} // end constructor

				/**
				 * Default construct the underlying RectangularType forwarding arguments to base class constructors.
				 */
				using BaseType::BaseType;

				/**
				 * Forwards arguments to the underlying RectangularType constructor
				 */
				template<typename... ArgTypes,
					typename = typename std::enable_if<std::is_constructible<RectangleType, ArgTypes &&...>::value,	void>::type>
				Rectangular(ArgTypes &&...args)
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

		} // end namespace Stateful

	} // end namespace Frames

} // end namespace graphene

#endif // STATEFUL_H
