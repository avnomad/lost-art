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

#ifndef EVENT_ADAPTORS_H
#define EVENT_ADAPTORS_H

#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "graphene/frames/interface.hpp"

namespace graphene
{
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

			static Frames::Interface::NonAsciiKey grapheneKey(int glutKey)
			{
				using Frames::Interface::NonAsciiKey;

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

#endif // EVENT_ADAPTORS_H
