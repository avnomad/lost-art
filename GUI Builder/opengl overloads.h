#ifndef OPENGL_OVERLOADS_H
#define OPENGL_OVERLOADS_H

#include <GL/glew.h>

inline void glRect(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	glRectd(x1,y1,x2,y2);
} // end function glRect

inline void glRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glRectf(x1,y1,x2,y2);
} // end function glRect

inline void glRect(GLint x1, GLint y1, GLint x2, GLint y2)
{
	glRecti(x1,y1,x2,y2);
} // end function glRect

inline void glRect(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
	glRects(x1,y1,x2,y2);
} // end function glRect

#endif
