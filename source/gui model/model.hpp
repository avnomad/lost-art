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

#ifndef MODEL_H
#define MODEL_H

#include <ratio>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <exception>
#include <functional>
#include <type_traits>

#include <cassert>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/rational.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <Eigen/Dense>

#include "geometry.hpp"
#include "graphene.hpp"
#include "symbol table.hpp"
#include "portable type names.hpp"
#include "linear system solving.hpp"
#include "gui model/controls.hpp"
#include "gui model/constraint.hpp"

namespace GUIModel
{
	/**
	 * Invokes the supplied function on the supplied arguments and prints any exceptions to the standard error stream.
	 *
	 * The return value and type are those of the invoked function if no exceptions are thrown.
	 */
	template<typename Function, typename... Arguments>
	decltype(auto) reportExceptions(Function &&function, Arguments &&...arguments)
	{
		try {
			return std::forward<Function>(function)(std::forward<Arguments>(arguments)...);
		} catch(const std::exception &exception) {
			std::cerr << "Error: " << exception.what() << std::endl;
		} // end catch
	} // end method reportExceptions

	namespace Controls
	{
		// TODO: split button and control handling code and move them into separate classes. ButtonManager?
		// TODO: split Model class into frames.
		template<typename CoordinateType, typename TextType = std::string>
		class Model : public geometry::Rectangle<CoordinateType>
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			// parameters
			using coordinate_type = CoordinateType;
			using text_type = TextType;

			// components
			using control_type = Control<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType>;
			using constraint_type = Constraint<geometry::Rectangle<CoordinateType>,std::vector<control_type>,std::ratio<1,2>,std::ratio<1,2>,std::ratio<1>,TextType>;
			using button_type = Button<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,TextType>;
			using text_box_type = TextBox<geometry::Rectangle<CoordinateType>,std::ratio<-1>,std::ratio<2>,std::ratio<1>,TextType>;

			// containers
			using control_container_type = std::vector<control_type>;
			using constraint_container_type = std::vector<constraint_type>;
			using button_container_type = std::vector<std::pair<button_type,std::function<void()>>>;

			// iterators
			using control_iterator = typename control_container_type::iterator;
			using constraint_iterator = typename constraint_container_type::iterator;
			using button_iterator = typename button_container_type::iterator;

			// shorthands
			using property_tree_type = boost::property_tree::ptree;
			using char_type = typename TextType::value_type;
			using end_point_type = typename constraint_type::EndPoint;
			using side_type = typename end_point_type::side_type;

			/***************
			*    Fields    *
			***************/
		public: // TODO: make private and add methods to manipulate...

			static constexpr int margin = 10; // in millimeters
			static constexpr int buttonHeight = 15;  // in millimeters
			static constexpr int buttonWidth = static_cast<int>(buttonHeight*1.61803)/* golden ratio */;  // in millimeters

			static constexpr int borderSize = 2;
			static constexpr int buttonTextHeight = 10;
			static constexpr int textBoxTextHeight = buttonTextHeight;
			static constexpr int controlTextHeight = buttonTextHeight;
			static constexpr int constraintTextHeight = 7;
			static constexpr int constraintThickness = 7;

			// Colors
			std::array<float,4> buttonColour = {{0.94f, 0.9f, 0.55f, 1.0f}}; // khaki
			std::array<float,4> controlColour = {{1.0f, 0.75f, 0.0f, 1.0f}};	// gold
			std::array<float,4> constraintColour = {{0.0f, 0.5f, 0.0f, 1.0f}}; // dark green

			// TODO: change vector to list for faster operations, updating ConstraintEndPoints.
			control_container_type controls;
			constraint_container_type constraints;
			button_container_type buttons;
			text_box_type tbFileName;

			button_iterator highlightedButton;
			button_iterator pressedButton;

			control_iterator highlightedControl;
			control_iterator selectedControl;
			control_iterator focusedControl;

			constraint_iterator highlightedConstraint;
			constraint_iterator selectedConstraint;
			constraint_iterator focusedConstraint;

			std::unique_ptr<IShapePart<CoordinateType>> selectedPart;

			std::unique_ptr<ICaret<CoordinateType, std::false_type, char_type>> caret;

