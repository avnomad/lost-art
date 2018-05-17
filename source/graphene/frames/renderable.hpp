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

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <utility>
#include <algorithm>
#include <type_traits>

#include <GL/glew.h>

#include "opengl overloads.hpp"

namespace graphene
{
	namespace Introspection
	{
		/**
		 * Return object.borderSize() if the method exists, otherwise defaultValue.
		 */
		template<typename Class, typename DefaultValueType>
		auto optionalBorderSize(Class &&object, DefaultValueType &&)->decltype(std::declval<Class &&>().borderSize())
		{
			return object.borderSize();
		}

		template<typename Class, typename DefaultValueType, typename... Dummy>
		DefaultValueType &&optionalBorderSize(Class &&, DefaultValueType &&defaultValue, Dummy &&...)
		{
			return std::forward<DefaultValueType>(defaultValue);
		}
	} // end namespace Introspection

	namespace Frames
	{
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

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						glPushAttrib(GL_POLYGON_BIT);
							glPolygonMode(GL_FRONT,GL_FILL);
							auto borderSize = Introspection::optionalBorderSize(*this, typename BaseType::coordinate_type());
							auto bSize = (CTRational::den * borderSize + CTRational::num) / CTRational::den;
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
				 *  Margin should be a compile-time rational type representing the margin reserved between the rectangle sides and the text.
				 *  If the text cannot fit in the rectangle, it is silently scaled down for rendering. This does not affect
				 *  the size returned by the object methods.
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
					using text_concept_map = TextConceptMap;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BoxedText() = default;

					using BaseType::BaseType;

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						typename BaseType::font_engine_type fontEngine;
						auto effectiveTextSize = TextConceptMap::effectiveTextSize(*this);
						auto effectiveTextWidth = effectiveTextSize.first;
						auto effectiveTextHeight = effectiveTextSize.second;

						glPushAttrib(GL_TRANSFORM_BIT);
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
								glTranslated((((this->width() - effectiveTextWidth)*Margin::den - 2*Margin::num) / Margin::den) / 2 + (std::min(this->left(),this->right())*Margin::den + Margin::num) / Margin::den,
											 (((this->height() - effectiveTextHeight)*Margin::den - 2*Margin::num) / Margin::den) / 2 + (std::min(this->bottom(),this->top())*Margin::den + Margin::num) / Margin::den,
											 0); // center text in inner rectangle
								glScaled(effectiveTextWidth / fontEngine.stringWidth(TextConceptMap::text(*this)) , effectiveTextHeight / fontEngine.fontHeight() , 1);
								glTranslated(0,fontEngine.fontBelowBaseLine(),0);
								fontEngine.render(TextConceptMap::text(*this));
							glPopMatrix();
						glPopAttrib();
					} // end method render
				}; // end class BoxedText

				/** BaseType should be Rectangular, Textual, SizedText. Margin and LineSpacing should be compile-time rational types
				 *  representing the margin reserved between the rectangle sides and the paragraph text and the empty space left
				 *  between lines respectively.
				 *  Newline characters in the text are respected, but new ones may be added as text is wrapped to fit
				 *  in the rectangle.
				 *  If the text cannot fit in the rectangle, it is silently scaled down for rendering. This does not affect
				 *  the size returned by the object methods.
				 */
				// TODO: Is silent scaling the best option?
				// TODO: Add support for dynamic line spacing using SFINAE
				template<typename BaseType, typename TextConceptMap, typename LineSpacing = typename BaseType::line_spacing, typename Margin = typename BaseType::margin>
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
						auto lineHeight = TextConceptMap::textHeight(*this); // change later...

						// TODO: add line wrapping.
						glPushAttrib(GL_TRANSFORM_BIT);
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
								glTranslated(innerLeft,innerTop - fontEngine.fontAboveBaseLine()*lineHeight/fontEngine.fontHeight(),0); // set caret to initial position
								for(auto character : TextConceptMap::text(*this))
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
				 *  colors swapped. Transparencies aren't swapped!
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
						auto borderSize = Introspection::optionalBorderSize(*this->pointer(),0);

						auto textLeft = left + (this->pointer()->width() - TextConceptMap::effectiveTextSize(*this->pointer()).first)/2;
						auto caretMiddle = textLeft + (TextConceptMap::text(*this->pointer()).empty() ? 0 :
							this->xOffset()*TextConceptMap::effectiveTextSize(*this->pointer()).first / FontEngineType().stringWidth(TextConceptMap::text(*this->pointer())));

						float fgColor[4], bgColor[4];
						glPushAttrib(GL_CURRENT_BIT);
							glGetFloatv(GL_CURRENT_COLOR,fgColor);
							glGetFloatv(GL_COLOR_CLEAR_VALUE,bgColor);

							glColor4f(bgColor[0],bgColor[1],bgColor[2],fgColor[3]);
							glRect((caretMiddle*2*width::den - width::num)/(2*width::den),bottom+borderSize,
									(caretMiddle*2*width::den + width::num)/(2*width::den),top-borderSize);
							glColor4fv(fgColor);
							glRect((caretMiddle*6*width::den - width::num)/(6*width::den),((bottom+borderSize)*3*width::den + width::num)/(3*width::den),
									(caretMiddle*6*width::den + width::num)/(6*width::den),((top-borderSize)*3*width::den - width::num)/(3*width::den));
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
				/***********************
				*    Concept Checks    *
				***********************/

				static_assert(std::is_base_of<BaseType, TrueWrapper>() && std::is_base_of<BaseType, FalseWrapper>(),
					"Wrappers must derive from base type!");
				static_assert(sizeof(BaseType) == sizeof(TrueWrapper) && sizeof(BaseType) == sizeof(FalseWrapper),
					"Wrappers must not add extra members!");

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

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					if(UnaryPredicate()(*this))
						reinterpret_cast<const TrueWrapper*>(static_cast<const BaseType*>(this))->render();
					else
						reinterpret_cast<const FalseWrapper*>(static_cast<const BaseType*>(this))->render();
				} // end method render
			}; // end class Conditional

			template<typename BaseType, typename FirstWrapper, typename SecondWrapper>
			class Sequential : public BaseType // TODO: use variadic templates with fold expressions when available
			{
				/***********************
				*    Concept Checks    *
				***********************/

				static_assert(std::is_base_of<BaseType, FirstWrapper>() && std::is_base_of<BaseType, SecondWrapper>(),
					"Wrappers must derive from base type!");
				static_assert(sizeof(BaseType) == sizeof(FirstWrapper) && sizeof(BaseType) == sizeof(SecondWrapper),
					"Wrappers must not add extra members!");

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

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					reinterpret_cast<const FirstWrapper*>(static_cast<const BaseType*>(this))->render();
					reinterpret_cast<const SecondWrapper*>(static_cast<const BaseType*>(this))->render();
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
			 *  Useful when you only want to render something in one clause of
			 *  the conditional rendering frame.
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

	} // end namespace Frames

} // end namespace graphene

#endif // RENDERABLE_H
