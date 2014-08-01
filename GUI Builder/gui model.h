#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace gui
{
	void runTestSuite();

	enum class RectangleSide:unsigned {LEFT = 0, BOTTOM = 1, RIGHT = 2, TOP = 3};

	inline std::string rectangleSideToString(RectangleSide side)
	{
		static const std::string sideNames[4] = {"left","bottom","right","top"};
		return sideNames[size_t(side)];
	} // end function rectangleSideToString

	inline RectangleSide stringToRectangleSide(const std::string &side)
	{
		if(side == "left")
			return RectangleSide::LEFT;
		if(side == "bottom")
			return RectangleSide::BOTTOM;
		if(side == "right")
			return RectangleSide::RIGHT;
		if(side == "top")
			return RectangleSide::TOP;
		throw std::runtime_error(side + "is not a valid rectangle side!");
	} // end function stringToRectangleSide

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


	template<typename CoordinateType>
	class Control : public geometry::Rectangle<CoordinateType>
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;

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

		Control(const property_tree_type &tree)
			:Rectangle(tree.get<CoordinateType>("sides.left"),tree.get<CoordinateType>("sides.bottom"),
						tree.get<CoordinateType>("sides.right"),tree.get<CoordinateType>("sides.top"))
		{
			// empty body
		} // end Control conversion constructor

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("sides.left",left());
			tree.put("sides.bottom",bottom());
			tree.put("sides.right",right());
			tree.put("sides.top",top());

			return tree;
		} // end method operator property_tree_type

	}; // end class Control


	struct ConstraintEndPoint
	{
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;

		/***************
		*    Fields    *
		***************/

		size_t control; // ordinal number of the referred control
		RectangleSide side; // enumerator of the referred side

		/*********************
		*    Constructors    *
		*********************/

		ConstraintEndPoint(){/* empty body */}

		ConstraintEndPoint(size_t control, RectangleSide side)
			:control(control),side(side)
		{
			// empty body
		} // end ConstraintEndPoint constructor

		ConstraintEndPoint(const property_tree_type &tree)
			:control(tree.get<size_t>("control")),side(stringToRectangleSide(tree.get<std::string>("side")))
		{
			// empty body
		} // end ConstraintEndPoint conversion constructor

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("control",control);
			tree.put("side",rectangleSideToString(side));

			return tree;
		} // end method operator property_tree_type

	}; // end struct ConstraintEndPoint


	template<typename TextType = std::string>
	class Constraint
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef TextType text_type;
		typedef boost::property_tree::ptree property_tree_type;
		typedef ConstraintEndPoint EndPoint;

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

		Constraint(TextType text, size_t control1, RectangleSide side1, size_t control2, RectangleSide side2)
			:iText(std::move(text))
		{
			iEndPoints[0].control = control1;
			iEndPoints[0].side = side1;
			iEndPoints[1].control = control2;
			iEndPoints[1].side = side2;
		} // end Control constructor

		Constraint(const property_tree_type &tree)
			:iText(tree.get<TextType>("text"))
		{
			iEndPoints[0] = EndPoint(tree.get_child("first-end-point"));
			iEndPoints[1] = EndPoint(tree.get_child("second-end-point"));
		} // end Constraint conversion constructor


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

		/****************
		*    Methods    *
		****************/

		operator property_tree_type() const
		{
			property_tree_type tree;

			tree.put("text",iText);
			tree.put_child("first-end-point",iEndPoints[0]);
			tree.put_child("second-end-point",iEndPoints[1]);

			return tree;
		} // end method operator property_tree_type

	}; // end class Constraint


	template<typename CoordinateType, typename TextType = std::string>
	class Model
	{
	public:
		/*********************
		*    Member Types    *
		*********************/

		typedef boost::property_tree::ptree property_tree_type;
		typedef CoordinateType coordinate_type;
		typedef TextType text_type;

	public: // TODO: make private and add methods to manipulate...
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

		void clear()
		{
			controls.clear();
			constraints.clear();
		} // end method clear

		void load(const std::string &fileName)
		{
			clear();
			property_tree_type tree;
			
			boost::property_tree::read_xml(fileName,tree);

			for(const auto &control : tree.get_child("gui-model.controls"))
				controls.emplace_back(control.second);

			for(const auto &constraint : tree.get_child("gui-model.constraints"))
				constraints.emplace_back(constraint.second);
		} // end method load

		void save(const std::string &fileName)
		{
			property_tree_type tree;

			for(const auto &control : controls)
				tree.add_child("gui-model.controls.control",control);

			for(const auto &constraint : constraints)
				tree.add_child("gui-model.constraints.constraint",constraint);

			boost::property_tree::write_xml(fileName,tree);
		} // end method save

	}; // end class Model

} // end namespace gui

#endif // GUI_MODEL_H
