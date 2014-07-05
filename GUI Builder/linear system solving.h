#ifndef LINEAR_SYSTEM_SOLVING_H
#define LINEAR_SYSTEM_SOLVING_H

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


#endif // LINEAR_SYSTEM_SOLVING_H
