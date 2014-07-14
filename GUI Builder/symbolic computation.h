#ifndef SYMBOLIC_COMPUTATION_H
#define SYMBOLIC_COMPUTATION_H

#include <map>
#include <vector>
#include <utility>
#include <stdexcept>

namespace Symbolic
{
	void runTestSuite();

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

		}; // end class SymbolTable


	} // end namespace Common

	namespace FreeForms
	{


	} // end namespace FreeForms

	namespace CanonicalForms
	{


	} // end namespace CanonicalForms

	namespace DSEL
	{


	} // end namespace DSEL

} // end namespace Symbolic

#endif // SYMBOLIC_COMPUTATION_H
