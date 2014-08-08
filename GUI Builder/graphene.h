#ifndef GRAPHENE_H
#define GRAPHENE_H

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

		template<typename CoordinateType, typename BaseType>
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

		template<typename CoordinateType, typename BaseType>
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

		template<typename FrameStackType, typename BaseType>
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

		template<typename FrameStackType, typename BaseType>
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

		template<typename BorderSizeType, typename BaseType>
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

	} // end namespace Bases


	namespace Frames
	{
		template<typename FrameStackType, typename BaseType>
		class Selectable : public BaseType
		{
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
			} // end method select

			FrameStackType &deselect()
			{
				iSelected = false;
			} // end method deselect

			bool &selected()
			{
				return iSelected
			} // end method selected

			const bool &selected() const
			{
				return iSelected
			} // end method selected
		}; // end class Selectable


		template<typename FrameStackType, typename BaseType>
		class Pressable : public BaseType
		{
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
			} // end method press

			FrameStackType &depress()
			{
				iPressed = false;
			} // end method depress

			bool &pressed()
			{
				return iPressed
			} // end method pressed

			const bool &pressed() const
			{
				return iPressed
			} // end method pressed
		}; // end class Pressable

		template<typename BorderSizeType, typename BaseType>
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

	} // end namespace Frames

	namespace Components
	{
		template<typename BaseType>
		class Renderable;

		template<typename CoordinateType>
		class Renderable<geometry::Rectangle<CoordinateType>> : geometry::Rectangle<CoordinateType>
		{
		public:
			/*********************
			*    Member Types    *
			*********************/

			typedef geometry::Rectangle<CoordinateType> base_type;

			/*********************
			*    Constructors    *
			*********************/
			// (workaround compilers without inherited constructors)

			/** Construct an uninitialized Renderable Rectangle.
			 */
			Renderable(){/* emtpy body */}

			/** Construct a renderable rectangle with the specified sides.
			 */
			Renderable(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
				:Rectangle(left,bottom,right,top)
			{
				// empty body
			} // end Renderable constructor

			/****************
			*    Methods    *
			****************/
			void render() const
			{
				glRect(left(),bottom(),right(),top());
			} // end method render
		}; // end class template specialization Renderable

	} // end namespace Components

	namespace Controls
	{

	} // end namespace Controls


} // end namespace graphene

#endif // GRAPHENE_H