			coordinate_type lastX; // TODO: consider making lastPressX (this will require saving initial movable control position as well)
			coordinate_type lastY;
			unsigned long long controlIndex;

			bool firstResize; // GLUT workaround (can't do first resize in constructor)
			bool createOnMove;
			bool inConstraintAddMode;

			std::unique_ptr<IShapePart<CoordinateType>> endPoint1;
			std::unique_ptr<IShapePart<CoordinateType>> endPoint2;
			size_t control1;
			size_t control2;
			side_type side1;
			side_type side2;

			/*********************
			*    Constructors    *
			*********************/
		public:
			/** Construct an empty Model.
			 */
			Model()
				:tbFileName(textBoxTextHeight,"last session.las",borderSize,0,0,0,0),
				controlIndex(0),firstResize(true),createOnMove(false),inConstraintAddMode(false)
			{
				// initialize buttons
				buttons.emplace_back(button_type(buttonTextHeight,"Load",borderSize,0,0,0,0),[this](){load(tbFileName.text());});
				buttons.emplace_back(button_type(buttonTextHeight,"Save",borderSize,0,0,0,0),[this](){save(tbFileName.text());});
				// TODO: allow customizable header and executable names
				buttons.emplace_back(button_type(buttonTextHeight,"Compile",borderSize,0,0,0,0),
					[this](){compile<boost::rational<long long>,float,size_t,TextType>("application customization.hpp", "build");});
				buttons.emplace_back(button_type(buttonTextHeight,"Run",borderSize,0,0,0,0),
					[this](){run("build");});

				// initialize controls
				controls.emplace_back(controlTextHeight,"Screen",borderSize,0,0,0,0);

				// initialize pointers and iterators
				highlightedButton = buttons.end();
				pressedButton = buttons.end();

				clearControlIterators();
				clearConstraintIterators();

				endPoint1 = nullptr;
				endPoint2 = nullptr;
				selectedPart = nullptr;
				caret = nullptr;
			} // end Model constructor

			/****************
			*    Methods    *
			****************/
		public:
			// TODO: remove when converted to use std::list
			void clearControlIterators()
			{
				highlightedControl = selectedControl = focusedControl = controls.end();
			} // end method clearControlIterators

			void clearConstraintIterators()
			{
				highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
			} // end method clearConstraintIterators

			void clear()
			{
				controls.clear();
				constraints.clear();

				clearControlIterators();
				clearConstraintIterators();

				selectedPart = nullptr;
				caret = nullptr; // TODO: change to only become null if not pointing to text box.
			} // end method clear

			auto eraseControl(control_iterator control)
			{
				size_t index = control - controls.begin();
				auto result = controls.erase(control);
				// TODO: replace with STL algorithms
				for(auto constraint = constraints.begin() ; constraint < constraints.end() ; )
					if(constraint->endPoints()[0].control == index || constraint->endPoints()[1].control == index)
						constraint = constraints.erase(constraint);
					else
					{
						if(constraint->endPoints()[0].control > index)
							--constraint->endPoints()[0].control;
						if(constraint->endPoints()[1].control > index)
							--constraint->endPoints()[1].control;
						++constraint;
					} // end else
				return result;
			} // end method eraseControl

			auto eraseConstraint(constraint_iterator constraint)
			{
				return constraints.erase(constraint);
			} // end method eraseConstraint

			void dehighlightAll()
			{
				if(highlightedButton != buttons.end())
				{
					highlightedButton->first.dehighlight();
					highlightedButton = buttons.end();
				} // end if
				if(highlightedControl != controls.end())
				{
					highlightedControl->dehighlight();
					highlightedControl = controls.end();
				} // end if
				if(highlightedConstraint != constraints.end())
				{
					highlightedConstraint->dehighlight();
					highlightedConstraint = constraints.end();
				} // end if
			} // end method dehighlightAll

			void deselectAll()
			{
				if(selectedConstraint != constraints.end())
				{
					selectedConstraint->deselect();
					selectedConstraint = constraints.end();
				} // end if
				if(selectedControl != controls.end())
				{
					selectedControl->deselect();
					selectedControl = controls.end();
				} // end if
				selectedPart = nullptr;
			} // end method deselectAll

