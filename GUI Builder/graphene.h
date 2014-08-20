#ifndef GRAPHENE_H
#define GRAPHENE_H

#include <ratio>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "opengl overloads.h"

#include "geometry.h"

namespace graphene
{
	void runTestSuite();

	// TODO: add overloads taking suitable vector types

	namespace Bases
	{
		class Empty {};

		template<typename BaseType>
		class Renderable : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;

			// Methods
		public:
			virtual void render() const = 0;
		}; // end class Renderable

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Movable : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef CoordinateType coordinate_type;

			// Methods
		public:
			virtual void move(CoordinateType xOffset, CoordinateType yOffset) = 0;
		}; // end class Movable

		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class Rectangular : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef CoordinateType coordinate_type;

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
			typedef BaseType base_type;
			typedef CoordinateType coordinate_type;

			// Methods
		public:
			virtual bool contains(CoordinateType x, CoordinateType y) const = 0;
		}; // end class Movable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Selectable : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

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
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

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
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

			// Methods
		public:
			virtual FrameStackType &highlight() = 0;
			virtual FrameStackType &dehighlight() = 0;
			virtual bool &highlighted() = 0;
			virtual const bool &highlighted() const = 0;
		}; // end class Highlightable

		template<typename BaseType, typename BorderSizeType = typename BaseType::border_size_type>
		class UniformlyBordered : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef BorderSizeType border_size_type;

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
			typedef BaseType base_type;
			typedef TextType text_type;

			// Methods
		public:
			virtual TextType &text() = 0;
			virtual const TextType &text() const = 0;
		}; // end class Textual

		/** SizedText instances have a fixed 'natural' aspect ration, so changing the height,
		 *	actually changes the width as well. Some frame stacks, may interpret  text sizes, as
		 *	preferred instead of mandatory.
		 */
		template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedText : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef CoordinateType coordinate_type;

			// Methods
		public:
			virtual CoordinateType &textHeight() = 0;
			virtual const CoordinateType &textHeight() const = 0;
			// TODO: add support for setting the text width using properties, and perhaps the ability
			// to set/get the aspect ratio.
			virtual CoordinateType textWidth() const = 0;
			virtual void setTextWidth(const CoordinateType &value) = 0; // deprecated
		}; // end class SizedText

		template<typename BaseType, typename PartType = typename BaseType::part_type, typename ConstPartType = typename BaseType::const_part_type, typename CoordinateType = typename BaseType::coordinate_type>
		class MultiPart : public BaseType
		{
			// Member Types
		public:
			typedef BaseType base_type;
			typedef PartType part_type;
			typedef ConstPartType const_part_type;
			typedef CoordinateType coordinate_type;

			// Methods
		public:
			virtual PartType partUnderPoint(CoordinateType x, CoordinateType y) = 0;
			virtual ConstPartType partUnderPoint(CoordinateType x, CoordinateType y) const = 0;
		}; // end class MultiPart

		namespace EventHandling
		{
			/** Controls implementing this interface should document whether they expect to recieve all
			 *	mouse events or only the ones that happen above them.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class KeyboardAndMouse : public BaseType
			{
				// Member Types
			public:
				typedef BaseType base_type;
				typedef CoordinateType coordinate_type;

				// Methods
			public:
				virtual void keyboardAscii(unsigned char charCode, bool down, CoordinateType x, CoordinateType y) = 0;
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
				typedef BaseType base_type;
				typedef CoordinateType coordinate_type;

				/****************
				*    Methods    *
				****************/
			public:
				void move(CoordinateType xOffset, CoordinateType yOffset)
				{
					left() += xOffset;
					bottom() += yOffset;
					right() += xOffset;
					top() += yOffset;
				} // end method move
			}; // end class Rectangular

			/** The base type should be Movable
			 */
			template<typename BaseType, bool allowHorizontal = true, bool allowVertical = true, typename CoordinateType = typename BaseType::coordinate_type>
			class HVMovable : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				typedef BaseType base_type;
				typedef CoordinateType coordinate_type;

				/****************
				*    Methods    *
				****************/
			public:
				void move(CoordinateType xOffset, CoordinateType yOffset)
				{
					BaseType::move(allowHorizontal?xOffset:0 , allowVertical?yOffset:0);
				} // end method move
			}; // end class HVMovable

		} // end namespace Movable

		template<typename BaseType, typename FrameStackType = typename BaseType::frame_stack_type>
		class Selectable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Selectable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/***************
			*    Fields    *
			***************/
		private:
			bool iSelected;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &select()
			{
				iSelected = true;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
			} // end method select

			FrameStackType &deselect()
			{
				iSelected = false;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
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

			/***************
			*    Fields    *
			***************/
		private:
			bool iPressed;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &press()
			{
				iPressed = true;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
			} // end method press

			FrameStackType &depress()
			{
				iPressed = false;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
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
		class Hightlightable : public BaseType
		{
			// must test that FrameStackType is indeed a subclass of Hightlightable and that
			// the this pointer indeed points to a FrameStackType...
			// Is there something wrong with VS2012 std::is_base_of?

			/***************
			*    Fields    *
			***************/
		private:
			bool iHighlighted;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef FrameStackType frame_stack_type;

			/****************
			*    Methods    *
			****************/
		public:
			FrameStackType &highlight()
			{
				iHighlighted = true;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
			} // end method highlight

			FrameStackType &dehighlight()
			{
				iHighlighted = false;
				return *static_cast<FrameStackType *>(this); // can be dengerous without check...
			} // end method dehighlight

			bool &highlighted()
			{
				return iHighlighted;
			} // end method highlighted

			const bool &highlighted() const
			{
				return iHighlighted;
			} // end method highlighted
		}; // end class Hightlightable

		template<typename BaseType, typename BorderSizeType = typename BaseType::border_size_type>
		class UniformlyBordered : public BaseType
		{
			/***************
			*    Fields    *
			***************/
		private:
			BorderSizeType iBorderSize;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef BorderSizeType border_size_type;

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
			/***************
			*    Fields    *
			***************/
		private:
			TextType iText;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef TextType text_type;

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

		/** BaseType should be Textual, TextEndineType should be default constructible
		 */
		template<typename BaseType, typename FontEngineType = typename BaseType::font_engine_type, typename CoordinateType = typename BaseType::coordinate_type>
		class SizedText : public BaseType
		{
			/***************
			*    Fields    *
			***************/
		private:
			CoordinateType iTextHeight;

			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef CoordinateType coordinate_type;
			typedef FontEngineType font_engine_type;

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
				return fontEngine.stringWidth(text()) * textHeight() / fontEngine.fontHeight();
			} // end method textWidth

			void setTextWidth(const CoordinateType &value) // deprecated
			{
				FontEngineType fontEngine;
				return textHeight() = fontEngine.fontHeight() * value / fontEngine.stringWidth(text());
			} // end method setTextWidth
		}; // end class SizedText

		/**	BaseType should be Rectangular, UniformlyBordered and Containing, and PartType should be a (preferably smart) pointer type.
		 *	ConcretePartTemplate instantiations should be constructible from 4 rectangle sides.
		 */
		template<typename BaseType, template<typename CoordinateType, bool horizontal, bool vertical, bool leftRef, bool bottomRef, bool rightRef, bool topRef> class ConcretePartTemplate,
			typename PartType = typename BaseType::part_type, typename ConstPartType = typename BaseType::const_part_type, typename CoordinateType = typename BaseType::coordinate_type>
		class MultiPartBorderedRectangle : public BaseType
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef BaseType base_type;
			typedef PartType part_type;
			typedef ConstPartType const_part_type;
			typedef CoordinateType coordinate_type;
			// TODO: add template alias for ConcretePartTemplate when implemented

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: modify to work even if left() > right() || bottom() > top()
#define partUnderPointMacro(PartType,Const) \
			PartType partUnderPoint(CoordinateType x, CoordinateType y) Const\
			{\
				if(left() <= x && x <= right() && bottom() <= y && y <= top())\
				{\
					if(x <= left()+borderSize())\
					{\
						if(y <= bottom()+borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,true,true,true,false,false>(left(),bottom(),left()+borderSize(),bottom()+borderSize()));\
						}\
						else if(y < top()-borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,false,true,false,false,false>(left(),bottom()+borderSize(),left()+borderSize(),top()-borderSize()));\
						}\
						else /* top()-borderSize() <= y */\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,true,true,false,false,true>(left(),top()-borderSize(),left()+borderSize(),top()));\
						} /* end else */\
					}\
					else if(x < right()-borderSize())\
					{\
						if(y <= bottom()+borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,false,true,false,true,false,false>(left()+borderSize(),bottom(),right()-borderSize(),bottom()+borderSize()));\
						}\
						else if(y < top()-borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,true,true,true,true,true>(left(),bottom(),right(),top()));\
						}\
						else /* top()-borderSize() <= y */\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,false,true,false,false,false,true>(left()+borderSize(),top()-borderSize(),right()-borderSize(),top()));\
						} /* end else */\
					}\
					else /* right()-borderSize() <= x */\
					{\
						if(y <= bottom()+borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,true,false,true,true,false>(right()-borderSize(),bottom(),right(),bottom()+borderSize()));\
						}\
						else if(y < top()-borderSize())\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,false,false,false,true,false>(right()-borderSize(),bottom()+borderSize(),right(),top()-borderSize()));\
						}\
						else /* top()-borderSize() <= y */\
						{\
							return PartType(new ConcretePartTemplate<CoordinateType,true,true,false,false,true,true>(right()-borderSize(),top()-borderSize(),right(),top()));\
						} /* end else */\
					} /* end else */\
				}\
				else\
					return nullptr;\
			} // end method partUnderPoint

			partUnderPointMacro(PartType,/* omit */)
			partUnderPointMacro(ConstPartType,const)
