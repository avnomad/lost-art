#ifndef SYMBOLIC_COMPUTATION_H
#define SYMBOLIC_COMPUTATION_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <functional>

//#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/include/lex_lexertl.hpp>
//#include <boost/spirit/include/phoenix.hpp>

namespace Symbolic
{
	void runTestSuite();

	// perhaps wrap following namespaces in namespace "Formulae"?

	namespace Common
	{
		/** Single-scope symbol table for mathematical expressions.
		 *	For efficiency, string names are mapped to integer IDs 
		 *	that are used internally.
		 *	The implementation can be improved by using radix trees 
		 *	and/or storing IDs implicitly.
		 *	IDType should be usable as an index to std::vector and
		 *	it is assumed sizeof(IDType) <= sizeof(IDType *) in 
		 *	efficiency considerations.
		 */
		template<typename NameType, typename IDType>
		class SymbolTable
		{
			// Fields
			std::map<NameType,IDType> names;
			std::vector<typename std::map<NameType,IDType>::iterator> IDs;

		public:
			// Types
			typedef typename std::vector<typename std::map<NameType,IDType>::iterator>::size_type size_type;
			typedef NameType name_type;
			typedef IDType id_type;


			// Constructors / Destructor

			/**	Construct an empty symbol table
			 */
			SymbolTable(){/* empty body */}

			SymbolTable(const SymbolTable &other)
				:names(other.names),IDs(other.IDs.size())
			{
				for(auto begin = names.begin() ; begin != names.end() ; ++begin)
					IDs[begin->second] = begin;
			} // end SymbolTable copy constructor


			/**	Leaves other in a valid, but unspecified state
			 */
			SymbolTable(SymbolTable &&other)
				:names(std::move(other.names)),IDs(std::move(other.IDs))
			{
				// empty body
			} // end SymbolTable move constructor

			/** Construct a symbol table from a sequence of names
			 */
			template<typename InputIterator>
			SymbolTable(InputIterator begin, InputIterator end)
			{
				while(begin != end)
				{
					declare(*begin);
					++begin;
				} // end while
			} // end SymbolTable constructor

			~SymbolTable(){/* empty body */}


			// Methods
			bool declared(const NameType &name) const
			{
				return names.count(name);
			} // end method declared

			bool declared(IDType id) const
			{
				return 0 <= id && id < IDs.size();
			} // end method declared

			IDType id(const NameType &name) const
			{
				return names.at(name);
			} // end method id

			const NameType &name(IDType id) const
			{
				return IDs.at(id)->first;
			} // end method name

			/**	Returns the number of currently declared symbols.
			 */
			size_type size() const
			{
				return IDs.size();
			} // end method size

			bool empty() const
			{
				return IDs.empty();
			} // end method empty

			IDType declare(const NameType &name)
			{
				auto result = names.insert(std::make_pair(name,IDs.size()));
				if(result.second)
					IDs.push_back(result.first);
				return result.first->second;
			} // end method declare

			/** Undeclares a symbol removing it from the symbol table.
			 *	Symbols can only be undeclared in reverse declaration order!
			 *	Return the old numerical id corresponding to name.
			 */
			IDType undeclare(const NameType &name)
			{
				auto id = this->id(name);
				undeclare(id);
				return id;
			} // end method undeclare

			void undeclare(IDType id)
			{
				if(id+1 != IDs.size())
					throw std::logic_error("Symbols can only be undeclared in reverse declaration order!");
				names.erase(IDs[id]);
				IDs.pop_back();
			} // end method undeclare

			void clear()
			{
				IDs.clear();
				names.clear();
			} // end method clear

			SymbolTable &operator=(SymbolTable other)
			{
				names = std::move(other.names);
				IDs = std::move(other.IDs);
				return *this;
			} // end method operator=

		}; // end class SymbolTable


	} // end namespace Common

	namespace FreeForms
	{
		// Using and namespace declarations to easy development with Spirit
		//namespace qi = boost::spirit::qi;
		//namespace ascii = boost::spirit::ascii;
		//namespace lex = boost::spirit::lex;
		//namespace phoenix = boost::phoenix;
		//using qi::rule;
		//using qi::_val;
		//using qi::_r1;
		//using qi::_1;
		//using qi::_2;
		//using qi::_3;
		//using qi::_4;
		//using qi::int_;
		//using phoenix::val;
		//using phoenix::ref;
		//using phoenix::bind;
		//using phoenix::construct;
		//using phoenix::new_;
		//using ascii::space_type;

		namespace OpTags
		{
			typedef int priority_type;

			const priority_type noParenPriority = 0; // should be less than any other priority
			const priority_type additivePriority = 5;
			const priority_type multiplicativePriority = 7;
			const priority_type unaryPriority = 9;
			const priority_type exponentiationPriority = 11;

			// fill in function object not appearing in standard headers
			template<typename T>
			struct unary_plus
			{
				typedef T argument_type;
				typedef T result_type;

				T operator()(const T &x) const
				{
					return +x;
				} // end method operator()

