#ifndef LINEAR_SYSTEM_SOLVING_H
#define LINEAR_SYSTEM_SOLVING_H

#include <utility>
#include <vector>

#include <cassert>

#include <Eigen/Dense>

void runLinearSystemSolvingTestSuite();

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


/** Struct for describing the properties of a linear system.
 */
struct LinearSystemProperties
{
	// fields
	size_t nEquations;
	size_t nVariables;
	size_t nIndependentEquations;
	std::vector<size_t> freeVariables;
	bool isImpossible;
	bool hasUniqueSolution;
	
	// constructors
	LinearSystemProperties(){/* empty body */}
	
	LinearSystemProperties(size_t _nEquations, size_t _nVariables, size_t _nIndependentEquations,
		std::vector<size_t> _freeVariables, bool _isImpossible, bool _hasUniqueSolution)
		:nEquations(_nEquations),nVariables(_nVariables),nIndependentEquations(_nIndependentEquations),
		freeVariables(_freeVariables),isImpossible(_isImpossible),hasUniqueSolution(_hasUniqueSolution)
	{
		// empty body
	} // end LinearSystemProperties constructor

	// operators
	bool operator==(const LinearSystemProperties &other)
	{
		return this->nEquations == other.nEquations && this->nVariables == other.nVariables && this->nIndependentEquations == other.nIndependentEquations
			&& this->freeVariables == other.freeVariables && this->isImpossible == other.isImpossible && this->hasUniqueSolution == other.hasUniqueSolution;
	} // end function operator==

}; // end struct LinearSystemProperties


/** Takes the augmented matrix A|b of a linear system of equations in reduced 
 *	row echelon form and returns a structure describing the system.
 */
template<typename Derived>
LinearSystemProperties numericInvestigateLinearSystem(const Eigen::MatrixBase<Derived> &system)
{
	typedef typename Eigen::MatrixBase<Derived>::Index IndexType;

	LinearSystemProperties properties;
	properties.nEquations = system.rows();
	properties.nVariables = system.cols() > 0 ? system.cols()-1 : 0;

	//properties.freeVariables.reserve(properties.nEquations);
	IndexType i = 0; // after the for-loop it will contain the number of linearly independent rows
	IndexType j = 0;
	for( ; i < system.rows() && j+1 < system.cols() ; ++j)
	{
		if(system(i,j) != 0)
			++i;
		else
			properties.freeVariables.push_back(j);
	} // end for
	for( ; i == system.rows() && j+1 < system.cols() ; ++j)
		properties.freeVariables.push_back(j);
	properties.nIndependentEquations = i;

	assert(properties.nIndependentEquations + properties.freeVariables.size() == properties.nVariables); // sanity check :)

	properties.isImpossible = properties.nIndependentEquations < properties.nEquations && system(i,system.cols()-1) != 0;
	properties.hasUniqueSolution = !properties.isImpossible && properties.nIndependentEquations == properties.nVariables;

	return properties;
} // end function numericInvestigateLinearSystem


/** Takes the augmented matrix A|b of a linear system of equations in reduced 
 *	row echelon form and returns a pair (C,d) such as for each column vector x
 *	with as many rows as C has columns it holds that A*(C*x+d)==b. In other 
 *	words, C*x+d is a solution of the system.
 */
template<typename Derived>
auto numericSolveLinearSystem(const Eigen::MatrixBase<Derived> &system)->
	std::pair<Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic>,
				Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,1>>
{
	typedef typename Eigen::MatrixBase<Derived>::Index IndexType;

	LinearSystemProperties properties = numericInvestigateLinearSystem(system);
	Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar,Eigen::Dynamic,Eigen::Dynamic> base;
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
} // end function numericSolveLinearSystem

#endif // LINEAR_SYSTEM_SOLVING_H
