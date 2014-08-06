#ifndef LINEAR_SYSTEM_SOLVING_H
#define LINEAR_SYSTEM_SOLVING_H

#include <utility>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include <cassert>

#include <Eigen/Dense>

namespace LinearSystem
{
	void runTestSuite();

	/** Transforms the input matrix to its reduced row echelon form, without 
	 *	doing any pivoting. The operations are done numerically as opposed to 
	 *	symbolic operations. The current version will work best with matrices 
	 *	comprising elements supporting exact operations (like rational numbers), 
	 *	but should also work with floating point numbers as well (albeit the 
	 *	lack of pivoting may cause numerical instability).
	 */
	template<typename Derived>	// should optimize later... e.g. set to 0s and 1s instead of computing them and swap whole rows if that's faster in eigen.
	Eigen::MatrixBase<Derived> &numericReducedRowEchelonFormNoPivot(Eigen::MatrixBase<Derived> &input)
	{
		typedef Eigen::MatrixBase<Derived> MatrixType;
		typedef MatrixType::Index IndexType;

		for(IndexType row = 0, col = 0 ; col < input.cols() ; ++col)
		{
			// find a row with nonzero element in current column
			IndexType leaderRow = row;
			while(leaderRow < input.rows() && input(leaderRow,col) == 0)
				++leaderRow;
			if(leaderRow < input.rows()) // if there is one
			{
				if(row != leaderRow)
					input.row(row).rightCols(input.cols()-col).swap(input.row(leaderRow).rightCols(input.cols()-col));
				input.row(row).rightCols(input.cols()-col) /= input(row,col);
				for(IndexType r = 0 ; r < input.rows() ; ++r)
					if(r != row && input(r,col) != 0)
						input.row(r).rightCols(input.cols()-col) -= input(r,col) * input.row(row).rightCols(input.cols()-col);
				++row;
			} // end if
		} // end for

		return input;
	} // end function numericReducedRowEchelonFormNoPivot


	/** Struct for describing the properties of a linear system. It can be used for systems with
	 *	unknown constant terms as well. Variable indices in freeVariables and boundUnknownConstants 
	 *	are in ascending order.
	 */
	struct Properties
	{
		// fields
		size_t nEquations;
		size_t nVariables;
		size_t nIndependentEquations;
		size_t nUnknownConstants;
		std::vector<size_t> freeVariables;
		std::vector<size_t> boundUnknownConstants;
		bool isImpossible;
		bool hasUniqueSolution;
	
		// constructors
		Properties(){/* empty body */}
	
		Properties(size_t nEquations, size_t nVariables, size_t nIndependentEquations,
			bool isImpossible, bool hasUniqueSolution, size_t nUnknownConstants = 0,
			std::vector<size_t> freeVariables = std::vector<size_t>(), std::vector<size_t> boundUnknownConstants = std::vector<size_t>())
			:nEquations(nEquations),nVariables(nVariables),nIndependentEquations(nIndependentEquations),
			nUnknownConstants(nUnknownConstants),freeVariables(freeVariables),boundUnknownConstants(boundUnknownConstants),
			isImpossible(isImpossible),hasUniqueSolution(hasUniqueSolution)
		{
			// empty body
		} // end Properties constructor

		// operators
		bool operator==(const Properties &other)
		{
			return this->nEquations == other.nEquations && this->nVariables == other.nVariables 
				&& this->nIndependentEquations == other.nIndependentEquations && this->nUnknownConstants == other.nUnknownConstants 
				&& this->freeVariables == other.freeVariables && this->boundUnknownConstants == other.boundUnknownConstants 
				&& this->isImpossible == other.isImpossible && this->hasUniqueSolution == other.hasUniqueSolution;
		} // end function operator==

	}; // end struct Properties


	/** Takes the augmented matrix A|b of a linear system of equations in reduced 
	 *	row echelon form and returns a structure describing the system.
	 */
	template<typename Derived>
	Properties numericInvestigate(const Eigen::MatrixBase<Derived> &system)
	{
		typedef typename Eigen::MatrixBase<Derived>::Index IndexType;

		Properties result;
		result.nEquations = system.rows();
		result.nVariables = system.cols() > 0 ? system.cols()-1 : 0;
		result.nUnknownConstants = 0;

		//result.freeVariables.reserve(result.nEquations);
		IndexType i = 0; // after the for-loop it will contain the number of linearly independent rows
		IndexType j = 0;
		for( ; i < system.rows() && j+1 < system.cols() ; ++j)
		{
			if(system(i,j) != 0)
				++i;
			else
				result.freeVariables.push_back(j);
		} // end for
		for( ; i == system.rows() && j+1 < system.cols() ; ++j)
			result.freeVariables.push_back(j);
		result.nIndependentEquations = i;

		assert(result.nIndependentEquations + result.freeVariables.size() == result.nVariables); // sanity check :)

		result.isImpossible = result.nIndependentEquations < result.nEquations && system(i,system.cols()-1) != 0;
		result.hasUniqueSolution = !result.isImpossible && result.nIndependentEquations == result.nVariables;

		return result;
	} // end function numericInvestigate