#undef partUnderPointMacro

		}; // end class MultiPartBorderedRectangle

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
					typedef BaseType base_type;

					/*********************
					*    Constructors    *
					*********************/
				public:
					FilledRectangle(){/* empty body */}

					template<typename OtherType>
					FilledRectangle(OtherType &&other) // this class does not add extra members
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end FilledRectangle forwarding constructor (may move/copy/convert)

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						glPushAttrib(GL_POLYGON_BIT);
							glPolygonMode(GL_FRONT,GL_FILL);
							glRect(std::min(left(),right()),std::min(bottom(),top()),std::max(left(),right()),std::max(bottom(),top()));
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
					typedef BaseType base_type;
					typedef CTRational compile_time_rational_type;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BorderedRectangle(){/* empty body */}

					template<typename OtherType>
					BorderedRectangle(OtherType &&other) // this class does not add extra members
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end BorderedRectangle forwarding constructor (may move/copy/convert)

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						glPushAttrib(GL_POLYGON_BIT);
							glPolygonMode(GL_FRONT,GL_FILL);
							auto bSize = (CTRational::den * borderSize() + CTRational::num) / CTRational::den;
							auto minX = std::min(left(),right());
							auto maxX = std::max(left(),right());
							auto minY = std::min(bottom(),top());
							auto maxY = std::max(bottom(),top());
							glRect(minX,minY,minX+bSize,maxY-bSize);
							glRect(minX,maxY-bSize,maxX-bSize,maxY);
							glRect(maxX-bSize,minY+bSize,maxX,maxY);
							glRect(minX+bSize,minY,maxX,minY+bSize);
						glPopAttrib();
					} // end method render
				}; // end class BorderedRectangle

				/** BaseType should be Rectangular, Textual, SizedText. Margin should be a compile-time rational type
				 *	representing the margin reserved between the rectangle sides and the text.
				 */
				template<typename BaseType, typename Margin = typename BaseType::margin>
				class BoxedText : public BaseType
				{
					/*********************
					*    Member Types    *
					*********************/
				public:
					typedef BaseType base_type;
					typedef Margin margin;

					/*********************
					*    Constructors    *
					*********************/
				public:
					BoxedText(){/* empty body */}

					template<typename OtherType>
					BoxedText(OtherType &&other) // this class does not add extra members
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end BoxedText forwarding constructor (may move/copy/convert)

					/****************
					*    Methods    *
					****************/
				public:
					void render() const
					{
						// TODO: consider scaling text down to fit in box.
						typename BaseType::font_engine_type fontEngine;

						glPushAttrib(GL_TRANSFORM_BIT);
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
								glTranslated((((width() - textWidth())*Margin::den - 2.0*Margin::num) / Margin::den) / 2 + (std::min(left(),right())*Margin::den + Margin::num) / Margin::den,
											 (((height() - textHeight())*Margin::den - 2.0*Margin::num) / Margin::den) / 2 + (std::min(bottom(),top())*Margin::den + Margin::num) / Margin::den,
											 0); // center text in inner rectangle
								glScaled(textWidth() / fontEngine.stringWidth(text()) , textHeight() / fontEngine.fontHeight() , 1);
								glTranslated(0,fontEngine.fontBelowBaseLine(),0);
								fontEngine.render(text());
							glPopMatrix();
						glPopAttrib();
					} // end method render
				}; // end class BoxedText

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
					typedef BaseType base_type;

					/*********************
					*    Constructors    *
					*********************/
				public:
					InversedColor(){/* empty body */}

					template<typename OtherType>
					InversedColor(OtherType &&other) // this class does not add extra members
						:BaseType(std::forward<OtherType>(other))
					{
						// empty body
					} // end InversedColor forwarding constructor (may move/copy/convert)

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
				typedef BaseType base_type;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Stippled(){/* empty body */}

				template<typename OtherType>
				Stippled(OtherType &&other) // this class does not add extra members
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Stippled forwarding constructor (may move/copy/convert)

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
				typedef BaseType base_type;
				typedef UnaryPredicate unary_predicate_type;
				typedef TrueWrapper true_wrapper_type;
				typedef FalseWrapper false_wrapper_type;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Conditional(){/* empty body */}

				template<typename OtherType>
				Conditional(OtherType &&other) // this class does not add extra members
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Conditional forwarding constructor (may move/copy/convert)

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
				typedef BaseType base_type;
				typedef FirstWrapper first_wrapper_type;
				typedef SecondWrapper second_wrapper_type;

				/*********************
				*    Constructors    *
				*********************/
			public:
				Sequential(){/* empty body */}

				template<typename OtherType>
				Sequential(OtherType &&other) // this class does not add extra members
					:BaseType(std::forward<OtherType>(other))
				{
					// empty body
				} // end Sequential forwarding constructor (may move/copy/convert)

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
				typedef BaseType base_type;
				typedef CoordinateType coordinate_type;

				/****************
				*    Methods    *
				****************/
			public:
				virtual void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y){}
				virtual void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y){}
				virtual void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y){}
				virtual void mouseEnter(CoordinateType x, CoordinateType y){}
				virtual void mouseMove(CoordinateType x, CoordinateType y){}
				virtual void mouseExit(CoordinateType x, CoordinateType y){}
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
			 *	except when it is has the focus in which case it expects all.
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
				typedef BaseType base_type;
				typedef CoordinateType coordinate_type;
				typedef Focus<TwoStagePressable<Bases::Empty,void>> focus_type;

				/****************
				*    Methods    *
				****************/
			public:
				virtual void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					// ignore keyboard events
				} // end method keyboardAscii

				virtual void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
				{
					if(button == 0)
					{
						pressed() = down;
						if(down)
							target = this;
						else
						{
							target = nullptr;
							highlighted() = contains(x,y);
						} // end else
					} // end if
				} // end method mouseButton

				virtual void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y)
				{
					// ignore mouse wheel events
				} // end method mouseWheel

				virtual void mouseEnter(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						highlight();
					else if(target == this)
						press();
				} // end method mouseEnter

				virtual void mouseMove(CoordinateType x, CoordinateType y)
				{
					// nothing to do
				} // end method mouseMove

				virtual void mouseExit(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						dehighlight();
					else if(target == this)
						depress();
				} // end method mouseExit
			}; // end class TwoStagePressable

		} // end namespace EventHandling

	} // end namespace Frames

	namespace FunctionObjects
	{
		struct Pressed
		{
			template<typename PressableType>
			bool operator()(const PressableType &pressable)
			{
				return pressable.pressed();
			} // end method operator()
		}; // end struct Pressed

		struct Selected
		{
			template<typename SelectableType>
			bool operator()(const SelectableType &selectable)
			{
				return selectable.selected();
			} // end method operator()
		}; // end struct Selected

		struct Highlighted
		{
			template<typename HighlightableType>
			bool operator()(const HighlightableType &highlightable)
			{
				return highlightable.highlighted();
			} // end method operator()
		}; // end struct Highlighted

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
			GlutStrokeFontEngine(void *font = GLUT_STROKE_ROMAN)
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

	/** The intention is to let the client easily combine frames to create controls as needed.
	 *	But my compiler does not support inheriting constructors, so the created controls would only
	 *	be default constructible. To workaround that in the common case, I specialize common control 
	 *	types and add constructors to them.
	 */
	namespace Controls
	{
		template<typename RectangleType, typename CTRational> class Button;
		template<typename RectangleType, typename CTRational> class ButtonBase : public Frames::EventHandling::TwoStagePressable<
																					Frames::Hightlightable<
																					Frames::Pressable<
																						Frames::UniformlyBordered<RectangleType, typename RectangleType::coordinate_type>,
																					Button<RectangleType,CTRational>>>>{}; // poor man's template alias

		template<typename RectangleType, typename CTRational>
		class Button : public Frames::Renderable::Conditional<ButtonBase<RectangleType,CTRational>,
								Frames::Renderable::Colorblind::FilledRectangle<ButtonBase<RectangleType,CTRational>>,
								Frames::Renderable::Conditional<ButtonBase<RectangleType,CTRational>,
									Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,CTRational>,CTRational>,
									Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,CTRational>,std::ratio<0>>,
									FunctionObjects::Highlighted>,
								FunctionObjects::Pressed>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef Frames::Renderable::Conditional<ButtonBase<RectangleType,CTRational>,
								Frames::Renderable::Colorblind::FilledRectangle<ButtonBase<RectangleType,CTRational>>,
								Frames::Renderable::Conditional<ButtonBase<RectangleType,CTRational>,
									Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,CTRational>,CTRational>,
									Frames::Renderable::Colorblind::BorderedRectangle<ButtonBase<RectangleType,CTRational>,std::ratio<0>>,
									FunctionObjects::Highlighted>,
								FunctionObjects::Pressed> base_type;
			typedef typename Button::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Button(){/* empty body */}

			Button(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, bool pressed = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->pressed() = pressed;
				this->highlighted() = highlighted;
			} // end Button constructor
		}; // end class Button


		template<typename CoordinateType>
		class IShapePart : public Bases::Renderable<Bases::Containing<Bases::Movable<Bases::Empty,CoordinateType>>>{}; // poor man's template alias

		template<typename CoordinateType, bool horizontallyMovable, bool verticallyMovable, bool leftRef, bool bottomRef, bool rightRef, bool topRef>
		class ControlPart : public Frames::Renderable::Colorblind::InversedColor<
									Frames::Renderable::Stippled<
									Frames::Renderable::Colorblind::FilledRectangle<
									Frames::Movable::HVMovable<
										Frames::Movable::Rectangular<
										Bases::Rectangular<
										IShapePart<CoordinateType>
									>>,horizontallyMovable,verticallyMovable>>>>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef typename ControlPart::coordinate_type coordinate_type;
			typedef geometry::RefRectangle<CoordinateType,leftRef,bottomRef,rightRef,topRef> rectangle_type;
			typedef typename rectangle_type::Side Side;

			/***************
			*    Fields    *
			***************/
		private:
			rectangle_type iRectangle; // TODO: find a way to use frame inheritance instaead

			/*********************
			*    Constructors    *
			*********************/
		public:
			ControlPart(){/* empty body */}

			ControlPart(typename rectangle_type::left_type left, typename rectangle_type::bottom_type bottom, typename rectangle_type::right_type right, typename rectangle_type::top_type top)
				:iRectangle(left,bottom,right,top)
			{
				// empty body
			} // end ControlPart constructor

			/****************
			*    Methods    *
			****************/
		public:

#define accessor(name) \
			CoordinateType &name()\
			{\
				return iRectangle.name();\
			} /* end method name */\
			\
			const CoordinateType &name() const\
			{\
				return iRectangle.name();\
			} /* end method name */

			accessor(left)
			accessor(bottom)
			accessor(right)
			accessor(top)
#undef accessor

			CoordinateType &side(Side sideName)
			{
				return iRectangle.side(sideName);
			} // end method side

			const CoordinateType &side(Side sideName) const
			{
				return iRectangle.side(sideName);
			} // end method side

			CoordinateType width() const
			{
				return iRectangle.width();
			} // end method width

			CoordinateType height() const
			{
				return iRectangle.height();
			} // end method height

			bool contains(CoordinateType x, CoordinateType y) const
			{
				return iRectangle.contains(x,y);
			} // end method contains
		}; // end class ControlPart


		template<typename RectangleType, typename CTRational> class Control;
		template<typename RectangleType, typename CTRational> class ControlBase : public Frames::MultiPartBorderedRectangle<
																							Frames::Movable::Rectangular<
																							Frames::Hightlightable<
																							Frames::Selectable<
																								Frames::UniformlyBordered<RectangleType,typename RectangleType::coordinate_type>																	
																							,Control<RectangleType,CTRational>>>>
																						,ControlPart,std::unique_ptr<IShapePart<typename RectangleType::coordinate_type>>,
																									std::unique_ptr<IShapePart<const typename RectangleType::coordinate_type>>>{}; // poor man's template alias

		template<typename RectangleType, typename CTRational>
		class Control : public Frames::Renderable::Conditional<ControlBase<RectangleType,CTRational>,
									Frames::Renderable::Colorblind::FilledRectangle<ControlBase<RectangleType,CTRational>>,
									Frames::Renderable::Conditional<ControlBase<RectangleType,CTRational>,
										Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,CTRational>,CTRational>,
										Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,CTRational>,std::ratio<0>>,
										FunctionObjects::Highlighted>,
									FunctionObjects::Selected>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef Frames::Renderable::Conditional<ControlBase<RectangleType,CTRational>,
									Frames::Renderable::Colorblind::FilledRectangle<ControlBase<RectangleType,CTRational>>,
									Frames::Renderable::Conditional<ControlBase<RectangleType,CTRational>,
										Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,CTRational>,CTRational>,
										Frames::Renderable::Colorblind::BorderedRectangle<ControlBase<RectangleType,CTRational>,std::ratio<0>>,
										FunctionObjects::Highlighted>,
									FunctionObjects::Selected> base_type;
			typedef typename Control::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Control(){/* empty body */}

			Control(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, bool selected = false, bool highlighted = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->selected() = selected;
				this->highlighted() = highlighted;
			} // end Control constructor
		}; // end class Control

		template<typename RectangleType, typename TextType> class LabelBase : public Frames::SizedText<
																						Frames::Textual<
																							Frames::Movable::Rectangular<RectangleType>,
																							TextType>,
																						FunctionObjects::GlutStrokeFontEngine>{}; // poor man's template alias

		template<typename RectangleType, typename TextType, typename Margin>
		class Label : public Frames::Renderable::Sequential<
								LabelBase<RectangleType,TextType>,
								Frames::Renderable::Colorblind::FilledRectangle<LabelBase<RectangleType,TextType>>,
								Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedText<LabelBase<RectangleType,TextType>,Margin>>>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef Frames::Renderable::Sequential<
						LabelBase<RectangleType,TextType>,
						Frames::Renderable::Colorblind::FilledRectangle<LabelBase<RectangleType,TextType>>,
						Frames::Renderable::Colorblind::InversedColor<Frames::Renderable::Colorblind::BoxedText<LabelBase<RectangleType,TextType>,Margin>>> base_type;
			typedef typename Label::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Label(){/* empty body */}

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
		template<typename RootControlType>
		class GLUT
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef RootControlType root_control_type;

			/***************
			*    Fields    *
			***************/
		private:
			static RootControlType rootControl;

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: add static methods implementing GLUT event handling functions
		}; // end class GLUT

	} // end namespace EventAdaptors

} // end namespace graphene

#endif // GRAPHENE_H