			void unfocusAll()
			{
				if(focusedConstraint != constraints.end())
				{
					focusedConstraint->unfocus();
					focusedConstraint = constraints.end();
				} // end if
				if(focusedControl != controls.end())
				{
					focusedControl->unfocus();
					focusedControl = controls.end();
				} // end if
				tbFileName.unfocus();
				caret = nullptr;
			} // end method unfocusAll

			// TODO: check that there is at least one control (the screen) and that all constraints refer to
			// existent controls! Also that endpoints are consistent.
			void load(const std::string &fileName)
			{
				clear();
				property_tree_type tree, emptyTree;

				boost::property_tree::read_xml(fileName,tree);

				for(const auto &control : tree.get_child("gui-model.controls"))
					controls.emplace_back(control.second);

				for(const auto &constraint : tree.get_child("gui-model.constraints",emptyTree))
					constraints.emplace_back(constraint.second,&controls);

				clearControlIterators();
				clearConstraintIterators();
			} // end method load

			void save(const std::string &fileName) const
			{
				property_tree_type tree;

				for(const auto &control : controls)
					tree.add_child("gui-model.controls.control",control);

				for(const auto &constraint : constraints)
					tree.add_child("gui-model.constraints.constraint",constraint);

				using namespace boost::property_tree;
				write_xml(fileName, tree, std::locale(), xml_writer_make_settings<std::string>('\t', 1));
			} // end method save


			template<typename RationalType, typename IDType = size_t, typename NameType = std::string>
			Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> generateSystemMatrix() const
			{
				auto symbols = std::make_shared<Symbolic::Common::SymbolTable<NameType,IDType>>();
				std::vector<typename constraint_type::template ParseResult<IDType,RationalType>> parseResults;

				for(const auto &constraint : constraints)
					parseResults.push_back(constraint.template parse<RationalType,IDType,NameType>(symbols));
				// TODO: optimize to not include unknown constants that are not present.

				Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> result;
				result.setZero(constraints.size(),4*controls.size() + symbols->size() + 1); // 4*(nControls-1) + nSymbols + 4 + 1
				// the layout is control sides, then named variables, then unknown constants then known constant.

				// Fill in the augmented system matrix
				size_t i = 0;
				for(const auto &constraint : constraints)
				{
					int coeffs[2] = {1,-1};
					if(controls[constraint.endPoints()[0].control].side(constraint.endPoints()[0].side) > controls[constraint.endPoints()[1].control].side(constraint.endPoints()[1].side))
						std::swap(coeffs[0],coeffs[1]);

					for(size_t ep = 0 ; ep < 2 ; ++ep)
					{
						if(constraint.endPoints()[ep].control != 0)
							result(i,4*(constraint.endPoints()[ep].control-1) + size_t(constraint.endPoints()[ep].side)) += coeffs[ep];
						else if(constraint.endPoints()[ep].side == geometry::RectangleSide::RIGHT || constraint.endPoints()[ep].side == geometry::RectangleSide::TOP)
							result(i,4*(controls.size()-1) + symbols->size() + size_t(constraint.endPoints()[ep].side) - 2) += coeffs[ep];
					} // end for

					for(const auto &var : parseResults[i].varCoeff)
						result(i,4*(controls.size()-1) + var.first) = var.second;

					size_t offset = constraint.endPoints()[0].side == geometry::RectangleSide::LEFT || constraint.endPoints()[0].side == geometry::RectangleSide::RIGHT ? 0 : 1;
					result(i,4*(controls.size()-1) + symbols->size() + 2 + offset) = parseResults[i].pxSizeCoeff;

					result(i,4*(controls.size()-1) + symbols->size() + 4) = parseResults[i].rhsConstant; // already moved to rhs during parsing

					++i;
				} // end foreach

				return result;
			} // end method generateSystemMatrix