	/** Takes the augmented matrix A|B|b of a linear system of equations that may have a number of unknown constant terms in 
	 *	addition to the known ones. The matrix encodes the variable coefficients in the left columns and the known constant 
	 *	terms in the right column as usual, but has also columns encoding the coefficients of the unknown constants between 
	 *	them. The unknown constants are assumed to be in the left hand side of the equations (that directly relates to the 
	 *	signs of the coefficients) and are otherwise encoded like variables. The matrix must be in reduced row echelon form.
	 *	It returns a structure describing the system.
	 */
	template<typename Derived>
	Properties semiSymbolicInvestigate(const Eigen::MatrixBase<Derived> &system, size_t nUnknownConstants)
	{
		Properties result = numericInvestigate(system);
		if(nUnknownConstants > result.nVariables)
			throw std::out_of_range("You can't have more unknown constants than columns!");
		result.nVariables -= nUnknownConstants;
		result.nUnknownConstants = nUnknownConstants;

		for(size_t constant = nUnknownConstants ; constant-- > 0 ; )
			if(!result.freeVariables.empty() && result.freeVariables.back() == result.nVariables + constant)
				result.freeVariables.pop_back();
			else
				result.boundUnknownConstants.push_back(constant);
		std::reverse(result.boundUnknownConstants.begin(),result.boundUnknownConstants.end());

		result.nIndependentEquations = result.nVariables - result.freeVariables.size();
		result.hasUniqueSolution = !result.isImpossible && result.freeVariables.empty();

		return result;
	} // end function semiSymbolicInvestigate


	/** Takes the augmented matrix A|b of a linear system of equations in reduced 
	 *	row echelon form and returns a pair (C,d) such as for each column vector x
	 *	with as many rows as C has columns it holds that A*(C*x+d)==b. In other 
	 *	words, C*x+d is a solution of the system.
	 */
	template<typename Derived>
	auto numericSolve(const Eigen::MatrixBase<Derived> &system)->
		std::pair<Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic>,
					Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1>>
	{
		typedef typename Eigen::MatrixBase<Derived>::Index IndexType;

		Properties properties = numericInvestigate(system);
		Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic> base(properties.nVariables,0);
		Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1> offset;

		if(properties.hasUniqueSolution)
		{
			base.setZero(properties.nVariables,1);
			if(system.cols())
				offset = system.topRightCorner(properties.nVariables,1);
		}
		else if(!properties.isImpossible)
		{
			base.resize(properties.nVariables,properties.freeVariables.size());
			offset.resize(properties.nVariables,1);

			size_t f = 0;
			for(IndexType i = 0 ; i < base.rows() ; ++i)
			{
				if(f < properties.freeVariables.size() && i == properties.freeVariables[f])
				{
					for(IndexType j = 0 ; j < base.cols() ; ++j)
						base(i,j) = (j==f) ? 1 : 0;
					offset(i) = 0;
					++f;
				}
				else
				{
					for(IndexType j = 0 ; j < base.cols() ; ++j)
						base(i,j) = -system(i-f,properties.freeVariables[j]);
					offset(i) = system(i-f,system.cols()-1);
				} // end else
			} // end for
		} // end if

		return std::make_pair(base,offset);
	} // end function numericSolve


	/** Takes the augmented matrix A|B|b of a linear system of equations that may have a number of unknown constant terms in 
	 *	addition to the known ones. The matrix encodes the variable coefficients in the left columns and the known constant 
	 *	terms in the right column as usual, but has also columns encoding the coefficients of the unknown constants between 
	 *	them. The unknown constants are assumed to be in the left hand side of the equations (that directly relates to the 
	 *	signs of the coefficients) and are otherwise encoded like variables. The matrix must be in reduced row echelon form.
	 *	It returns a tuple of 4 matrices encoding the solution to the system. The first two generate the solutions to the system
	 *	given a vector of values for the free variables and unknown constants (like in the numeric counterpart of the function)
	 *	The other two generate the values of the unknown constants that make the system solvable given a vector of values
	 *	for the free unknown constants.
	 */
	template<typename Derived>
	auto semiSymbolicSolve(const Eigen::MatrixBase<Derived> &system, size_t nUnknownConstants)->
		std::tuple<Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic>,
					Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1>,
					Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic>,
					Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1>>
	{
		typedef Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic> MatrixType;
		typedef Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1> ColumnType;

		Properties properties = semiSymbolicInvestigate(system,nUnknownConstants);
		auto numSolution = numericSolve(system);
	
		MatrixType constBase = properties.isImpossible ? MatrixType(properties.nUnknownConstants,0) 
			: properties.nUnknownConstants-properties.boundUnknownConstants.size() == 0 ? MatrixType::Zero(properties.nUnknownConstants,1)
			: numSolution.first.bottomRightCorner(properties.nUnknownConstants,properties.nUnknownConstants-properties.boundUnknownConstants.size()).eval();

		return std::make_tuple(numSolution.first.topRows(properties.nVariables),
								numSolution.second.topRows(properties.isImpossible ? 0 : properties.nVariables),
								constBase,
								numSolution.second.bottomRows(properties.isImpossible ? 0 : properties.nUnknownConstants));
	} // end function semiSymbolicSolve

} // end namespace LinearSystem

#endif // LINEAR_SYSTEM_SOLVING_H
