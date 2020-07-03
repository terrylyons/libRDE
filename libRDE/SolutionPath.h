/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __SOLUTIONPATH__
#define __SOLUTIONPATH__


#include "Path.h"
#include "GroupElement.h"
#include "LieMatrix.h"
#include "dyadic_interval.h"
#include <vector>


/// Gives the solution path to the RDE if the RDE is linear.

/// Uses the Pade approximation to exponentiate the linear vector field (ie matrix) 
/// obtained by combining the path over an interval with the equation.
template <typename my_alg_type_IN, typename my_alg_type_OUT>
class SolutionPath
{
private:
	/// The input path (eg Brownian path)
	Path<my_alg_type_IN> & theControl;

	/// The equation is stored in here.
	lietovectorfield< my_alg_type_IN, LieMatrix<my_alg_type_OUT> > l2vf;


public:
	/// Constructor. Takes the input path and equation.
	SolutionPath(Path<my_alg_type_IN> & theControlIn, std::vector< LieMatrix<my_alg_type_OUT> > theEquationIn)
		: theControl(theControlIn), l2vf(theEquationIn)
	{
	};

	/// Default Destructor.
	~SolutionPath(void)
	{
	};

	/// Describes the path over an increment.

	/// Combines the Lie element corresponding to the path on the dyadic_increment given
	/// with the equation and exponentiates the resulting matrix.
	GroupElement<my_alg_type_OUT> DescribePath(dyadic_interval increment)
	{
		LieMatrix<my_alg_type_OUT> temp;
		temp = l2vf.lie2vectorfield(theControl.DescribePath(increment, increment.n));
		return temp.exp();
	};

	GroupElement<my_alg_type_OUT> DescribePath(dyadic_interval increment, const int accuracy)
	{
		LieMatrix<my_alg_type_OUT> temp;
		temp = l2vf.lie2vectorfield(theControl.DescribePath(increment, accuracy));
		return temp.exp();
	};

};// END CLASS DEFINITION SolutionPath

#endif // __SOLUTIONPATH__
