#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>

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
	}; // end stuct Rectangle

} // end namespace geometry

#endif
