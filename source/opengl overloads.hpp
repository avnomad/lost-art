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

#ifndef OPENGL_OVERLOADS_H
#define OPENGL_OVERLOADS_H

#include <type_traits>

#include <GL/glew.h>

inline void glRect(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	glRectd(x1,y1,x2,y2);
} // end function glRect

inline void glRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glRectf(x1,y1,x2,y2);
} // end function glRect

/**
 * Will implicitly convert GLint64 to GLint. Its purpose is to prevent ambiguities arising from the C++ overload resolution mechanism.
 */
inline void glRect(GLint64 x1, GLint64 y1, GLint64 x2, GLint64 y2)
{
	glRecti(x1,y1,x2,y2);
}

inline void glRect(GLint x1, GLint y1, GLint x2, GLint y2)
{
	glRecti(x1,y1,x2,y2);
} // end function glRect

inline void glRect(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
	glRects(x1,y1,x2,y2);
} // end function glRect

/**
 * Converts all arguments to a common type, so that there is no ambiguity as to which overload to call.
 */
template<typename X1, typename Y1, typename X2, typename Y2>
inline void glRect(X1 x1, Y1 y1, X2 x2, Y2 y2)
{
	using CT = typename std::common_type<X1, Y1, X2, Y2>::type;
	glRect(static_cast<CT>(x1), static_cast<CT>(y1), static_cast<CT>(x2), static_cast<CT>(y2));
}

#endif
