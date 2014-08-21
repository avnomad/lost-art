#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

namespace geometry
{
	void runTestSuite();

	enum class RectangleSide:unsigned {LEFT = 0, BOTTOM = 1, RIGHT = 2, TOP = 3};
} // end namespace geometry


template<typename To, typename From>
To to(const From &);

template<>
inline std::string to<std::string,geometry::RectangleSide>(const geometry::RectangleSide &side)
{
	static const std::string sideNames[4] = {"left","bottom","right","top"};
	return sideNames[size_t(side)];
} // end function to

template<>
inline geometry::RectangleSide to<geometry::RectangleSide,std::string>(const std::string &side)
{
	if(side == "left")
		return geometry::RectangleSide::LEFT;
	if(side == "bottom")
		return geometry::RectangleSide::BOTTOM;
	if(side == "right")
		return geometry::RectangleSide::RIGHT;
	if(side == "top")
		return geometry::RectangleSide::TOP;
	throw std::runtime_error(side + "is not a valid rectangle side!");
} // end function to


namespace geometry
{
	// TODO: consider changing the interface in order to enforce left <= right && bottom <= top.
	// Use properties?
	template<typename CoordinateType>
	struct Rectangle
	{
		/*********************
		*    Member Types    *
		*********************/

		typedef RectangleSide Side;
		typedef CoordinateType coordinate_type;

	private:
		/***************
		*    Fields    *
		***************/

		CoordinateType iSides[4];

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized Rectangle.
		 */
		Rectangle(){/* emtpy body */}

		/** Construct a rectangle with the specified sides.
		 */
		Rectangle(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
		{
			this->left() = left;
			this->bottom() = bottom;
			this->right() = right;
			this->top() = top;
		} // end Rectangle constructor

		/*************************
		*    Accessor Methods    *
		*************************/

#define accessor(name,enumerator) \
		CoordinateType &name()\
		{\
			return iSides[size_t(Side::enumerator)];\
		} /* end method name */\
		\
		const CoordinateType &name() const\
		{\
			return iSides[size_t(Side::enumerator)];\
		} /* end method name */

		accessor(left,LEFT)
		accessor(bottom,BOTTOM)
		accessor(right,RIGHT)
		accessor(top,TOP)
#undef accessor

		CoordinateType (&sides())[4]
		{
			return iSides;
		} // end method sides

		const CoordinateType (&sides() const)[4]
		{
			return iSides;
		} // end method sides

		CoordinateType &side(Side sideName)
		{
			return iSides[size_t(sideName)];
		} // end method side

		const CoordinateType &side(Side sideName) const
		{
			return iSides[size_t(sideName)];
		} // end method side

		/****************
		*    Methods    *
		****************/

		CoordinateType height() const
		{
			return std::max(bottom(),top()) - std::min(bottom(),top());
		} // end method height

		CoordinateType width() const
		{
			return std::max(left(),right()) - std::min(left(),right());
		} // end method height

		/** Returns whether the point (x,y) is inside the rectangle with 
		 *	points on the border considered to be inside.
		 */
		bool contains(CoordinateType x, CoordinateType y) const
		{
			return std::min(left(),right()) <= x && x <= std::max(left(),right())
				&& std::min(bottom(),top()) <= y && y <= std::max(bottom(),top());
		} // end method contains

	}; // end stuct Rectangle

	// TODO: consider changing the interface in order to enforce left <= right && bottom <= top.
	// Use properties?
	/** This rectangled type can capture a designated subset of its coordinate by reference.
	 *	In which case, it should not outlive the objects it refers to.
	 *	Note: there does not seem to be a way to implement transitive constness in C++ so const instances
	 *	should have constant == true while non-const instances should have constant == false.
	 */
	template<typename CoordinateType, bool constant = false, bool leftRef = false, bool bottomRef = false, bool rightRef = false, bool topRef = false>
	struct RefRectangle
	{
		/*********************
		*    Member Types    *
		*********************/

		typedef RectangleSide Side;
		typedef CoordinateType coordinate_type;
		typedef typename std::conditional<constant,typename std::add_const<coordinate_type>::type,coordinate_type>::type effective_coordinate_type;
		typedef typename std::add_reference<effective_coordinate_type>::type effective_coordinate_reference_type;
		typedef typename std::conditional<leftRef,effective_coordinate_reference_type,effective_coordinate_type>::type left_type;
		typedef typename std::conditional<bottomRef,effective_coordinate_reference_type,effective_coordinate_type>::type bottom_type;
		typedef typename std::conditional<rightRef,effective_coordinate_reference_type,effective_coordinate_type>::type right_type;
		typedef typename std::conditional<topRef,effective_coordinate_reference_type,effective_coordinate_type>::type top_type;

	private:
		/***************
		*    Fields    *
		***************/

		left_type iLeft;
		bottom_type iBottom;
		right_type iRight;
		top_type iTop;

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized RefRectangle.
		 */
		RefRectangle(){/* emtpy body */}

		/** Construct a rectangle with the specified sides.
		 *	Depending on the template parameters some arguments may be captured by reference! 
		 */
		RefRectangle(left_type left, bottom_type bottom, right_type right, top_type top)
			:iLeft(left),iBottom(bottom),iRight(right),iTop(top)
		{
			// empty body
		} // end RefRectangle constructor

		/*************************
		*    Accessor Methods    *
		*************************/

#define accessor(name,iName) \
		CoordinateType &name()\
		{\
			if(constant)\
				throw std::logic_error("Incorrect instantiation of RefRectangle template: constant==true should only be used with const objects!");\
			return const_cast<CoordinateType &>(iName);\
		} /* end method name */\
		\
		const CoordinateType &name() const\
		{\
			return iName;\
		} /* end method name */

		accessor(left,iLeft)
		accessor(bottom,iBottom)
		accessor(right,iRight)
		accessor(top,iTop)
#undef accessor

		CoordinateType &side(Side sideName)
		{
			if(constant)
				throw std::logic_error("Incorrect instantiation of RefRectangle template: constant==true should only be used with const objects!");

			switch(sideName)
			{
			case Side::LEFT:
				return const_cast<CoordinateType &>(iLeft);
			case Side::BOTTOM:
				return const_cast<CoordinateType &>(iBottom);
			case Side::RIGHT:
				return const_cast<CoordinateType &>(iRight);
			case Side::TOP:
				return const_cast<CoordinateType &>(iTop);
			default:
				throw std::invalid_argument("Invalid enum value!");
			} // end switch
		} // end method side

		const CoordinateType &side(Side sideName) const
		{
			switch(sideName)
			{
			case Side::LEFT:
				return iLeft;
			case Side::BOTTOM:
				return iBottom;
			case Side::RIGHT:
				return iRight;
			case Side::TOP:
				return iTop;
			default:
				throw std::invalid_argument("Invalid enum value!");
			} // end switch
		} // end method side

		/****************
		*    Methods    *
		****************/

		CoordinateType height() const
		{
			return std::max(bottom(),top()) - std::min(bottom(),top());
		} // end method height

		CoordinateType width() const
		{
			return std::max(left(),right()) - std::min(left(),right());
		} // end method height

		/** Returns whether the point (x,y) is inside the rectangle with 
		 *	points on the border considered to be inside.
		 */
		bool contains(CoordinateType x, CoordinateType y) const
		{
			return std::min(left(),right()) <= x && x <= std::max(left(),right())
				&& std::min(bottom(),top()) <= y && y <= std::max(bottom(),top());
		} // end method contains

	}; // end stuct RefRectangle

} // end namespace geometry

#endif
