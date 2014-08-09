#ifndef GRAPHENE_H
#define GRAPHENE_H

#include <utility>
#include <algorithm>

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

		template<typename FrameStackType, typename BaseType>
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

		template<typename CoordinateType, typename PartType, typename ConstPartType, typename BaseType>
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

	} // end namespace Bases


	namespace Frames
	{
		template<typename FrameStackType, typename BaseType>
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


		template<typename FrameStackType, typename BaseType>
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

		template<typename FrameStackType, typename BaseType>
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


		namespace Renderable
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

				FilledRectangle(const BaseType &other) // this class does not add extra members
					:BaseType(other)
				{
					// empty body
				} // end FilledRectangle copy/conversion constructor

				FilledRectangle(BaseType &&other) // this class does not add extra members
					:BaseType(std::move(other))
				{
					// empty body
				} // end FilledRectangle move/conversion constructor

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

			template<typename BaseType>
			class BorderedRectangle : public BaseType
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
				BorderedRectangle(){/* empty body */}

				BorderedRectangle(const BaseType &other) // this class does not add extra members
					:BaseType(other)
				{
					// empty body
				} // end BorderedRectangle copy/conversion constructor

				BorderedRectangle(BaseType &&other) // this class does not add extra members
					:BaseType(std::move(other))
				{
					// empty body
				} // end BorderedRectangle move/conversion constructor

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					glPushAttrib(GL_POLYGON_BIT);
						glPolygonMode(GL_FRONT,GL_FILL);
						auto minX = std::min(left(),right());
						auto maxX = std::max(left(),right());
						auto minY = std::min(bottom(),top());
						auto maxY = std::max(bottom(),top());
						glRect(minX,minY,minX+borderSize(),maxY-borderSize());
						glRect(minX,maxY-borderSize(),maxX-borderSize(),maxY);
						glRect(maxX-borderSize(),minY+borderSize(),maxX,maxY);
						glRect(minX+borderSize(),minY,maxX,minY+borderSize());
					glPopAttrib();
				} // end method render
			}; // end class BorderedRectangle

			template<template<typename BaseType> class TrueWrapper, template<typename BaseType> class FalseWrapper, typename UnaryPredicate, typename BaseType>
			class Conditional : public BaseType
			{
				/*********************
				*    Member Types    *
				*********************/
			public:
				typedef BaseType base_type;
				typedef UnaryPredicate unary_predicate_type;
				template<typename BaseType> class true_wrapper_template : public TrueWrapper<BaseType>{};
				template<typename BaseType> class false_wrapper_template : public FalseWrapper<BaseType>{};

				/****************
				*    Methods    *
				****************/
			public:
				void render() const
				{
					if(UnaryPredicate()(*this))
						TrueWrapper<BaseType>(*static_cast<const BaseType *const>(this)).render();
					else
						FalseWrapper<BaseType>(*static_cast<const BaseType *const>(this)).render();
				} // end method render
			}; // end class Conditional

		} // end namespace Renderable

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

	} // end namespace FuntionObjects

	/** The intention is to let the client to easily combine frames to create controls as needed.
	 *	But my compiler does not support inheriting constructors, so the created controls would only
	 *	be default constructible. To workaround that in the common case, I specialize common control 
	 *	types and add constructors to them.
	 */
	namespace Controls
	{
		template<typename RectangleType>
		class Button : public Frames::Renderable::Conditional<Frames::Renderable::FilledRectangle,Frames::Renderable::BorderedRectangle,FunctionObjects::Pressed,
								Frames::Pressable<Button<RectangleType>,Frames::UniformlyBordered<typename RectangleType::coordinate_type,RectangleType>>>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef Frames::Renderable::Conditional<Frames::Renderable::BorderedRectangle,Frames::Renderable::FilledRectangle,FunctionObjects::Pressed,
					Frames::Pressable<Button<RectangleType>,Frames::UniformlyBordered<typename RectangleType::coordinate_type,RectangleType>>> base_type;
			typedef typename Button::coordinate_type coordinate_type;
			typedef RectangleType rectangle_type;

			/*********************
			*    Constructors    *
			*********************/
		public:
			Button(){/* empty body */}

			Button(coordinate_type left, coordinate_type bottom, coordinate_type right, coordinate_type top, coordinate_type borderSize, bool pressed = false)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;
				this->borderSize() = borderSize;
				this->pressed() = pressed;
			} // end Button constructor
		}; // end class Button

	} // end namespace Controls

} // end namespace graphene

#endif // GRAPHENE_H
