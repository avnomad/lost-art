#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <vector>
#include <string>
#include <utility>

namespace gui
{
	void runTestSuite();

	namespace geometry
	{

		template<typename CoordinateType>
		struct Rectangle
		{
			/*********************
			*    Member Types    *
			*********************/

			enum Side:unsigned {LEFT = 0, BOTTOM = 1, RIGHT = 2, TOP = 3};
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
				iSides[Side::LEFT] = left;
				iSides[Side::BOTTOM] = bottom;
				iSides[Side::RIGHT] = right;
				iSides[Side::TOP] = top;
			} // end Rectangle constructor

			/*************************
			*    Accessor Methods    *
			*************************/

#define accessor(name,enumerator) \
			CoordinateType &name()\
			{\
				return iSides[Side::enumerator];\
			} /* end method name */\
			\
			const CoordinateType &name() const\
			{\
				return iSides[Side::enumerator];\
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

		}; // end stuct Rectangle

	} // end namespace geometry


	template<typename CoordinateType>
	class Control : public geometry::Rectangle<CoordinateType>
	{
		// TODO: add color
		// TODO: add type

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized Control.
		 */
		Control(){/* emtpy body */}

		/** Construct a Control with the specified sides.
		 */
		Control(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
			:Rectangle(left,bottom,right,top)
		{
			// empty body
		} // end Control constructor

	}; // end class Control


	template<typename TextType = std::string>
	class Constraint
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef TextType text_type;

		struct EndPoint
		{
			// Fields
			size_t control; // ordinal number of the referred control
			size_t side; // ordinal number of the referred side

			// Constructors
			EndPoint(){/* empty body */}

			EndPoint(size_t control, size_t side)
				:control(control),side(side)
			{
				// empty body
			}
		}; // end struct EndPoint

	private:
		/***************
		*    Fields    *
		***************/

		TextType iText;
		EndPoint iEndPoints[2];

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an uninitialized Constraint.
		 */
		Constraint(){/* emtpy body */}

		/** Construct a Constraint with the specified sides.
		 */
		Constraint(TextType text, const EndPoint &endPoint1, const EndPoint &endPoint2)
			:iText(std::move(text))
		{
			iEndPoints[0] = endPoint1;
			iEndPoints[1] = endPoint2;
		} // end Control constructor

		/*************************
		*    Accessor Methods    *
		*************************/

		TextType &text()
		{
			return iText;
		} // end method text

		const TextType &text() const
		{
			return iText;
		} // end method text

		EndPoint (&endPoints())[2]
		{
			return iEndPoints;
		} // end method endPoints

		const EndPoint (&endPoints() const)[2]
		{
			return iEndPoints;
		} // end method endPoints

	}; // end class Constraint


	template<typename CoordinateType, typename TextType = std::string>
	class Model
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef CoordinateType coordinate_type;
		typedef TextType text_type;

	private:
		/***************
		*    Fields    *
		***************/

		std::vector<Control<CoordinateType>> controls;
		std::vector<Constraint<TextType>> constraints;

	public:
		/*********************
		*    Constructors    *
		*********************/

		/** Construct an empty Model.
		 */
		Model(){/* emtpy body */}


		/****************
		*    Methods    *
		****************/

		void load(const std::string &fileName)
		{

		} // end method load

		void save(const std::string &fileName)
		{

		} // end method save

	}; // end class Model

} // end namespace gui

#endif // GUI_MODEL_H
