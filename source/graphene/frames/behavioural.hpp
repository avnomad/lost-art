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

#ifndef BEHAVIOURAL_H
#define BEHAVIOURAL_H

#include <utility>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

#include "std overloads.hpp"

namespace graphene
{
	namespace Frames
	{
		namespace Behavioural
		{
			/**
			 * The base type should be Rectangular
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class Movable : public BaseType
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
				Movable() = default;

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
			}; // end class Movable

			/**
			 * The base type should be Movable
			 */
			template<typename BaseType, typename allowHorizontal = std::true_type, typename allowVertical = std::true_type, typename CoordinateType = typename BaseType::coordinate_type>
			class HVMovable : public BaseType
			{
				/***********************
				*    Concept Checks    *
				***********************/

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

#define boxedAdaptableSizeMacro(ClassAffix, methodAffix) \
			/**\
			 * BaseType should be Rectangular and Sized##ClassAffix, TextEngineType should be default constructible\
			 */\
			template<typename BaseType,\
					typename Margin = typename BaseType::margin,\
					typename FontEngineType = typename BaseType::font_engine_type,\
					typename CoordinateType = typename BaseType::coordinate_type>\
			class BoxedAdaptableSize##ClassAffix : public BaseType\
			{\
				/*********************\
				*    Member Types    *\
				*********************/\
			public:\
				using base_type = BaseType;\
				using coordinate_type = CoordinateType;\
				using font_engine_type = FontEngineType;\
				using margin = Margin;\
\
				/*********************\
				*    Constructors    *\
				*********************/\
			public:\
				BoxedAdaptableSize##ClassAffix() = default;\
\
				using BaseType::BaseType;\
\
				/****************\
				*    Methods    *\
				****************/\
			public:\
				std::pair<CoordinateType,CoordinateType> effective##ClassAffix##Size() const\
				{\
					font_engine_type fontEngine;\
\
					/* scale methodAffix down to fit in rectangle: */\
					auto effective##ClassAffix##Height = utility::min(this->methodAffix##Height(),\
																	(this->height()*margin::den - 2*margin::num) / margin::den);\
					auto effective##ClassAffix##Width = utility::min(effective##ClassAffix##Height * fontEngine.stringWidth(this->methodAffix())\
																								/ fontEngine.fontHeight(),\
																	(this->width()*margin::den - 2*margin::num) / margin::den);\
					if(this->methodAffix().empty())\
						return std::make_pair(static_cast<CoordinateType>(0),effective##ClassAffix##Height); /* avoid division by zero */\
					effective##ClassAffix##Height = utility::min(effective##ClassAffix##Height,\
																effective##ClassAffix##Width * fontEngine.fontHeight()\
																							/ fontEngine.stringWidth(this->methodAffix()));\
\
					return std::make_pair(effective##ClassAffix##Width, effective##ClassAffix##Height);\
				} /* end method effective##ClassAffix##Size */\
\
				/* TODO: This is too inefficient. Consider caching. */\
				std::pair<CoordinateType,CoordinateType> effective##ClassAffix##CharSize(size_t index) const\
				{\
					font_engine_type fontEngine;\
					auto result = effective##ClassAffix##Size();\
					result.first = fontEngine.charWidth(this->methodAffix().at(index)) * result.second / fontEngine.fontHeight();\
\
					return result;\
				} /* end method effective##ClassAffix##CharSize */\
			}; /* end class BoxedAdaptableSize##ClassAffix */

			boxedAdaptableSizeMacro(Text, text)
			boxedAdaptableSizeMacro(Name, name)
#undef boxedAdaptableSizeMacro

			template<template<typename ConcretePartCoordinateType, typename horizontal, typename vertical, bool constant,
								bool leftRef, bool bottomRef, bool rightRef, bool topRef> class ConcretePartTemplate,
					typename CoordinateType>
			struct ConcreteReturnTypesBuilder
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
			}; // end struct ConcreteReturnTypesBuilder