			/** Takes the solution of a linear system (in the form of a vector space base and point space offset
			 *	generating the solutions) describing a GUI and generates C++ code for an application
			 *	that implements that GUI. The output code is intended to be saved in a file and #included be a
			 *	suitable application-template .cpp file. Generated code is not tied to a specific GUI toolkit.
			 */
			template<typename RationalType, typename AppCoordType>
			void outputCppApp(const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &base, const Eigen::Matrix<RationalType,Eigen::Dynamic,Eigen::Dynamic> &offset, std::ostream &output) const
			{
				output << "// NO INCLUDE GUARD!!!\n\n";
				output << "std::vector<geometry::Rectangle<" << name<AppCoordType>() << ">> controls(" << controls.size()-1 << ");\n\n";
				output << "void updateCoordinates(" << name<AppCoordType>() << " screenWidth, " << name<AppCoordType>() << " screenHeight, "
					   << name<AppCoordType>() << " pixelWidth, " << name<AppCoordType>() << " pixelHeight)\n";
				output << "{\n";

				std::string unknownConstants[4] = {"screenWidth","screenHeight","pixelWidth","pixelHeight"};
				for(size_t i = 0 ; i < controls.size()-1 ; ++i)
				{
					for(size_t j = 0 ; j < 4 ; ++j)
					{ // TODO: optimize for constant functions (assign at application initialization)
						output << "\tcontrols[" << i << "].sides()[" << j << "] = ";
						bool nonZeroBefore = false;
						for(size_t uc = 0 ; uc < 4 ; ++uc)
							if(base(4*i+j,uc) != 0)
							{
								if(nonZeroBefore)
									output << (base(4*i+j,uc).numerator() > 0 ? " + " : " - ");
								else if(base(4*i+j,uc).numerator() < 0)
									output << '-';
								nonZeroBefore = true;

								if(std::abs(base(4*i+j,uc).numerator()) != 1)
									output << std::abs(base(4*i+j,uc).numerator()) << '*';
								output << unknownConstants[uc];
								if(base(4*i+j,uc).denominator() != 1)
									output << '/' << base(4*i+j,uc).denominator();
							} // end if
						if(offset(4*i+j) != 0 || !nonZeroBefore)
						{
							if(nonZeroBefore)
								output << (offset(4*i+j).numerator() > 0 ? " + " : " - ");
							else if(offset(4*i+j).numerator() < 0)
								output << '-';
							nonZeroBefore = true;

							output << std::abs(offset(4*i+j).numerator());
							if(offset(4*i+j).denominator() != 1)
								output << "/(" << name<AppCoordType>() << ")" << offset(4*i+j).denominator();
						} // end if
						output << ";\n";
					} // end for
					if(i != controls.size()-2) output << "\n";
				} // end for

				output << "} // end function updateCoordinates\n";
			} // end method outputOpenGLCppApp

			template<typename RationalType, typename AppCoordType, typename IDType = size_t, typename NameType = std::string>
			void compile(const std::string &headerName, const std::string &buildDirectoryName) const
			{
				auto systemMatrix = generateSystemMatrix<RationalType,IDType,NameType>();

				LinearSystem::numericReducedRowEchelonFormNoPivot(systemMatrix);
				auto investigation = LinearSystem::semiSymbolicInvestigate(systemMatrix,4);
				auto solution = LinearSystem::semiSymbolicSolve(systemMatrix,4);
				auto base = std::get<0>(solution);
				auto offset = std::get<1>(solution);

				// temporary code for outputting errors and warnings.
				if(investigation.nIndependentEquations < investigation.nEquations)
					std::cout << "Warning: " << investigation.nEquations - investigation.nIndependentEquations << " constraint(s) are redundant!" << std::endl;
				if(investigation.isImpossible)
					std::cout << "Error: The specified constraint system is impossible!" << std::endl;
				if(!investigation.boundUnknownConstants.empty())
					std::cout << "Error: " << investigation.boundUnknownConstants.size() << " unknown constant(s) have been bound!" << std::endl;
				if(!investigation.freeVariables.empty())
					std::cout << "Error: " << investigation.freeVariables.size() << " variable(s) have unspecified value! Consider adding more constraints." << std::endl;
				std::cout << std::endl;

				if(investigation.hasUniqueSolution && investigation.boundUnknownConstants.empty())
				{
					std::ofstream output(headerName);
					outputCppApp<RationalType,AppCoordType>(base,offset,output);
					output.close();

					std::thread([buildDirectoryName](){
						std::system(("cmake -E make_directory \""+buildDirectoryName+"\"").c_str());
						std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake .. ").c_str());
						std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake --build .").c_str());
					}).detach();
				} // end if
			} // end method compile

