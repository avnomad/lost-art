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

#ifndef EVENT_HANDLING_H
#define EVENT_HANDLING_H

#include <memory>
#include <type_traits>

#include "graphene/frames/interface.hpp"

namespace graphene
{
	namespace Frames
	{
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
				using base_type = BaseType;
				using coordinate_type = CoordinateType;

				/*********************
				*    Constructors    *
				*********************/
			public:
				KeyboardAndMouseStub() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y){}
				void keyboardNonAscii(Interface::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y){}
				void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y){}
				void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y){}
				void mouseEnter(CoordinateType x, CoordinateType y){}
				void mouseMove(CoordinateType x, CoordinateType y){}
				void mouseExit(CoordinateType x, CoordinateType y){}
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
			 *  pressable and highlightable and expects to receive only mouse events than happen over it,
			 *  except when it has the focus, in which case it expects all.
			 *  It ignores keyboard and wheel events for now.
			 */
			// TODO: formally define behaviour using a state machine.
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class TwoStagePressable : public BaseType, public Focus<TwoStagePressable<Interface::Empty,void>>
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				using base_type = BaseType;
				using coordinate_type = CoordinateType;
				using focus_type = Focus<TwoStagePressable<Interface::Empty,void>>;

				/*********************
				*    Constructors    *
				*********************/
			public:
				TwoStagePressable() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					// ignore keyboard events
				} // end method keyboardAscii

				void keyboardNonAscii(Interface::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
				{
					// ignore keyboard events... for now.
				} // end method keyboardNonAscii

				void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
				{
					if(button == 0)
					{
						this->pressed() = down;
						if(down)
							target = this;
						else
						{
							target = nullptr;
							this->highlighted() = contains(x,y);
						} // end else
					} // end if
				} // end method mouseButton

				void mouseWheel(CoordinateType wx, CoordinateType wy, CoordinateType x, CoordinateType y)
				{
					// ignore mouse wheel events
				} // end method mouseWheel

				void mouseEnter(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						this->highlight();
					else if(target == this)
						this->press();
				} // end method mouseEnter

				void mouseMove(CoordinateType x, CoordinateType y)
				{
					// nothing to do
				} // end method mouseMove

				void mouseExit(CoordinateType x, CoordinateType y)
				{
					if(target == nullptr)
						this->dehighlight();
					else if(target == this)
						this->depress();
				} // end method mouseExit
			}; // end class TwoStagePressable

			/** BaseType must conform to the CaretLike interface.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class ConstCaretLike : public BaseType
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
				ConstCaretLike() = default;

				using BaseType::BaseType;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					// TODO: consider printing a warning message instead of just ignoring key strokes.
				} // end method keyboardAscii

				void keyboardNonAscii(Interface::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
				{
					using Interface::NonAsciiKey;

					if(down) // ignore up events
					{
						switch(key)
						{
						case NonAsciiKey::LEFT:
							this->prevPosition();
							return;
						case NonAsciiKey::RIGHT:
							this->nextPosition();
							return;
						case NonAsciiKey::HOME:
							this->firstPosition();
							return;
						case NonAsciiKey::END:
							this->lastPosition();
							return;
						default:
							// do nothing
							return;
						} // end switch
					} // end if
				} // end method keyboardNonAscii
			}; // end class ConstCaretLike

			/** BaseType must conform to the CaretLike interface.
			 */
			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			class MutCaretLike : public ConstCaretLike<BaseType, CoordinateType>
			{
				/*********************
				*    Member Types    *
				*********************/
			private:
				using base = ConstCaretLike<BaseType, CoordinateType>;

				/*********************
				*    Constructors    *
				*********************/
			public:
				MutCaretLike() = default;

				using base::base;

				/****************
				*    Methods    *
				****************/
			public:
				void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
				{
					if(down) // ignore up events
					{
						switch(code)
						{
						case 0x7f: // delete key
							this->eraseNext();
							return;
						case '\b': // backspace key
							this->erasePrev();
							return;
						default:
							this->insert(code);
							return;
						} // end switch
					} // end if
				} // end method keyboardAscii

			}; // end class MutCaretLike

			template<typename BaseType, typename CoordinateType = typename BaseType::coordinate_type>
			using CaretLike = typename std::conditional<
				std::is_const<typename std::pointer_traits<typename BaseType::pointer_type>::element_type>::value,
				ConstCaretLike<BaseType, CoordinateType>,
				MutCaretLike<BaseType, CoordinateType>
			>::type;

		} // end namespace EventHandling

	} // end namespace Frames

} // end namespace graphene

#endif // EVENT_HANDLING_H