			/**
			 * BaseType should be Rectangular, UniformlyBordered and Containing, and PartType should be a (preferably smart) pointer type.
			 * ConcretePartTemplate instantiations should be constructible from 4 rectangle sides.
			 */
			template<typename BaseType,
				typename ConcreteReturnTypes = typename BaseType::concrete_return_types,
				typename PartType = typename BaseType::part_type,
				typename ConstPartType = typename BaseType::const_part_type,
				typename CoordinateType = typename BaseType::coordinate_type>
			class MultiPartBorderedRectangle : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using concrete_return_types = ConcreteReturnTypes;
				using part_type = PartType;
				using const_part_type = ConstPartType;
				using coordinate_type = CoordinateType;

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
								return PartType(new typename concrete_return_types::const_##bottom_left(left,bottom,left+borderSize,bottom+borderSize));\
							}\
							else if(y < top-borderSize)\
							{\
								return PartType(new typename concrete_return_types::const_##left(left,bottom+borderSize,left+borderSize,top-borderSize));\
							}\
							else /* top-borderSize <= y */\
							{\
								return PartType(new typename concrete_return_types::const_##top_left(left,top-borderSize,left+borderSize,top));\
							} /* end else */\
						}\
						else if(x < right-borderSize)\
						{\
							if(y <= bottom+borderSize)\
							{\
								return PartType(new typename concrete_return_types::const_##bottom(left+borderSize,bottom,right-borderSize,bottom+borderSize));\
							}\
							else if(y < top-borderSize)\
							{\
								return PartType(new typename concrete_return_types::const_##center(left,bottom,right,top));\
							}\
							else /* top-borderSize <= y */\
							{\
								return PartType(new typename concrete_return_types::const_##top(left+borderSize,top-borderSize,right-borderSize,top));\
							} /* end else */\
						}\
						else /* right-borderSize <= x */\
						{\
							if(y <= bottom+borderSize)\
							{\
								return PartType(new typename concrete_return_types::const_##bottom_right(right-borderSize,bottom,right,bottom+borderSize));\
							}\
							else if(y < top-borderSize)\
							{\
								return PartType(new typename concrete_return_types::const_##right(right-borderSize,bottom+borderSize,right,top-borderSize));\
							}\
							else /* top-borderSize <= y */\
							{\
								return PartType(new typename concrete_return_types::const_##top_right(right-borderSize,top-borderSize,right,top));\
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

			/**
			 * Represents a 2D rectangular area containing a line of text and allows retrieving individual characters from
			 * it using points and/or indices.
			 *
			 * BaseType should be Rectangular, UniformlyBordered, Containing, have a font_engine_type member type and
			 * expose text traits via the supplied TextConceptMap. CharType should be a (preferably smart) pointer type.
			 * ConcreteCharTemplate instantiations should be constructible from a reference to *this, an index and an instance of CoordinateType.
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
				/***********************
				*    Concept Checks    *
				***********************/

				static_assert(std::is_unsigned<IndexType>(), "IndexType must be an unsigned integral type!");

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
					FontEngineType fontEngine;\
					auto left   = std::min(this->left(),this->right());\
					auto bottom = std::min(this->bottom(),this->top());\
					auto right  = std::max(this->left(),this->right());\
					auto top    = std::max(this->bottom(),this->top());\
\
					if(this->contains(x,y))\
					{\
						index_type i = 0;\
						coordinate_type sceneTextLeft = left + (this->width() - TextConceptMap::effectiveTextSize(*this).first)/2;\
						coordinate_type fontX = (x-sceneTextLeft)*fontEngine.stringWidth(TextConceptMap::text(*this)) / TextConceptMap::effectiveTextSize(*this).first;\
						coordinate_type fontCharLeft = 0; /* fontX calculation can cause a division by zero, but then the loop won't run and return value will be correct. */\
						for( ; i < TextConceptMap::text(*this).size() ; fontCharLeft += fontEngine.charWidth(TextConceptMap::text(*this)[i]), ++i)\
							if(fontX <= fontCharLeft + fontEngine.charWidth(TextConceptMap::text(*this)[i])/2)\
								break;\
						return CharType(new ConcreteCharType(static_cast<typename ConcreteCharType::pointer_type>(this),fontCharLeft,0,i));\
																/* TODO: check that this indeed points to that type */\
					}\
					else\
						return nullptr;\
				} // end method charUnderPoint

				charUnderPointMacro(     CharType,     ConcreteCharType,/* omit */)
				charUnderPointMacro(ConstCharType,ConstConcreteCharType,const     )
#undef charUnderPointMacro

				/**
				 * IndexType is expected to be numeric and i should take values in [0,text().size()]
				 */
#define charAtIndexMacro(CharType,ConcreteCharType,Const) \
				CharType charAtIndex(IndexType i) Const\
				{\
					FontEngineType fontEngine;\
\
					if(i > TextConceptMap::text(*this).size())\
						throw std::out_of_range("charAtIndex called with i outside the valid range of [0,text().size()].");\
\
					coordinate_type fontCharLeft = 0;\
					for(IndexType c = 0 ; c < i ; ++c)\
						fontCharLeft += fontEngine.charWidth(TextConceptMap::text(*this)[c]);\
					return CharType(new ConcreteCharType(static_cast<typename ConcreteCharType::pointer_type>(this),fontCharLeft,0,i));\
				} // end method charAtIndex

				charAtIndexMacro(     CharType,     ConcreteCharType,/* omit */)
				charAtIndexMacro(ConstCharType,ConstConcreteCharType,const     )
#undef charAtIndexMacro
			}; // end class MultiCharBorderedRectangle

		} // end namespace Behavioural

	} // end namespace Frames

} // end namespace graphene

#endif // BEHAVIOURAL_H