			void run(const std::string &buildDirectoryName) const
			{
				std::thread([buildDirectoryName](){
					std::system(("cmake -E chdir \""+buildDirectoryName+"\" cmake --build . --target run").c_str());
				}).detach();
			} // end method run

			// UI
			void render() const
			{
				// render controls
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(controlColour.data());
					for(const auto &control : controls)
						control.render();
				glPopAttrib();

				// render constraints
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(constraintColour.data());
					for(const auto &constraint : constraints)
						constraint.render();
				glPopAttrib();

				// render buttons (buttons should be in front)
				glPushAttrib(GL_CURRENT_BIT);
					glColor4fv(buttonColour.data());
					for(const auto &button : buttons)
						button.first.render();

					// render text boxes (text boxes as well)
					tbFileName.render();

					if(selectedPart)
						selectedPart->render();

					if(caret)
						caret->render();

					if(endPoint1)
						endPoint1->render();

					if(endPoint2)
						endPoint2->render();
				glPopAttrib();
			} // end method render

			void keyboardAscii(unsigned char code, bool down, CoordinateType x, CoordinateType y)
			{
				// TODO: move application exit somewhere outside Model class (or any UI widget for that matter)
				if(code == 27 && down) // escape key
					if(caret)
						unfocusAll();
					else
						std::exit(0);
				else if(caret)
					caret->keyboardAscii(code,down,x,y);
				else if(down && code == 0x7f) // delete key
				{
					if(selectedConstraint != constraints.end())
					{
						eraseConstraint(selectedConstraint);
						clearConstraintIterators();
						selectedPart = nullptr;
					}
					else if(selectedControl != controls.end() && selectedControl != controls.begin()) // not screen
					{
						eraseControl(selectedControl);
						clearControlIterators();
						clearConstraintIterators();
						// assumes more invalidations than actually happen but will fix when I switch to std::list
						selectedPart = nullptr;
					} // end if
				} // end else
			} // end method keyboardAscii

			void keyboardNonAscii(graphene::Frames::Interface::NonAsciiKey key, bool down, CoordinateType x, CoordinateType y)
			{
				if(key == graphene::Frames::Interface::NonAsciiKey::L_CTRL)
				{
					if(!(inConstraintAddMode = down))
					{
						endPoint1 = nullptr;
						endPoint2 = nullptr;
					} // end else
				}
				else if(caret)
					caret->keyboardNonAscii(key,down,x,y);
			} // end method keyboardNonAscii