				static const char symbol = '+';
				static const priority_type priority = unaryPriority;
			}; // end struct unary_plus

			// extend function objects appearing in standard headers
			// with application-specific information
			template<typename T>
			struct negate : public std::negate<T>
			{
				static const char symbol = '-';
				static const priority_type priority = unaryPriority;
			}; // end struct negate

			template<typename T>
			struct plus : public std::plus<T>
			{
				static const char symbol = '+';
				static const priority_type priority = additivePriority;
			}; // end struct plus

			template<typename T>
			struct minus : public std::minus<T>
			{
				static const char symbol = '-';
				static const priority_type priority = additivePriority;
			}; // end struct minus

			template<typename T>
			struct multiplies : public std::multiplies<T>
			{
				static const char symbol = '*';
				static const priority_type priority = multiplicativePriority;
			}; // end struct multiplies

			template<typename T>
			struct divides : public std::divides<T>
			{
				static const char symbol = '/';
				static const priority_type priority = multiplicativePriority;
			}; // end struct divides

			template<typename T>
			struct bit_xor : public std::bit_xor<T>
			{
				static const char symbol = '^';
				static const priority_type priority = exponentiationPriority;
			}; // end struct bit_xor

		} // end namespace OpTags



		/** A class to represent free-form mathematical expressions.
		 */
		template<typename RationalType, typename NameType = std::string, typename IDType = int>
		class Expression
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef RationalType rational_type;
			typedef NameType name_type;
			typedef IDType id_type;
			typedef Common::SymbolTable<NameType,IDType> symbol_table_type;

		private:
			enum class Child{LEFT,RIGHT};

			struct Extends
			{
				// Fields
				size_t width;
				// height == aboveBaseLine + belowBaseLine + 1
				size_t aboveBaseLine;
				size_t belowBaseLine;

				Extends(){/* empty body */}

				Extends(size_t width, size_t aboveBaseLine, size_t belowBaseLine)
					:width(width),aboveBaseLine(aboveBaseLine),belowBaseLine(belowBaseLine)
				{
					// empty body
				} // end Extends constructor
			}; // end struct Extends

			typedef std::vector<Extends> extends_container; // I would like to make that a template parameter...
			static const char quotientSymbol = '\304'; // probably should change to something more portable...
				// '_' is too low and '-' creates ambiguities with minus which must be resolved with extra parenthesis...

			struct AbstractNode
			{
				// Currently assumes all binary operators are left associative. Should generalize later...
				// Also assumes unary operators do not share priority with other operators.
				virtual void print1D(std::ostream &out, const symbol_table_type &symbols, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const = 0;
				/** Stores the extends of the rectangle an expression subtree will occupy when 
				 *	printed in 2D in the vector extends in right postorder (right subtree before 
				 *	left subtree). Returns the extends of the subtree it's called on.
				 */
				virtual std::pair<Extends,bool> getPrint2DExtends(const symbol_table_type &symbols, extends_container &extends, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const = 0;
				// uses the property rev(preorder(t) == postorder(reflect(t)) to cache the node attributes outside the nodes!
				// Should probably cache the serialized literal nodes as well...
				// TODO: change top to bottom and have (0,0) be in the bottom-left corner to somewhat simplify code.
				virtual void print2D(std::vector<NameType> &out, size_t left, size_t top, const symbol_table_type &symbols, bool fullyParenthesized, 
										typename extends_container::const_reverse_iterator &currentExtends, OpTags::priority_type parentPriority, Child thisChild) const = 0;
				virtual std::unique_ptr<AbstractNode> deepCopy() const = 0;
				virtual ~AbstractNode(){/* empty body */}
			}; // end struct AbstractNode


			struct LiteralNode : public AbstractNode
			{
				// Fields
				RationalType value;

				// Constructors / Destructor
				LiteralNode(const RationalType &value)
					:value(value)
				{
					// empty body
				} // end LiteralNode constructor

				virtual ~LiteralNode(){/* empty body */}

				// Methods
				virtual void print1D(std::ostream &out, const symbol_table_type &symbols, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || value.denominator() != 1 && (parentPriority > OpTags::divides<RationalType>::priority 
							|| parentPriority == OpTags::divides<RationalType>::priority && thisChild == Child::RIGHT);

					if(needsParenthesis) out << '(';
					out << value.numerator();
					if(value.denominator() != 1)
						out << '/' << value.denominator();
					if(needsParenthesis) out << ')';
				} // end method print1D

				virtual std::pair<Extends,bool> getPrint2DExtends(const symbol_table_type &symbols, extends_container &extends, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					// Unless I'm missing something it never needs parenthesis. The size of the fraction bar disambiguates!
					// Need to make parent's fraction bar wider some times though...
					std::ostringstream numout, denout;
					Extends result;

					numout << value.numerator();
					denout << value.denominator();
					result = Extends(std::max(numout.str().size(),denout.str().size())+(fullyParenthesized<<1) , value.denominator() != 1 , value.denominator() != 1);

					extends.push_back(result);
					return std::make_pair(result,value.denominator() != 1); // report whether we use a fraction bar.
				} // end method getPrint2DExtends

				virtual void print2D(std::vector<NameType> &out, size_t left, size_t top, const symbol_table_type &symbols, bool fullyParenthesized, 
										typename extends_container::const_reverse_iterator &currentExtends, OpTags::priority_type parentPriority, Child thisChild) const
				{
					// Unless I'm missing something it never needs parenthesis. The size of the fraction bar disambiguates!
					NameType snumout, sdenout;
					std::ostringstream numout, denout;

					numout << value.numerator();
					denout << value.denominator();

					snumout = numout.str();
					sdenout = denout.str();

					std::copy(snumout.begin(),snumout.end(),out[top].begin() + (left + ((currentExtends->width-snumout.size()+1) >> 1))); // can handle parenthesis
					if(fullyParenthesized)
					{
						out[top+currentExtends->aboveBaseLine][left] = '(';
						out[top+currentExtends->aboveBaseLine][left + currentExtends->width-1] = ')';
					} // end if
					if(value.denominator() != 1)
					{
						std::fill_n(out[top+1].begin() + (left+fullyParenthesized),currentExtends->width-(fullyParenthesized<<1),quotientSymbol);
						std::copy(sdenout.begin(),sdenout.end(),out[top+2].begin() + (left + ((currentExtends->width-sdenout.size()+1) >> 1))); // can handle parenthesis
					} // end if

					++currentExtends;
				} // end method print2D

				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new LiteralNode(value));
				} // end method deepCopy
			}; // end struct LiteralNode


