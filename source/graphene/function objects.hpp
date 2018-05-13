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

#ifndef FUNCTION_OBJECTS_H
#define FUNCTION_OBJECTS_H

#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace graphene
{
	namespace FunctionObjects
	{
#define ableMacro(ClassName, variableName, testVerb) \
		struct ClassName\
		{\
			template<typename ClassName##Type>\
			bool operator()(const ClassName##Type &variableName) const\
			{\
				return variableName.testVerb();\
			} /* end method operator() */\
		}; /* end struct ClassName */

		ableMacro(Selected, selectable, selected)
		ableMacro(Pressed, pressable, pressed)
		ableMacro(Highlighted, highlightable, highlighted)
		ableMacro(Focused, focusable, focused)
#undef ableMacro

#define textualMacro(ClassName, prefixName, infixName) \
		/* TODO: use concept maps instead when available */\
		struct ClassName\
		{\
			template<typename ClassName##Type>\
			static inline decltype(auto) text(ClassName##Type &&textual)\
			{\
				return textual.prefixName();\
			} /* end method text */\
\
			template<typename ClassName##Type>\
			static inline decltype(auto) textHeight(ClassName##Type &&textual)\
			{\
				return textual.prefixName##Height();\
			} /* end method textHeight */\
\
			template<typename ClassName##Type>\
			static inline decltype(auto) textWidth(ClassName##Type &&textual)\
			{\
				return textual.prefixName##Width();\
			} /* end method textWidth */\
\
			template<typename ClassName##Type>\
			static inline decltype(auto) textCharWidth(ClassName##Type &&textual, size_t index)\
			{\
				return textual.prefixName##CharWidth(index);\
			} /* end method textCharWidth */\
\
			template<typename ClassName##Type>\
			static inline void setTextWidth(ClassName##Type &&textual, const typename ClassName##Type::coordinate_type &value)\
			{\
				textual.set##infixName##Width(value);\
			} /* end method setTextWidth */\
\
			/* TODO: use SFINAE to fallback to (textWidth,textHeight) if effectiveTextSize is not available. */\
			template<typename ClassName##Type>\
			static inline decltype(auto) effectiveTextSize(ClassName##Type &&textual)\
			{\
				return textual.effective##infixName##Size();\
			} /* end method effectiveTextSize */\
\
			/* TODO: use SFINAE to fallback to (textCharWidth,textHeight) if effectiveTextCharSize is not available. */\
			template<typename ClassName##Type>\
			static inline decltype(auto) effectiveTextCharSize(ClassName##Type &&textual, size_t index)\
			{\
				return textual.effective##infixName##CharSize(index);\
			} /* end method effectiveTextCharSize */\
		}; /* end struct ClassName */

		textualMacro(Textual, text, Text)
		textualMacro(Named, name, Name)
#undef textualMacro

		/** Font engines encapsulate low level font metric and rendering functions, to present
		 *  them in a uniform manner. They should be constructible from a font and default
		 *  constructible (that should give font a default value).
		 *  The actual method signatures depend on what the underlying toolkit supports.
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

} // end namespace graphene

#endif // FUNCTION_OBJECTS_H