			void mouseButton(unsigned button, bool down, CoordinateType x, CoordinateType y)
			{
				if(button == 0)
				{
					if(down)
					{
						if(inConstraintAddMode)
						{
							if(highlightedControl != controls.end())
							{
								endPoint1 = highlightedControl->partUnderPoint(x,y);
								control1 = highlightedControl - controls.begin();
								// Currently partUnderPoint returns a 'left' object if (x,y) is really left (where left < right),
								// not on the LEFT side of the object.
								// TODO: investigate if this is what we want.
								if(dynamic_cast<typename control_type::concrete_return_types::left*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::LEFT) == endPoint1->side(side_type::LEFT) ? side_type::LEFT : side_type::RIGHT;
								else if(dynamic_cast<typename control_type::concrete_return_types::right*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::RIGHT) == endPoint1->side(side_type::RIGHT) ? side_type::RIGHT : side_type::LEFT;
								else if(dynamic_cast<typename control_type::concrete_return_types::bottom*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::BOTTOM) == endPoint1->side(side_type::BOTTOM) ? side_type::BOTTOM : side_type::TOP;
								else if(dynamic_cast<typename control_type::concrete_return_types::top*>(endPoint1.get()))
									side1 = highlightedControl->side(side_type::TOP) == endPoint1->side(side_type::TOP) ? side_type::TOP : side_type::BOTTOM;
								else
									endPoint1 = nullptr;

								if(endPoint1)
								{
									if(endPoint2 && geometry::isHorizontal(side1) == geometry::isHorizontal(side2))
									{
										// create new constraint and focus it
										unfocusAll();
										deselectAll();
										auto avg = geometry::isHorizontal(side1) ? (endPoint1->left() + endPoint1->right() + endPoint2->left() + endPoint2->right())/4.0
																				 : (endPoint1->bottom() + endPoint1->top() + endPoint2->bottom() + endPoint2->top())/4.0;
										constraints.emplace_back(&controls,control1,side1,control2,side2,avg-0.5*constraintThickness,avg+0.5*constraintThickness,"",constraintTextHeight);
										highlightedConstraint = constraints.end();
										(selectedConstraint = focusedConstraint = --constraints.end())->select().focus(); // no constraint was highlighted
										caret = focusedConstraint->charAtIndex(focusedConstraint->text().size());

										endPoint1 = nullptr;
										endPoint2 = nullptr;
									}
									else
									{
										endPoint2 = std::move(endPoint1); // endPoint1 should become empty
										control2 = control1;
										side2 = side1;
									} // end else
								} // end else
							} // end if
						}
						else
						{
							assert(highlightedButton == buttons.end() || highlightedControl == controls.end() || !tbFileName.highlighted());

							if(highlightedButton != buttons.end())
							{
								(pressedButton = highlightedButton)->first.press();
							} // pressing a button should not deselect
							else if(tbFileName.highlighted())
							{
								unfocusAll();
								tbFileName.focus();
								caret = tbFileName.charUnderPoint(x,y);
							} // clicking the text box should not deselect either
							else
							{
								deselectAll();
							} // end else

							if(highlightedControl != controls.end())
							{
								// TODO: bring to front
								selectedPart = (selectedControl = highlightedControl)->select().partUnderPoint(x,y);

								// TODO: consider representing screen with a different control type and encapsulate special case in part selection code.
								if(selectedPart && selectedPart->left() == selectedControl->left() && selectedPart->bottom() == selectedControl->bottom()
									&& selectedPart->right() == selectedControl->right() && selectedPart->top() == selectedControl->top() && selectedControl == controls.begin())
								{ // deselect screen central area (effectively make it transparent to mouse clicks)
									selectedPart = nullptr;
									selectedControl->deselect();
									selectedControl = controls.end();
								}
								else
								{
									unfocusAll();
									caret = (focusedControl = highlightedControl)->focus().charUnderPoint(x,y);
								} // end else
							} // end if

							if(highlightedConstraint != constraints.end())
							{
								selectedPart = (selectedConstraint = highlightedConstraint)->select().partUnderPoint(x,y);
								unfocusAll();
								caret = (focusedConstraint = highlightedConstraint)->focus().charUnderPoint(x,y);
							} // end if

							if(pressedButton == buttons.end() && selectedControl == controls.end() && !tbFileName.highlighted() && selectedConstraint == constraints.end())
								createOnMove = true;

							lastX = x;
							lastY = y;
						} // end else
					}
					else
					{
						if(pressedButton != buttons.end() && pressedButton->first.pressed())
						{
							reportExceptions(pressedButton->second);
							pressedButton->first.depress();
						} // end if
						pressedButton = buttons.end();

						selectedPart = nullptr;

						createOnMove = false;
					} // end else
				} // end if
			} // end method mouseButton

			void mouseEnter(CoordinateType x, CoordinateType y)
			{
				// do nothing
			} // end method mouseEnter