			struct VariableNode : public AbstractNode
			{
				// Fields
				IDType id;

				// Constructors / Destructor
				VariableNode(IDType id)
					:id(id)
				{
					// empty body
				} // end VariableNode constructor

				virtual ~VariableNode(){/* empty body */}

				// Methods
				virtual void print1D(std::ostream &out, const symbol_table_type &symbols, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					if(fullyParenthesized) out << '(';
					out << symbols.name(id);
					if(fullyParenthesized) out << ')';
				} // end method print1D

				virtual std::pair<Extends,bool> getPrint2DExtends(const symbol_table_type &symbols, extends_container &extends, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					auto result = Extends(symbols.name(id).size() + (fullyParenthesized<<1) , 0 , 0);

					extends.push_back(result);
					return std::make_pair(result,false); // we do not use a fraction bar
				} // end method getPrint2DExtends

				virtual void print2D(std::vector<NameType> &out, size_t left, size_t top, const symbol_table_type &symbols, bool fullyParenthesized, 
										typename extends_container::const_reverse_iterator &currentExtends, OpTags::priority_type parentPriority, Child thisChild) const
				{
					if(fullyParenthesized)
					{
						out[top][left] = '(';
						out[top][left + currentExtends->width-1] = ')';
					} // end if

					std::copy(symbols.name(id).begin(),symbols.name(id).end(),out[top].begin() + (left+fullyParenthesized));

					++currentExtends;
				} // end method print2D

				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new VariableNode(id));
				} // end method deepCopy
			}; // end struct VariableNode


			template<template<class> class Operator>
			struct UnaryNode : public AbstractNode
			{
				// Fields
				std::unique_ptr<AbstractNode> child;

				// Constructors / Destructor
				UnaryNode(std::unique_ptr<AbstractNode> child)
					:child(std::move(child))
				{
					// empty body
				} // end VariableNode constructor

				virtual ~UnaryNode(){/* empty body */}

				// Methods
				virtual void print1D(std::ostream &out, const symbol_table_type &symbols, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority;

					if(needsParenthesis) out << '(';
					out << Operator<RationalType>::symbol;
					child->print1D(out,symbols,fullyParenthesized,Operator<RationalType>::priority,Child::RIGHT);
					if(needsParenthesis) out << ')';
				} // end method print1D

				virtual std::pair<Extends,bool> getPrint2DExtends(const symbol_table_type &symbols, extends_container &extends, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority;
					Extends result = child->getPrint2DExtends(symbols,extends,fullyParenthesized,Operator<RationalType>::priority,Child::RIGHT).first;
					result.width += needsParenthesis ? 1 + 2 : 1;

					extends.push_back(result);
					return std::make_pair(result,false); // we do not use a fraction bar
				} // end method getPrint2DExtends

				virtual void print2D(std::vector<NameType> &out, size_t left, size_t top, const symbol_table_type &symbols, bool fullyParenthesized, 
										typename extends_container::const_reverse_iterator &currentExtends, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority;
					if(needsParenthesis)
					{
						out[top + currentExtends->aboveBaseLine][left] = '(';
						out[top + currentExtends->aboveBaseLine][left + currentExtends->width-1] = ')';
					} // end if
					out[top + currentExtends->aboveBaseLine][left+needsParenthesis] = Operator<RationalType>::symbol;

					++currentExtends;

					child->print2D(out,left+1+needsParenthesis,top,symbols,fullyParenthesized,currentExtends,Operator<RationalType>::priority,Child::RIGHT);
				} // end method print2D

				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new UnaryNode(child->deepCopy()));
				} // end method deepCopy
			}; // end struct UnaryNode


			template<template<class> class Operator>
			struct BinaryNode : public AbstractNode
			{
				// Fields
				std::unique_ptr<AbstractNode> leftChild;
				std::unique_ptr<AbstractNode> rightChild;

				// Constructors
				BinaryNode(std::unique_ptr<AbstractNode> leftChild, std::unique_ptr<AbstractNode> rightChild)
					:leftChild(std::move(leftChild)),rightChild(std::move(rightChild))
				{
					// empty body
				} // end BinaryNode constructor

				virtual ~BinaryNode(){/* empty body */}

				// Methods
				virtual void print1D(std::ostream &out, const symbol_table_type &symbols, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority 
							|| parentPriority == Operator<RationalType>::priority && thisChild == Child::RIGHT;

					if(needsParenthesis) out << '(';
					leftChild->print1D(out,symbols,fullyParenthesized,Operator<RationalType>::priority,Child::LEFT);
					out << Operator<RationalType>::symbol;
					rightChild->print1D(out,symbols,fullyParenthesized,Operator<RationalType>::priority,Child::RIGHT);
					if(needsParenthesis) out << ')';
				} // end method print1D

				virtual std::pair<Extends,bool> getPrint2DExtends(const symbol_table_type &symbols, extends_container &extends, bool fullyParenthesized, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority 
							|| parentPriority == Operator<RationalType>::priority && thisChild == Child::RIGHT;

					auto priority = Operator<RationalType>::symbol == '/' ? OpTags::noParenPriority : Operator<RationalType>::priority; // numerator and denominator don't need parenthesis
					Extends leftExtends, rightExtends, result;
					bool leftUsesFractionBar, rightUsesFractionBar;
					std::tie(rightExtends,rightUsesFractionBar) = rightChild->getPrint2DExtends(symbols,extends,fullyParenthesized,priority,Child::RIGHT); // must be the right first!
					std::tie(leftExtends,leftUsesFractionBar) = leftChild->getPrint2DExtends(symbols,extends,fullyParenthesized,priority,Child::LEFT);

					if(Operator<RationalType>::symbol == '/')
					{
						// size of fraction bar disambiguates instead of parenthesis!
						result.width = std::max(rightExtends.width+((rightUsesFractionBar && !fullyParenthesized)<<1) , leftExtends.width+((leftUsesFractionBar && !fullyParenthesized)<<1)) + (fullyParenthesized<<1);
						result.aboveBaseLine = leftExtends.aboveBaseLine + leftExtends.belowBaseLine + 1;
						result.belowBaseLine = rightExtends.aboveBaseLine + rightExtends.belowBaseLine + 1;
					}
					else if(Operator<RationalType>::symbol == '^')
					{
						result.width = leftExtends.width + rightExtends.width + (needsParenthesis<<1);
						result.aboveBaseLine = leftExtends.aboveBaseLine + rightExtends.aboveBaseLine + rightExtends.belowBaseLine + 1;
						result.belowBaseLine = leftExtends.belowBaseLine;
					}
					else
					{
						result.width = rightExtends.width + leftExtends.width + (needsParenthesis<<1) + (Operator<RationalType>::symbol == '*' ? 1 : 3);
						result.aboveBaseLine = std::max(rightExtends.aboveBaseLine,leftExtends.aboveBaseLine);
						result.belowBaseLine = std::max(rightExtends.belowBaseLine,leftExtends.belowBaseLine);
					} // end else

					extends.push_back(result);
					return std::make_pair(result,Operator<RationalType>::symbol == '/'); // report whether we use a fraction bar.
				} // end method getPrint2DExtends

				virtual void print2D(std::vector<NameType> &out, size_t left, size_t top, const symbol_table_type &symbols, bool fullyParenthesized, 
										typename extends_container::const_reverse_iterator &currentExtends, OpTags::priority_type parentPriority, Child thisChild) const
				{
					bool needsParenthesis = fullyParenthesized || parentPriority > Operator<RationalType>::priority 
							|| parentPriority == Operator<RationalType>::priority && thisChild == Child::RIGHT;

					auto &thisExtends = *currentExtends;
					++currentExtends;

					if(needsParenthesis)
					{
						out[top + thisExtends.aboveBaseLine][left] = '(';
						out[top + thisExtends.aboveBaseLine][left + thisExtends.width-1] = ')';
					} // end if

					if(Operator<RationalType>::symbol == '/')
					{
						std::fill_n(out[top+thisExtends.aboveBaseLine].begin()+(left+fullyParenthesized),thisExtends.width-(fullyParenthesized<<1),quotientSymbol);
						leftChild->print2D(out,left + ((thisExtends.width-currentExtends->width+1) >> 1),top,
											symbols,fullyParenthesized,currentExtends,OpTags::noParenPriority,Child::LEFT); // numerator does not need parenthesis...
						rightChild->print2D(out,left + ((thisExtends.width-currentExtends->width+1) >> 1),top+thisExtends.aboveBaseLine+1,
											symbols,fullyParenthesized,currentExtends,OpTags::noParenPriority,Child::RIGHT); // ...neither denominator (unless fully parenthesized)
					}
					else if(Operator<RationalType>::symbol == '^')
					{
						leftChild->print2D(out,left+needsParenthesis,top + (thisExtends.aboveBaseLine-currentExtends->aboveBaseLine),
											symbols,fullyParenthesized,currentExtends,Operator<RationalType>::priority,Child::LEFT);
						rightChild->print2D(out,left+thisExtends.width-needsParenthesis - currentExtends->width,top,
											symbols,fullyParenthesized,currentExtends,Operator<RationalType>::priority,Child::RIGHT);
					}
					else
					{
						if(Operator<RationalType>::symbol == '*')
							out[top + thisExtends.aboveBaseLine][left+needsParenthesis+currentExtends->width] = Operator<RationalType>::symbol;
						else
						{
							out[top + thisExtends.aboveBaseLine][left+needsParenthesis+currentExtends->width] = ' ';
							out[top + thisExtends.aboveBaseLine][left+needsParenthesis+1+currentExtends->width] = Operator<RationalType>::symbol;
							out[top + thisExtends.aboveBaseLine][left+needsParenthesis+2+currentExtends->width] = ' ';
						} // end else
						leftChild->print2D(out,left+needsParenthesis,top + (thisExtends.aboveBaseLine-currentExtends->aboveBaseLine),
											symbols,fullyParenthesized,currentExtends,Operator<RationalType>::priority,Child::LEFT);
						rightChild->print2D(out,left+thisExtends.width-needsParenthesis - currentExtends->width,
											top + (thisExtends.aboveBaseLine-currentExtends->aboveBaseLine),
											symbols,fullyParenthesized,currentExtends,Operator<RationalType>::priority,Child::RIGHT);
					} // end else
				} // end method print2D

				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new BinaryNode(leftChild->deepCopy(),rightChild->deepCopy()));
				} // end method deepCopy
			}; // end struct BinaryNode


			/***************
			*    Fields    *
			***************/

			std::shared_ptr<symbol_table_type> symbols; // should never be a null shared pointer, due to getSymbols method
			std::unique_ptr<AbstractNode> expressionTree;


			/**********************************
			*    Constructors / Destructor    *
			**********************************/
		public:
			/** Construct an empty Expression
			 */
			Expression(std::shared_ptr<symbol_table_type> symbolTable = std::shared_ptr<symbol_table_type>(new symbol_table_type())) // make_shared doesn't work for some reason...
				:symbols(std::move(symbolTable))
			{
				// empty body
			} // end Expression default constructor

			Expression(const Expression &other)
				:symbols(other.symbols),expressionTree(other.expressionTree->deepCopy())
			{
				// empty body
			} // end Expression copy constructor

			Expression(Expression &&other)
				:symbols(std::move(other.symbols)),expressionTree(std::move(other.expressionTree))
			{
				// empty body
			} // end Expression move constructor

			/** Construct an expression containing a single literal value
			 */
			Expression(const RationalType &literalValue, std::shared_ptr<symbol_table_type> symbolTable = std::shared_ptr<symbol_table_type>(new symbol_table_type()))
				:symbols(std::move(symbolTable)),expressionTree(new LiteralNode(literalValue))
			{
				// empty body
			} // end Expression constructor

			/** Construct an expression containing a single symbol.
			 *	The symbolTable argument plays the role of the namespace.
			 */
			Expression(const NameType &variableName, std::shared_ptr<symbol_table_type> symbolTable = std::shared_ptr<symbol_table_type>(new symbol_table_type()))
				:symbols(std::move(symbolTable))
			{
				expressionTree.reset(new VariableNode(symbols->declare(variableName)));
			} // end Expression constructor

			/** Construct an expression object from a sequence of characters.
			 *	The constructor parses the sequence as if it was a string and 
			 *	creates an expression object representing the string.
			 *
			 *	Spirit does not support unique_ptrs so this may leak in case exceptions are thrown...
			 */
			//template<typename ForwardIterator>
			//Expression(ForwardIterator begin, ForwardIterator end, std::shared_ptr<symbol_table_type> symbolTable = std::shared_ptr<symbol_table_type>(new symbol_table_type()))
			//	:symbols(std::move(symbolTable))
			//{
			//	typedef lex::lexertl::token<ForwardIterator,boost::mpl::vector<NameType>,boost::mpl::false_> token_type;
			//	typedef lex::lexertl::actor_lexer<token_type> lexer_type;
			//	typedef typename lexer_type::iterator_type iterator_type;

			//	std::stringstream sout;
			//	Grammar<lexer_type> scanner(sout);
			//	Syntax<iterator_type> parser(scanner,sout);
			//	AbstractNode *parseTree = nullptr;

			//	bool success = lex::tokenize_and_parse(begin,end,scanner,parser(symbols.get()),parseTree);
			//	expressionTree.reset(parseTree);

			//	if(!success || begin != end)
			//		throw std::runtime_error(sout.str().empty() ? "Parse error reading mathematical expression!" : sout.str());
			//} // end Expression constructor

			~Expression(){/* empty body */}


			/****************
			*    Methods    *
			****************/

			const symbol_table_type &getSymbols() const
			{
				return *symbols;
			} // end method getSymbols

			/** Checks whether an expression is empty. i.e. Constructed with the default constructor
			 *	and not assigned a non-empty expression.
			 */
			bool empty() const
			{
				return !expressionTree;
			} // end function empty

			void print1D(std::ostream &out, bool fullyParenthesized = false) const
			{
				expressionTree->print1D(out,*symbols,fullyParenthesized,OpTags::noParenPriority,Child::LEFT);
			} // end method print1D

			void print2D(std::ostream &out, bool fullyParenthesized = false) const
			{
				extends_container extends;
				Extends rootExtends = expressionTree->getPrint2DExtends(*symbols,extends,fullyParenthesized,OpTags::noParenPriority,Child::LEFT).first;

				std::vector<NameType> temporaryStorage(rootExtends.aboveBaseLine+rootExtends.belowBaseLine+1);
				std::for_each(temporaryStorage.begin(),temporaryStorage.end(),[&rootExtends](NameType &row){row.resize(rootExtends.width,' ');});

				expressionTree->print2D(temporaryStorage,0,0,*symbols,fullyParenthesized,extends.crbegin(),OpTags::noParenPriority,Child::LEFT);

				std::copy(temporaryStorage.begin(),temporaryStorage.end(),std::ostream_iterator<NameType>(out,"\n"));
			} // end method print2D

			/** Construct an expression object form a smaller one and a unary operator
			 */
			template<template<class> class Operator>
			static Expression unaryCombine(Expression subExpression)
			{
				if(subExpression.empty())
					throw std::logic_error("Empty Expression objects cannot be used to construct larger Expression objects!");
				subExpression.expressionTree.reset(new UnaryNode<Operator>(std::move(subExpression.expressionTree)));
				return subExpression;
			} // end static method unaryCombine

			/** Construct an expression object form two smaller ones and a binary operator
			 */
			template<template<class> class Operator>
			static Expression binaryCombine(Expression leftSubExpression, Expression rightSubExpression)
			{
				if(leftSubExpression.empty() || rightSubExpression.empty())
					throw std::logic_error("Empty Expression objects cannot be used to construct larger Expression objects!");
				if(leftSubExpression.symbols->empty())
					leftSubExpression.symbols = std::move(rightSubExpression.symbols);
				else if(leftSubExpression.symbols != rightSubExpression.symbols && !rightSubExpression.symbols->empty())
					throw std::logic_error("Combining sub-expressions with different symbol tables (or namespaces) is not supported!");
				leftSubExpression.expressionTree.reset(new BinaryNode<Operator>(std::move(leftSubExpression.expressionTree),std::move(rightSubExpression.expressionTree)));
				return leftSubExpression;
			} // end static method binaryCombine


			/******************
			*    Operators    *
			******************/

			Expression &operator=(Expression other)
			{
				symbols = std::move(other.symbols);
				expressionTree = std::move(other.expressionTree);
				return *this;
			} // end method operator=


			/***********************
			*    Parser Support    *
			***********************/
		private:
			//template<typename Lexer>
			//struct Grammar : public lex::lexer<Lexer>
			//{
			//	// Tokens
			//	lex::token_def<NameType> identifier;
			//	lex::token_def<NameType> rationalLiteral;
			//	lex::token_def<NameType> whiteSpace;
			//	lex::token_def<NameType> illegalCharacter;

			//	// Regular Expressions
			//	/** A Grammar object should not outlive the std::ostream 
			//	 *	one given as argument to its constructor.
			//	 */
			//	Grammar(std::ostream &errorStream)
			//		:identifier("[a-zA-Z_][a-zA-Z_0-9]*"),
			//		rationalLiteral("[0-9]+(\\.[0-9]+)?"),
			//		whiteSpace("[ \t\v\f\n\r]"),
			//		illegalCharacter(".")
			//	{
			//		this->self
			//			= identifier | rationalLiteral
			//			| '(' | ')' | '+' | '-' | '*' | '/' | '^'
			//			| whiteSpace[lex::_pass = lex::pass_flags::pass_ignore]
			//			| illegalCharacter[errorStream << val("Ignoring invalid character '") << *lex::_start << "' in input!\n"
			//								, lex::_pass = lex::pass_flags::pass_ignore];
			//				// should be pass_fail but spirit parser loops forever on failure...
			//	} // end Grammar constructor
			//}; // end struct Grammar

			//typedef AbstractNode *attribute_signature(symbol_table_type *);

			//template<typename ForwardIterator>
			//struct Syntax : public qi::grammar<ForwardIterator,attribute_signature>
			//{
			//	// Operator parsers
			//	struct AdditiveOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
			//	{
			//		AdditiveOperator()
			//		{
			//			add("+",binaryCombine<OpTags::plus>)
			//			   ("-",binaryCombine<OpTags::minus>);
			//		} // end AdditiveOperator constructor
			//	} additiveOperator; // end struct AdditiveOperator

			//	struct MultiplicativeOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
			//	{
			//		MultiplicativeOperator()
			//		{
			//			add("*",binaryCombine<OpTags::multiplies>)
			//			   ("/",binaryCombine<OpTags::divides>);
			//		} // end MultiplicativeOperator constructor
			//	} multiplicativeOperator; // end struct MultiplicativeOperator

			//	struct ExponentiationOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
			//	{
			//		ExponentiationOperator()
			//		{
			//			add("^",binaryCombine<OpTags::bit_xor>);
			//		} // end ExponentiationOperator constructor
			//	} exponentiationOperator; // end struct ExponentiationOperator

			//	struct PrefixOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *)>
			//	{
			//		PrefixOperator()
			//		{
			//			add("+",unaryCombine<OpTags::unary_plus>)
			//			   ("-",unaryCombine<std::negate>);
			//		} // end PrefixOperator constructor
			//	} prefixOperator; // end struct PrefixOperator

			//	// Non-Terminals
			//	rule<ForwardIterator,attribute_signature> expression;
			//	rule<ForwardIterator,attribute_signature> term;
			//	rule<ForwardIterator,attribute_signature> factor;
			//	rule<ForwardIterator,attribute_signature> prefix;
			//	rule<ForwardIterator,attribute_signature> primary;

			//	// Rules
			//	/** A Syntax object should not outlive the std::ostream and 
			//	 *	Grammar ones given as arguments to its constructor.
			//	 */
			//	template<typename Lexer>
			//	Syntax(const Grammar<Lexer> &g, std::ostream &errorStream)
			//		:Syntax::base_type(expression)
			//	{
			//		expression = term(_r1)[_val = _1] > *(additiveOperator > term(_r1))[_val = bind(indirectCall,_1,_val,_2)];
			//		term = factor(_r1)[_val = _1] > *(multiplicativeOperator > factor(_r1))[_val = bind(indirectCall,_1,_val,_2)];
			//		factor = prefix(_r1)[_val = _1] | (prefixOperator > prefix(_r1))[_val = bind(indirectCall,_1,_2)];
			//		prefix = primary(_r1)[_val = _1] > *(exponentiationOperator > primary(_r1))[_val = bind(indirectCall,_1,_val,_2)];	// parses more expressions
			//		primary = g.rationalLiteral[_val = new_<LiteralNode>(bind(stringToRational,_1))] // than I can currently bring to canonical form
			//				| g.identifier[_val = bind(createVariable,_1,_r1)]	// but I should issue proper error messages form transformation routines.
			//				| '(' > expression(_r1)[_val = _1] > ')';

			//		expression.name("expression");
			//		term.name("term");
			//		factor.name("factor");
			//		prefix.name("prefix-expression");
			//		primary.name("primary-expression");

			//		qi::on_error<qi::fail>
			//		(
			//			expression, errorStream << val("Parse error: \"") << construct<std::string>(_1, _2) << "\"!\n"
			//									<< val("Here:         ") << construct<std::string>(bind(std::distance<ForwardIterator>,_1,_3),'-') << "^\n"
			//									<< "Exprected a " << _4 << ".\n"
			//		);
			//	} // end Syntax constructor

			//private:
			//	// Boost::Spirit and Boost::Phoenix work-arrounds:
			//	template<template<class> class Operator>
			//	static AbstractNode *unaryCombine(AbstractNode *subExpression)
			//	{
			//		return new UnaryNode<Operator>(std::unique_ptr<AbstractNode>(subExpression));
			//	} // end function unaryCombine

			//	template<template<class> class Operator>
			//	static AbstractNode *binaryCombine(AbstractNode *leftSubExpression, AbstractNode *rightSubExpression)
			//	{
			//		return new BinaryNode<Operator>(std::unique_ptr<AbstractNode>(leftSubExpression),std::unique_ptr<AbstractNode>(rightSubExpression));
			//	} // end function binaryCombine

			//	static AbstractNode *indirectCall(AbstractNode *(*f)(AbstractNode *),AbstractNode *arg)
			//	{
			//		return f(arg);
			//	} // end function inderectCall

			//	static AbstractNode *indirectCall(AbstractNode *(*f)(AbstractNode *,AbstractNode *),AbstractNode *arg1, AbstractNode *arg2)
			//	{
			//		return f(arg1,arg2);
			//	} // end function indirectCall

			//	static AbstractNode *createVariable(const NameType &name, symbol_table_type *symbolTable)
			//	{
			//		return new VariableNode(symbolTable->declare(name));
			//	} // end function createVariable

			//	/**	s must be a string of digits [0-9] containing zero or one '.'.
			//	 */
			//	static RationalType stringToRational(const NameType &s)
			//	{
			//		RationalType::int_type numerator = 0, denominator = 1;
			//		auto inBegin = s.begin();
			//		auto inEnd = s.end();

			//		while(inBegin != inEnd && *inBegin != '.')
			//		{
			//			numerator *= 10;
			//			numerator += *inBegin - '0';
			//			++inBegin;
			//		} // end while
			//		if(inBegin != inEnd)
			//			++inBegin;
			//		while(inBegin != inEnd)
			//		{
			//			numerator *= 10;
			//			numerator += *inBegin - '0';
			//			denominator *= 10;
			//			++inBegin;
			//		} // end while

			//		return RationalType(numerator,denominator);
			//	} // end function sequenceToRational

			//}; // end struct Syntax

		}; // end class Expression


		class Relation;
		class RelationSystem;

	} // end namespace FreeForms

	namespace CanonicalForms
	{


	} // end namespace CanonicalForms

	namespace DSEL
	{
		enum class Primitive {Variable,Constant};

		// Support Metafunction
		template<Primitive primitive,typename RationalType, typename NameType> struct ArgType;
		template<typename RationalType, typename NameType>
		struct ArgType<Primitive::Variable,RationalType,NameType>{typedef NameType type;};
		template<typename RationalType, typename NameType>
		struct ArgType<Primitive::Constant,RationalType,NameType>{typedef RationalType type;};

		template<Primitive primitive, typename RationalType, typename NameType = std::string, typename IDType = int>
		struct ExpressionBuilder
		{
			// Member Types
			typedef RationalType rational_type;
			typedef NameType name_type;
			typedef IDType id_type;
			typedef FreeForms::Expression<RationalType,NameType,IDType> expression_type;
			typedef typename expression_type::symbol_table_type symbol_table_type;
	
			// Fields
		public:
			static const Primitive build_primitive = primitive;
		private:
			std::shared_ptr<symbol_table_type> spSymbolTable;

			// Constructors / Destructor
		public:
			ExpressionBuilder(std::shared_ptr<symbol_table_type> spSymbolTable = std::make_shared<symbol_table_type>())
				:spSymbolTable(std::move(spSymbolTable))
			{
				// empty body
			} // end ExpressionBuilder default constructor

			// Methods
			expression_type operator()(const typename ArgType<primitive,RationalType,NameType>::type &argument)
			{
				return expression_type(argument,spSymbolTable);
			} // end method operator()
		}; // end struct template ExpressionBuilder


#define UNARY_EXPRESSION(op,tag) \
		template<typename RationalType, typename NameType, typename IDType> \
		inline FreeForms::Expression<RationalType,NameType,IDType> operator op (FreeForms::Expression<RationalType,NameType,IDType> subExpression) \
		{\
			return FreeForms::Expression<RationalType,NameType,IDType>::unaryCombine<tag>(std::move(subExpression));\
		} // end function operator op

		UNARY_EXPRESSION(+,FreeForms::OpTags::unary_plus);
		UNARY_EXPRESSION(-,FreeForms::OpTags::negate);
#undef UNARY_EXPRESSION

#define BINARY_EXPRESSION(op,tag) \
		template<typename RationalType, typename NameType, typename IDType> \
		inline auto operator op(FreeForms::Expression<RationalType,NameType,IDType> leftSubExpression, decltype(leftSubExpression) rightSubExpression)->decltype(leftSubExpression) \
		{\
			return FreeForms::Expression<RationalType,NameType,IDType>::\
				binaryCombine<tag>(std::move(leftSubExpression),std::move(rightSubExpression));\
		} /* end function operator op*/\
		\
		template<typename RationalType, typename NameType, typename IDType, typename OtherOpType> \
		inline auto operator op(FreeForms::Expression<RationalType,NameType,IDType> leftSubExpression, const OtherOpType &rightSubExpression)->decltype(leftSubExpression) \
		{\
			return FreeForms::Expression<RationalType,NameType,IDType>::\
				binaryCombine<tag>(std::move(leftSubExpression),decltype(leftSubExpression)(rightSubExpression));\
		} /* end function operator op*/\
		\
		template<typename RationalType, typename NameType, typename IDType, typename OtherOpType> \
		inline auto operator op(const OtherOpType &leftSubExpression, FreeForms::Expression<RationalType,NameType,IDType> rightSubExpression)->decltype(rightSubExpression) \
		{\
			return FreeForms::Expression<RationalType,NameType,IDType>::\
				binaryCombine<tag>(decltype(rightSubExpression)(leftSubExpression),std::move(rightSubExpression));\
		} /* end function operator op*/

		BINARY_EXPRESSION(+,FreeForms::OpTags::plus);
		BINARY_EXPRESSION(-,FreeForms::OpTags::minus);
		BINARY_EXPRESSION(*,FreeForms::OpTags::multiplies);
		BINARY_EXPRESSION(/,FreeForms::OpTags::divides);
		BINARY_EXPRESSION(^,FreeForms::OpTags::bit_xor);
#undef BINARY_EXPRESSION
	} // end namespace DSEL

} // end namespace Symbolic

#endif // SYMBOLIC_COMPUTATION_H
