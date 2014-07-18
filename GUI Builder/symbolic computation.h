#ifndef SYMBOLIC_COMPUTATION_H
#define SYMBOLIC_COMPUTATION_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
#include <functional>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

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
		namespace qi = boost::spirit::qi;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;
		using qi::rule;
		using qi::_val;
		using qi::_r1;
		using qi::_1;
		using qi::_2;
		using qi::int_;
		using phoenix::bind;
		using phoenix::construct;
		using phoenix::new_;
		using ascii::space_type;


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
			typedef std::shared_ptr<symbol_table_type> symbol_table_pointer_type;

			struct AbstractNode
			{
				virtual std::unique_ptr<AbstractNode> deepCopy() const = 0;
				virtual ~AbstractNode(){/* empty body */}
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
				BinaryNode(std::unique_ptr<AbstractNode> leftChild_, std::unique_ptr<AbstractNode> rightChild_)
					:leftChild(std::move(leftChild_)),rightChild(std::move(rightChild_))
				{
					// empty body
				} // end BinaryNode constructor

				virtual ~BinaryNode(){/* empty body */}

				// Methods
				virtual std::unique_ptr<AbstractNode> deepCopy() const
				{
					return std::unique_ptr<AbstractNode>(new BinaryNode(leftChild->deepCopy(),rightChild->deepCopy()));
				} // end method deepCopy
			}; // end struct BinaryNode


			/***************
			*    Fields    *
			***************/

			std::shared_ptr<Common::SymbolTable<NameType,IDType>> symbols; // should never be a null shared pointer, due to getSymbols method
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
			template<typename ForwardIterator>
			Expression(ForwardIterator begin, ForwardIterator end, std::shared_ptr<symbol_table_type> symbolTable = std::shared_ptr<symbol_table_type>(new symbol_table_type()))
				:symbols(symbolTable)
			{
				ExpressionGrammar<ForwardIterator> grammar;
				AbstractNode *parseTree = nullptr;

				bool success = qi::phrase_parse(begin,end,grammar(symbols.get()),ascii::space,parseTree);
				expressionTree.reset(parseTree);

				if(!success || begin != end)
					throw std::runtime_error("Error parsing mathematical expression!");
			} // end Expression constructor

			~Expression(){/* empty body */}


			/****************
			*    Methods    *
			****************/

			const symbol_table_type &getSymbols() const
			{
				return *symbols;
			} // end method getSymbols

			/** Construct an expression object form a smaller one and a unary operator
			 */
			template<template<class> class Operator>
			static Expression unaryCombine(Expression subExpression)
			{
				subExpression.expressionTree.reset(new UnaryNode<Operator>(std::move(subExpression.expressionTree)));
				return subExpression;
			} // end static method unaryCombine

			/** Construct an expression object form two smaller ones and a binary operator
			 */
			template<template<class> class Operator>
			static Expression binaryCombine(Expression leftSubExpression, Expression rightSubExpression)
			{
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
			typedef AbstractNode *attribute_signature(symbol_table_type *);

			template<typename ForwardIterator>
			struct ExpressionGrammar : public qi::grammar<ForwardIterator,attribute_signature,ascii::space_type>
			{
				// Operator parsers
				struct AdditiveOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
				{
					AdditiveOperator()
					{
						add("+",binaryCombine<std::plus>)
						   ("-",binaryCombine<std::minus>);
					} // end AdditiveOperator constructor
				} additiveOperator; // end struct AdditiveOperator

				struct MultiplicativeOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
				{
					MultiplicativeOperator()
					{
						add("*",binaryCombine<std::multiplies>)
						   ("/",binaryCombine<std::divides>);
					} // end MultiplicativeOperator constructor
				} multiplicativeOperator; // end struct MultiplicativeOperator

				struct ExponentiationOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *,AbstractNode *)>
				{
					ExponentiationOperator()
					{
						add("^",binaryCombine<std::bit_xor>);
					} // end ExponentiationOperator constructor
				} exponentiationOperator; // end struct ExponentiationOperator

				struct PrefixOperator : public qi::symbols<char,AbstractNode *(*)(AbstractNode *)>
				{
					PrefixOperator()
					{
						add("+",unaryCombine<DSEL::unary_plus>)
						   ("-",unaryCombine<std::negate>);
					} // end PrefixOperator constructor
				} prefixOperator; // end struct PrefixOperator

				// Non-Terminals
				rule<ForwardIterator,attribute_signature,space_type> expression;
				rule<ForwardIterator,attribute_signature,space_type> term;
				rule<ForwardIterator,attribute_signature,space_type> factor;
				rule<ForwardIterator,attribute_signature,space_type> prefix;
				rule<ForwardIterator,attribute_signature,space_type> primary;
				rule<ForwardIterator,AbstractNode *(),space_type> literal;

				// Rules
				ExpressionGrammar()
					:ExpressionGrammar::base_type(expression)
				{
					expression = term(_r1)[_val = _1] >> *(additiveOperator >> term(_r1))[_val = bind(indirectCall,_1,_val,_2)];
					term = factor(_r1)[_val = _1] >> *(multiplicativeOperator >> factor(_r1))[_val = bind(indirectCall,_1,_val,_2)];
					factor = prefix(_r1)[_val = _1] | (prefixOperator >> prefix(_r1))[_val = bind(indirectCall,_1,_2)];
					prefix = primary(_r1)[_val = _1] >> *(exponentiationOperator >> literal)[_val = bind(indirectCall,_1,_val,_2)];
					primary = literal[_val = _1] | '(' >> expression(_r1)[_val = _1] >> ')';
					literal = int_[_val = new_<LiteralNode>(_1)] /*>> -('.' >> int_)*/;					
				} // end ExpressionGrammar constructor

			private:
				// Boost::Spirit and Boost::Phoenix workarounds:
				template<template<class> class Operator>
				static AbstractNode *unaryCombine(AbstractNode *subExpression)
				{
					return new UnaryNode<Operator>(std::unique_ptr<AbstractNode>(subExpression));
				} // end function unaryCombine

				template<template<class> class Operator>
				static AbstractNode *binaryCombine(AbstractNode *leftSubExpression, AbstractNode *rightSubExpression)
				{
					return new BinaryNode<Operator>(std::unique_ptr<AbstractNode>(leftSubExpression),std::unique_ptr<AbstractNode>(rightSubExpression));
				} // end function binaryCombine

				static AbstractNode *indirectCall(AbstractNode *(*f)(AbstractNode *),AbstractNode *arg)
				{
					return f(arg);
				} // end function inderectCall

				static AbstractNode *indirectCall(AbstractNode *(*f)(AbstractNode *,AbstractNode *),AbstractNode *arg1, AbstractNode *arg2)
				{
					return f(arg1,arg2);
				} // end function indirectCall
			}; // end struct ExpressionGrammar

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
			ExpressionBuilder(std::shared_ptr<symbol_table_type> spSymbolTable_ = std::make_shared<symbol_table_type>())
				:spSymbolTable(std::move(spSymbolTable_))
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

		template<typename T>	// fill in function object not appearing in standard headers
		struct unary_plus
		{
			typedef T argument_type;
			typedef T result_type;

			T operator()(const T &x) const
			{
				return +x;
			} // end method operator()
		}; // end struct unary_plus

		UNARY_EXPRESSION(+,unary_plus);
		UNARY_EXPRESSION(-,std::negate);
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

		BINARY_EXPRESSION(+,std::plus);
		BINARY_EXPRESSION(-,std::minus);
		BINARY_EXPRESSION(*,std::multiplies);
		BINARY_EXPRESSION(/,std::divides);
		BINARY_EXPRESSION(^,std::bit_xor);
#undef BINARY_EXPRESSION
	} // end namespace DSEL

} // end namespace Symbolic

#endif // SYMBOLIC_COMPUTATION_H
