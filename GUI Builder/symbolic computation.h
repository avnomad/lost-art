#ifndef SYMBOLIC_COMPUTATION_H
#define SYMBOLIC_COMPUTATION_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
#include <functional>

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
		template<typename RationalType, typename NameType = std::string, typename IDType = int>
		class Formula
		{
			/*********************
			*    Member Types    *
			*********************/
		public:
			typedef RationalType rational_type;
			typedef NameType name_type;
			typedef IDType id_type;

		private:

			struct AbstractNode
			{
				virtual std::unique_ptr<AbstractNode> deepCopy() const = 0;
				virtual ~AbstractNode() = 0;
			}; // end struct AbstractNode


			struct LiteralNode : public AbstractNode
			{
				// Fields
				RationalType value;

				// Constructors / Destructor
				LiteralNode(const RationalType &value_)
					:value(value_)
				{
					// empty body
				} // end LiteralNode constructor

				virtual ~LiteralNode(){/* empty body */}

				// Methods
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
				VariableNode(IDType id_)
					:id(id_)
				{
					// empty body
				} // end VariableNode constructor

				virtual ~VariableNode(){/* empty body */}

				// Methods
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
				UnaryNode(std::unique_ptr<AbstractNode> child_)
					:child(std::move(child_))
				{
					// empty body
				} // end VariableNode constructor

				virtual ~UnaryNode(){/* empty body */}

				// Methods
				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new UnaryNode(child->copy()));
				} // end method deepCopy
			}; // end struct UnaryNode


			template<template<class> class Operator>
			struct BinaryNode : public AbstractNode
			{
				// Fields
				std::unique_ptr<AbstractNode> leftChild;
				std::unique_ptr<AbstractNode> rightChild;

				// Constructors
				BinaryNode(std::unique_ptr<AbstractNode> leftChild_, std::unique_ptr<AbstractNode> rightChild_)
					:leftChild(std::move(leftChild_)),rightChild(std::move(rightChild_))
				{
					// empty body
				} // end BinaryNode constructor

				virtual ~BinaryNode(){/* empty body */}

				// Methods
				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new BinaryNode(leftChild->copy(),rightChild->copy()));
				} // end method deepCopy
			}; // end struct BinaryNode


			/***************
			*    Fields    *
			***************/

			Symbolic::Common::SymbolTable<NameType,IDType> symbols;
			std::unique_ptr<AbstractNode> expressionTree;


			/**********************************
			*    Constructors / Destructor    *
			**********************************/
		public:
			/** Construct an empty Formula
			 */
			Formula(){/* empty body */}

			Formula(const Formula &other)
				:symbols(other.symbols),expressionTree(other.expressionTree->deepCopy())
			{
				// empty body
			} // end Formula copy constructor

			Formula(Formula &&other)
				:symbols(std::move(other.symbols)),expressionTree(std::move(other.expressionTree))
			{
				// empty body
			} // end Formula move constructor

			///** Construct a formula containing a single literal value
			// */
			//Formula(const RationalType &literalValue)
			//	:expressionTree(new LiteralNode(literalValue))
			//{
			//	// empty body
			//} // end Formula constructor

			///** Construct a formula containing a single symbol
			// */
			//Formula(const NameType &variableName)
			//{
			//	expressionTree.reset(new VariableNode(symbols.declare(variableName)));
			//} // end Formula constructor

			///** Construct a formula object form a smaller one and a unary operator
			// */
			//template<template<class> class Operator>
			//Formula(Formula subExpression)
			//	:symbols(std::move(subExpresion.symbols)),
			//	expressionTree(new UnaryNode<Operator>(std::move(subExpression.expressionTree)))
			//{
			//	// empty body
			//} // end Formula constructor

			///** Construct a formula object form two smaller ones and a binary operator
			// */
			//template<template<class> class Operator>
			//Formula(Formula leftSubExpression, rightSubExpression)
			//	:symbols(std::move(leftSubExpression.symbols)),
			//	expressionTree(new BinaryNode<Operator>(std::move(subExpression.expressionTree)))
			//{
			//	// empty body
			//} // end Formula constructor


			~Formula(){/* empty body */}


			/****************
			*    Methods    *
			****************/

			/******************
			*    Operators    *
			******************/

			Formula &operator=(Formula other)
			{
				symbols = std::move(other.symbols);
				expressionTree = std::move(other.expressionTree);
				return *this;
			} // end method operator=

		}; // end class Formula


		class Relation;
		class RelationSystem;




	} // end namespace FreeForms

	namespace CanonicalForms
	{


	} // end namespace CanonicalForms

	namespace DSEL
	{


	} // end namespace DSEL

} // end namespace Symbolic

#endif // SYMBOLIC_COMPUTATION_H
