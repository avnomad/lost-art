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

#include <GL/glew.h>
#include <GL/glut.h>

#include "linear system solving.hpp"
#include "symbolic computation.hpp"
#include "symbol table.hpp"
#include "geometry.hpp"
#include "graphene.hpp"
#include "gui model.hpp"

int main(int argc, char **argv)
{
	// glut initialization
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(700,160);
	glutCreateWindow("LostArt");

	// glew initialization
	glewInit();

	// OpenGL initialization
	glColor3f(1.0,0.75,0.0);	// gold
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	// application initialization
	typedef graphene::EventAdaptors::GLUT<GUIModel::Controls::Model<float>> AdaptorType;
	AdaptorType::initialize();

	// event handling initialization
	glutIdleFunc(AdaptorType::idle);
	glutDisplayFunc(AdaptorType::display);
	glutKeyboardFunc(AdaptorType::keyboard);
	glutKeyboardUpFunc(AdaptorType::keyboardUp);
	glutSpecialFunc(AdaptorType::special);
	glutSpecialUpFunc(AdaptorType::specialUp);
	glutMouseFunc(AdaptorType::mouse);
	glutMotionFunc(AdaptorType::motion);
	glutPassiveMotionFunc(AdaptorType::passiveMotion);
	glutReshapeFunc(AdaptorType::reshape);
	glutMainLoop();

	return 0;
} // end function main