			void mouseMove(CoordinateType x, CoordinateType y)
			{
				if(pressedButton != buttons.end())
					pressedButton->first.pressed() = pressedButton->first.contains(x,y);
				else
				{
					if(createOnMove)
					{
						createOnMove = false;
						// create new control
						// TODO: push at front
						unfocusAll();
						if(highlightedControl != controls.end())
							highlightedControl->dehighlight();
						controls.emplace_back(controlTextHeight,"control"+std::to_string(controlIndex++),borderSize,x,y,x,y);
						(focusedControl = highlightedControl = selectedControl = --controls.end())->highlight().select().focus();
						selectedPart = selectedControl->partUnderPoint(x,y); // TODO: guarantee this will be a corner
						caret = focusedControl->charUnderPoint(x,y);

						// add automatic constraints (temporary code)
						//constraints.emplace_back(&controls,controls.size()-1,side_type::LEFT,controls.size()-1,side_type::RIGHT,y,y+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,controls.size()-1,side_type::BOTTOM,controls.size()-1,side_type::TOP,x,x+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,0,side_type::LEFT,controls.size()-1,side_type::LEFT,y,y+constraintThickness,"0mm",constraintTextHeight);
						//constraints.emplace_back(&controls,0,side_type::BOTTOM,controls.size()-1,side_type::BOTTOM,x,x+constraintThickness,"0mm",constraintTextHeight);
						//highlightedConstraint = selectedConstraint = focusedConstraint = constraints.end();
					} // end if

					dehighlightAll();

					if(selectedPart)
					{
						selectedPart->move(x-lastX,y-lastY);
						lastX = x;
						lastY = y;

						for(auto &constraint : constraints) // TODO: detect and update only the affected constraints.
							constraint.updateSides();
					}
					else // if dropping an object on another makes sence, then highlighting should be done regardless of selectedPart
					{
						// highlight only the front-most
						for(auto button = buttons.begin() ; button < buttons.end() ; ++button)
							if(button->first.contains(x,y))
							{
								(highlightedButton = button)->first.highlight();
								break;
							} // end if

						if(highlightedButton == buttons.end())
							tbFileName.highlighted() = tbFileName.contains(x,y);

						if(!tbFileName.highlighted())
							for(auto constraint = constraints.begin() ; constraint < constraints.end() ; ++constraint)
								if(constraint->contains(x,y))
								{
									(highlightedConstraint = constraint)->highlight();
									break;
								} // end if

						if(highlightedConstraint == constraints.end())
							for(auto control = controls.rbegin() ; control < controls.rend() ; ++control) // TODO: restore front to back iteration when screen-at-front issue fixed
								if(control->contains(x,y))
								{
									(highlightedControl = --control.base())->highlight();
									break;
								} // end if
					} // end else
				} // end else
			} // end method mouseMove

			void mouseExit(CoordinateType x, CoordinateType y)
			{
				// do nothing
			} // end method mouseExit

			// TODO: perhaps replace resize with use of properties for left,right,bottom,top?
			void resize(CoordinateType left, CoordinateType bottom, CoordinateType right, CoordinateType top)
			{
				this->left() = left;
				this->bottom() = bottom;
				this->right() = right;
				this->top() = top;

				// load button
				buttons[0].first.left() = left+margin;
				buttons[0].first.bottom() = top-margin-buttonHeight;
				buttons[0].first.right() = left+margin+buttonWidth;
				buttons[0].first.top() = top-margin;
				// save button
				buttons[1].first.left() = right-3*margin-3*buttonWidth;
				buttons[1].first.bottom() = top-margin-buttonHeight;
				buttons[1].first.right() = right-3*margin-2*buttonWidth;
				buttons[1].first.top() = top-margin;
				// compile button
				buttons[2].first.left() = right-2*margin-2*buttonWidth;
				buttons[2].first.bottom() = top-margin-buttonHeight;
				buttons[2].first.right() = right-2*margin-buttonWidth;
				buttons[2].first.top() = top-margin;
				// run button
				buttons[3].first.left() = right-margin-buttonWidth;
				buttons[3].first.bottom() = top-margin-buttonHeight;
				buttons[3].first.right() = right-margin;
				buttons[3].first.top() = top-margin;

				// file name text box
				tbFileName.left() = left+2*margin+buttonWidth;
				tbFileName.bottom() = top-margin-buttonHeight;
				tbFileName.right() = right-4*margin-3*buttonWidth;
				tbFileName.top() = top-margin;

				// screen control
				if(firstResize)
				{
					firstResize = false;
					controls.front().left() = left+margin;
					controls.front().bottom() = bottom+margin;
					controls.front().right() = right-margin;
					controls.front().top() = top-2*margin-buttonHeight;
				} // end if
			} // end method resize
		}; // end class Model

		// out-of-class initializations
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::borderSize;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::controlTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::buttonTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::textBoxTextHeight;
		template<typename CoordinateType, typename TextType>
		constexpr int Model<CoordinateType,TextType>::constraintTextHeight;

	} // end namespace Controls

} // end namespace GUIModel

#endif // MODEL_H
